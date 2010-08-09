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
/*  TEXAS INSTRUMENTS, INC.                                                 */
/*                                                                          */
/*  NAME                                                                    */
/*      IMG_corr_3x3_i8_c16s -> Natural C Implementation                    */
/*      3x3 correlation for 8-bit input, 16-bit masks and 32-bit output     */
/*                                                                          */
/*  USAGE                                                                   */
/*      This routine is C-callable and can be called as:                    */
/*                                                                          */
/*          void IMG_corr_3x3_i8_c16s                                       */
/*          (                                                               */
/*              const unsigned char    *restrict  imgin_ptr,                */
/*                    int              *restrict imgout_ptr,                */
/*                    short                           width,                */
/*                    short                           pitch,                */
/*              const short            *restrict   mask_ptr,                */
/*          )                                                               */
/*                                                                          */
/*      imgin_ptr :  pointer to an input  array of 8 bit pixels             */
/*      imgout_ptr:  pointer to an output array of 32 bit pixels            */
/*      pitch     :   No. of columns in the Input Image.                    */
/*      width     :   No. of output pixels to be calculated.                */
/*      mask_ptr  :  pointer to 16 bit signed filter mask.                  */
/*                                                                          */
/*  DESCRIPTION                                                             */
/*      The correlation kernel accepts 3 rows of 'pitch' input pixels       */
/*      and  produces one row of 'width' output pixels using the input      */
/*      mask of of 3x3. This correlation performs a point by point          */
/*      multiplications of 3x3 mask with the input image. The result of     */
/*      the 9 multiplications are then summed together to produce a         */
/*      32-bit sum  and then stored in an output array. The mask is         */
/*      moved one column at a time, advancing the mask over the entire      */
/*      image until the entire 'width' is covered. Masks are provided as    */
/*      16-bit signed values and the input image pixels xels are            */
/*      provided as 8-bit unsigned values and the output pixels will be     */
/*      32-bit signed. The mask to be correlated is typically part of       */
/*      the input image or another image.                                   */
/*                                                                          */
/*      The natural C implementation has no restrictions. The Optimized     */
/*      and IntrinsiC codes has restrictions as noted in the ASSUMPTIONS    */
/*      below.                                                              */
/*                                                                          */
/*  ASSUMPTIONS                                                             */
/*      1. width:       'width' >= 2 and multiple of 2.                     */
/*      2. pitch:       'pitch' >= 'width'                                  */
/*      3. Input and Output arrays should not overlap.                      */
/*      4. Output array should be double word-aligned.                      */
/*      5. no restrictions on the alignment of Input.                       */
/*      6. mask_ptr should be half-word aligned.                            */
/*                                                                          */
/*  COMPATIBILITY                                                           */
/*      This code is compatible for c64x+                                   */
/*                                                                          */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */

#ifndef _IMG_CORR_3X3_I8_C16S_I_H_
#define _IMG_CORR_3X3_I8_C16S_I_H_ 1

void IMG_corr_3x3_i8_c16s
(
    const unsigned char *restrict  imgin_ptr,
    int                 *restrict imgout_ptr,
    short                              width,
    short                              pitch,
    const          short *restrict  mask_ptr
);


#endif

/* ======================================================================== */
/*  End of file:  IMG_corr_3x3_i8_c16s_i.h                                  */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */
