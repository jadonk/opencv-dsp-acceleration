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
/*   USAGE                                                                  */
/*      This routine has the following C prototype:                         */
/*                                                                          */
/*      void conv_3x3   (    const unsigned char *restrict inptr,           */
/*                                 unsigned char *restrict outptr,          */
/*                                          int            x_dim,           */
/*                           const          char *restrict mask,            */
/*                                          int            shift)           */
/*                                                                          */
/*     The convolution routine accepts three rows of 'x_dim' input points   */
/*     and performs some operation on each.  A total of 'x_dim' outputs     */
/*     are written to the output array. The 'mask' array has the 3 by 3     */
/*     array of coefficients.                                               */
/*                                                                          */
/*                                                                          */
/*  DESCRIPTION                                                             */
/*                                                                          */
/*     The convolution kernel accepts three rows of 'x_dim' input points    */
/*     and produces one output row of 'x_dim' points using the input mask   */
/*     of 3 by 3. The user defined shift value is used to shift the convo-  */
/*     lution value, down to the byte range. The convolution sum is also    */
/*     range limited to 0..255. The shift amount is non-zero for low pass   */
/*     filters, and zero for high pass and sharpening filters.              */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/*   ASSUMPTIONS                                                            */
/*        x_dim must be a multiple of 8.                                    */
/*                                                                          */
/*                                                                          */
/*    TECHNIQUES                                                            */
/*     The inner loop that computes the convolution sum is completely       */
/*     unrolled and 8 output pixels are computed together. The mask         */
/*     values are loaded and packed as double words.                        */
/*                                                                          */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */
#ifndef CONV_3X3_I_H_
#define CONV_3X3_I_H_ 1

void IMG_conv_3x3_i8_c8s (const unsigned char *restrict inptr,
                                unsigned char *restrict outptr,
                                         int            x_dim,
                          const          char *restrict mask,
                                         int            shift);

#endif
/* ======================================================================== */
/*  End of file:  IMG_conv_3x3_i8_c8s_i.h                                   */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */

