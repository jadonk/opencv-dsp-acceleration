This wiki-page describes the required setup for installing and building DSPLink and Codec-Engine.

# Note #

**It is suggested to refer http://code.google.com/p/opencv-dsp-acceleration/wiki/Instruction_For_Building_Examples instead of this page.**

The easy and convenient way to get all the necessary tools for this project is using bitbake if you are using OpenEmbedded based development environment. For example, #bitbake ti-dsplink will build dsplink and sample examples. Similarly #bitbake ti-codec-engine will give you codec-engine framework and all of its dependencies. The instruction below can be useful if you want to build everything individually and out-side OpenEmbedded. Also, there is version mis-match. Its better to refer codec-engine bitbake recipe to find the correct version of the tools required.

# Introduction #

Installing DSP/BIOS Link, Code Generation Tools, DSP BIOS, XDC Tools, LPM, CMEM and Codec Engine.

The following tools were used for this project. The download link is also provided.

DSP/BIOS Link 1.64         [[DSPLink](http://software-dl.ti.com/dsps/dsps_public_sw/sdo_sb/targetcontent/DSPLink/1_64/index_FDS.html)]

cgt\_c6000\_7.0.1            [[Code Generation Tools](https://www-a.ti.com/downloads/sds_support/CodeGenerationTools.htm#C6000)]

DSP/BIOS 5.41.00.06        [[DSP/BIOS](http://software-dl.ti.com/dsps/dsps_public_sw/sdo_sb/targetcontent/bios/dspbios/index.html)]

XDC tools 3.16.00.18       [[XDC Tools](http://software-dl.ti.com/dsps/dsps_public_sw/sdo_sb/targetcontent/rtsc/3_16_00_18/index_FDS.html)]

Codec Engine 2.25.02.11    [[Codec Engine with cetools](http://software-dl.ti.com/dsps/dsps_public_sw/sdo_sb/targetcontent/ce/2_25_02_11/index_FDS.html)]

All the tools were saved on my Desktop  at /home/jars/Desktop. Which will be called `<Base_Download_Dir>`.

## Install DSP/BIOS Link ##

First untar the DSP/BIOS Link (DSP Link) inside a folder in your working directory. In my case it is /home/jars/Dsp\_gsoc, which I will name '`<work>`' for quick reference.
```
 # mkdir -p <work>/dsplink
 # cd <work>/dsplink
 # tar -xvzf <Base_Download_Dir>/dsplink_linux_1_64.tar.gz
```
Before we can proceed further, we need to install Code Generation Tools, DSP/BIOS and XDC Tools. These comes in binary form and can be directly installed after making them executable.
```
 # chmod +x <Base_Download_Dir>/ti_cgt_c6000_7.0.1_setup_linux_x86.bin
 # chmod +x <Base_Download_Dir>/bios_setuplinux_5_41_00_06.bin
 # chmod +x <Base_Download_Dir>/xdctools_setuplinux_3_16_00_18.bin
```
After making them executable, we can start installing by executing the binaries.

### Installing Code Generation Tools ###

```
 # <Base_Download_Dir>/ti_cgt_c6000_7.0.1_setup_linux_x86.bin
```
It is simple afterward as graphical installation wizard starts and you just have to confirm with certain settings and select your installation directory. In my case the installation directory was selected to be 'work/code\_gen'.

### Installing DSP/BIOS ###

```
 # <Base_Download_Dir>/bios_setuplinux_5_41_00_06.bin
```
It is simple afterward as graphical installation wizard starts and you just have to confirm with certain settings and select your installation directory. In my case the installation directory was selected to be 'work/bios'.

### Installing XDC Tools ###

```
 # <Base_Download_Dir>/xdctools_setuplinux_3_16_00_18.bin
```
It is simple afterward as graphical installation wizard starts and you just have to confirm with certain settings and select your installation directory. In my case the installation directory was selected to be 'work/xdctools'.

### Continue with DSPLink ###
After installing all the dependencies, we can now proceed further to install DSP Link.

To start with, export the base dsplink path.
```
 # export DSPLINK /home/jars/Dsp_gsoc/dsplink/dsplink_linux_1_64/dsplink
```

First, we-need to run DSPLink configuration

```
 # cd <DSPLink_Base_Dir>/dsplink/config/bin
 #  perl dsplinkcfg.pl --platform=OMAP3530 --nodsp=1  --dspcfg_0=OMAP3530SHMEM     --dspos_0=DSPBIOS5XX --gppos=OMAPLSP --comps=ponslrmc --DspTskMode=1

 Note: We can also give seperate destination for build libraries and executables by providing options '--gpp_temp=<path_of_dir_our_choice> --dsp0_temp=<path_of_dir_of_our_choice>'. But this  can give trouble later when building codec-engine-server examples. The error is shown in the build procedure for codec-engine examples.

 Successful configuration will give message like this at the end.

 =========================================================

Configuration done successfully!!
Generating CURRENTCFG.MK file...
Generating multimake script...
Generating CFG_system.c File...
Generating GPP RTSC xdc file...
Generating DSP RTSC xdc file...
=========================================================
Please edit the following files for toolchains, kernel sources, etc changes.
GPP side distribution file: $DSPLINK/make/Linux/omap3530_2.6.mk
GPP side distribution file: $DSPLINK/gpp/src/Rules.mk
DSP side distribution file: $DSPLINK/make/DspBios/c64xxp_5.xx_linux.mk
==========================================================
Users consuming DSPLINK as XDC package, Need to do the following.
cd into the $(DSPLINK)/dsp directory and run: 
 $(XDC_INSTALL_DIR)/xdc clean 
 $(XDC_INSTALL_DIR)/xdc .interfaces 
cd into the $(DSPLINK)/gpp directory and run: 
 $(XDC_INSTALL_DIR)/xdc clean 
 $(XDC_INSTALL_DIR)/xdc .interfaces 
==========================================================

```

As said in the message if you are using DSPLink for XDC package. Execute those four instruction.

Next, we need to edit three make files as shown in the message to provide path of various tools. I will mention only necessary changes to be done on the make files. Rest can be left untouched. Please find the respective line in the make file mentioned below.

1) Find omap3530\_2.6.mk in the directory $DSPLINK/make/Linux/ by typing # cd $DSPLINK/make/Linux/. Edit this file using VI or Gedit.
```
 ..........
 ..........
 BASE_BUILDOS    := <Base_OE_Dir>/build/tmp-angstrom_2008_1/sysroots/beagleboard-angstrom-linux-gnueabi/kernel
 ...........
 ...........
 BASE_TOOLCHAIN  := <Base_OE_Dir>/build/tmp-angstrom_2008_1/cross/armv7a
 ...........
 ...........
 OSINC_PLATFORM  := $(BASE_TOOLCHAIN)/lib/gcc/arm-angstrom-linux-gnueabi/4.3.3/include
 ...........
 ...........
 COMPILER        := $(BASE_CGTOOLS)/arm-angstrom-linux-gnueabi-gcc
 ..........
 ..........
 LD              := $(BASE_CGTOOLS)/arm-angstrom-linux-gnueabi-ld
 ..........
 ..........
 CROSS_COMPILE   := arm-angstrom-linux-gnueabi-
 ..........
 ..........
 ARCHIVER        := $(BASE_CGTOOLS)/arm-angstrom-linux-gnueabi-ar
 .......... 
 ..........
 LINKER      := $(BASE_CGTOOLS)/arm-angstrom-linux-gnueabi-gcc
 .........
 .........
 
```

Don't forget to save the changes at the end.

2) Find Rules.mk in the directory $DSPLINK/gpp/src/ by typing # cd $DSPLINK/gpp/src/. Edit this file using VI or Gedit.

```
 .........
 .........
 KERNEL_DIR    := <Base_OE_Dir>/build/tmp-angstrom_2008_1/sysroots/beagleboard-angstrom-linux-gnueabi/kernel
 .........
 .........
 TOOL_PATH     := <Base_OE_Dir>/build/tmp-angstrom_2008_1/cross/armv7a/bin
 .........
 .........
 CG_PREFIX = arm-angstrom-linux-gnueabi-
 .........
 .........
 MAKE_OPTS = ARCH=arm CROSS_COMPILE=$(TOOL_PATH)/arm-angstrom-linux-gnueabi-
 .........
 .........

```
Again, don't forget to save the changes.

3) Find c64xxp\_5.xx\_linux.mk in the directory $DSPLINK/make/DspBios/ by typing # cd $DSPLINK/make/DspBios/. Edit this file using VI or Gedit.
```
 ...........
 ...........
 BASE_INSTALL    := <work>
 ...........
 ...........
 BASE_SABIOS     := $(BASE_INSTALL)/bios/bios_5_41_00_06
 ...........
 ...........
 XDCTOOLS_DIR    := $(BASE_INSTALL)/xdctools_3_16_00_18
 ............
 ............
 BASE_CGTOOLS    := $(BASE_INSTALL)/code_gen
 ............
 ............
  
```

That's all. You are done with editing and its time to make it work. But, before that make sure that you have saved all the changes.

Now, goto $DSPLINK/gpp/src/api/ and execute following commands.

```
 # cd $DSPLINK/gpp/src/api/
 # make -s clean
 # make -s debug
 # make -s release
 Or you can do 'make -s' to build both debug and release version.
```

Next, goto $DSPLINK/gpp/src/ and execute following commands which builds OMAP3530 loadable module. The module 'dsplinkk.ko' will be save at '$DSPLINK/gpp/export/BIN/Linux/OMAP3530/<RELEASE & DEBUG>/
```
 # cd $DSPLINK/gpp/src/
 # make -s clean
 # make -s debug
 # make -s release
 Or you can do 'make -s' to build both debug and release version.
```

Next, goto $DSPLINK/gpp/src/samples/ and execute following commands to build GPP side sample examples. The all GPP side executables will be save at '$DSPLINK/gpp/export/BIN/Linux/OMAP3530/<RELEASE & DEBUG>/

```
 # cd $DSPLINK/gpp/src/samples/
 # make -s clean
 # make -s debug
 # make -s release
 Or you can do 'make -s' to build both debug and release version.
 
```

Next, goto $DSPLINK/dsp/src/ and execute following commands to build dsp libraries.

```
 # cd $DSPLINK/dsp/src/
 # make -s clean
 # make -s debug
 # make -s release
 Or you can do 'make -s' to build both debug and release version.
 
```

Next, goto $DSPLINK/dsp/src/samples/ and execute following commands to build dsp side sample examples. Examples will be save at '$DSPLINK/dsp/export/BIN/DspBios/OMAP3530/OMAP3530\_0/<RELEASE & DEBUG>/ with .out extension.

```
 # cd $DSPLINK/dsp/src/
 # make -s clean
 # make -s debug
 # make -s release
 Or you can do 'make -s' to build both debug and release version.
 
```

### Missing something! LPM?? ###

If everything goes well, you are good to test some sample examples. But before that it is suggested to build LPM modules if you are using DSPLink outside the codec-engine. You need LPM module to power-cycle the DSP while switching between different applications. It is not required to run the LPM for the first time. Before running different application you need to power off & then on the DSP using lpmOFF.x470uC. and lpmONx470uC. If you don't, the DSP does not function properly. LPM package can be downloaded at http://software-dl.ti.com/dsps/dsps_public_sw/sdo_sb/targetcontent/lpm/1_24_02_09/. Save the file 'local\_power\_manager\_linux\_1\_24\_02\_09.tar.gz' and untar it to the destination directory.

```
 # cd <Base_LPM_Dir>
 # tar -xvzf <Base_Download_Dir>/local_power_manager_linux_1_24_02_09.tar.gz
```

You need to edit Makefile inside the folder after you do '# cd' as mentioned below before start building the LPM module. I will mention only necessary changes. Rest of the file can be left untouched.

```
 # cd  <Base_Dir_LPM>/local_power_manager_linux_1_24_02_09/packages/ti/bios/power/modules/omap3530/lpm
 #gedit Makefile     

Note: You can also use vi editer to edit Makefile by typing '#vi Makefile'
```

Following changes are to be done inside Makefile.
```
 ...................
 ...................
 LINUXKERNEL_INSTALL_DIR = <Base_OE_Dir>/build/tmp-angstrom_2008_1/sysroots/beagleboard-angstrom-linux-gnueabi/kernel
 MVTOOL_PREFIX = <Base_OE_Dir>/build/tmp-angstrom_2008_1/cross/armv7a/bin/arm-angstrom-linux-gnueabi-
 DSPLINK_REPO = <work>/dsplink/dsplink_linux_1_64
 ...................
 ...................
```

Finally the LPM module is ready to build. To build, you need to be in the same directory where the Makefile is.

```
 # make 
```

LPM module is now built and you can see it in the same path from where you invoked make command. The binary file needed for LpmOff and LpmOn are provided at
```
 <work>/lpm/local_power_manager_linux_1_24_02_09/packages/ti/bios/power/utils/bin/ti_platforms_evm3530/linux/<debug & release>
```

### Copying Modules and sample application to beagleboard ###
Now the time has come to test your work.
Make a folder called 'dsp' inside '/lib/modules/2.6.32/kernel/drivers/' of your target beagleboard by typing following in your target board command prompt.
```
 $ mkdir /lib/modules/2.6.32/kernel/drivers/dsp
```

First copy your dsplinkk.ko and lpm\_omap3530.ko modules to your target board. If your board is NFS mount, it is just matter of copying these files from respective path to the
`<Board_Target_Mount>/lib/modules/2.6.32/kernel/drivers/dsp/`. If not, you can copy these in the particular folder mentioned above in your mmc card.
```
 # cp <work>/lpm/local_power_manager_linux_1_24_02_09/packages/ti/bios/power/modules/omap3530/lpm/lpm_omap3530.ko <Board_Target_Dir>/lib/modules/2.6.32/kernel/drivers/dsp/
 # cp $DSPLINK/gpp/export/BIN/Linux/OMAP3530/<DEBUG or RELEASE>/dsplinkk.ko <Board_Target_Mount>/lib/modules/2.6.32/kernel/drivers/dsp/
```

> Copy LPM binaries that is already provided for us at ` <work>/lpm/local_power_manager_linux_1_24_02_09/packages/ti/bios/power/utils/bin/ti_platforms_evm3530/linux/` to `<Board_Target_Mount>/usr/bin`.

```
 # cp <work>/lpm/local_power_manager_linux_1_24_02_09/packages/ti/bios/power/utils/bin/ti_platforms_evm3530/linux/lpmOFF.x470uC <Board_Target_Mount>/usr/bin
 # cp <work>/lpm/local_power_manager_linux_1_24_02_09/packages/ti/bios/power/utils/bin/ti_platforms_evm3530/linux/lpmON.x470uC <Board_Target_Mount>/usr/bin
```

To make things easy, script to load and unload these recently built modules is already provided to us at ` <work>/lpm/local_power_manager_linux_1_24_02_09/packages/ti/bios/power/modules/omap3530/`. First create a folder in your target beagleboard filesystem where you want to preserve all your work related to DSPLink. Let's name this folder as **`<Target_work>`**. Now there is a little editing you need to do in file 'loadmodule.sh' before you copy them. Specify the correct path of your modules by editing the lines in the file loadmodule.sh as hown below.

```
 #!/bin/sh
 ...........
 ...........
 insmod /lib/modules/2.6.32/kernel/drivers/dsp/dsplinkk.ko
 ...........
 ...........
 insmod /lib/modules/2.6.32/kernel/drivers/dsp/lpm_omap3530.ko

```

Also, you need to change the first line of unloadmodules.sh
```
 #!/bin/sh
 ...........
 ...........
```


Now copy these files inside your target work folder in beagleboard.

```
 # cp <work>/lpm/local_power_manager_linux_1_24_02_09/packages/ti/bios/power/modules/omap3530/loadmodules.sh <Board_Target_Mount>/<Target_work>/
 cp <work>/lpm/local_power_manager_linux_1_24_02_09/packages/ti/bios/power/modules/omap3530/unloadmodules.sh <Board_Target_Mount>/<Target_work>/
```

Now copy the sample examples to the board files system. You can copy as many samples as you want to test but I will mention here about only one.

```
 # cp $DSPLINK/gpp/export/BIN/Linux/OMAP3530/<DEBUG or RELEASE>/loopgpp <Board_Target_Mount>/<Target_work>
 # cp $DSPLINK/dsp/export/BIN/DspBios/OMAP3530/OMAP3530_0/<DEBUG or RELEASE>/loop.out <Board_Target_Mount>/<Target_work>
```

### Running DSPLink Sample application ###

Switch to your target platform(Beagleboard) and in the command prompt type following

```
 $ cd <target_work>
 $ ./loadmodules.sh
 $ ./loopgpp loop.out 256 5

```

That is all will DSPLink. If you did not have any build error in-between you should be able to see output similar to the one shown below. Also, if you want to test some other examples, don't forget to run commands below before you start executing.

```
 $ lpmOFF.x470uC
 $ lpmON.x470uC
 
 
     
     root@beagleboard:/media/pramod/dsplink# ./loopgpp loop.out 256 5
=============== Sample Application : LOOP ==========
==== Executing sample for DSP processor Id 0 ====
Entered LOOP_Create ()
Leaving LOOP_Create ()
Entered LOOP_Execute ()
Leaving LOOP_Execute ()
Entered LOOP_Delete ()
Leaving LOOP_Delete ()
====================================================
root@beagleboard:/media/pramod/dsplink#
 
     
```

For more information you can also refer to,

[1](1.md) http://processors.wiki.ti.com/index.php/Building_DSPLink
[2](2.md) http://ossie.wireless.vt.edu/trac/wiki/BeagleBoard_DSPLink


# Building CMEM module #

CMEM module is used to assign continious memroy from the user space.

CMEM package comes along with Codec-Engine and tools. You can find it at ` <work>/codec_engine_n_tools/codec_engine_2_25_02_11/cetools/packages/ti/sdo/linuxutils/cmem `. If you are not using codec-engine then you can download it from http://software-dl.ti.com/dsps/dsps_public_sw/sdo_sb/targetcontent/linuxutils/index.html.CMEM comes inside Linux-Utils package.Save it and untar it to your
`<work>` folder.

```
 # cd <work>
 # mkdir linux_util
 # cd linux_util
 # tar -xvzf <Base_Download_Dir>/linuxutils_2_25_02_08.tar.gz
```

If using codec-engine

```
 # cd <work>/codec_engine_n_tools/codec_engine_2_25_02_11/cetools/packages/ti/sdo/linuxutils/cmem/
```

Else,

```
 # cd <work>/linux_util/linuxutils_2_25_02_08/packages/ti/sdo/linuxutils/cmem
```

Now, edit the Rules.make file. The only necessary changes to be done is shown below.
```
 ...........
 ...........
 #If you are using codec-engine un-comment the line below and comment out the line next to it.
 #EXEC_DIR=<work>/codec_engine_n_tools/codec_engine_2_25_02_11/cetools/packages/ti/sdo/linuxutils/cmem
 EXEC_DIR=<work>/linux_util/linuxutils_2_25_02_08/packages/ti/sdo/linuxutils/cmem
 ...........
 ...........
 MVTOOL_PREFIX=<OE_Base_Dir>/build/tmp-angstrom_2008_1/cross/armv7a/bin/arm-angstrom-linux-gnueabi-
 ...........
 ...........
 #You need to comment the line below
 #UCTOOL_PREFIX=/db/toolsrc/library/vendors2005/opensource/buildroot/10122007/build_arm/staging_dir/usr/bin/arm-linux-
 ...........
 ...........
 LINUXKERNEL_INSTALL_DIR=<OE_Base_Dir>/build/tmp-angstrom_2008_1/sysroots/beagleboard-angstrom-linux-gnueabi/kernel
 ...........
 ...........
```

Run the following commands
```
 # make 
 # make install
```

The freshly built module cmemk.ko will be present at the folder `<work>/linux_util/linuxutils_2_25_02_08/packages/ti/sdo/linuxutils/cmem/`. Copy it to the folder `<Board_Target_Mount>/lib/modules/2.6.32/kernel/drivers/dsp/` and test it using following command
```
 # cp cmemk.ko <Board_Target_Mount>/lib/modules/2.6.32/kernel/drivers/dsp/}
 # insmod /lib/modules/2.6.32/kernel/drivers/dsp/cmemk.ko pools=1x1000000 phys_start=0x85000000 phys_end=0x86000000
```

If you see no error message, you have successfully tested it.

# Building Codec-Engine and Examples #

To start building codec-engine examples you need to edit xdcpaths.mak file found in folder `<work>/codec_engine_n_tools/codec_engine_2_25_02_11/examples`. The necessary changes is shown below. The xdcpaths.mak file is well commented and modification is very easy.

```
 ........
 ........
 DEVICES  := OMAP3530
 .......
 .......
 GPPOS := LINUX_GCC
 .......
 .......
 PROGRAMS := APP_CLIENT DSP_SERVER
 .......
 .......
 CE_INSTALL_DIR        := <work>/codec_engine_n_tools/codec_engine_2_25_02_11
 XDC_INSTALL_DIR       := <work>/xdctools_3_16_00_18
 BIOS_INSTALL_DIR      := <work>/bios/bios_5_41_00_06
 DSPLINK_INSTALL_DIR   := <work>/dsplink/dsplink_linux_1_64
 .......
 .......
 CGTOOLS_V5T := <OE_Base_Dir>/build/tmp-angstrom_2008_1/cross/armv7a
 CC_V5T := bin/arm-angstrom-linux-gnueabi-gcc
 #You can leave the line below as it is in the xdcpaths.mak file.
 CGTARGET := gnu.targets.arm.GCArmv5T
 .......
 .......
 CGTOOLS_C64P := /home/jars/Dsp_gsoc/code_gen
 .......
 .......
```

Goto `<work>/codec_engine_n_tools/codec_engine_2_25_02_11/examples/ti/sdo/ce/examples/codecs/` and type the following commands to build codec examples.
```
 # cd <work>/codec_engine_n_tools/codec_engine_2_25_02_11/examples/ti/sdo/ce/examples/codecs/
 # make clean
 # make
```

Goto `<work>/codec_engine_n_tools/codec_engine_2_25_02_11/examples/ti/sdo/ce/examples/extensions/` and type the following commands to build extensions examples.
```
 # cd <work>/codec_engine_n_tools/codec_engine_2_25_02_11/examples/ti/sdo/ce/examples/extensions/
 # make clean
 # make
```

Goto `<work>/codec_engine_n_tools/codec_engine_2_25_02_11/examples/ti/sdo/ce/examples/servers/` and type the following commands to build dsp server examples.

```
 # cd <work>/codec_engine_n_tools/codec_engine_2_25_02_11/examples/ti/sdo/ce/examples/servers/
 # make clean
 # make
```

Note: This is the place I had mentioned earlier while configuring DSPLink. If you had given `--gpp_temp=<path_of_dir_our_choice> --dsp0_temp=<path_of_dir_of_our_choice>` while configuring DSPLink, you may get error similar as shown below. There should be way to way to rectify it, but I did not bother much and rather opted easier path.

```
 Error while building codec-engine server if you have specified some other destination directory directory while building dsplink.
js: "/home/jars/Dsp_gsoc/xdctools_3_16_00_18/packages/xdc/cfg/Main.xs", line 40: XDC runtime error: can't find the library 'export/BIN/DspBios/OMAP3530/OMAP3530_0/RELEASE/dsplink.lib' specified by package dsplink.dsp. It wasn't found along the path '/home/jars/Dsp_gsoc/codec_engine_n_tools/codec_engine_2_25_02_11/examples/ti/sdo/ce/examples/servers/all_codecs/../../../../../..;/home/jars/Dsp_gsoc/codec_engine_n_tools/codec_engine_2_25_02_11/cetools/packages;/home/jars/Dsp_gsoc/codec_engine_n_tools/codec_engine_2_25_02_11/packages;/home/jars/Dsp_gsoc/bios/bios_5_41_00_06/packages;/home/jars/Dsp_gsoc/dsplink/dsplink_linux_1_64;/home/jars/Dsp_gsoc/xdctools_3_16_00_18/packages;../../../../../..;'.
gmake[1]: *** [package/cfg/bin/ti_platforms_evm3530/all_x64Pcfg_c.c] Error 1
gmake[1]: *** Deleting file `package/cfg/bin/ti_platforms_evm3530/all_x64Pcfg_c.c'
gmake[1]: *** [package/cfg/bin/ti_platforms_evm3530/all_x64Pcfg_c.c] Deleting file `package/cfg/bin/ti_platforms_evm3530/all_x64P.xdl'
gmake[1]: *** [package/cfg/bin/ti_platforms_evm3530/all_x64Pcfg_c.c] Deleting file `package/cfg/bin/ti_platforms_evm3530/all_x64P.h'
gmake[1]: *** [package/cfg/bin/ti_platforms_evm3530/all_x64Pcfg_c.c] Deleting file `package/cfg/bin/ti_platforms_evm3530/all_x64P.c'
gmake[1]: *** [package/cfg/bin/ti_platforms_evm3530/all_x64Pcfg_c.c] Deleting file `package/cfg/bin/ti_platforms_evm3530/all_x64Pcfg.cmd'
gmake[1]: *** [package/cfg/bin/ti_platforms_evm3530/all_x64Pcfg_c.c] Deleting file `package/cfg/bin/ti_platforms_evm3530/all_x64Pcfg.s62'
gmake: *** [/home/jars/Dsp_gsoc/codec_engine_n_tools/codec_engine_2_25_02_11/examples/ti/sdo/ce/examples/servers/all_codecs,.executables] Error 2
make[1]: *** [all] Error 2
make[1]: Leaving directory `/home/jars/Dsp_gsoc/codec_engine_n_tools/codec_engine_2_25_02_11/examples/ti/sdo/ce/examples/servers/all_codecs'
make: *** [all] Error 2
```

Goto `<work>/codec_engine_n_tools/codec_engine_2_25_02_11/examples/ti/sdo/ce/examples/apps/` and type the following commands to build gpp application.
```
 # cd <work>/codec_engine_n_tools/codec_engine_2_25_02_11/examples/ti/sdo/ce/examples/apps/
 # make clean
 # make
```

Done with the building! Its time to test.
Goto `<work>/codec_engine_n_tools/codec_engine_2_25_02_11/examples/ti/sdo/ce/examples/apps/audio_copy/bin/ti_platforms_evm3530` and copy app\_remote.xv5T to your work folder in target board file-system.
```
 # cd <work>/codec_engine_n_tools/codec_engine_2_25_02_11/examples/ti/sdo/ce/examples/apps/audio_copy/bin/ti_platforms_evm3530/
 # cp app_remote.xv5T <Board_Target_Mount>/<Target_work>
```

Copy the input file in.dat from `<work>/codec_engine_n_tools/codec_engine_2_25_02_11/examples/ti/sdo/ce/examples/apps/audio_copy/` to your target directory in your Beagleboard.
```
 # cp <work>/codec_engine_n_tools/codec_engine_2_25_02_11/examples/ti/sdo/ce/examples/apps/audio_copy/in.dat <Board_Target_Mount>/<Target_work>
```

Now copy the codec-server from `<work>/codec_engine_n_tools/codec_engine_2_25_02_11/examples/ti/sdo/ce/examples/servers/all_codecs/bin/ti_platforms_evm3530/` to your target directory in beagleboard.

```
 # cp /home/jars/Dsp_gsoc/codec_engine_n_tools/codec_engine_2_25_02_11/examples/ti/sdo/ce/examples/servers/all_codecs/bin/ti_platforms_evm3530/all.x64P <Board_Target_Mount>/<Target_work>
```

Befor starting to execute, we need to make sure that all the modules wh have built so far is loaded in Beagleboard. To make this easy, there is loadmodules.sh and unloadmodules.sh script files provided at `<work>/codec_engine_n_tools/codec_engine_2_25_02_11/examples/apps/system_files/OMAP3530/`. Edit loadmodules.sh to provide the correct path of the respective modules. Copy these files to the target directory in the Beagleboard. Type the following commands to test your examples.

Note: Few things should be noted and take care of before executing the examples.
1) you should pass mem=120 or less for the linux kernel in the u-boot argument.
2) run loadmodules.sh befor us start and confirm all the modules are loaded by '$ lsmod'.You can visually inspect the loaded modules. The necessary modules are dsplinkk.ko, lpm\_omap3530.ko and cmemk.ko.
```
 $ cd <Target_work>
 $./loadmodules.sh
 $./app_remote.xv5T
 
```

If everything goes well, you will be able to see following results at your screen.

```
 @0x000c5a79:[T:0x4001df90] ZZ - main> Welcome to app's main().
@0x000c63c5:[T:0x4001df90] ZZ - App-> Application started.
@0x000d052e:[T:0x4001df90] ZZ - App-> Processing frame 0...
@0x000d067e:[T:0x4001df90] ZZ - App-> Encoder frame 0 process returned - 0x0)
@0x000d0753:[T:0x4001df90] ZZ - App-> Decoder frame 0 process returned - 0x0)
@0x000d0847:[T:0x4001df90] ZZ - App-> Processing frame 1...
@0x000d093b:[T:0x4001df90] ZZ - App-> Encoder frame 1 process returned - 0x0)
@0x000d0a11:[T:0x4001df90] ZZ - App-> Decoder frame 1 process returned - 0x0)
............................................................................
............................................................................
@0x0031faf1:[T:0x4001df90] ZZ - App-> Processing frame 127...
@0x0031fbe5:[T:0x4001df90] ZZ - App-> Encoder frame 127 process returned - 0x0)
@0x0031fcd9:[T:0x4001df90] ZZ - App-> Decoder frame 127 process returned - 0x0)
@0x0031fd71:[T:0x4001df90] ZZ - 128 frames encoded/decoded
@0x003274a7:[T:0x4001df90] ZZ - app done.
root@beagleboard:/media/pramod/dsplink#
```


For more information you can also refer to

```
[1] <work>/codec_engine_n_tools/codec_engine_2_25_02_11/examples/build_instructions.html
```