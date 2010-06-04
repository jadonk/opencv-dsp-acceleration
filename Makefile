# Includes your Rules.make from the DVSDK directory
include ../Rules.make

# Location of TI Code Generation Tools if not provided in Rules.make
ifndef CODEGEN_INSTALL_DIR
    #CODEGEN_INSTALL_DIR = /home/a0270555/c60_6_1_7
    CODEGEN_INSTALL_DIR = /media/disk/OE/work/ti/ti-cgt6x-tree
endif




ifeq ($(PLATFORM),omap3530)
    XDC_PLATFORM = ti.platforms.evm3530
    XDC_TARGETS = "gnu.targets.arm.GCArmv5T ti.targets.C64P"
#     XDC_TARGETS = "ti.targets.C64P"
    CROSS_COMPILE = /media/disk/OE/build/tmp-angstrom_2008_1/cross/armv7a/bin/arm-angstrom-linux-gnueabi-

endif

# Location of IUNIVERSAL Examples
PACKAGE_DIR = $(shell pwd)

DSPLIB_PATH = /media/disk/OE/work/ti/dsplib_v210

XDCPATH = ^;$(CURDIR)/packages;$(PACKAGE_DIR);$(CE_INSTALL_DIR)/packages;$(BIOS_INSTALL_DIR)/packages;$(CODEC_INSTALL_DIR);$(FC_INSTALL_DIR)/packages;$(LINK_INSTALL_DIR);$(XDAIS_INSTALL_DIR)/packages;$(CMEM_INSTALL_DIR)/packages;$(BIOSUTILS_INSTALL_DIR)/packages;$(LPM_INSTALL_DIR)/packages;$(DSPLIB_PATH)

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
SERVER_PATH = $(wildcard gsoc/algos/*$(PLATFORM_SHORT_LC))
SERVER_PATH_SHORT  = $(shell echo $(SERVER_PATH) | cut -d' ' -f1)

ifeq ($(SERVER_PATH),"")
$(error No servers found with $(SERVER_EXTENSION) extension)
endif

.PHONY: clean clean_apps codecs servers apps cvDFT install all

help:
	@echo 
	@echo "You must specify a build target. Available targets are: "
	@echo "    all, codecs, servers, apps, fir, install, clean"

all: codecs servers apps

codecs:
	$(XDC) XDCBUILDCFG=$(PACKAGE_DIR)/config.bld -PR gsoc/algos/cvDFT

servers:
	$(XDC) XDCBUILDCFG=$(PACKAGE_DIR)/config.bld -P $(SERVER_PATH)

apps: cvDFT

cvDFT: $(if $(wildcard gsoc/apps/cvDFT/linux/remote_ti_platforms_$(PLATFORM_SHORT).cfg), cvDFT_remote) $(if $(wildcard gsoc/apps/cvDFT/linux/local_ti_platforms_$(PLATFORM_SHORT).cfg), cvDFT_local)

cvDFT_remote:
	make -C gsoc/apps/cvDFT -f $(PACKAGE_DIR)/Makefile.app TARGET=linux/remote_ti_platforms_$(PLATFORM_SHORT) APP_NAME=cvDFT

cvDFT_local:
	make -C gsoc/apps/cvDFT -f $(PACKAGE_DIR)/Makefile.app TARGET=linux/local_ti_platforms_$(PLATFORM_SHORT) APP_NAME=cvDFT

install: install_cvDFT 

install_cvDFT: $(if $(wildcard gsoc/apps/cvDFT/linux/local_ti_platforms_$(PLATFORM_SHORT)_cvDFT.x$(APP_EXT)), install_cvDFT_local) $(if $(wildcard gsoc/apps/cvDFT/linux/remote_ti_platforms_$(PLATFORM_SHORT)_cvDFT.x$(APP_EXT)), install_cvDFT_remote)

install_cvDFT_local:
	@install -d $(EXEC_DIR)
	@install -m 777 gsoc/apps/cvDFT/linux/local_ti_platforms_$(PLATFORM_SHORT)_fir.x$(APP_EXT) $(EXEC_DIR)

install_cvDFT_remote:
	@install -d $(EXEC_DIR)
	@install -m 777 gsoc/apps/cvDFT/linux/remote_ti_platforms_$(PLATFORM_SHORT)_cvDFT.x$(APP_EXT) $(EXEC_DIR)
	@install -m 777 $(wildcard $(SERVER_PATH_SHORT)/../cvDFT_unitserver_$(PLATFORM_SHORT_LC)/cvDFT_unitserver_$(PLATFORM_SHORT_LC).x*) $(EXEC_DIR)

clean_apps:
	make -C gsoc/apps/cvDFT APP_NAME=cvDFT -f $(PACKAGE_DIR)/Makefile.app TARGET=linux/remote_ti_platforms_$(PLATFORM_SHORT) clean
	make -C gsoc/apps/cvDFT APP_NAME=cvDFT -f $(PACKAGE_DIR)/Makefile.app TARGET=linux/local_ti_platforms_$(PLATFORM_SHORT) clean
	

clean: clean_apps
	$(XDC) clean -PR gsoc
