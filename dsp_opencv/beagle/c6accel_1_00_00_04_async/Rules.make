#
#  ======== Rules.make ========
#  This file specified variables used by the makefiles. After editing this file
#  you should not have to modify any of Makefiles to build this product.
#
#  The DVSDK_INSTALL_DIR variable is a local support variable only used in
#  this file and does not need to be set if individual components are used.
#

# Define target platform.
PLATFORM=omap3530

ifeq ($(PLATFORM),omapl138)
# Where the DVSDK is installed.
DVSDK_INSTALL_DIR=$(HOME)/OMAP-L138_arm_1_00_00_11

# Where the Codec Engine package is installed.
CE_INSTALL_DIR=$(DVSDK_INSTALL_DIR)/codec_engine_2_25_00_05

# Where the XDAIS package is installed.
XDAIS_INSTALL_DIR=$(DVSDK_INSTALL_DIR)/xdais_6_25_01_08

# Where the DSP Link package is installed.
LINK_INSTALL_DIR=$(DVSDK_INSTALL_DIR)/dsplink_linux_1_65_00_02

# Where the CMEM (contiguous memory allocator) package is installed.
CMEM_INSTALL_DIR=$(DVSDK_INSTALL_DIR)/linuxutils_2_25_01_06

# Where the EDMA3 Low Level Driver is installed.
EDMA3_LLD_INSTALL_DIR=$(DVSDK_INSTALL_DIR)/REL_EDMA3_LLD_BIOS5_01_11_00_03

# Where the TI C6x codegen tool is installed.
CODEGEN_INSTALL_DIR=$(HOME)/TI_CGT_C6000_6.1.9

# Where the RTSC tools package is installed.
XDC_INSTALL_DIR=$(HOME)/xdctools_3_16_01_27

# Where Framework Components product is installed.
FC_INSTALL_DIR=$(DVSDK_INSTALL_DIR)/framework_components_2_25_00_04

# Where DSP/BIOS is installed.
BIOS_INSTALL_DIR=$(HOME)/bios_5_41_02_14/

# Where biosutils package is installed.
BIOSUTILS_INSTALL_DIR=$(DVSDK_INSTALL_DIR)/biosutils_1_02_02

# RTSC platform
PLATFORM_XDC = ti.platforms.evmOMAPL138

# ARM Instruction set
ARM_ISA = armv5t

# DSP Instruction set
DSP_ISA = 674

else

# Where the DVSDK is installed.
DVSDK_INSTALL_DIR=$(HOME)/dvsdk/dvsdk_3_01_00_10

# Where the Codec Engine package is installed.
#CE_INSTALL_DIR=$(DVSDK_INSTALL_DIR)/codec_engine_2_25_02_11
CE_INSTALL_DIR=/media/disk/openembedded/build/tmp-angstrom_2008_1/sysroots/beagleboard-angstrom-linux-gnueabi/usr/share/ti/ti-codec-engine-tree

# Where the XDAIS package is installed.
#XDAIS_INSTALL_DIR=$(DVSDK_INSTALL_DIR)/xdais_6_25_02_11
XDAIS_INSTALL_DIR=/media/disk/openembedded/build/tmp-angstrom_2008_1/sysroots/beagleboard-angstrom-linux-gnueabi/usr/share/ti/ti-xdais-tree

# Where the DSP Link package is installed.
#LINK_INSTALL_DIR=$(DVSDK_INSTALL_DIR)/dsplink_linux_1_65_00_02
LINK_INSTALL_DIR=/media/disk/openembedded/build/tmp-angstrom_2008_1/sysroots/beagleboard-angstrom-linux-gnueabi/usr/share/ti/ti-dsplink-tree/packages

# Where the CMEM (contiguous memory allocator) package is installed.
#CMEM_INSTALL_DIR=$(DVSDK_INSTALL_DIR)/linuxutils_2_25_02_08
CMEM_INSTALL_DIR=/media/disk/openembedded/build/tmp-angstrom_2008_1/sysroots/beagleboard-angstrom-linux-gnueabi/usr/share/ti/ti-linuxutils-tree

