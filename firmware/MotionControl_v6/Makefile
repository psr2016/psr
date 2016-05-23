# Project settings
BUILDDIR := build/
OUTPUT_FILE := MotionControl_v6
TARGET_CHIP := 33FJ128MC802

# Robot selection
CAN_STDIO_AND_RTSP_NODE_ID := 4
CAN_RTSPPORT := 6661

SOURCEFILES += geometry.cpp
SOURCEFILES += kinematics.cpp
SOURCEFILES += speed_control.cpp
SOURCEFILES += followLine.cpp
SOURCEFILES += clocking.cpp
SOURCEFILES += pwm.cpp
SOURCEFILES += timers.cpp
SOURCEFILES += qei.cpp
SOURCEFILES += can_interface.cpp
SOURCEFILES += relative_rotation.cpp
SOURCEFILES += circ_rotation.cpp
SOURCEFILES += main.cpp


# Librerie utilizzate
include ../libs/can_stdio_and_rtsp/can_stdio_and_rtsp_endpoint.mk
include ../libs/ecan_lib/ecan_lib.mk
include ../libs/controllib/controllib.mk
include ../libs/c++support/c++support.mk

# Carica regole di compilazione. Deve essere incluso dopo aver riempito l'elenco SOURCEFILES
include ../rules.mk
