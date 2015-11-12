# Introduction #

This page explains the complete build procedure for the uploaded examples.


# Details #

This work use OpenEmbedded based development environment. Following is the detail procedure to build this work.

# Step 1 #
### Setting up OpenEmbedded ###

First, download the OpenEmbedded setup scripts and save it.

> ` # ` Click on this link http://gitorious.org/angstrom/angstrom-setup-scripts/commits/master

> ` # ` Click on **Download master as tar.gz**. This will download and save file "angstrom-angstrom-setup-scripts-master-tar.gz".

> ` # ` Double click on the file **angstrom-angstrom-setup-scripts-master-tar.gz** that you just downloaded.

> ` # ` Then extract it to the folder of your choice. This folder will be called **BASE\_DIR**.

Goto the folder **BASE\_DIR** and run the provided scripts as mentioned below.

> ` # ` cd BASE\_DIR/angstrom-angstrom-setup-scripts

> NOTE: This directory will be call **OE\_BASE**. ie, OE\_BASE = BASE\_DIR/angstrom-angstrom-setup-scripts

> ` # ` ./oebb.sh config beagleboard

> ` # ` . ~/.oe/environment

> The script to set-up OpenEmbedded environment uses 'git'. If you don't have **git** installed, you can install it using following command "sudo apt-get install git

> ` # ` ./oebb.sh update

> Now, you have a complete OpenEmbedded based development environment. If you want to build the kernel image and other necessary packages, you can simply type the following on the command prompt; "bitbake base-image" or "bitbake console-image" or "bitbake x11-image". For further details look into this link http://wiki.openembedded.org/index.php/Getting_Started.

#### Note: If you want to copy and paste the shell commands provided with this instruction, to avoid typing, ignore the '#' symbol while copying. ####


# Step 2 #
### Building Codec-Engine ###

> This project uses codec-engine 2.25.01.06 and it's dependencies, like DSPLink 1.64. Before you start bitbaking codec-engine, you need to download Code Generation Tools, ie, C6000 Code Generation Tools v6.1.9 for Linux from this link https://www-a.ti.com/downloads/sds_support/TICodegenerationTools/download.htm and save it to the directory **OE\_BASE/source/download**. Then execute the following command from **OE\_BASE** to build Codec-Engine and all of its dependencies.

> ` # ` bitbake ti-codec-engine

> The codec-engine and all other required tools will be build and saved at **OE\_BASE/build/tmp-angstrom\_2008\_1/sysroots/beagleboard-angstrom-linux-gnueabi/usr/share/ti**. This process will also build the necessary kernel modules needed to work with on-chip DSP found on OMAP3530.


# Step 3 #
### Building OpenCV 2.1 With continious memory allocation support ###

It is assumed that you are at **OE\_BASE**. Execute the svn check-out command to download the source of this project using following command.

> ` # ` svn checkout http://opencv-dsp-acceleration.googlecode.com/svn/trunk/ opencv-dsp-acceleration

Now, you can see **opencv-dsp-acceleration** folder if you execute "**ls**" command.The patch is provided at **opencv-dsp-acceleration/dsp\_opencv/patch**.

Since, we need to change the source code, it is preferred that you configure the OpenCV 2.1 and retain its source and configuration files. Following command can be used to do so.(It is assumed that you are at **OE\_BASE**.)

> ` # ` bitbake -f -c configure opencv

> The above command will unpack and configure OpenCV 2.1 at **`OE_BASE/build/tmp-angstrom_2008_1/work/armv7a-angstrom-linux-gnueabi/opencv-2.1.0+svnr3217-r1`**. Lets call this **OPENCV\_BASE**.

> Note:- Name of folder `opencv-2.1.0+svnr3217-r1` can vary depending on svn revision. In this case the numbers after +svnr can be different.

Now change directory to **OPENCV\_BASE**.ie, `OE_BASE/build/tmp-angstrom_2008_1/work/armv7a-angstrom-linux-gnueabi/opencv-2.1.0+svnr3217-r1` .

> ` # ` cd `OE_BASE/build/tmp-angstrom_2008_1/work/armv7a-angstrom-linux-gnueabi/opencv-2.1.0+svnr3217-r1`

> Execute following commands to apply the patch

> ` # ` patch -p0 < OE\_BASE/opencv-dsp-acceleration/dsp\_opencv/patch/opencvdsp.diff

> Execute following command to add include directory of CMEM library to the CXX\_FLAGS in the files OPENCV\_BASE/modules/core/CMakeFiles/opencv\_core.dir/flags.make and OPENCV\_BASE/modules/core/CMakeFiles/opencv\_core.dir/flags.make.

> ` # ` sed -i '/^CXX\_FLAGS/s:$: -I**`<OE_BASE>`**/build/tmp-angstrom\_2008\_1/sysroots/beagleboard-angstrom-linux-gnueabi/usr/share/ti/ti-linuxutils-tree/packages/ti/sdo/linuxutils/cmem/include:g' opencv/modules/core/CMakeFiles/opencv\_core.dir/flags.make

> Similarly,

> ` # ` sed -i '/^/s:$: **`<OE_BASE>`**/build/tmp-angstrom\_2008\_1/sysroots/beagleboard-angstrom-linux-gnueabi/usr/share/ti/ti-linuxutils-tree/packages/ti/sdo/linuxutils/cmem/lib/cmem.a470MV :g' opencv/modules/core/CMakeFiles/opencv\_core.dir/link.txt

