/* ======================================================================== */
/*  TEXAS INSTRUMENTS, INC.                                                 */
/*                                                                          */
/*  IMGLIB-2  DSP Image/Video Processing Library                            */
/*                                                                          */
/*      Release:        Revision 2.0.1                                      */
/*      Snapshot date:  6-May-2008                                          */
/*                                                                          */
/*  This library contains proprietary intellectual property of Texas        */
/*  Instruments, Inc.  The library and its source code are protected by     */
/*  various copyrights, and portions may also be protected by patents or    */
/*  other legal protections.                                                */
/*                                                                          */
/*  This software is licensed for use with Texas Instruments TMS320         */
/*  family DSPs.  This license was provided to you prior to installing      */
/*  the software.  You may review this license by consulting the file       */
/*  TI_license.PDF which accompanies the files in this library.             */
/* ------------------------------------------------------------------------ */
/*          Copyright (C) 2008 Texas Instruments, Incorporated.             */
/*                          All Rights Reserved.                            */
/* ======================================================================== */
/* ======================================================================== */
/*                                                                          */
/*  TEXAS INSTRUMENTS, INC.                                                 */
/*                                                                          */
/*  NAME                                                                    */
/*      IMG_boundary_16s -- Returns coordinates of boundary pixels(16 bit). */
/*                                                                          */
/*  USAGE                                                                   */
/*      This routine is C callable, and is called as follows:               */
/*                                                                          */
/*          void IMG_boundary_16s                                           */
/*          (                                                               */
/*              const  short *restrict  i_data,                             */
/*                       int              rows,                             */
/*                       int              cols,                             */
/*              unsigned int *restrict o_coord,                             */
/*                     short *restrict  o_grey                              */
/*          );                                                              */
/*                                                                          */
/*      The arguments are defined as follows:                               */
/*                                                                          */
/*          i_data   Input images that is cols-by-rows in size.             */
/*          rows     Height of the input image                              */
/*          cols     Width of the input image                               */
/*          o_coord  Array to write output coordinates to                   */
/*          o_grey   Array to write output grey levels to                   */
/*                                                                          */
/*  DESCRIPTION                                                             */
/*      This routine scans an image looking for non-zero pixels.            */
/*      The locations of those pixels are stored out to the o_coord         */
/*      as packed Y/X pairs, with Y in the upper half, and X in             */
/*      the lower half.  The grey levels encountered are stored             */
/*      in the o_grey array in parallel.                                    */
/*                                                                          */
/*      The natural C implementation has no restrictions. The Optimized     */
/*      and IntrinsiC codes has restrictions as noted in the ASSUMPTIONS    */
/*      below.                                                              */
/*                                                                          */
/*  ASSUMPTIONS                                                             */
/*      1. Rows should minimum of 1 and maximum of 32767.                   */
/*      2. Cols should be a multiple of 4 ,minimum of 4 and max of 32764.   */
/*      3. Input and output arrays should not overlap.                      */
/*      4. Input must be double word aligned.                               */
/*      5. Output arrays should be word alligned.                           */
/*                                                                          */
/*  TECHNIQUES                                                              */
/*      Outer and inner loops are merged.                                   */
/*      "Inner loop" is unrolled to process four pixels per iteration.      */
/*      Packed coordinate value is updated directly, rather than            */
/*      by repacking x, y every time it's needed.                           */
/*                                                                          */
/*  COMPATIBILITY                                                           */
/*      This code is compatible with both c64x+ and c64x  processors        */
/*                                                                          */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */

#ifndef IMG_BOUNDARY_16S_I_H_
#define IMG_BOUNDARY_16S_I_H_ 1

void IMG_boundary_16s
(
    const  short *restrict  i_data,
             int              rows,
             int              cols,
    unsigned int *restrict o_coord,
           short *restrict  o_grey
);

#endif
/* ======================================================================== */
/*  End of file:  IMG_boundary_16s_i.h                                      */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */
