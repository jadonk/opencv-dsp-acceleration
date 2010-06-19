#
_XDCBUILDCOUNT = 0
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = ^;/media/disk/OE/dspacceleration/opencv-dsp-acceleration/dsp_opencv/packages;/media/disk/OE/dspacceleration/opencv-dsp-acceleration/dsp_opencv;/media/disk/OE/work/ti/ti-codec-engine-tree/packages;/media/disk/OE/work/ti/ti-dspbios-tree/packages;/media/disk/OE/work/ti/dsp_opencv;/media/disk/OE/work/ti/ti-framework-components-tree/packages;/media/disk/OE/work/ti/ti-dsplink-tree/packages;/media/disk/OE/work/ti/ti-xdais-tree/packages;/media/disk/OE/work/ti/ti-linuxutils-tree/packages;/media/disk/OE/work/ti/ti-biosutils-tree/packages;/media/disk/OE/work/ti/ti-local-power-manager-tree/packages;/media/disk/OE/work/ti/dsplib_v210;/media/disk/OE/work/ti/imglib_v201
override XDCROOT = /media/disk/OE/work/ti/ti-xdctools-tree
override XDCBUILDCFG = /media/disk/OE/dspacceleration/opencv-dsp-acceleration/dsp_opencv/config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = gnu.targets.arm.GCArmv5T ti.targets.C64P
endif
#
ifeq (0,1)
PKGPATH = ^;/media/disk/OE/dspacceleration/opencv-dsp-acceleration/dsp_opencv/packages;/media/disk/OE/dspacceleration/opencv-dsp-acceleration/dsp_opencv;/media/disk/OE/work/ti/ti-codec-engine-tree/packages;/media/disk/OE/work/ti/ti-dspbios-tree/packages;/media/disk/OE/work/ti/dsp_opencv;/media/disk/OE/work/ti/ti-framework-components-tree/packages;/media/disk/OE/work/ti/ti-dsplink-tree/packages;/media/disk/OE/work/ti/ti-xdais-tree/packages;/media/disk/OE/work/ti/ti-linuxutils-tree/packages;/media/disk/OE/work/ti/ti-biosutils-tree/packages;/media/disk/OE/work/ti/ti-local-power-manager-tree/packages;/media/disk/OE/work/ti/dsplib_v210;/media/disk/OE/work/ti/imglib_v201;/media/disk/OE/work/ti/ti-xdctools-tree/packages;../../..
HOSTOS = Linux
endif
