ROOTDIR = ../..
TARGET = c6accel_app

include $(ROOTDIR)/Rules.make

# Comment this out if you want to see full compiler and linker output.
#VERBOSE = @

# Which compiler flags should be used?
BUILD_TYPE=release

# Package path for the XDC tools
XDCPATH = $(USER_XDC_PATH);../packages;$(CE_INSTALL_DIR)/packages;$(FC_INSTALL_DIR)/packages;$(LINK_INSTALL_DIR)/packages;$(LINK_INSTALL_DIR);$(XDAIS_INSTALL_DIR)/packages;$(CMEM_INSTALL_DIR)/packages;$(C6ACCEL_INSTALL_DIR)/packages;$(LPM_INSTALL_DIR)/packages;$(SERVER_INSTALL_DIR)/packages

# Where to output configuration files
XDC_CFG		= $(TARGET)_config

# Output compiler options
XDC_CFLAGS	= $(XDC_CFG)/compiler.opt

# Output linker file
XDC_LFILE	= $(XDC_CFG)/linker.cmd

# Input configuration file
XDC_CFGFILE	= $(PLATFORM)/$(TARGET).cfg

# Target tools
XDC_TARGET	= gnu.targets.arm.GCArmv5T

CROSS_COMPILE	= $(CSTOOL_PREFIX)

export CROSS_COMPILE
export CODEGEN_INSTALL_DIR
export XDCPATH
export PLATFORM_XDC

# The XDC configuration tool command line
CONFIGURO = $(XDC_INSTALL_DIR)/xs xdc.tools.configuro
CONFIG_BLD = ../../config.bld

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


ifeq ($(PLATFORM),omap3530)
    C_FLAGS	+= -DPLATFORM=3530
endif
ifeq ($(PLATFORM),omapl138)
    C_FLAGS	+= -DPLATFORM=138
endif

LD_FLAGS += -L$(LINUXLIBS_INSTALL_DIR)/lib -lm -lpthread
C6ACCEL_LIB += ../c6accelw/lib/c6accelw_$(PLATFORM).a470MV

COMPILE.c = $(VERBOSE) $(CSTOOL_PREFIX)gcc $(CPP_FLAGS) $(C_FLAGS) $(CPP_FLAGS) -c
LINK.c = $(VERBOSE) $(CSTOOL_PREFIX)gcc $(LD_FLAGS)

SOURCES = $(wildcard *.c)
HEADERS = $(wildcard *.h)
OBJFILES = $(SOURCES:%.c=%.o)

.PHONY: clean install 

all:	$(TARGET)

install:	$(if $(wildcard $(TARGET)), install_$(TARGET))

install_$(TARGET):
	@install -d $(EXEC_DIR_C6ACCEL)
	@install $(TARGET) $(EXEC_DIR_C6ACCEL)
	@cp -R test_files $(EXEC_DIR_C6ACCEL)
	@echo
	@echo Installed $(TARGET) binaries to $(EXEC_DIR_C6ACCEL)..

$(TARGET):	$(OBJFILES) $(C6ACCEL_LIB) $(XDC_LFILE)
	@echo
	@echo Linking $@ from $^..
	$(LINK.c) -o $@ $^ 

$(OBJFILES):	%.o: %.c $(HEADERS) $(XDC_CFLAGS)
	@echo Compiling $@ from $<..
	$(COMPILE.c) $(shell cat $(XDC_CFLAGS)) -o $@ $<

$(XDC_LFILE) $(XDC_CFLAGS):	$(XDC_CFGFILE)
	@echo
	@echo ======== Building $(TARGET) ========
	@echo Configuring application using $<
	@echo
	$(VERBOSE) $(CONFIGURO) -o $(XDC_CFG) -t $(XDC_TARGET) -p $(PLATFORM_XDC) -b $(CONFIG_BLD) $(XDC_CFGFILE)

clean:
	@echo Removing generated files..
	$(VERBOSE) -$(RM) -rf $(XDC_CFG) $(OBJFILES) $(TARGET) *~ *.d .dep
