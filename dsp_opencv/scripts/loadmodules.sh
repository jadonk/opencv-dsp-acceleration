#
#  Copyright (c) 2008, Texas Instruments Incorporated
#  All rights reserved.
# 
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions
#  are met:
# 
#  *  Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
# 
#  *  Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
# 
#  *  Neither the name of Texas Instruments Incorporated nor the names of
#     its contributors may be used to endorse or promote products derived
#     from this software without specific prior written permission.
# 
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
#  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
#  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
#  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
#  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
#  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
#  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
#  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

#
# Default Memory Map
#
# Start Addr    Size    Description
# -------------------------------------------
# 0x40200000    60 KB   CMEM (internal memory, see comment below)
# 0x80000000    80 MB   Linux
# 0x85000000    16 MB   CMEM (external memory)
# 0x86000000    24 MB   DDRALGHEAP
# 0x87800000     6 MB   DDR2 (BIOS, Codecs, Applications)
# 0x87E00000     1 MB   DSPLINK (MEM)
# 0x87F00000     4 KB   DSPLINK (RESET)
# 0x87F01000  1020 KB   unused


# insert CMEM driver, using one block of memory
#
# Note: phys_end = (addr + 1)
#
# block 0: 0x85000000 - 0x85FFFFFF (16 MB), External Memory
# pool:    80 KB, 20 x 4 KB buffers
# pool:  1280 KB, 10 x 128 KB buffers
# pool:  2048 KB, 2 x 1 MB buffers
# heap: 12976 KB, (~12.67 MB)
#
#CMEM_MODPARAMS="phys_start=0x85000000 phys_end=0x86000000 pools=20x4096,10x131072,3x1448576"
CMEM_MODPARAMS="phys_start=0x88000000 phys_end=0x89000000 pools=20x4096,10x131072,3x1448576"

# insert CMEM driver, using two blocks of memory
#
# To use this command, the kernel must be configured such that it does not
# use the internal memory located at 0x40200000 - 0x4020EFFF.
#
# block 1: 0x40200000 - 0x4020EFFF (60 KB), Internal Memory
# pool: 60 KB, 15 x 4 KB buffers (4 KB minimum size buffer allowed)
#
#CMEM_MODPARAMS="$CMEM_MODPARAMS phys_start_1=0x40200000 phys_end_1=0x4020F000 pools_1=15x4096 allowOverlap=1"

if [ -e cmemk.ko ]
    then
        insmod cmemk.ko $CMEM_MODPARAMS
    else
        modprobe cmemk $CMEM_MODPARAMS
fi

# Allow cmem driver to be used by all users
if [ -e /dev/cmem ]
    then
        chmod 666 /dev/cmem
fi

# insert DSP/BIOS Link driver
if [ -e dsplinkk.ko ]
    then
        insmod dsplinkk.ko
    else
        modprobe dsplinkk
fi

# Allow dsplink driver to be used by all users
if [ -e /dev/dsplink ]
    then
        chmod 666 /dev/dsplink
fi

# insert Local Power Manager driver
if [ -e lpm_omap3530.ko ]
    then
        insmod lpm_omap3530.ko
    else
        modprobe lpm_omap3530
fi

# Allow lpm driver to be used by all users
if [ -e /dev/lpm0 ]
    then
        chmod 666 /dev/lpm*
fi

