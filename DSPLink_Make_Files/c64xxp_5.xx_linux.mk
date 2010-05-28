#   ============================================================================
#   @file   c64xxp_5.xx_linux.mk
#
#   @path   $(DSPLINK)/make/DspBios/
#
#   @desc   This makefile defines OS specific macros used by MAKE system for
#           the DSP/BIOS version 5.xx with CGTOOLS C64XXP on linux.
#
#   @ver    1.64
#   ============================================================================
#   Copyright (C) 2002-2009, Texas Instruments Incorporated -
#   http://www.ti.com/
#
#   Redistribution and use in source and binary forms, with or without
#   modification, are permitted provided that the following conditions
#   are met:
#   
#   *  Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#   
#   *  Redistributions in binary form must reproduce the above copyright
#      notice, this list of conditions and the following disclaimer in the
#      documentation and/or other materials provided with the distribution.
#   
#   *  Neither the name of Texas Instruments Incorporated nor the names of
#      its contributors may be used to endorse or promote products derived
#      from this software without specific prior written permission.
#   
#   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
#   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
#   PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
#   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
#   OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
#   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
#   OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
#   EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#   ============================================================================


ifndef C64XXP_5_XX_LINUX_MK

define C64XXP_5_XX_LINUX_MK
endef


#   ============================================================================
#   Let the make system know that a specific distribution for the GPP OS
#   is being used.
#   ============================================================================
USE_DISTRIBUTION := 1


#   ============================================================================
#   Set the values of necessary variables to be used for the OS.
#   ============================================================================

#   ----------------------------------------------------------------------------
#   Base directory for the DSP OS
#   ----------------------------------------------------------------------------
BASE_INSTALL    := "$(TI_TOOLS_BASE_DIR)"
ifeq ($(BASE_INSTALL), "")
BASE_INSTALL    := /media/disk/OE/build/tmp-angstrom_2008_1/sysroots/beagleboard-angstrom-linux-gnueabi/usr/share/ti
endif
BASE_SABIOS     := "$(BIOS_INSTALL_DIR)"
ifeq ($(BASE_SABIOS), "")
BASE_SABIOS     := $(BASE_INSTALL)/ti-dspbios-tree
endif
BASE_BUILDOS    := $(BASE_SABIOS)/packages/ti/bios

#   ----------------------------------------------------------------------------
#   Base directory for the XDC tools
#   ----------------------------------------------------------------------------
XDCTOOLS_DIR    := "$(XDC_INSTALL_DIR)"
ifeq ($(XDCTOOLS_DIR), "")
XDCTOOLS_DIR    := $(BASE_INSTALL)/ti-xdctools-tree
endif

#   ----------------------------------------------------------------------------
#   Base for code generation tools - compiler, linker, archiver etc.
#   ----------------------------------------------------------------------------
BASE_CGTOOLS    := "$(CODEGEN_INSTALL_DIR)"
ifeq ($(BASE_CGTOOLS), "")
BASE_CGTOOLS    := $(BASE_INSTALL)/ti-cgt6x-tree
endif
BASE_CGTOOLSBIN := $(BASE_CGTOOLS)/bin

#   ----------------------------------------------------------------------------
#   Base for TCONF, platform files and dependent components
#   ----------------------------------------------------------------------------
BASE_TCONF      := $(XDCTOOLS_DIR)
BASE_PLATFORMS  := $(BASE_SABIOS)/packages
BASE_PSL        := $(BASE_SABIOS)/packages/ti/psl
BASE_CSL        :=
BASE_RTDX       := $(BASE_SABIOS)/packages/ti/rtdx

#   ----------------------------------------------------------------------------
#   Base directory for include files
#   ----------------------------------------------------------------------------
BASE_OSINC      := $(BASE_BUILDOS)/include
BASE_CGTOOLSINC := $(BASE_CGTOOLS)/include
BASE_RTDXINC    := $(BASE_RTDX)/include/c6000
BASE_PSLINC     := $(BASE_PSL)/include
BASE_CSLINC     :=

OSINC_GENERIC   := $(BASE_OSINC)
OSINC_PLATFORM  := $(BASE_CGTOOLSINC) $(BASE_RTDXINC) \
                   $(BASE_PSLINC) $(BASE_CSLINC)

