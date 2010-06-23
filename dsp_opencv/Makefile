# Includes your Rules.make from the DVSDK directory
include Rules.make

# Location of TI Code Generation Tools if not provided in Rules.make
ifndef CODEGEN_INSTALL_DIR
    #CODEGEN_INSTALL_DIR = /home/a0270555/c60_6_1_7
    CODEGEN_INSTALL_DIR = /media/disk/OE/work/ti/ti-cgt6x-tree
endif




ifeq ($(PLATFORM),omap3530)
    XDC_PLATFORM = ti.platforms.evm3530
    XDC_TARGETS = "gnu.targets.arm.GCArmv5T ti.targets.C64P"
#     XDC_TARGETS = "ti.targets.C64P"
#    CROSS_COMPILE = /media/disk/OE/build/tmp-angstrom_2008_1/cross/armv7a/bin/arm-angstrom-linux-gnueabi-
    CROSS_COMPILE = /media/disk/openembedded/build/tmp-angstrom_2008_1/cross/armv7a/bin/arm-angstrom-linux-gnueabi-
endif

# Location of IUNIVERSAL Examples
PACKAGE_DIR = $(shell pwd)


XDCPATH = ^;$(CURDIR);$(PACKAGE_DIR);$(CE_INSTALL_DIR)/packages;$(BIOS_INSTALL_DIR)/packages;$(CODEC_INSTALL_DIR);$(FC_INSTALL_DIR)/packages;$(LINK_INSTALL_DIR);$(XDAIS_INSTALL_DIR)/packages;$(CMEM_INSTALL_DIR)/packages;$(BIOSUTILS_INSTALL_DIR)/packages;$(LPM_INSTALL_DIR)/packages;$(DSPLIB_PATH);$(IMGLIB_PATH)

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
export DSPLIB_PATH
export IMGLIB_PATH

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

ifeq ($(SERVER_PATH),"")
$(error No servers found with $(SERVER_EXTENSION) extension)
endif

# Vars needed to build ARM Linux library
HEADERS = $(wildcard beagle/library/opencv/*.h)
SOURCES = $(wildcard beagle/library/opencv/*.c)
OBJFILES = $(SOURCES:%.c=%.ov5T)
TARGET = beagle/library/opencv/lib/libopencv.a
GNU_C_FLAGS = $(C_FLAGS) -Wall -Dxdc_target_name__=MVArm9 -I$(XDC_INSTALL_DIR)/packages -I$(CE_INSTALL_DIR)/packages -I$(XDAIS_INSTALL_DIR)/packages -I$(BIOS_INSTALL_DIR)/packages -I$(FC_INSTALL_DIR)/packages -I$(LINK_INSTALL_DIR)/packages -I$(FC_INSTALL_DIR)/packages -I$(BIOSUTILS_INSTALL_DIR)/packages -I$(LPM_INSTAL_DIR)/packages -I$(LINUXKERNEL_INSTALL_DIR)/include -I$(BIOS_INSTALL_DIR)/packages/ti/bios/include -I$(DMAI_INSTALL_DIR)/packages -I$(CURDIR) -Dxdc_target_types__=gnu/targets/std.h -D_$(PLATFORM)
GNU_AR_FLAGS = $(AR_FLAGS)
COMPILE = $(PRE) $(CROSS_COMPILE)gcc $(GNU_C_FLAGS) $(GNU_CPP_FLAGS) -c
AR = $(PRE) $(CROSS_COMPILE)ar rc $(GNU_AR_FLAGS)

.PHONY: clean clean_app clean_lib codec server library app install all

help:
	@echo 
	@echo "You must specify a build target. Available targets are: "
	@echo "    all, codec, server, library, app, install, clean"

all: codec server library app

codec:
	@echo "$(XDCPATH)"
	$(XDC) XDCBUILDCFG=$(PACKAGE_DIR)/config.bld -PR beagle/algos/opencv

server:
	@echo "$(SERVER_PATH)"
	$(XDC) XDCBUILDCFG=$(PACKAGE_DIR)/config.bld -P $(SERVER_PATH)

$(OBJFILES):	%.ov5T: %.c $(HEADERS)
	@echo $(shell pwd)
	@echo Compiling $@ from $<
	@mkdir -p $(dir $@)
	$(COMPILE) $(CPP_FLAGS) -o $@ $<

$(TARGET):	$(OBJFILES)
	@echo
	@echo Archiving $@ from $^..
	@mkdir -p $(dir $@)
	$(AR) $@ $^

package/package.defs.h:beagle/library/opencv/package.xdc beagle/library/opencv/package.xs beagle/library/opencv/package.bld
	@echo
	@echo Building package interface for $(TARGET)
	$(XDC) XDCBUILDCFG=$(PACKAGE_DIR)/config.bld -PR beagle/library

library:	$(TARGET) package/package.defs.h

app: opencv

opencv: $(if $(wildcard beagle/apps/opencv/linux/remote_ti_platforms_$(PLATFORM_SHORT).cfg), opencv_remote)

opencv_remote:
	make -C beagle/apps/opencv -f $(PACKAGE_DIR)/Makefile.app TARGET=linux/remote_ti_platforms_$(PLATFORM_SHORT) APP_NAME=opencv PLATFORM=$(PLATFORM) PACKAGE_DIR=$(PACKAGE_DIR) SERVER_PATH=$(SERVER_PATH)

install: install_opencv 

install_opencv: $(if $(wildcard beagle/apps/opencv/linux/remote_ti_platforms_$(PLATFORM_SHORT)_opencv.x$(APP_EXT)), install_opencv_remote)

install_opencv_remote:
	@echo "Installing at $(EXEC_DIR)"
	@install -d $(EXEC_DIR)
	@install -m 777 beagle/apps/opencv/linux/remote_ti_platforms_$(PLATFORM_SHORT)_opencv.x$(APP_EXT) $(EXEC_DIR)
	@install -m 777 $(wildcard $(SERVER_PATH_SHORT)/../opencv_unitserver_$(PLATFORM_SHORT_LC)/opencv_unitserver_$(PLATFORM_SHORT_LC).x*) $(EXEC_DIR)
	@install -d $(EXEC_DIR)/test_Data
	@install -m 777 beagle/apps/opencv/test_data/* $(EXEC_DIR)/test_Data
	@chmod -x $(EXEC_DIR)/*.x64P
	@echo done
	@echo

clean_app:
	make -C beagle/apps/opencv APP_NAME=opencv -f $(PACKAGE_DIR)/Makefile.app TARGET=linux/remote_ti_platforms_$(PLATFORM_SHORT) clean
	make -C beagle/apps/opencv APP_NAME=opencv -f $(PACKAGE_DIR)/Makefile.app TARGET=linux/local_ti_platforms_$(PLATFORM_SHORT) clean

clean_lib:
	$(XDC) clean -PR beagle/library
	rm -rf beagle/library/opencv/*.ov5T
	rm -rf beagle/library/opencv/lib/*
	

clean: clean_app clean_lib
	$(XDC) clean -PR beagle
