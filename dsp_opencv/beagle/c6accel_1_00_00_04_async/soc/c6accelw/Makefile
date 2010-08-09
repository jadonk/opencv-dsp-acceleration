# Makefile
#
# ============================================================================
# Copyright (c) Texas Instruments Inc 2009
#
# Use of this software is controlled by the terms and conditions found in the
# license agreement under which this software has been supplied or provided.
# ============================================================================

ROOTDIR = ../..
TARGET = $(notdir $(CURDIR))

include $(ROOTDIR)/Rules.make

# Comment this out if you want to see full compiler and linker output.
VERBOSE = @

# Which compiler flags should be used?
BUILD_TYPE=release

CPP_FLAGS += -I../packages -I$(XDC_INSTALL_DIR)/packages -I$(CE_INSTALL_DIR)/packages -I$(XDAIS_INSTALL_DIR)/packages -Dxdc_target_name__=arm/GCArmv5T -Dxdc_target_types__=gnu/targets/std.h

ifeq ($(BUILD_TYPE), release)
ifeq ($(ARM_ISA),armv7-a)
    C_FLAGS	+= -O3 -march=armv7-a -mtune=cortex-a8 -mfpu=neon -ftree-vectorize -mfloat-abi=softfp
 endif
ifeq ($(ARM_ISA),armv5t)
    C_FLAGS	+= -mlittle-endian -march=armv5t -mtune=arm9tdmi -mabi=aapcs-linux -O
 endif

else
    CPP_FLAGS	+= -DNDEBUG
    C_FLAGS	+= -Wall -g
endif

LD_FLAGS += -L$(LINUXLIBS_INSTALL_DIR)/lib
AR_FLAGS += 

COMPILE.c = $(VERBOSE) $(CSTOOL_PREFIX)gcc $(CPP_FLAGS) $(C_FLAGS) -c
LINK.c = $(VERBOSE) $(CSTOOL_PREFIX)gcc $(LD_FLAGS)
CS_AR.c = $(VERBOSE) $(CSTOOL_PREFIX)ar rc $(AR_FLAGS)

SOURCES = $(wildcard *.c)
HEADERS = $(wildcard *.h)

OBJFILES = $(SOURCES:%.c=%.o)

TARGET_AR = lib/$(TARGET)_$(PLATFORM).a470MV

.PHONY: clean install 

all:	$(TARGET_AR)

$(TARGET_AR):	$(OBJFILES) 
	@echo
	@echo Archiving $@ from $^..
	$(CS_AR.c) $@ $^

$(OBJFILES):	%.o: %.c $(HEADERS)
	@echo Compiling $@ from $<..
	$(COMPILE.c) -o $@ $<

clean:
	@echo Removing generated files..
	$(VERBOSE) -$(RM) -rf $(OBJFILES) lib/* *~ *.d .dep
