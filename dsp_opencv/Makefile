# Includes your Rules.make from the DVSDK directory
include Rules.make

# Location of TI Code Generation Tools if not provided in Rules.make
ifndef CODEGEN_INSTALL_DIR
    CODEGEN_INSTALL_DIR = /media/disk/openembedded/build/tmp-angstrom_2008_1/sysroots/beagleboard-angstrom-linux-gnueabi/usr/share/ti/ti-cgt6x-tree
endif



ifndef CROSS_COMPILE
    CROSS_COMPILE = /media/disk/openembedded/build/tmp-angstrom_2008_1/sysroots/i686-linux/usr/bin/arm-angstrom-linux-gnueabi- 
endif

ifeq ($(PLATFORM),omap3530)
    XDC_PLATFORM = ti.platforms.evm3530
    XDC_TARGETS = "gnu.targets.arm.GCArmv5T ti.targets.C64P"
endif

# Location of IUNIVERSAL Examples
PACKAGE_DIR = $(shell pwd)
SERVER_DIR = $(PACKAGE_DIR)/beagle/c6accel_1_00_00_04_async/soc/packages

export SERVER_DIR


XDCPATH = ^;$(CURDIR);$(PACKAGE_DIR);$(SERVER_DIR);$(CE_INSTALL_DIR)/packages;$(BIOS_INSTALL_DIR)/packages;$(CODEC_INSTALL_DIR);$(FC_INSTALL_DIR)/packages;$(LINK_INSTALL_DIR);$(XDAIS_INSTALL_DIR)/packages;$(CMEM_INSTALL_DIR)/packages;$(BIOSUTILS_INSTALL_DIR)/packages;$(LPM_INSTALL_DIR)/packages


# Set the number of concurrent build threads (i.e. jobs)
JOBS = 1

# Conditionally set the XDCOPTIONS
XDCOPTIONS=v

########################################################
## Shouldn't have to modify anything be low this line ##
########################################################

# Currently only Linux apps are supported thus the app extension is always the same
APP_EXT = v5T

# Parse XDC_TARGETS to find the ARM and DSP targets
XDC_ARM_TARGET = $(shell echo $(XDC_TARGETS) | cut -d' ' -f1)
XDC_DSP_TARGET = $(shell echo $(XDC_TARGETS) | cut -d' ' -f2)

# Export environment variables needed by config.bld and Makefile.app
export CODEGEN_INSTALL_DIR
export XDC_INSTALL_DIR
export XDC_PLATFORM
export XDC_TARGETS
export XDCPATH
export XDCOPTIONS
export XDCARGS
export PACKAGE_DIR
export PLATFORM_SHORT
export CROSS_COMPILE
export XDC_ARM_TARGET
export XDC_DSP_TARGET
export APP_EXT


# Set the location of the xdc executable
XDC = $(XDC_INSTALL_DIR)/xdc XDCOPTIONS=$(XDCOPTIONS) XDCTARGETS=$(XDC_TARGETS)

# Get the suffix of the XDC_PLATFORM to identify appropriate paths
PLATFORM_SHORT = $(patsubst .%,%,$(suffix $(XDC_PLATFORM)))

ifeq ($(PLATFORM_SHORT),"")
$(error Failed to get short platform name from $(XDC_PLATFORM) $(PLATFORM_SHORT))
endif

# Search for the server path with all lower case letters
PLATFORM_SHORT_LC = $(shell echo $(PLATFORM_SHORT) | tr [:upper:] [:lower:])
SERVER_PATH = $(wildcard beagle/server/*$(PLATFORM_SHORT_LC))
SERVER_PATH_SHORT  = $(shell echo $(SERVER_PATH) | cut -d' ' -f1)

C6ACCEL_PATH = beagle/c6accel_1_00_00_04_async

ifeq ($(SERVER_PATH),"")
$(error No servers found with $(SERVER_EXTENSION) extension)
endif

# Vars needed to build ARM Linux library
HEADERS = $(wildcard beagle/library/opencv/*.h)
SOURCES = $(wildcard beagle/library/opencv/*.c)
OBJFILES = $(SOURCES:%.c=%.ov5T)
TARGET = beagle/library/opencv/lib/libopencvdsp.so
GNU_C_FLAGS = $(C_FLAGS) $(CFLAGS) -Wall -Dxdc_target_name__=MVArm9 -I$(XDC_INSTALL_DIR)/packages -I$(CE_INSTALL_DIR)/packages -I$(XDAIS_INSTALL_DIR)/packages -I$(BIOS_INSTALL_DIR)/packages -I$(FC_INSTALL_DIR)/packages -I$(LINK_INSTALL_DIR)/packages -I$(FC_INSTALL_DIR)/packages -I$(BIOSUTILS_INSTALL_DIR)/packages -I$(LPM_INSTAL_DIR)/packages -I$(LINUXKERNEL_INSTALL_DIR)/include -I$(BIOS_INSTALL_DIR)/packages/ti/bios/include -I$(DMAI_INSTALL_DIR)/packages -I$(CURDIR) -I$(SERVER_DIR) -Dxdc_target_types__=gnu/targets/std.h -D_$(PLATFORM)
GNU_AR_FLAGS = $(AR_FLAGS)
GNU_LD_FLAGS = $(LD_FLAGS) $(LDFLAGS)
COMPILE = $(PRE) $(CROSS_COMPILE)g++ $(GNU_C_FLAGS) $(GNU_CPP_FLAGS) -c
AR = $(PRE) $(CROSS_COMPILE)ar rc $(GNU_AR_FLAGS)
LD = $(PRE) $(CROSS_COMPILE)g++ $(GNU_LD_FLAGS)

.PHONY: clean clean_app clean_lib clean_c6accel library c6accel app install all

help:
	@echo 
	@echo "You must specify a build target. Available targets are: "
	@echo "    all, c6accel, library, app, install, clean"

all: c6accel library app install

c6accel:
	@echo "$(C6ACCEL_PATH)"
	make -C $(C6ACCEL_PATH) all

$(OBJFILES):	%.ov5T: %.c $(HEADERS)
	@echo $(shell pwd)
	@echo Compiling $@ from $<
	@mkdir -p $(dir $@)
	$(COMPILE) $(CPP_FLAGS) -o $@ $<

$(TARGET): $(OBJFILES)
	@echo
	@echo Building Shared Library $@ from $^..
	@mkdir -p $(dir $@)
	$(LD) -shared  -o $@ $^

package/package.defs.h:beagle/library/opencv/package.xdc beagle/library/opencv/package.xs beagle/library/opencv/package.bld
	@echo
	@echo Building package interface for $(TARGET)
	$(XDC) XDCBUILDCFG=$(PACKAGE_DIR)/config.bld -PR beagle/library

library: $(TARGET) package/package.defs.h

app: opencv

opencv: $(if $(wildcard beagle/apps/opencv/linux/remote_ti_platforms_$(PLATFORM_SHORT).cfg), opencv_remote)

opencv_remote:
	make -C beagle/apps/opencv -f $(PACKAGE_DIR)/Makefile.app TARGET=linux/remote_ti_platforms_$(PLATFORM_SHORT) APP_NAME=opencv PLATFORM=$(PLATFORM) PACKAGE_DIR=$(PACKAGE_DIR) SERVER_PATH=$(SERVER_PATH)

install:
	@echo "Installing at $(EXEC_DIR)"
	@install -d $(EXEC_DIR)
	@install -m 777 $(SERVER_DIR)/ti/c6accel_unitservers/omap3530/c6accel_omap3530.x64P $(EXEC_DIR)
	@install -m 777 beagle/apps/opencv/linux/remote_ti_platforms_$(PLATFORM_SHORT)_opencv.x$(APP_EXT) $(EXEC_DIR)
	@install -d $(EXEC_DIR)/lib
	@install -m 777 beagle/library/opencv/lib/* $(EXEC_DIR)/lib
	@install -d $(EXEC_DIR)/test_Data
	@install -m 777 beagle/apps/opencv/test_data/* $(EXEC_DIR)
	@chmod -x $(EXEC_DIR)/*.x64P
	@echo done
	@echo

clean_app:
	make -C beagle/apps/opencv APP_NAME=opencv -f $(PACKAGE_DIR)/Makefile.app TARGET=linux/remote_ti_platforms_$(PLATFORM_SHORT) clean
	make -C beagle/apps/opencv APP_NAME=opencv -f $(PACKAGE_DIR)/Makefile.app TARGET=linux/local_ti_platforms_$(PLATFORM_SHORT) clean

clean_c6accel:
	make -C $(C6ACCEL_PATH) clean

clean_lib:
	rm -rf beagle/library/opencv/*.ov5T
	rm -rf beagle/library/opencv/lib/*
	rm -rf beagle/apps/opencv/lib/*
	
clean: clean_c6accel clean_app clean_lib
