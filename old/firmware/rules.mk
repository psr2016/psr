# Regole di compilazione per i firmware dsPIC
# Questo file viene incluso dai Makefile nelle sottodirectory di ciascun firmware
ifeq ($(TARGET_CHIP),33FJ128MC802)
TARGET_FAMILY := dsPIC33F
else ifeq ($(TARGET_CHIP),33EP512MC502)
TARGET_FAMILY := dsPIC33E
else
$(error TARGET_CHIP must be 33FJ128MC802 or 33EP512MC502)
endif

# Ottiene il percorso di questa cartella
ROOTDIR := $(dir $(lastword $(MAKEFILE_LIST)))

# Ottiene il percorso degli eseguibili della toolchain
include $(ROOTDIR)/compile.mk
BIN := $(XC16DIR)/bin
LIB := $(XC16DIR)/lib
CC := $(BIN)/xc16-gcc
CXX := $(BIN)/xc16-g++
LD := $(BIN)/xc16-ld
BIN2HEX := $(BIN)/xc16-bin2hex
DIS := $(BIN)/xc16-objdump
PK2 := $(PK2DIR)/pk2cmd
PK2CMD := $(PK2)/pk2cmd

CFLAGS += -omf=coff -Wall -mcpu=$(TARGET_CHIP) -mno-eds-warn -I. -I$(ROOTDIR) -no-legacy-libc
CXXFLAGS += $(CFLAGS) -fno-exceptions -fno-rtti -D__bool_true_and_false_are_defined
LDSCRIPT = $(XC16DIR)/support/$(TARGET_FAMILY)/gld/p$(TARGET_CHIP).gld
LDFLAGS += -omf=coff --local-stack -p$(TARGET_CHIP) --script $(LDSCRIPT) --report-mem --heap=512 -L$(LIB) -L$(LIB)/$(TARGET_FAMILY)
LIBS +=  -lc -lpic30 -ldsp -lfastm -lq-dsp -lq -lsol -lm  #  -lp$(TARGET_CHIP)

# Attivare per info dettagliate su memoria allocata da ciascun file sorgente
# CFLAGS += -Wa,-ai

# Funzione che restituisce percorso relativo a $(ROOTDIR) dei file passati come parametro
relpath = $(subst $(abspath $(ROOTDIR))/,,$(abspath $(1)))

# Path di ricerca dei file sorgenti
VPATH := $(ROOTDIR)

# Build files
OBJECTFILES = $(addprefix $(BUILDDIR)/,$(call relpath,$(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCEFILES)))))
DEP_FILES = $(OBJECTFILES:%.o=%.deps)
COF_FILE = $(OUTPUT_FILE).cof
HEX_FILE = $(OUTPUT_FILE).hex
MAP_FILE = $(OUTPUT_FILE).map

.DEFAULT_GOAL := all
.PHONY: all program dis check clean

all: $(BUILDDIR)/$(HEX_FILE)

# Carica lista delle dipendenze di ciascun file .c (elenco dei file .h #inclusi)
# Fallisce silenziosamente se tale lista non esiste (avviene alla prima compilazione)
-include $(DEP_FILES)

# Build rules
$(BUILDDIR)/%.o: %.c $(REQUIRED_TARGETS)
	@echo "=> Compiling $(call relpath,$<)..."
	@mkdir -p $(dir $@)
	@$(CC) -std=gnu99 $(CFLAGS) -MF $(BUILDDIR)/$*.deps -MMD -MP -MQ $@ -c $< -o $@

$(BUILDDIR)/%.o: %.cpp $(REQUIRED_TARGETS)
	@echo "=> Compiling $(call relpath,$<)..."
	@mkdir -p $(dir $@)
	@$(CXX) -std=gnu++0x $(CXXFLAGS) -MF $(BUILDDIR)/$*.deps -MMD -MP -MQ $@ -c $< -o $@

$(BUILDDIR)/$(COF_FILE) $(BUILDDIR)/$(HEX_FILE) $(BUILDDIR)/$(MAP_FILE): $(OBJECTFILES)
	@echo "=> Linking $(HEX_FILE)..."
	@$(LD) $(LDFLAGS) $^ $(LIBS) \
		-Map=$(BUILDDIR)/$(MAP_FILE) --cref --save-gld=$(BUILDDIR)/ldscript -o$(BUILDDIR)/$(COF_FILE)
	@$(BIN2HEX) -omf=coff $(BUILDDIR)/$(COF_FILE)

program: $(BUILDDIR)/$(HEX_FILE)
	@echo "=> Programming $(HEX_FILE) over pk2..."
	sudo $(PK2CMD) -B$(PK2) -PdsPIC$(TARGET_CHIP) -F$(BUILDDIR)/$(HEX_FILE) -M -R

reset: $(BUILDDIR)/$(HEX_FILE)
	sudo $(PK2CMD) -B$(PK2) -PdsPIC$(TARGET_CHIP) -R

dis: $(BUILDDIR)/$(COF_FILE)
	@$(DIS) -omf=coff -CD $(BUILDDIR)/$(COF_FILE)

check:
	sudo $(PK2CMD) -B$(PK2) -I -P

clean:
	@echo "=> Cleaning build files..."
	@-$(RM) -rf $(BUILDDIR)/*
