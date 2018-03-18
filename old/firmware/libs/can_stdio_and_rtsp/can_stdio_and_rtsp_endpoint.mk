# Ottiene il percorso di questa cartella
CAN_STDIO_AND_RTSP_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

# IP e porta del servizio XBeeRtsp da contattare
# Seleziona 127.0.0.1 se xbeemux è in esecuzione sulla macchina locale,
# altrimenti seleziona l'IP del Raspberry
RASPBERRYIP := 192.168.70.102
RTSPSERVER = $(shell pidof xbeemux > /dev/null && echo '127.0.0.1' || echo '$(RASPBERRYIP)')

ifndef CAN_STDIO_AND_RTSP_NODE_ID
$(error Impostare CAN_STDIO_AND_RTSP_NODE_ID in Makefile)
endif

ifndef CAN_RTSPPORT
$(error Impostare CAN_RTSPPORT in Makefile)
endif

RTSPCMD := $(CAN_STDIO_AND_RTSP_DIR)../../RaspberryPi/xbee-tools/rtspcmd
RTSPCMDFLAGS = -d dsPIC$(TARGET_CHIP) -p $(RTSPSERVER):$(CAN_RTSPPORT) -n $(CAN_STDIO_AND_RTSP_NODE_ID)

CFLAGS += -I$(CAN_STDIO_AND_RTSP_DIR)
CFLAGS += -DCAN_STDIO_AND_RTSP_NODE_ID=$(CAN_STDIO_AND_RTSP_NODE_ID)
override LDSCRIPT = $(CAN_STDIO_AND_RTSP_DIR)/p$(TARGET_CHIP)-modified.gld

SOURCEFILES += $(CAN_STDIO_AND_RTSP_DIR)/canstdio_endpoint.c
SOURCEFILES += $(CAN_STDIO_AND_RTSP_DIR)/canrtsp_endpoint.c

xprogram: all
	@echo "=> Programming $(HEX_FILE) over xbee..."
	@$(RTSPCMD) $(RTSPCMDFLAGS) -wvr $(BUILDDIR)/$(HEX_FILE)

xverify: all
	@$(RTSPCMD) $(RTSPCMDFLAGS) -vr $(BUILDDIR)/$(HEX_FILE)

xreset:
	@$(RTSPCMD) $(RTSPCMDFLAGS) -r