# Where the EDMA3 Low Level Driver is installed.
#EDMA3_LLD_INSTALL_DIR=$(DVSDK_INSTALL_DIR)/edma3_lld_01_11_00_03
EDMA3_LLD_INSTALL_DIR=/media/disk/openembedded/build/tmp-angstrom_2008_1/sysroots/beagleboard-angstrom-linux-gnueabi/usr/share/ti/ti-edma3lld-tree

# Where the TI C6x codegen tool is installed.
#CODEGEN_INSTALL_DIR=$(HOME)/TI_CGT_C6000_6.1.9
CODEGEN_INSTALL_DIR=/media/disk/openembedded/build/tmp-angstrom_2008_1/sysroots/beagleboard-angstrom-linux-gnueabi/usr/share/ti/ti-cgt6x-tree

# Where the RTSC tools package is installed.
#XDC_INSTALL_DIR=$(DVSDK_INSTALL_DIR)/xdctools_3_16_01_27
XDC_INSTALL_DIR=/media/disk/openembedded/build/tmp-angstrom_2008_1/sysroots/beagleboard-angstrom-linux-gnueabi/usr/share/ti/ti-xdctools-tree

# Where Framework Components product is installed.
#FC_INSTALL_DIR=$(DVSDK_INSTALL_DIR)/framework_components_2_25_01_05
FC_INSTALL_DIR=/media/disk/openembedded/build/tmp-angstrom_2008_1/sysroots/beagleboard-angstrom-linux-gnueabi/usr/share/ti/ti-framework-components-tree

# Where DSP/BIOS is installed.
#BIOS_INSTALL_DIR=$(DVSDK_INSTALL_DIR)/bios_5_41_00_06
BIOS_INSTALL_DIR=/media/disk/openembedded/build/tmp-angstrom_2008_1/sysroots/beagleboard-angstrom-linux-gnueabi/usr/share/ti/ti-dspbios-tree

# Where biosutils package is installed.
#BIOSUTILS_INSTALL_DIR=$(DVSDK_INSTALL_DIR)/biosutils_1_02_02
BIOSUTILS_INSTALL_DIR=/media/disk/openembedded/build/tmp-angstrom_2008_1/sysroots/beagleboard-angstrom-linux-gnueabi/usr/share/ti/ti-biosutils-tree

# Where the local power manager is installed.
#LPM_INSTALL_DIR=$(DVSDK_INSTALL_DIR)/local_power_manager_linux_1_24_02_09
LPM_INSTALL_DIR=/media/disk/openembedded/build/tmp-angstrom_2008_1/sysroots/beagleboard-angstrom-linux-gnueabi/usr/share/ti/ti-local-power-manager-tree

# RTSC platform
PLATFORM_XDC = ti.platforms.evm3530

# ARM Instruction set
ARM_ISA = armv7-a

# DSP Instruction set
DSP_ISA = 64P

endif

# Where C6ACCEL package is installed 
#C6ACCEL_INSTALL_DIR=  = $(DVSDK_INSTALL_DIR)/c6accel_1_00_00_01/soc
C6ACCEL_INSTALL_DIR=/media/disk/openembedded/opencv-dsp-acceleration/dsp_opencv_c6accel/beagle/c6accel_1_00_00_04_async/soc

# The prefix to be added before the GNU compiler tools (optionally including
# path), i.e. "arm_v5t_le-" or "/opt/bin/arm_v5t_le-".
#CSTOOL_DIR=$(HOME)/arm-2009q1/
CSTOOL_DIR=/media/disk/openembedded/build/tmp-angstrom_2008_1/sysroots/i686-linux/usr
#CSTOOL_PREFIX=$(CSTOOL_DIR)/bin/arm-none-linux-gnueabi-
CSTOOL_PREFIX=$(CSTOOL_DIR)/bin/arm-angstrom-linux-gnueabi-
MVTOOL_PREFIX=$(CSTOOL_PREFIX)


# Where to copy the resulting executables and data to (when executing 'make
# install') in a proper file structure. This EXEC_DIR should either be visible
# from the target, or you will have to copy this (whole) directory onto the
# target filesystem.
#EXEC_DIR=$(HOME)/c6accel/$(PLATFORM)
EXEC_DIR=/home/jars/beagleboard/c6accel
