#include "defines.h"
#include <p33FJ128MC802.h>
#include <libpic30++.h>
#include "Controllore.h"

#define USE_ANTIWINDUP
#define FULLSCALE_245dps     0.00875
#define FULLSCALE_500dps     0.0175
#define FULLSCALE_2000dps    0.07

Controllore::Controllore(I2C & IMU, SpeedControlTask & Controllo_Vel, float kp, float ki,float Saturazione)
:PeriodicTask("Controllore",POSITION_CONTROL_PERIOD,TIME_UNIT,POSITION_CONTROL_JITTER), IMU_comunication(IMU),
m_Controllo_Vel(Controllo_Vel)
{
    Kp = kp;
    Ki = ki;
    SAT = Saturazione;

    SAT_FLAG = false;
    OUT_I = 0;
    OMEGA_X = 0;
    OMEGA_Y = 0;
    OMEGA_Z = 0;

    OFFSET_X = 0;
    OFFSET_Y = 0;
    OFFSET_Z = 0;

    V_lineare = 0;
    V_Left = 0;
    V_Right = 0;
}

void Controllore::set_KP(float KP_ext)
{
    Kp = KP_ext;
}

void Controllore::set_KI(float KI_ext)
{
    Ki = KI_ext;
}

void Controllore::set_SAT(float SAT_ext)
{
    SAT = SAT_ext;
}

void Controllore::set_V(float V_ext)
{
    V_lineare = V_ext;
}

void Controllore::on()
{
    SAT_FLAG = 0;
    OUT_I = 0;
    PeriodicTask::on();
}

void Controllore::InitGyro()
{
    //Inizializzazione Giroscopio IMU (DEVICE_REG = 0xD6)
    IMU_comunication.write_register(0xD6, 0x20, 0x8F);  //CTRL1(20h)---->10001111 (ODR = 400Hz, Cut-Off = 20Hz)
    IMU_comunication.write_register(0xD6, 0x21, 0x00);  //CTRL2(21h)---->00000000 (Hig-Pass Filter in normal mode)
    IMU_comunication.write_register(0xD6, 0x22, 0x00);  //CTRL3(22h)---->00000000 (NO Interrupt driven to INT1 pad)
    IMU_comunication.write_register(0xD6, 0x23, 0x00);  //CTRL4(23h)---->00000000 (FullScale 245dps, Self-Test Disable mode)
    IMU_comunication.write_register(0xD6, 0x24, 0x00);  //CTRL5(24h)---->00000000 (High-Pass Filt. Data are used for interrupt gen.)

    //Calcolo OFFSET giroscopio su 100 misure
    float Somma_X = 0;
    float Somma_Y = 0;
    float Somma_Z = 0;

    for(int i = 0; i < 100; i++)
    {
       unsigned char omega_x8[2];
       unsigned char omega_y8[2];
       unsigned char omega_z8[2];

       IMU_comunication.read_buffer(0xD6, 0x28 | 0x80, &omega_x8[0], 2);
       IMU_comunication.read_buffer(0xD6, 0x2A | 0x80, &omega_y8[0], 2);
       IMU_comunication.read_buffer(0xD6, 0x2C | 0x80, &omega_z8[0], 2);

       short int omega_x16 = omega_x8[0] | (omega_x8[1]<<8);
       short int omega_y16 = omega_y8[0] | (omega_y8[1]<<8);
       short int omega_z16 = omega_z8[0] | (omega_z8[1]<<8);

       Somma_X = Somma_X + (omega_x16*FULLSCALE_245dps);
       Somma_Y = Somma_Y + (omega_y16*FULLSCALE_245dps);
       Somma_Z = Somma_Z + (omega_z16*FULLSCALE_245dps);
    }

    OFFSET_X = Somma_X/100;
    OFFSET_Y = Somma_Y/100;
    OFFSET_Z = Somma_Z/100;
}

void Controllore::ReadGyro()
{
    //Creiamo i vettori a due elementi da 8 bit contenenti parte alta e parte bassa delle misure delle velocità angolariu
    unsigned char omega_x8[2];   //omega_x[0] = BL = 0x28, omega_x[1] = BH = 0x29
    unsigned char omega_y8[2];   //omega_y[0] = BL = 0x2A, omega_y[1] = BH = 0x2B
    unsigned char omega_z8[2];   //omega_z[0] = BL = 0x2C, omega_z[1] = BH = 0x2D
		
    //Lettura giroscopio IMU (mi restituisce ho letto-non ho letto)
    IMU_comunication.read_buffer(0xD6, 0x28 | 0x80, &omega_x8[0], 2);   //data_len in byte
    IMU_comunication.read_buffer(0xD6, 0x2A | 0x80, &omega_y8[0], 2);   //data_len in byte
    IMU_comunication.read_buffer(0xD6, 0x2C | 0x80, &omega_z8[0], 2);   //data_len in byte
  	
    //Composizione valori a 8 bit in una da 16 bit (Velocità angolare 16 bit)
    m_omega_x16 = omega_x8[0] | (omega_x8[1]<<8);
    m_omega_y16 = omega_y8[0] | (omega_y8[1]<<8);
    m_omega_z16 = omega_z8[0] | (omega_z8[1]<<8);

    //Conversione in base al fondo scala (Vel_Angolari in deg)
    OMEGA_X = m_omega_x16*FULLSCALE_245dps - OFFSET_X;
    OMEGA_Y = m_omega_y16*FULLSCALE_245dps - OFFSET_Y;
    OMEGA_Z = m_omega_z16*FULLSCALE_245dps - OFFSET_Z;
}

float Controllore::Controller_PI(float Input)
{
    //Calcolo uscita proporzionale
    float OUT_P = Kp*Input;

    //Calcolo uscita integrale
    #ifdef USE_ANTIWINDUP
	if(!SAT_FLAG)
	{
	     OUT_I = OUT_I + Ki*(TIME_UNIT)*Input;
	}
	#else
	OUT_I = OUT_I + Ki*(TIME_UNIT)*Input;
	#endif // USE_ANTIWINDUP
	
    //Calcolo uscita PI
    float OUT_PI = OUT_P + OUT_I;
	
	//Saturazione
	if(OUT_PI < -SAT)
	{
	    OUT_PI = -SAT;
	    SAT_FLAG = true;
	}
	else if(OUT_PI > SAT)
	{
	    OUT_PI = SAT;
            SAT_FLAG = true;
	}
	else
	{
	    SAT_FLAG = false;
	}

	return OUT_PI;
}

void Controllore::SPEED_CONTROL()
{
    //Calcolo velocità delle ruote in base al riferimento V e all'uscita del controllore PI
    float Errore = -OMEGA_X;
    float Uscita_PI = Controller_PI(Errore);
    V_Left = V_lineare - Uscita_PI;
    V_Right = V_lineare + Uscita_PI;

    //Invio delle velocità controllate alla classe speed_control
    m_Controllo_Vel.set_targets(V_Left,V_Right);
}

void Controllore::run()
{
    ReadGyro();
    SPEED_CONTROL();
}
