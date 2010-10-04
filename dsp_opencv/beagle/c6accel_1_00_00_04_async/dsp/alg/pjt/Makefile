include ../../../Rules.make

# Comment this out if you want to see full compiler and linker output.
#VERBOSE = @

# uncomment the lines below if you are using VLIB. The library file must be stored at [c6accel_Base_Dir]/dsp/libs
#VLIB = -l"../../libs/vlib.l64p"

# uncomment the lines below if you are using CXCORE. The library file must be stored at [c6accel_Base_Dir]/dsp/libs
CXCORE =  -l"../../libs/cv.lib" -l"../../libs/cxcore.lib" -l"../../libs/fastrts64x.lib"

# Which compiler flags should be used?
BUILD_TYPE=release

C_FLAGS += -mo -i"$(XDC_INSTALL_DIR)/packages" -i"$(FC_INSTALL_DIR)/packages" -i"$(XDAIS_INSTALL_DIR)/packages" -i"$(CODEGEN_INSTALL_DIR)/include" -i"../include" -i"../../libs/include" -d"_DEBUG" -d"xdc_target__=<ti/targets/std.h>" --mem_model:data=far

LD_FLAGS = -z -l lnk.cmd -r -h -g _C6ACCEL_TI_IALG -g _UNIVERSAL_TI_PARAMS -g _C6ACCEL_TI_IC6ACCEL  



ifeq ($(DSP_ISA),64P)
EXT		= 64P
#VLIB has been added by Pramod
LD_LIBS = -l"../../libs/dsplib64plus.lib" -l="../../libs/C64P_LIBPLUS.lib" -l="../../libs/fastrts64x.lib" -l"../../libs/imglib2.l64P" -l"../../libs/IQmath_c64x+.lib" -l"../../libs/IQmath_RAM_c64x+.lib"
ifdef VLIB 
LD_LIBS        += $(VLIB)
endif
ifdef CXCORE 
LD_LIBS        += $(CXCORE)
endif
ifeq ($(BUILD_TYPE), release)
LTARGET		= ../lib/C6accel_c64P/Release/C6Accel.l64P
ATARGET		= ../lib/C6accel_c64P/Release/C6Accel.a64P
C_FLAGS	       += -mv6400+ -o3 -eoo$(EXT)
ifdef VLIB 
C_FLAGS        += -DUSE_VLIB
endif
ifdef CXCORE 
C_FLAGS        += -DUSE_CXCORE
endif
OBJDIR		= c6accel_c64P_release
PARTIAL		= $(OBJDIR)/c6accel_c64p_release.o64P
else
LTARGET		= ../lib/C6accel_c64P/Debug/C6Accel.l64P
ATARGET		= ../lib/C6accel_c64P/Debug/C6Accel.a64P
C_FLAGS	       += -mv6400+ -g -eoo$(EXT)
ifdef VLIB 
C_FLAGS        += -DUSE_VLIB
endif
ifdef CXCORE 
C_FLAGS        += -DUSE_CXCORE
endif
OBJDIR		= c6accel_c64P_debug
PARTIAL		= $(OBJDIR)/c6accel_c64p_debug.o64P
endif
endif

ifeq ($(DSP_ISA),674)
EXT		= 674
C_FLAGS        += -DDEVICE_FLOAT 
#VLIB has been added by Pramod
LD_LIBS = -l"../../libs/dsplib64plus.lib" -l="../../libs/C64P_LIBPLUS.lib" -l="../../libs/fastrts64x.lib" -l"../../libs/imglib2.l64P" -l"../../libs/IQmath_c64x+.lib" -l"../../libs/IQmath_RAM_c64x+.lib" -l"../../libs/fastrts67x.lib" -l"../../libs/dsplib674x.lib"
ifdef VLIB 
LD_LIBS        += $(VLIB)
endif
ifdef CXCORE 
LD_LIBS        += $(CXCORE)
endif
ifeq ($(BUILD_TYPE), release)
LTARGET		= ../lib/C6accel_c674/Release/C6Accel.l674
ATARGET		= ../lib/C6accel_c674/Release/C6Accel.a674
C_FLAGS	       += -mv6740 -o3 -eoo$(EXT)
ifdef VLIB 
C_FLAGS        += -DUSE_VLIB
endif
ifdef CXCORE 
C_FLAGS        += -DUSE_CXCORE
endif
OBJDIR		= c6accel_c674_release
PARTIAL		= $(OBJDIR)/c6accel_c674_release.o674
else
LTARGET		= ../lib/C6accel_c674/Debug/C6Accel.l674
ATARGET		= ../lib/C6accel_c674/Debug/C6Accel.a674
C_FLAGS	       += -mv6740 -g -eoo$(EXT)
ifdef VLIB 
C_FLAGS        += -DUSE_VLIB
endif
ifdef CXCORE 
C_FLAGS        += -DUSE_CXCORE
endif
OBJDIR		= c6accel_c674_debug
PARTIAL		= $(OBJDIR)/c6accel_c674_debug.o674
endif
endif

COMPILE.c	= $(VERBOSE) $(CODEGEN_INSTALL_DIR)/bin/cl6x $(C_FLAGS)
LD.c		= $(VERBOSE) $(CODEGEN_INSTALL_DIR)/bin/cl6x $(LD_FLAGS)
AR.c		= $(VERBOSE) $(CODEGEN_INSTALL_DIR)/bin/ar6x $(AR_FLAGS)

HEADERS		=  $(wildcard ../include/*.h)
SOURCES		=  $(wildcard ../src/*.c)
OBJFILES	=  $(addprefix $(OBJDIR)/,$(SOURCES:../src/%.c=%.o$(EXT)))

.PHONY: clean install all

all:	$(LTARGET) $(ATARGET)

# No install required, it needs to be copied to packages/ti/c6accel/lib as build so ap can consume it
#install:	$(if $(wildcard $(ATARGET)), $(ATARGET)_install) \
#		$(if $(wildcard $(LTARGET)), $(LTARGET)_install)

#$(ATARGET)_install:
#	install $(ATARGET) ../../../soc/packages/ti/c6accel/lib/

#$(LTARGET)_install:
#	install $(LTARGET) ../../../soc/packages/ti/c6accel/lib/


# add following line to this rule to invoke wizard
# 	$(XDC_INSTALL_DIR)/xs --xdcpath="$(CE_INSTALL_DIR)/packages;$(XDAIS_INSTALL_DIR)/packages" ti.sdo.ce.wizards.gencodecpkg
# but it currently ce 2.25 can't handle IDMA so no way to automate, so we just copy lib to existing codec package.
# this existing codec package has had ce/C6ACCEL.xdc manually edited to add idma_fxns
$(LTARGET):	$(PARTIAL)
	@echo
	@echo Archiving $@ from $^..
	@mkdir -p $(dir $@)
	$(AR.c) -rq $@ $^
	@cp $@ ../../../soc/packages/ti/c6accel/lib/

$(ATARGET):	$(OBJFILES)
	@echo
	@echo Archiving $@ from $^..
	@mkdir -p $(dir $@)
	$(AR.c) -r $@ $^
	@cp $@ ../../../soc/packages/ti/c6accel/lib/

$(PARTIAL):	$(OBJFILES)
	@echo
	@echo Linking $@ from $^..
	@mkdir -p $(dir $@)
	$(LD.c) -o $@ $^ $(LD_LIBS)

$(OBJFILES): $(OBJDIR)/%.o$(EXT): ../src/%.c $(HEADERS)
	@echo Compiling $@ from $<..
	@mkdir -p $(dir $@)
	$(COMPILE.c) -c $< -fr"$(OBJDIR)"

clean:
	@echo Removing generated files..
	$(PRE) -$(RM) -rf $(LTARGET) $(ATARGET) $(PARTIAL) $(OBJFILES) *~
