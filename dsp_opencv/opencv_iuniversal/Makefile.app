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

# The XDC configuration tool command line
CONFIGURO               = $(XDC_INSTALL_DIR)/xs xdc.tools.configuro

GNU_C_FLAGS             = $(C_FLAGS) -Wall -g 
C64P_C_FLAGS            = $(C_FLAGS) -g 
GNU_CPP_FLAGS           = $(CPP_FLAGS) -I$(LINUXKERNEL_INSTALL_DIR)/include 
C64P_CPP_FLAGS          = $(CPP_FLAGS) -pdse225 -I$(TI6X_CGTOOL_DIR)/include -mv6400+ 

GNU_LD_FLAGS            = $(LD_FLAGS) -lpthread -lm 
C64P_LD_FLAGS           = $(LD_FLAGS) -z -w -x -c -i$(RTDX_INSTALL_DIR)/packages/ti/rtdx/iom/lib/debug -i$(RTDX_INSTALL_DIR)/packages/ti/rtdx/cio/lib/release -i$(RTDX_INSTALL_DIR)/packages/ti/rtdx/lib/c6000 

SOURCES                 = $(wildcard *.c)
LINUX_SOURCES           = $(SOURCES) $(wildcard linux/*.c)
BIOS_SOURCES            = $(SOURCES) $(wildcard bios/*.c)
HEADERS                 = $(wildcard *.h)

LINUX_OBJFILES = $(LINUX_SOURCES:%.c=%.$(PLATFORM_SHORT).o$(APP_EXT))

ARM_COMPILE.c = $(PRE) $(CROSS_COMPILE)gcc $(GNU_C_FLAGS) $(GNU_CPP_FLAGS) -c
ARM_LINK.c = $(PRE) $(CROSS_COMPILE)gcc $(GNU_LD_FLAGS)
C64P_COMPILE.c = $(PRE) $(CODEGEN_INSTALL_DIR)/bin/cl6x $(C64P_C_FLAGS) $(C64P_CPP_FLAGS) -c
C64P_LINK.c = $(PRE) $(CODEGEN_INSTALL_DIR)/bin/cl6x $(C64P_LD_FLAGS)

all: $(if $(wildcard $(XDC_CFGFILE)), $(XDC_EXEC))

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
	-$(RM) -rf cfg $(LINUX_OBJFILES) $(XDC_EXEC) $(MAP) *~ *.d .dep
