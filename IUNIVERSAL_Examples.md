# Introduction #

IUNIVERSAL API allows developer to use Codec-Engine framework for non-VISA(Video, Imaging, Speech or Audio) XDAIS complaint algorithms.

The examples can be downloaded from the link shown below.
```
[https://www-a.ti.com/downloads/sds_support/applications_packages/iuniversal_examples/index.htm IUNIVERSAL Examples Download]
```

# Details #
Download and save the examples file iuniversal\_examples-v0.2-Linux-x86-Install from the link shown above.

Start installation. Self guided graphical installation will start.
```
 # cd <Base_Download_Dir>
 # chmod +x iuniversal_examples-v0.2-Linux-x86-Install
 #./iuniversal_examples-v0.2-Linux-x86-Install
```

Now go to your base IUNIVERSAL folder and edit Makefile. Following changes are necessary.

```
 ..............
 ..............
 CODEGEN_INSTALL_DIR = <work>/code_gen
 ..............
 ..............
 PLATFORM = omap3530
 XDC_INSTALL_DIR = <work>/xdctools_3_16_00_18
 BIOS_INSTALL_DIR = <work>/bios/bios_5_41_00_06
 LINUXKERNEL_INSTALL_DIR = <OE_Base_Dir>/build/tmp-angstrom_2008_1/sysroots/beagleboard-angstrom-linux-gnueabi/kernel
 CE_INSTALL_DIR = <work>/codec_engine_n_tools/codec_engine_2_25_02_11
 FC_INSTALL_DIR = <work>/codec_engine_n_tools/codec_engine_2_25_02_11/cetools/packages/ti/sdo/fc
 LINK_INSTALL_DIR = <work>/dsplink/dsplink_linux_1_64
 XDAIS_INSTALL_DIR = <work>/codec_engine_n_tools/codec_engine_2_25_02_11/cetools/packages/ti/xdais
 CMEM_INSTALL_DIR = <work>/codec_engine_n_tools/codec_engine_2_25_02_11/cetools/packages/ti/sdo/linuxutils
 BIOSUTILS_INSTALL_DIR = <work>/codec_engine_n_tools/codec_engine_2_25_02_11/cetools/packages/ti/bios/utils
 CODEC_INSTALL_DIR = 
 LPM_INSTALL_DIR = 
 EDMA3_LLD_ROOT = 
 EXEC_DIR = <work>/iuniversal/output
 ..............
 ..............
 ifeq ($(PLATFORM),omap3530)
    XDC_PLATFORM = ti.platforms.evm3530
    XDC_TARGETS = "gnu.targets.arm.GCArmv5T ti.targets.C64P"
    CROSS_COMPILE = <OE_Base_Dir>/build/tmp-angstrom_2008_1/cross/armv7a/bin/arm-angstrom-linux-gnueabi-
 ..............
 ..............
 #change $(LINK_INSTALL_DIR)/packages to $(LINK_INSTALL_DIR)
 XDCPATH = ^;$(CURDIR)/packages;$(PACKAGE_DIR)/packages;$(CE_INSTALL_DIR)/packages;$(CE_INSTALL_DIR)/cetools/packages;$(BIOS_INSTALL_DIR)/packages;$(CODEC_INSTALL_DIR);$(FC_INSTALL_DIR)/packages;$(LINK_INSTALL_DIR);$(XDAIS_INSTALL_DIR)/packages;$(CMEM_INSTALL_DIR)/packages;$(BIOSUTILS_INSTALL_DIR)/packages;$(LPM_INSTALL_DIR)/packages
 ..............
 ..............
```

Save the changes.

To build, goto IUNIVERSAL examples base folder and run make command followed by make all to build all the examples.
```
# cd <work>/iuniversal
# make clean
# make all
# make install
```

Executables will be stored at `<work>/iuniversal/output/`. To test FIR example, copy fir\_unitserver\_evm3530.x64P and remote\_ti\_platforms\_evm3530\_fir.xv5T to your `Target_work>` folder in beagleboard.
```
 # cd <work>/iuniversal/output
 # cp fir_unitserver_evm3530.x64 <Board_Target_mount>/<Target_work>/
 # cp fir_unitserver_evm3530.x64 <Board_Target_mount>/<Target_work>/
```

Now switch to the Beagleboard command prompt and type
```
 $ cd <Target_work>
 $ ./remote_ti_platforms_evm3530_fir.xv5T
```

If everything went all right then, following output can be seen on the screen.
```
root@beagleboard:/media/pramod/dsplink# ./remote_ti_platforms_evm3530_fir.xv5T
@0x000e6127:[T:0x4001e290] ZZ - main> Welcome to app's main().
App-> Application started.
App-> Codec created.
App-> Control completed.
FIR Input: 16384 20480 20480 0 0 0 0 0 0 0 0 0 0 0
FIR Output: 1150 268 -899 -3283 -2633 -2216 -1799 -1257 -527 394 1480 2665 3865
4985
App-> Completed Successfully
app ended.
root@beagleboard:/media/pramod/dsplink#
```

> Similarly, you can also the test the Randgen example.