# Ottiene il percorso di questa cartella
ECANLIBDIR := $(dir $(lastword $(MAKEFILE_LIST)))

CFLAGS += -I$(ECANLIBDIR)
SOURCEFILES += $(ECANLIBDIR)/ecan_config.c
SOURCEFILES += $(ECANLIBDIR)/ecan_driver.c
SOURCEFILES += $(ECANLIBDIR)/ecan_lib.c
