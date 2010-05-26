/* ======================================================================= */
/* lnk.cmd -- Linker command file                                          */
/*                                                                         */
/* Rev 0.0.1                                                               */
/*                                                                         */
/*                                                                         */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2007 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */

-c
-heap  0x1000
-stack 0x1000
-l rts64plus.lib

/* 
    Memory regions.
    If executing on hardware and not simulator, please
    adjust the below definitions to match your target
    memory map.
*/
MEMORY {
   EXT_MEM     : origin = 0x80000000,  len = 0x8000000
   INT_MEM     : origin = 0x10800000,  len = 0x8000
}

SECTIONS
{
    vectors            :> INT_MEM
    .cinit             :> EXT_MEM
    .text              :> EXT_MEM
    .stack             :> EXT_MEM
    .bss               :> INT_MEM
    .const             :> INT_MEM
    .data              :> INT_MEM
    .far               :> EXT_MEM
    .switch            :> INT_MEM
    .sysmem            :> INT_MEM
    .tables            :> INT_MEM
    .cio               :> INT_MEM
}