> NOTE: Don't forget the space after '$:' in the commands above. Also expand **OE\_BASE** to real path.

> Now, change the directory to OE\_BASE and then bitbake opencv

> ` # ` cd OE\_BASE

> ` # ` bitbake opencv

Note: The above patch effects only **libopencv\_core.so, libopencv\_core.so.2.1 and libopencv\_core.so.2.1.1**. If you already have a working copy of opencv, you can copy only these library files related to libopencv\_core to your beagleboard at **/usr/lib**. These libraries can be found at OPENCV\_BASE/lib.


If you don't hava a working copy of OpenCV 2.1, install all OpenCV packages generated by bitbake on Beagleboard.


# Step 4 #
### Building This Project ###

To build this project, go to the folder you just checkedout from SVN. ie,

> `#`cd OE\_BASE/opencv-dsp-acceleration/dsp\_opencv

Edit file Rules.make to point to the correct path for each tools. In most of the case editing varialble **ROOTHPATH** is sufficient, if you have followed the method described in step 2 to install Codec-engine and tools. Also verify the path of cross-compiler in the variable **CROSS\_COMPILE** to point to correct path.
It is advisable that you edit **EXEC\_DIR** variable to point to the folder where you want all of the executables and test files to be saved after the project is built. If you are using NFS to share folder with Beagleboard, the EXEC\_DIR variable can also be the path to this folder.

**Note**: If you have VLIB and CXCORE library, you need to uncomment the line in file **Makefile** at **OE\_BASE/opencv-dsp-acceleration/dsp\_opencv/beagle/c6accel\_1\_00\_00\_04\_async/dsp/alg/pjt**. Which can be done as shown below.

**Before**
```
# uncomment the lines below if you are using VLIB. The library file must be stored at [c6accel_Base_Dir]/dsp/libs
#VLIB = -l"../../libs/vlib.l64p"

# uncomment the lines below if you are using CXCORE. The library file must be stored at [c6accel_Base_Dir]/dsp/libs
#CXCORE = -l"../../libs/cxcore.lib"
```

**After**
```
# uncomment the lines below if you are using VLIB. The library file must be stored at [c6accel_Base_Dir]/dsp/libs
VLIB = -l"../../libs/vlib.l64p"

# uncomment the lines below if you are using CXCORE. The library file must be stored at [c6accel_Base_Dir]/dsp/libs
CXCORE = -l"../../libs/cxcore.lib"
```

Now,

Execute following command to build the project

> `#`**make all**

This command will build the DSP executable **c6accel\_omap3530.x64P**, Arm-side shared library **lib/libopencvdsp.so** and Arm executable **remote\_ti\_platforms\_evm3530\_opencv.xv5T**. The two executables and lib folder will be located at **EXEC\_DIR**. Now,copy **c6accel\_omap3530.x64P, remote\_ti\_platforms\_evm3530\_opencv.xv5T** to the **Beagleboard**  at your work directory. Lets call this directory **WORK\_DIR**. Copy **libopencvdsp.so** to the Beagleboard at **/usr/lib**. Also, copy the movie clip **tree.avi** found on the EXEC\_DIR folder to the **WORK\_DIR**.


# Step 5 #
### Restart Beagleboard and Set-up Memory ###

### Set Memory for Linux ###

Before booting up the beagleboard, set aside memory for linux from uboot prompt as shown below.
> OMAP3 beagleboard.org # setenv bootargs console=${console} vram=${vram} mapfb.mode=dvi:${dvimode} omapfb.debug=y omapdss.def\_disp=${defaultdisplay} root=${mmcroot} rootfstype=${mmcrootfstype} mem=80M@0x80000000 128M@0x88000000

### Copy Kernel Modules for DSPLink ###

Copy the kernel modules to the **WORK\_DIR** on Beagleboard.
These modules can be found at **OE\_BASE/build/tmp-angstrom\_2008\_1/sysroots/beagleboard-angstrom-linux-gnueabi/lib/modules/2.6.32/kernel/drivers/dsp**.

Load all the kernel modules, dsplinkk.ko, lpm\_omap3530.ko and cmemk.ko.

You can load these modules using the script **loadmodules.sh** provided in the **OE\_BASE/opencv-dsp-acceleration/dsp\_opencv/scripts**. To unload the modules, **unloadmodules.sh** is provided in the same folder. Copy these files to your WORK\_DIR in Beagleboard and type following
> ` $ ` ./loadmoudles.sh

You can check whether the modules are loaded or not by using **lsmod** command.

Also, it is suggested that you load the **uvc** module since the application for demonstration of this project uses webcam.

# Step 6 #
### Execute ###
Type the following command at the command prompt on Beagleboard to run the application. This will show the usage of the command. You can then follow the usage after that.

> ` $ `cd WORK\_DIR

> ` $ `**./remote\_ti\_platforms\_evm3530\_opencv.xv5T**


## Note ##
1) To test the example you may need to copy the test data file **tree.avi** as mentioned above or need to have a webcam. I have tested the app using Logitech webcam Pro 9000 to demonstrate Sobel algorithm.

2) The example demonstrates 2-D 63x63 forward DFT. The output data is displayed on the screen.

3) The example also demonstrates 3x3 Sobel algorithm for frames captured using webcam. Due to low visibility of signed-image when using cvShowImage(), the image is converted back to unsigned and then displayed.

5) The example also demonstrates 3x3 Sobel algorithm for input movie clip tree.avi.

6) Integral algorithm is also demonstrated. You need to install **VLIB** for DSP based cvIntegral()
