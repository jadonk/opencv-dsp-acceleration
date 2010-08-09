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

/* 
    Memory regions.
    If executing on hardware and not simulator, please
    adjust the below definitions to match your target
    memory map.
*/

SECTIONS
{
 .text:algProcess
 .text:algControl
 .text:algAlloc
 .text:algFree
 .text:algInit

 .const:
 {
  "../../libs/fastrts67x.lib"(.data)
 }
 .text:
 {
  "../../libs/fastrts67x.lib"(text)
 }

}

