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
/* ===========================================================================*/
/*   NAME                                                                     */
/*      IMG_conv_3x3_i16_c16s  --  3x3 convolution                            */
/*                                                                            */
/*   USAGE                                                                    */
/*       This routine has the following C prototype:                          */
/*                                                                            */
/*       void IMG_conv_3x3_i16_c16s(                                          */
/*                            const unsigned short *restrict inptr,           */
/*                                  unsigned short *restrict outptr,          */
/*                                           int             x_dim,           */
/*                            const          short *restrict mask,            */
/*                                           int             shift)           */
/*                                                                            */
/*      inptr     :   pointer to an input array of unsigned 16 bit pixels     */
/*      outptr    :   pointer to an output array of 16 bit pixels             */
/*      x_dim     :   number of output pixels                                 */
/*      mask      :   pointer to 3x3 16 bit filter mask.                      */
/*      shift     :   user specified shift amount                             */
/*                                                                            */
/*   DESCRIPTION                                                              */
/*      The convolution kernel accepts three rows of 'x_dim' input points     */
/*      and produces one output row of 'x_dim' points using the input mask    */
/*      of 3 by 3. The user defined shift value is used to shift the convo-   */
/*      lution value, down to the 16-bit range. The convolution sum is also   */
/*      range limited to 0..(2^40 - 1). The shift amount is non-zero for low  */
/*      pass filters, and zero for high pass and sharpening filters.          */
/*                                                                            */
/*   TECHNIQUES                                                               */
/*      The inner loop that computes the convolution sum is completely        */
/*      unrolled and 4 output pixels are computed together. The mask          */
/*      values are loaded and packed as double words.                         */
/*      Since 40-bit intermediates are used, intermediate results will        */
/*      not overflow                                                          */
/*                                                                            */
/*   ASSUMPTIONS                                                              */
/*      - x_dim must be a multiple of 4.                                      */
/*      - I/O buffers do not overlap.                                         */
/*      - Input, output and mask pointers should be half word alligned.       */
/*                                                                            */
/*   CYCLES                                                                   */
/*      34 + x_dim/4 * 18                                                     */
/*      x_dim = 256, cycles = 1186                                            */
/* -------------------------------------------------------------------------- */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.             */
/*                           All Rights Reserved.                             */
/* ========================================================================== */

#ifndef  IMG_CONV_3X3_I16_C16S_H_
#define  IMG_CONV_3X3_I16_C16S_H_ 1

void IMG_conv_3x3_i16_c16s(const unsigned short *restrict inptr,
                                 unsigned short *restrict outptr,
                                 int                      x_dim,
                           const short          *restrict mask,
                                 int                      shift);

#endif  /* IMG_CONV_3X3_I16_C16S_H_ */

/*========================================================================= */
/*   End of file:  IMG_conv_3x3_i16_c16s_c.h                                */
/* ------------------------------------------------------------------------ */
/*             Copyright (c) 2008 Texas Instruments, Incorporated.          */
/*                            All Rights Reserved.                          */
/* ======================================================================== */

