#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = ^;/media/disk/OE/work/ti/my_fir_test/packages;/media/disk/OE/work/ti/my_fir_test;/media/disk/OE/work/ti/ti-codec-engine-tree/packages;/media/disk/OE/work/ti/ti-dspbios-tree/packages;/media/disk/OE/work/ti/my_fir_test;/media/disk/OE/work/ti/ti-framework-components-tree/packages;/media/disk/OE/work/ti/ti-dsplink-tree/packages;/media/disk/OE/work/ti/ti-xdais-tree/packages;/media/disk/OE/work/ti/ti-linuxutils-tree/packages;/media/disk/OE/work/ti/ti-biosutils-tree/packages;/media/disk/OE/work/ti/ti-local-power-manager-tree/packages
override XDCROOT = /media/disk/OE/work/ti/ti-xdctools-tree
override XDCBUILDCFG = /media/disk/OE/work/ti/my_fir_test/config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = ti.targets.C64P
endif
#
ifeq (0,1)
PKGPATH = ^;/media/disk/OE/work/ti/my_fir_test/packages;/media/disk/OE/work/ti/my_fir_test;/media/disk/OE/work/ti/ti-codec-engine-tree/packages;/media/disk/OE/work/ti/ti-dspbios-tree/packages;/media/disk/OE/work/ti/my_fir_test;/media/disk/OE/work/ti/ti-framework-components-tree/packages;/media/disk/OE/work/ti/ti-dsplink-tree/packages;/media/disk/OE/work/ti/ti-xdais-tree/packages;/media/disk/OE/work/ti/ti-linuxutils-tree/packages;/media/disk/OE/work/ti/ti-biosutils-tree/packages;/media/disk/OE/work/ti/ti-local-power-manager-tree/packages;/media/disk/OE/work/ti/ti-xdctools-tree/packages;../../../..
HOSTOS = Linux
endif
