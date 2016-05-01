# Ottiene il percorso di questa cartella
CONTROLLIBDIR := $(dir $(lastword $(MAKEFILE_LIST)))

CFLAGS += -I$(CONTROLLIBDIR)
SOURCEFILES += $(CONTROLLIBDIR)/periodic_task.cpp
SOURCEFILES += $(CONTROLLIBDIR)/pid.cpp
SOURCEFILES += $(CONTROLLIBDIR)/chebyshev.cpp
SOURCEFILES += $(CONTROLLIBDIR)/low_pass.cpp
