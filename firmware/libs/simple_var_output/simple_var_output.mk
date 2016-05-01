# Ottiene il percorso di questa cartella
SIMPLEVARDIR := $(dir $(lastword $(MAKEFILE_LIST)))

CFLAGS += -I$(SIMPLEVARDIR)
SOURCEFILES += $(SIMPLEVARDIR)/simple_var_output.c
