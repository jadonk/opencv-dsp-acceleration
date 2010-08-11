#define platform used
PLATFORM = omap3530

#define root path for all the tools
ROOTPATH = /media/disk/openembedded/build/tmp-angstrom_2008_1/sysroots/beagleboard-angstrom-linux-gnueabi

#define path for your cross-compiler and tag for your cross-compiler. Verify if this path is correct
#for you or not and edit accordingly. For all Angstrom Linux distrubuiton, the name of gcc corss-compiler
#begin with arm-angstrom-linux-gnueabi-
CROSS_COMPILE = $(ROOTPATH)/../i686-linux/usr/bin/arm-angstrom-linux-gnueabi-

#define path to code generation tools
CODEGEN_INSTALL_DIR = $(ROOTPATH)/usr/share/ti/ti-cgt6x-tree

#define path to XDC tools
XDC_INSTALL_DIR = $(ROOTPATH)/usr/share/ti/ti-xdctools-tree

#define path to BIOS installed directory
BIOS_INSTALL_DIR = $(ROOTPATH)/usr/share/ti/ti-dspbios-tree

#define path to kernel installed directory
LINUXKERNEL_INSTALL_DIR = $(ROOTPATH)/kernel

#define path to codec engine install directory
CE_INSTALL_DIR = $(ROOTPATH)/usr/share/ti/ti-codec-engine-tree

#define path to framework components install directory
FC_INSTALL_DIR = $(ROOTPATH)/usr/share/ti/ti-framework-components-tree

#define path to DSPLink install directory
LINK_INSTALL_DIR = $(ROOTPATH)/usr/share/ti/ti-dsplink-tree/packages

#define path to XDAIS install directory
XDAIS_INSTALL_DIR = $(ROOTPATH)/usr/share/ti/ti-xdais-tree

#define path to CMEM install directory
CMEM_INSTALL_DIR = $(ROOTPATH)/usr/share/ti/ti-linuxutils-tree

#define path to BIOSUTILS install directory
BIOSUTILS_INSTALL_DIR = $(ROOTPATH)/usr/share/ti/ti-biosutils-tree

#define path to your DSP side codec install directory. In this project,
#it will be <PATH_YOU_CHECKED_OUT_THIS_PROJECT>/opencv-dsp-acceleration/dsp_opencv/beagle/c6accel_1_00_00_04_async.
#This is generally used for designing new codec.
#You may or may not specify this.
CODEC_INSTALL_DIR = /home/jars/opencv-dsp-acceleration/dsp_opencv/beagle/c6accel_1_00_00_04_async

#define path to LPM install directory
LPM_INSTALL_DIR = $(ROOTPATH)/usr/share/ti/ti-local-power-manager-tree

#define path to EDMA3 install directory
EDMA3_LLD_ROOT = $(ROOTPATH)/usr/share/ti/ti-edma3lld-tree

#define path to your install directory. This is where all your executables and test files will be copied
EXEC_DIR = /home/jars/beagleboard/dsplink

