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
/* ======================================================================= */
/*  TEXAS INSTRUMENTS, INC.                                                */
/*                                                                         */
/*  NAME                                                                   */
/*      IMG_clipping_16s : Thresholding Values of a matrix                 */
/*                      Intrinsic C Implementation                         */
/*  USAGE                                                                  */
/*      This routine is C-callable and has the prototype                   */
/*                                                                         */
/*      void IMG_clipping_16s                                              */
/*      (                                                                  */
/*          const short *restrict x,    // Input matrix Pointer         // */
/*          short              rows,    // Height of input matrix       // */
/*          short              cols,    // Width of input matrix        // */
/*          short       *restrict r,    // Output matrix Pointer        // */
/*          short         THRES_MAX,    // User-Defined THRES_MAX value // */
/*          short         THRES_MIN     // User-Defined THRES_MIN value // */
/*      )                                                                  */
/*                                                                         */
/*  DESCRIPTION                                                            */
/*      The function truncates elements of a matrix to the maximum and     */
/*      minimum values defined by the user. Each element is 16-bit signed  */
/*      and the size of the matrix is user determined dimensions. The      */
/*      output matrix has the same size as that of the input matrix and    */
/*      each value will be truncated to minimum or maximum value defined   */
/*      by user based on whether it is lesser than the minimum value       */
/*      or greater than the maximum value respectively.                    */
/*                                                                         */
/*      The natural C implementation has no restrictions.The Optimized and */
/*      IntrinsiC codes has restrictions as noted in the the ASSUMPTIONS   */
/*      below.                                                             */
/*                                                                         */
/*  ASSUMPTIONS                                                            */
/*      1. (rows * cols) >= 8 and should be a  multiples of 8              */
/*      2. THRES_MAX >= THRES_MIN                                          */
/*      3. Input and output arrays are double word - aligned.              */
/*      4. Input and output arrays should not overlap.                     */
/*                                                                         */
/*  COMPATIBILITY                                                          */
/*      This code is compatible for both C64x+ and C64x processors         */
/*                                                                         */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */

#ifndef IMG_CLIPPING_16S_I_H_
#define IMG_CLIPPING_16S_I_H_ 1

void IMG_clipping_16s
(
    const short *restrict x,        /* Input Matrix Pointer                */
    short              rows,        /* Height of input matrix              */
    short              cols,        /* Width of input matrix               */
    short       *restrict r,        /* Output matrix Pointer               */
    short         THRES_MAX,        /* Maximum Threshold Value             */
    short         THRES_MIN         /* Minimum Threshold Value             */
);

#endif
/* ======================================================================= */
/*  End of file:  IMG_clipping_16s_i.h                                     */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */
