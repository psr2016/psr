#include <xc.h>
#include <stddef.h>
#include <stdint.h>

#include "bus_objects.h"
#include "ecan_driver.h"
#include "ecan_lib.h"

#if defined(__dsPIC33FJ128MC802__)
# define FAMILY_ID		0x33F
# define DEVICE_ID		0x629
# define KERNEL_SECTION_ADDRESS	0x15000
# define WORDS_PER_PAGE		512
#elif defined(__dsPIC33EP512MC502__)
# define FAMILY_ID		0x33E
# define DEVICE_ID		0x1785
# define KERNEL_SECTION_ADDRESS	0x54800
# define WORDS_PER_PAGE		1024
#else
# error Tipo di PIC non supportato
#endif

// Le funzioni con questo attributo vengono messe nella zona non riprogrammabile tramite CAN
#define KERNEL_SECTION __attribute__((section("canrtsp_kernel")))

// Costanti per CRC16
#define CRC16_INITIAL	0xFFFF
#define CRC16_POLY	0xA001

#define stringify(x) stringify2(x)
#define stringify2(x) #x

// Variabili temporanee, il codice in KERNEL_SECTION non può contenere
// riferimenti assoluti a questa variabile, perché l'indirizzo viene
// allocato dal linker in modo diverso a seconda del firmware
typedef struct
{
	// Puntatori ai message buffer CAN per comunicazione con coordinatore
	uint8_t *dma_tx_buff;
	uint8_t *dma_rx_buff;

	// Buffer temporaneo (grande quanto una riga di Flash memory)
	uint8_t flashbuffer[192];

	// Target address (per operazioni di scrittura)
	uint32_t target_address;

	// Offset per trasmissione frammenti di flashbuffer
	int flashbuffer_offs; // posto a -1 in caso di buffer overflow
} kernel_parameters;
static kernel_parameters kp __attribute__((far));

// Invia messaggio CAN a coordinatore
static void KERNEL_SECTION send_can_message(kernel_parameters *kp, const void *message_buffer)
{
	int i = 8;

	// Il tag e la dimensione del messaggio (fissata a 8 byte) sono già
	// stati impostati in canrtsp_enter()

	// Imposta buffer dati
	while (i--)
		kp->dma_tx_buff[6 + i] = ((const uint8_t*)message_buffer)[i];

	// Invia messaggio e attendi completamento dell'operazione
	C1TR01CONbits.TXREQ0 = 1;
	while (C1TR01CONbits.TXREQ0 == 1);
}

// Attende e riceve messaggio CAN da coordinatore
static void KERNEL_SECTION recv_can_message(kernel_parameters *kp, void *message_buffer)
{
	int i = 8;

	// Attendi ricezione
	while (C1RXFUL1bits.RXFUL1 == 0);

	// Copia dati su buffer di output
	while (i--)
		((uint8_t*)message_buffer)[i] = kp->dma_rx_buff[6 + i];

	// Rilascia buffer dma
	C1RXFUL1bits.RXFUL1 = 0;
}

// Calcola CRC16 del contenuto di kp->flashbuffer (val. iniziale crc = CRC16_INITIAL)
static uint16_t KERNEL_SECTION crc16(kernel_parameters *kp, uint16_t crc)
{
	const uint8_t *buffer = kp->flashbuffer;
	int i, j;

	for (i = 0; i < 192; i++)
	{
		crc ^= *buffer++;

		for (j = 0; j < 8; ++j)
		{
			if (crc & 1)
				crc = (crc >> 1) ^ CRC16_POLY;
			else
				crc = (crc >> 1);
		}
	}

	return crc;
}

// Copia riga da flash memory a kp->flashbuffer (base_addr deve essere multiplo di 128)
static void KERNEL_SECTION load_row(kernel_parameters *kp, uint32_t base_addr)
{
	unsigned int i;
	uint16_t base_offs = base_addr;
	TBLPAG = base_addr >> 16;

	uint8_t *buffer = kp->flashbuffer;
	for (i = 0; i < 64; i++)
	{
		uint16_t tbloffs = base_offs + 2*i;
		uint16_t low_word = __builtin_tblrdl(tbloffs);
		*buffer++ = low_word & 0xff;
		*buffer++ = low_word >> 8;
		*buffer++ = __builtin_tblrdh(tbloffs);
	}
}

// Calcola CRC16 di un'intera pagina (base_addr deve essere multiplo di 2*WORDS_PER_PAGE)
static uint16_t KERNEL_SECTION page_crc(kernel_parameters *kp, uint32_t base_addr)
{
	int rows = WORDS_PER_PAGE / 64;
	uint16_t crc = CRC16_INITIAL;

	while (rows--)
	{
		load_row(kp, base_addr);
		base_addr += 128;
		crc = crc16(kp, crc);
	}

	return crc;
}

// Accoda nuovi dati a kp->flashbuffer
static void KERNEL_SECTION append_to_flashbuffer(kernel_parameters *kp, const uint8_t *newdata)
{
	int i;

	if (kp->flashbuffer_offs == -1)
	{
		// Non fare nulla, eravamo già in overflow
	}
	else if (kp->flashbuffer_offs == 192)
	{
		kp->flashbuffer_offs = -1;
	}
	else
	{
		for (i = 0; i < 7 && kp->flashbuffer_offs < 192; i++)
			kp->flashbuffer[kp->flashbuffer_offs++] = newdata[i];
	}
}

// Copia 6 byte da kp->flashbuffer a data
static void KERNEL_SECTION pop_from_flashbuffer(kernel_parameters *kp, uint8_t *data)
{
	int i;

	if (kp->flashbuffer_offs == -1 || kp->flashbuffer_offs == 192)
	{
		kp->flashbuffer_offs = -1;
	}
	else
	{
		for (i = 0; i < 6 && kp->flashbuffer_offs < 192; i++)
			data[i] = kp->flashbuffer[kp->flashbuffer_offs++];
	}
}

#if defined(__dsPIC33FJ128MC802__)
// Effettua ERASE di un'intera pagina (base_addr deve essere multiplo di 2*WORDS_PER_PAGE)
static void KERNEL_SECTION page_erase(uint32_t base_addr)
{
	TBLPAG = base_addr >> 16;
	__builtin_tblwtl(((uint16_t)base_addr), 0);
	NVMCON = 0x4042;
	__builtin_write_NVM();
}

// Scrive riga da kp->flashbuffer a memoria flash (kp->target_address deve essere multiplo di 128)
static int KERNEL_SECTION row_flush(kernel_parameters *kp, uint16_t expected_crc)
{
	uint16_t base_offs = kp->target_address;
	const uint8_t *buffer = kp->flashbuffer;
	unsigned int i;

	// Controlli di consistenza
	if (kp->flashbuffer_offs != 192 || crc16(kp, CRC16_INITIAL) != expected_crc)
		return 0;

	NVMCON = 0x4001;
	TBLPAG = kp->target_address >> 16;

	for (i = 0; i < 64; i++)
	{
		uint16_t tbloffs = base_offs + 2*i;
		uint16_t low_word = *buffer++;
		low_word |= ((uint16_t)*buffer++) << 8;
		__builtin_tblwtl(tbloffs, low_word);
		__builtin_tblwth(tbloffs, *buffer++);
	}

	__builtin_write_NVM();
	return 1;
}
#elif defined(__dsPIC33EP512MC502__)
// Effettua ERASE di un'intera pagina (base_addr deve essere multiplo di 2*WORDS_PER_PAGE)
static void KERNEL_SECTION page_erase(uint32_t base_addr)
{
	NVMADRU = base_addr >> 16;
	NVMADR = ((uint16_t)base_addr);
	NVMCON = 0x4003;
	__builtin_write_NVM();
}

// Scrive riga da kp->flashbuffer a memoria flash (kp->target_address deve essere multiplo di 128)
static int KERNEL_SECTION row_flush(kernel_parameters *kp, uint16_t expected_crc)
{
	uint16_t base_offs = kp->target_address;
	const uint8_t *buffer = kp->flashbuffer;
	unsigned int i;

	// Controlli di consistenza
	if (kp->flashbuffer_offs != 192 || crc16(kp, CRC16_INITIAL) != expected_crc)
		return 0;

	NVMADRU = kp->target_address >> 16;
	TBLPAG = 0xFA;

	for (i = 0; i < 64; i += 2)
	{
		uint16_t low_word1 = *buffer++;
		low_word1 |= ((uint16_t)*buffer++) << 8;
		uint8_t high_word1 = *buffer++;
		uint16_t low_word2 = *buffer++;
		low_word2 |= ((uint16_t)*buffer++) << 8;
		uint8_t high_word2 = *buffer++;

		NVMADR = base_offs + 2*i;

		__builtin_tblwtl(0, low_word1);
		__builtin_tblwth(0, high_word1);
		__builtin_tblwtl(2, low_word2);
		__builtin_tblwth(2, high_word2);

		NVMCON = 0x4001;
		__builtin_write_NVM();
	}

	return 1;
}
#endif

void KERNEL_SECTION canrtsp_kernel(kernel_parameters *kp)
{
	t_can_remote_rtsp_command m_in;
	t_can_remote_rtsp_reply m_out;

	while (1)
	{
		recv_can_message(kp, &m_in);
		m_out.seqnum_echo = m_in.seqnum;

		switch (m_in.cmd)
		{
			case REMOTE_RTSP_COMMAND_GET_INFO:
				m_out.data = FAMILY_ID;
				m_out.data2 = DEVICE_ID;
				break;
			case REMOTE_RTSP_COMMAND_PAGE_CRC:
				m_out.data = page_crc(kp, m_in.target_address);
				break;
			case REMOTE_RTSP_COMMAND_PAGE_ERASE:
				page_erase(m_in.target_address);
				break;
			case REMOTE_RTSP_COMMAND_ROW_SELECT:
				kp->target_address = m_in.target_address;
				load_row(kp, kp->target_address);
				kp->flashbuffer_offs = 0;
				break;
			case REMOTE_RTSP_COMMAND_ROW_PUTDATA:
				append_to_flashbuffer(kp, m_in.payload);
				break;
			case REMOTE_RTSP_COMMAND_ROW_FLUSH:
				m_out.data = row_flush(kp, m_in.data);
				break;
			case REMOTE_RTSP_COMMAND_ROW_GETDATA:
				pop_from_flashbuffer(kp, (uint8_t*)m_out.payload);
				break;
			case REMOTE_RTSP_COMMAND_RESET:
			default:
				asm("reset");
				while (1);
				break;
		}

		send_can_message(kp, &m_out);
	}
}

void canrtsp_enter()
{
	int i;

	// Disattiva tutti i filtri CAN
	for (i = 0; i < ECAN_MAX_OBJECTS; i++)
		ecan_unset_rx_object(i);

	// Configura CAN per comunicazione con coordinatore
	kp.dma_tx_buff = (uint8_t*)ecan_get_buffer_ptr(0); // usa buffer #0 per messaggi in uscita
	ecan_set_rx_object(0, REMOTE_RTSP_CAN_ID(CAN_STDIO_AND_RTSP_NODE_ID), NULL, NULL, 0); // objnum=0, buffnum=1
	kp.dma_rx_buff = (uint8_t*)ecan_get_buffer_ptr(1);

	// Imposta tag e dimensione messaggi CAN in uscita
	*(uint16_t*)(kp.dma_tx_buff + 0) = REMOTE_RTSP_CAN_ID(REMOTE_RTSP_COORD_ID) << 2;
	*(uint16_t*)(kp.dma_tx_buff + 2) = 0;
	*(uint16_t*)(kp.dma_tx_buff + 4) = 8; // dim. fissa = 8 byte

	// Disattiva interrupt, da ora in poi gestiremo la comunicazione CAN in polling
	SET_CPU_IPL(7);

	// Inizializzazione variabili
	kp.flashbuffer_offs = -1;

	// Salta al kernel di programmazione (indirizzo KERNEL_SECTION_ADDRESS), passando l'indirizzo di kp come parametro
	asm(
		".set ___PA___,0\n\t"
		"mov %0, W0\n\t"
		"call " stringify(KERNEL_SECTION_ADDRESS) "\n\t"
		:: "d" (&kp)
	);
}
