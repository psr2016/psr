# Ottiene il percorso di questa cartella
CXXSUPPORTDIR := $(dir $(lastword $(MAKEFILE_LIST)))

CFLAGS += -I$(CXXSUPPORTDIR)
SOURCEFILES += $(CXXSUPPORTDIR)/minilibstdc++.cpp
