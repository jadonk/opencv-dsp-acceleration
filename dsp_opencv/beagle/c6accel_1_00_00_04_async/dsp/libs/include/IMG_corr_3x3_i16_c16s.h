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
/* ========================================================================== */
/*   NAME                                                                     */
/*      IMG_corr_3x3_i16_c16s: 3x3 correlation with rounding for 16 bit data  */
/*                                                                            */
/*   USAGE                                                                    */
/*       This routine is C-callable and can be called as:                     */
/*                                                                            */
/*           void IMG_corr_3x3_i16_c16s                                       */
/*           (                                                                */
/*               const unsigned short *i_data,       // input image           */
/*               long        *restrict o_data,       // output image          */
/*               const unsigned short  mask[3][3],   // convolution mask      */
/*               int                   x_dim,        // width of image        */
/*               int                   n_out         // number of outputs     */
/*           );                                                               */
/*                                                                            */
/*   DESCRIPTION                                                              */
/*       The correlation performs a point by point multiplication of the      */
/*       3 by 3 mask with the input image.  The result of the nine            */
/*       multiplications are then summed up together to produce a             */
/*       convolution sum.  This sum is then stored to the output array.       */
/*                                                                            */
/*       The image mask to be correlated is typically part of the input       */
/*       image and indicates the area of the best match between the           */
/*       input image and mask.  The mask is moved one column at a time,       */
/*       advancing the mask over the portion of the row specified by          */
/*       'n_out'.  When 'n_out' is larger than 'x_dim', multiple rows         */
/*       will be processed.                                                   */
/*                                                                            */
/*       An application may call this kernel once per row to calculate        */
/*       the correlation for an entire image:                                 */
/*                                                                            */
/*           for (i = 0; i < rows; i++)                                       */
/*           {                                                                */
/*               IMG_corr_3x3(&i_data[i * x_dim], &o_data[i * n_out],         */
/*                           mask, x_dim, n_out);                             */
/*           }                                                                */
/*                                                                            */
/*   ASSUMPTIONS                                                              */
/*      - Input snd Output buffers do not alias.                              */
/*      - 'n_out' should be a multiple of 4                                   */
/*                                                                            */
/*   MEMORY NOTE                                                              */
/*      - This kernel places no restrictions on the alignment of its input.   */
/*      - No bank conflicts occur.                                            */
/*      - This code assumes a LITTLE ENDIAN configuration.                    */
/*                                                                            */
/*   TECHNIQUES                                                               */
/*       The inner loops are unrolled completely, and the outer loop is       */
/*       unrolled 4 times.                                                    */
/*       Half-word unsigned multiplication is used here.                      */
/*       Non-aligned loads and stores are used to avoid alignment issues.     */
/*                                                                            */
/*   CYCLES                                                                   */
/*       cycles = 18 / 4 * n_out + 53                                         */
/*       For n_out = 256, cycles = 1205.                                      */
/* -------------------------------------------------------------------------- */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.             */
/*                           All Rights Reserved.                             */
/* ========================================================================== */


void IMG_corr_3x3_i16_c16s
(
    const unsigned short *i_data,   /* input image       */
    long       *restrict o_data,    /* output image      */
    unsigned short  mask[3][3],     /* convolution mask  */
    int                  x_dim,     /* width of image    */
    int                  n_out      /* number of outputs */
);

/* ======================================================================== */
/*  End of file:  IMG_corr_3x3_i16_c16s.h                                   */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */

