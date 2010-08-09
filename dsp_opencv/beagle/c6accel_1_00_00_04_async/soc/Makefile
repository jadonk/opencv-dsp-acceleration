#
#  ======== Makefile ========
#

include ../Rules.make

XDCPATH = $(CURDIR);packages;$(XDAIS_INSTALL_DIR)/packages;$(FC_INSTALL_DIR)/packages;$(FC_INSTALL_DIR)/fctools/packages;$(BIOS_INSTALL_DIR)/packages;$(BIOSUTILS_INSTALL_DIR)/packages;$(CMEM_INSTALL_DIR)/packages;$(LINK_INSTALL_DIR)/packages;$(LINK_INSTALL_DIR);$(CE_INSTALL_DIR)/packages;$(CE_INSTALL_DIR)/cetools/packages;$(EDMA3_LLD_INSTALL_DIR)/packages;$(SERVER_INSTALL_DIR)/packages

XDC = $(XDC_INSTALL_DIR)/xdc

CROSS_COMPILE = $(CSTOOL_PREFIX)
XDCTARGETS = ti.targets.C$(DSP_ISA)
XDCBUILDCFG=../config.bld

# Uncomment for verbose output
#XDCOPTIONS=v

export MVTOOL_DIR
export XDCOPTIONS
export XDCPATH
export XDCARGS
export CODEGEN_INSTALL_DIR
export XDCBUILDCFG
export PLATFORM_XDC
export CROSS_COMPILE
export XDCTARGETS

.PHONY: c6accelw codec_packages server c6accel_app release_packages clean clean_packages

all: codec_packages server c6accelw c6accel_app

release: release_packages

install: server_install
	$(MAKE) -C app install

clean: clean_packages clean_app clean_c6accelw

# This recursively locates all packages in the packages/ directory, builds
# and releases them
codec_packages: .check-BIOS
	$(XDC) -P packages/ti/c6accel
	$(XDC) -P packages/ti/c6accel/ce

server: .check-BIOS
	$(XDC) -P packages/ti/c6accel_unitservers/$(PLATFORM)

release_packages:
	$(XDC) release -P packages/ti/c6accel
	$(XDC) release -P packages/ti/c6accel/ce
	$(XDC) release -P packages/ti/c6accel_unitservers/$(PLATFORM)

server_install:
	@install -d $(EXEC_DIR_C6ACCEL)
ifeq ($(PLATFORM),omap3530)
	@echo
	@echo Copying kernel modules to target file system: $(EXEC_DIR_C6ACCEL)/
	@echo Copying cmemk.ko
	@install -m 755 $(CMEM_INSTALL_DIR)/packages/ti/sdo/linuxutils/cmem/src/module/cmemk.ko $(EXEC_DIR_C6ACCEL)/
	@echo Copying dsplinkk.ko
	@install -m 755 $(LINK_INSTALL_DIR)/packages/dsplink/gpp/export/BIN/Linux/OMAP3530/RELEASE/dsplinkk.ko $(EXEC_DIR_C6ACCEL)/
	@echo Copying lpm_omap3530.ko
	@install -m 755 $(LPM_INSTALL_DIR)/packages/ti/bios/power/modules/omap3530/lpm/lpm_omap3530.ko $(EXEC_DIR_C6ACCEL)/
	@echo Copying sdmak.ko
	@install -m 755 $(CMEM_INSTALL_DIR)/packages/ti/sdo/linuxutils/sdma/src/module/sdmak.ko $(EXEC_DIR_C6ACCEL)/
	@echo Copying c6accel codec servers from $(C6ACCEL_INSTALL_DIR)
	@install -m 755 $(C6ACCEL_INSTALL_DIR)/packages/ti/c6accel_unitservers/omap3530/omap3530.x64P $(EXEC_DIR_C6ACCEL)/
	@echo Copying c6accel test app from $(C6ACCEL_INSTALL_DIR)
	@install -m 755 $(C6ACCEL_INSTALL_DIR)/app/app $(EXEC_DIR_C6ACCEL)/
	@echo Copying testfile from $(C6ACCEL_INSTALL_DIR)
	@install -m 755 $(C6ACCEL_INSTALL_DIR)/app/test_files/* $(EXEC_DIR_C6ACCEL)/test_files
	@chmod -x $(EXEC_DIR_C6ACCEL)/*.x64P $(EXEC_DIR_C6ACCEL)/*.ko 
	@chmod +x $(EXEC_DIR_C6ACCEL)/app
	@install -m 755 $(C6ACCEL_INSTALL_DIR)/app/omap3530/loadmodules_omap3530_c6accel.sh $(EXEC_DIR_C6ACCEL)
	@chmod +x $(EXEC_DIR_C6ACCEL)/loadmodules_omap3530_c6accel.sh
endif

ifeq ($(PLATFORM),omapl138)
	@echo
	@echo Copying kernel modules to target file system: $(EXEC_DIR_C6ACCEL)/
	@echo Copying cmemk.ko
	@install -m 755 $(CMEM_INSTALL_DIR)/packages/ti/sdo/linuxutils/cmem/src/module/cmemk.ko $(EXEC_DIR_C6ACCEL)/
	@echo Copying dsplinkk.ko
	@install -m 755 $(LINK_INSTALL_DIR)/dsplink/gpp/export/BIN/Linux/OMAPL138/RELEASE/dsplinkk.ko $(EXEC_DIR_C6ACCEL)/
	@echo Copying c6accel codec servers from $(C6ACCEL_INSTALL_DIR)
	@install -m 755 $(C6ACCEL_INSTALL_DIR)/packages/ti/c6accel_unitservers/omapl138/c6accel_omapl138.x674 $(EXEC_DIR_C6ACCEL)/
	@echo Copying c6accel test app from $(C6ACCEL_INSTALL_DIR)
	@install -m 755 $(C6ACCEL_INSTALL_DIR)/app/c6accel_app $(EXEC_DIR_C6ACCEL)/
	@echo Copying testfile from $(C6ACCEL_INSTALL_DIR)
	@install -m 755 $(C6ACCEL_INSTALL_DIR)/app/test_files/* $(EXEC_DIR_C6ACCEL)/test_files
	@chmod -x $(EXEC_DIR_C6ACCEL)/*.x674 $(EXEC_DIR_C6ACCEL)/*.ko 
	@chmod +x $(EXEC_DIR_C6ACCEL)/c6accel_app
	@install -m 755 $(C6ACCEL_INSTALL_DIR)/app/omapl138/loadmodules_omapl138_c6accel.sh $(EXEC_DIR_C6ACCEL)
	@chmod +x $(EXEC_DIR_C6ACCEL)/loadmodules_omapl138_c6accel.sh
endif

clean_packages:
	$(XDC) clean -PR packages

# This ensures that the BIOS you're using has been patched for SDSCM00004525.
# Specifically, the fix is to rename
# $(BIOS_INSTALL_DIR)/packages/ti/rtdx/package_export.xdc to package.xdc
.check-BIOS : $(BIOS_INSTALL_DIR)/packages/ti/rtdx/package.xdc


c6accel_app:
	@echo
	@echo Making all in subdirectory $@...
	$(MAKE) -C app         PLATFORM_XDC=$(PLATFORM_XDC) 

	
clean_app:	
	@echo
	@echo Cleaning all in subdirectory $@...
	$(MAKE) -C app clean

c6accelw:
	@echo
	@echo Making all in subdirectory $@...
	$(MAKE) -C c6accelw    PLATFORM_XDC=$(PLATFORM_XDC) 

	
clean_c6accelw:
	@echo
	@echo Cleaning all in subdirectory $@...
	$(MAKE) -C c6accelw clean	

$(BIOS_INSTALL_DIR)/packages/ti/rtdx/package.xdc :
	$(warning Your BIOS needs to be patched for SDSCM00004525!)
	$(error Please copy $(BIOS_INSTALL_DIR)/packages/ti/rtdx/package_export.xdc to $@)

