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

#inhirit Rules.make from base dir
include $(PACKAGE_DIR)/Rules.make

# RTSC platform
PLATFORM_XDC = ti.platforms.evm3530

# ARM Instruction set
ARM_ISA = armv7-a

# DSP Instruction set
DSP_ISA = 64P

endif

# Where C6ACCEL package is installed 
#C6ACCEL_INSTALL_DIR=  = $(DVSDK_INSTALL_DIR)/c6accel_1_00_00_01/soc
C6ACCEL_INSTALL_DIR=$(SERVER_DIR)/..

# The prefix to be added before the GNU compiler tools (optionally including
# path), i.e. "arm_v5t_le-" or "/opt/bin/arm_v5t_le-".
#CSTOOL_DIR=$(HOME)/arm-2009q1/
#CSTOOL_DIR=/media/disk/openembedded/build/tmp-angstrom_2008_1/sysroots/i686-linux/usr
#CSTOOL_PREFIX=$(CSTOOL_DIR)/bin/arm-none-linux-gnueabi-
CSTOOL_PREFIX=$(CROSS_COMPILE)
MVTOOL_PREFIX=$(CSTOOL_PREFIX)


# Where to copy the resulting executables and data to (when executing 'make
# install') in a proper file structure. This EXEC_DIR should either be visible
# from the target, or you will have to copy this (whole) directory onto the
# target filesystem.
#EXEC_DIR=$(HOME)/c6accel/$(PLATFORM)
EXEC_DIR=/home/jars/beagleboard/c6accel
