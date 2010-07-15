LICENSE = "BSD"
DEPENDS = "ti-codec-engine ti-dsplib"

PV = "0+svnr${SRCPV}"
SRCREV = "113"

SRC_URI = "svn://opencv-dsp-acceleration.googlecode.com/svn/;proto=http;module=trunk \
           http://focus.ti.com/lit/sw/sprc264/sprc264.zip;name=imglib \
"

SRC_URI[imglib.md5sum] = "97dca3f1612d7c35b3d3fe9f32ab3e0c"
SRC_URI[imglib.sha256sum] = "bf22c58cf81362ec3e7ed2d8debc39f00d1bfd0343094401cce2f928830858c1"


S = "${WORKDIR}/trunk/dsp_opencv"

require recipes/ti/ti-paths.inc
require recipes/ti/ti-staging.inc

export CROSS_COMPILE = "${TOOLCHAIN_PATH}/bin/${TARGET_PREFIX}"
export LINUXKERNEL_INSTALL_DIR = "${STAGING_KERNEL_DIR}"
export OPENCV_PATH = "${STAGING_INCDIR}"
export DSPLIB_PATH = "${DSPLIB_INSTALL_DIR}"


# You need to extract the imglib from the .exe manually and place it in /OE/tmp (or somewhere else and change the patch below)
export IMGLIB_PATH = "${OE_BASE}/build/tmp-angstrom_2008_1/sysroots/beagleboard-angstrom-linux-gnueabi/usr/share/ti/imglib_v201"

TARGET_CC_ARCH += "${LDFLAGS}"

do_configure() {

#DSPLIB uses '\' instead of '/' as directory seperator. Correct it

	sed -i -e 's:\\:\/:g' ${DSPLIB_PATH}/dsplib64plus.h

	echo "-l ${DSPLIB_INSTALL_DIR}/dsplib64plus.lib" >> ${S}/beagle/server/opencv_unitserver_evm3530/link.cmd
	echo "-l ${IMGLIB_PATH}/lib/target/imglib2.l64P" >> ${S}/beagle/server/opencv_unitserver_evm3530/link.cmd

	sed -e /DIR/d -e /PATH/d -i Rules.make

	for i in $(find ${S} -name "*.mak" ; find ${S} -name "*.cmd") ; do
		sed -i s:/media/disk/OE/work/ti/dsplib_v210:${DSPLIB_PATH}:g $i
		sed -i s:/media/disk/OE/work/ti/imglib_v201:${IMGLIB_PATH}:g $i                
	done
        sed -i s:/media/disk/OE/build/tmp-angstrom_2008_1/sysroots/armv7a-angstrom-linux-gnueabi/usr/include:${OPENCV_PATH}:g Makefile.app
	sed -i s:/media/disk/OE/build/tmp-angstrom_2008_1/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib:${OE_BUILD_TMPDIR}/sysroots/armv7a-angstrom-linux-gnueabi${libdir}:g Makefile
#	sed -i s:/media/disk/OE/build/tmp-angstrom_2008_1/sysroots/armv7a-angstrom-linux-gnueabi/usr/lib:${STAGING_LIBDIR}:g Makefile	


	# Add in build *FLAGS as well	
	for i in $(find ${S} -name "Makefile") ; do
		sed -i -e 's:LD_FLAGS):LD_FLAGS) $(LDFLAGS):g' -e 's:C_FLAGS):C_FLAGS) $(CFLAGS):g' $i
	done
       
        sed -i -e 's:$(LD_FLAGS):$(LD_FLAGS) $(LDFLAGS):g' -e 's:$(C_FLAGS):$(C_FLAGS) $(CFLAGS):g' Makefile.app
}

do_compile() {
	oe_runmake -e codec 
	oe_runmake -e server
	oe_runmake -e library
	oe_runmake -e app
}

do_install() {
	
		
	oe_runmake -e install EXEC_DIR=${D}${datadir}/ti/opencv-dsp
		
}

NATIVE_INSTALL_WORKS=1

FILES_${PN} += "${datadir}"
