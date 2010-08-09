# Where to output configuration files
XDC_CFG                 = cfg/$(TARGET)

# Executable names
XDC_EXEC                = $(TARGET)_$(APP_NAME).x$(APP_EXT)

# Map files
MAP                     = $(XDC_EXEC).map

# Output compiler options
XDC_CFLAGS              = $(XDC_CFG)/compiler.opt

# Output linker file
XDC_LFILE               = $(XDC_CFG)/linker.cmd

# Input configuration files
XDC_CFGFILE             = $(TARGET).cfg

export 

# Path to ARM Linux application library
#AL_LIBS = $(PACKAGE_DIR)/$(SERVER_PATH)/../../library/opencv/lib/libopencvdsp.so
#AL_LIBS = lib/libopencvdsp.so
#AL_LIBS = -lopencvdsp
# Path of OpenCV Header files
OPENCV_PATH             = /media/disk/openembedded/build/tmp-angstrom_2008_1/sysroots/armv7a-angstrom-linux-gnueabi/usr/include

# The XDC configuration tool command line
CONFIGURO               = $(XDC_INSTALL_DIR)/xs xdc.tools.configuro

GNU_C_FLAGS             = $(C_FLAGS) -Wall -g -I$(OPENCV_PATH)
OPENCV_C_FLAGS		= $(GNU_C_FLAGS) -I$(OPENCV_PATH) -I$(CURDIR) -I$(PACKAGE_DIR) -I$(SERVER_DIR) -I$(CE_INSTALL_DIR)/packages -I$(BIOS_INSTALL_DIR)/packages -I$(CODEC_INSTALL_DIR) -I$(FC_INSTALL_DIR)/packages -I$(LINK_INSTALL_DIR) -I$(XDAIS_INSTALL_DIR)/packages -I$(CMEM_INSTALL_DIR)/packages -I$(BIOSUTILS_INSTALL_DIR)/packages -I$(LPM_INSTALL_DIR)/packages -I$(DSPLIB_PATH) -I$(IMGLIB_PATH)
C64P_C_FLAGS            = $(C_FLAGS) -g 
GNU_CPP_FLAGS           = $(CPP_FLAGS) -I$(LINUXKERNEL_INSTALL_DIR)/include 
OPENCV_CPP_FLAGS	= $(GNU_CPP_FLAGS) -I$(OPENCV_PATH)
C64P_CPP_FLAGS          = $(CPP_FLAGS) -pdse225 -I$(TI6X_CGTOOL_DIR)/include -mv6400+ 

GNU_LD_FLAGS            = $(LD_FLAGS) -lpthread -lm 
OPENCV_LD_FLAGS		= $(GNU_LD_FLAGS) -lopencv_ml -lopencv_highgui -lopencv_core -lopencv_calib3d -lopencv_contrib -lopencv_features2d -lopencv_imgproc -lopencv_legacy -lopencv_objdetect -lopencv_video -lopencvdsp -L../../library/opencv/lib
C64P_LD_FLAGS           = $(LD_FLAGS) -z -w -x -c -i$(RTDX_INSTALL_DIR)/packages/ti/rtdx/iom/lib/debug -i$(RTDX_INSTALL_DIR)/packages/ti/rtdx/cio/lib/release -i$(RTDX_INSTALL_DIR)/packages/ti/rtdx/lib/c6000 

SOURCES                 = $(wildcard *.c)
LINUX_SOURCES           = $(SOURCES) $(wildcard linux/*.c)
BIOS_SOURCES            = $(SOURCES) $(wildcard bios/*.c)

HEADERS                 = $(wildcard *.h)
LINUX_HEADERS       = $(HEADERS) $(wildcard linux/*.h)
BIOS_HEADERS        = $(HEADERS) $(wildcard bios/*.h)

#LIB_OBJFILES = $(SOURCES:%.c=%.$(PLATFORM_SHORT).o$(APP_EXT))
LINUX_OBJFILES = $(LINUX_SOURCES:%.c=%.$(PLATFORM_SHORT).o$(APP_EXT))
BIOS_OBJFILES = $(BIOS_SOURCES:%.c=%.$(PLATFORM_SHORT).o$(APP_EXT))


C6ACCEL_LIB = $(SERVER_DIR)/../c6accelw/lib/c6accelw_$(PLATFORM).a470MV
ARM_COMPILE.c = $(PRE) $(CROSS_COMPILE)gcc $(OPENCV_C_FLAGS) $(OPENCV_CPP_FLAGS) -c
ARM_LINK.c = $(PRE) $(CROSS_COMPILE)g++ $(OPENCV_LD_FLAGS) $(C6ACCEL_LIB)
C64P_COMPILE.c = $(PRE) $(CODEGEN_INSTALL_DIR)/bin/cl6x $(C64P_C_FLAGS) $(C64P_CPP_FLAGS) -c
C64P_LINK.c = $(PRE) $(CODEGEN_INSTALL_DIR)/bin/cl6x $(C64P_LD_FLAGS)


all: $(if $(wildcard $(XDC_CFGFILE)), $(XDC_EXEC))

#
#opencv_lib: $(LIB_OBJFILES)
#	@echo
#	@echo ======Building ARM-SIDE library ====
#	@rm -f /lib/opencv_dsp_$(PLATFORM_SHORT).l$(APP_EXT)
#	$(ARM_LINK.c) -shared  -o lib/opencv_dsp_$(PLATFORM_SHORT).l$(APP_EXT) $(wildcard *.o*) -lc
#
#$(LIB_OBJFILES): %.$(PLATFORM_SHORT).o$(APP_EXT): %.c $(HEADERS) $(XDC_CFLAGS)
#	@echo Compiling $@ from $<..
#	$(ARM_COMPILE.c) $(shell cat $(XDC_CFLAGS)) $(GNU_CPP_FLAGS) -o $@ $<
#

$(XDC_EXEC): $(LINUX_OBJFILES) $(XDC_LFILE) $(AL_LIBS)
	@echo 
	@echo Linking $@ from $^..
	$(ARM_LINK.c) -Wl,-Map,$(MAP) -o $@ $^

$(LINUX_OBJFILES): %.$(PLATFORM_SHORT).o$(APP_EXT): %.c $(HEADERS) $(XDC_CFLAGS)
	@echo Compiling $@ from $<..
	$(ARM_COMPILE.c) $(shell cat $(XDC_CFLAGS)) $(GNU_CPP_FLAGS) -o $@ $<

$(XDC_LFILE) $(XDC_CFLAGS): $(XDC_CFGFILE)
	@echo
	@echo ======= Building $(TARGET) =======
	@echo Configuring application using $<
	@echo
	$(PRE) XDCPATH="$(XDCPATH)" $(CONFIGURO) -o $(XDC_CFG) -t $(XDC_ARM_TARGET) -p $(XDC_PLATFORM) -b $(PACKAGE_DIR)/config.bld $(XDC_CFGFILE)

clean:
	@echo Removing generated files...
	-$(RM) -rf cfg $(LINUX_OBJFILES) $(XDC_EXEC) $(MAP) *~ *.d .dep .xdcenv.mak
#	-find cfg -type f -name "*.*" -exec rm -f {} \;
#	-$(RM) -rf $(LINUX_OBJFILES) $(XDC_EXEC) $(MAP) *~ *.d .dep *.lv5T
