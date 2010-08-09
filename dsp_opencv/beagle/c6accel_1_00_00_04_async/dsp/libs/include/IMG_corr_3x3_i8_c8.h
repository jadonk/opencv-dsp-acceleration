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
/*==========================================================================*/
/*  TEXAS INSTRUMENTS, INC.                                                 */
/*                                                                          */
/*  NAME                                                                    */
/*      IMG_corr_3x3_i8_c8_cn: 3x3 correlation with rounding for 8 bit data */
/*                                                                          */
/*  USAGE                                                                   */
/*      This routine is C-callable and can be called as:                    */
/*                                                                          */
/*          void IMG_corr_3x3_i8_c8                                         */
/*          (                                                               */
/*              const unsigned char *i_data,       // input image       //  */
/*              int        *restrict o_data,       // output image      //  */
/*              int                  n_out         // number of outputs //  */
/*              int                  x_dim,        // width of image    //  */
/*              const unsigned char  mask[3][3],   // convolution mask  //  */
/*              const short          shift,                                 */
/*              int                  round         // rounding constant     */
/*          );                                                              */
/*                                                                          */
/*  DESCRIPTION                                                             */
/*      The correlation performs a point by point multiplication of the     */
/*      3 by 3 mask with the input image.  The result of the nine           */
/*      multiplications are then summed up together to produce a            */
/*      convolution sum. A rounding constant is added to the sum and        */
/*      shifted by user specified amount.                                   */
/*                                                                          */
/*      The image mask to be correlated is typically part of the input      */
/*      image and indicates the area of the best match between the          */
/*      input image and mask.  The mask is moved one column at a time,      */
/*      advancing the mask over the portion of the row specified by         */
/*      'n_out'.  When 'n_out' is larger than 'x_dim', multiple rows        */
/*      will be processed.                                                  */
/*                                                                          */
/*      An application may call this kernel once per row to calculate       */
/*      the correlation for an entire image:                                */
/*                                                                          */
/*          for (i = 0; i < rows; i++)                                      */
/*          {                                                               */
/*              IMG_corr_3x3(&i_data[i * x_dim], &o_data[i * n_out],        */
/*                          n_out, x_dim, mask, shift, round);              */
/*          }                                                               */
/*                                                                          */
/*      Alternately, the kernel may be invoked for multiple rows at         */
/*      a time, although the two outputs at the end of each row will        */
/*      have meaningless values.  For example:                              */
/*                                                                          */
/*      IMG_corr_3x3(i_data, o_data, 2 * x_dim, mask, x_dim, shift, round); */
/*                                                                          */
/*      This will produce two rows of outputs into 'o_data'.  The           */
/*      outputs at locations o_data[x_dim - 2], o_data[x_dim - 1],          */
/*      o_data[2*x_dim - 2] and o_data[2*x_dim - 1] will have               */
/*      meaningless values.  This is harmless, although the application     */
/*      will have to account for this when interpreting the results.        */
/*                                                                          */
/*                                                                          */
/*  ASSUMPTIONS                                                             */
/*      - The array pointed to by outptr does not alias with the array      */
/*        pointed to by inptr.                                              */
/*      - Shift should be appropriate to produce 8 bit result.              */
/*      - x_dim >= 4 and is a multiple of 2                                 */
/*      - n_out >= 4 and is a multiple of 4                                 */
/*      - This kernel is developed for Little Endian.                       */
/*                                                                          */
/*     TECHNIQUES                                                           */
/*        Data for the input image pixels is reused by pre-loading them     */
/*        outside the loop and issuing moves to bring them to the appro-    */
/*        priate registers once inside the loop. This is done to minimize   */
/*        the loads from nine to six within the loop, for each pair of      */
/*        pixels in the present computation of the correlation. The loop    */
/*        is unrolled once so that eighteen multiplies for the two output   */
/*        pixels can schedule in 9 cycles leading to 4.5 cycles per output  */
/*        pixel. In addition te trivial loop that did the loads three at a  */
/*        time, per row is collapsed to increase parallel operations.       */
/*                                                                          */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */

void IMG_corr_3x3_i8_c8
(
    const unsigned char *inptr,        /* input image       */
    int        *restrict outptr,       /* output image      */
    int                  n_out,        /* number of outputs */
    int                  x_dim,        /* width of image    */
    const unsigned char *mask,         /* convolution mask  */
    const short          shift,
    int                  round         /* rounding constant */
);

/*============================================================================*/
/*      Copyright (C) 2008 Texas Instruments Incorporated.                    */
/*                      All Rights Reserved                                   */
/*============================================================================*/

