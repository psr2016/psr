# Ottiene il percorso di questa cartella
PERLIBDIR := $(dir $(lastword $(MAKEFILE_LIST)))

CFLAGS += -I$(PERLIBDIR)
SOURCEFILES += $(PERLIBDIR)/i2c.cpp
SOURCEFILES += $(PERLIBDIR)/serial.cpp