ifneq ("$(VARIANT)", "")
OSINC_VARIANT   := $(BASE_OSINC)
endif


#   ----------------------------------------------------------------------------
#   Base directory for libraries
#   ----------------------------------------------------------------------------
BASE_OSLIB      := $(BASE_BUILDOS)/lib
BASE_CGTOOLSLIB := $(BASE_CGTOOLS)/lib
BASE_RTDXLIB    := $(BASE_RTDX)/lib/c6000
BASE_PSLLIB     := $(BASE_PSL)/lib
BASE_CSLLIB     :=

OSLIB_GENERIC   := $(BASE_OSLIB)
OSLIB_PLATFORM  := $(BASE_CGTOOLSLIB) $(BASE_RTDXLIB) \
                   $(BASE_PSLLIB) $(BASE_CSLLIB)

ifneq ("$(VARIANT)", "")
OSLIB_VARIANT   := $(BASE_OSLIB)
endif


#   ============================================================================
#   COMPILER
#   ============================================================================

#   ----------------------------------------------------------------------------
#   Name of the compiler
#   ----------------------------------------------------------------------------
COMPILER        := $(BASE_CGTOOLSBIN)/cl6x

#   ----------------------------------------------------------------------------
#   Command line switches used by the compiler
#
#   CC_SW_DEF       Command line defines
#   CC_SW_INC       Search path for header files
#   CC_SW_OBJ       Object file directory
#   CC_SW_DEB       Include debug information
#   CC_SW_REL       Release build
#   ----------------------------------------------------------------------------
CC_SW_DEF       := -d
CC_SW_INC       := -I
CC_SW_OBJ       := -fr
CC_SW_DEB       := -g -d"_DEBUG" --no_compress
CC_SW_REL       := -o3

#   ----------------------------------------------------------------------------
#   Standard flags for the compiler
#   ----------------------------------------------------------------------------
STD_CC_FLAGS    := -q -pdr -pdv -pden -ml3 -mv6400+ --disable:sploop

#   ----------------------------------------------------------------------------
#   Standard flags for the compiler when building an executable
#   ----------------------------------------------------------------------------
EXE_CC_FLAGS    :=

#   ----------------------------------------------------------------------------
#   Flags for the compiler when building an archive
#   ----------------------------------------------------------------------------
ARC_CC_FLAGS    :=

#   ----------------------------------------------------------------------------
#   Standard definitions for the compiler
#   ----------------------------------------------------------------------------
STD_CC_DEFNS    :=


#   ============================================================================
#   ARCHIVER
#   ============================================================================
ARCHIVER        := $(BASE_CGTOOLSBIN)/ar6x

#   ----------------------------------------------------------------------------
#   Standard flags for the archiver
#   ----------------------------------------------------------------------------
STD_AR_FLAGS    := -r

#   ----------------------------------------------------------------------------
#   Archiver flags for extracting object files
#   ----------------------------------------------------------------------------
EXT_AR_FLAGS    := xq


#   ============================================================================
#   LINKER
#   ============================================================================
LINKER      := $(BASE_CGTOOLSBIN)/cl6x -z

#   ----------------------------------------------------------------------------
#   Command line switches used by the linker
#
#   LD_SW_INC       Search path for libraries
#   LD_SW_LIB       Include library name
#   LD_SW_OUT       Output file name
#   LD_SW_MAP       Map file name
#   LD_SW_RELOC     Generate relocateable output
#   ----------------------------------------------------------------------------
LD_SW_INC       := -i
LD_SW_LIB       := -l
LD_SW_OUT       := -o
LD_SW_MAP       := -m
LD_SW_RELOC     := -r

#   ----------------------------------------------------------------------------
#   Standard flags for the linker
#   ----------------------------------------------------------------------------
STD_LD_FLAGS    := -c -q -x

#   ----------------------------------------------------------------------------
#   Flags for the linker when building an executable
#   ----------------------------------------------------------------------------
EXE_LD_FLAGS    :=

#   ============================================================================
#   TCONF
#   ============================================================================
TCONF           := $(BASE_TCONF)/tconf


#   ----------------------------------------------------------------------------
#   Standard flags for TCONF
#   ----------------------------------------------------------------------------
STD_TCF_FLAGS   :=


endif   # ifndef C64XXP_5_XX_LINUX_MK
