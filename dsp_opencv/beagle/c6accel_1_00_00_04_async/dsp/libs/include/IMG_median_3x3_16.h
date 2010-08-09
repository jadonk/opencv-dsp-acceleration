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
/*      IMG_median_3x3_16: 3x3 median filter on un-signed 16-bit data.        */
/*                                                                            */
/*   USAGE                                                                    */
/*       This routine is C-callable and can be called as:                     */
/*                                                                            */
/*       void IMG_median_3x3_16                                               */
/*            (                                                               */
/*                const unsigned short *restrict i_data, // Input image       */
/*                int n,                                 // Length of input   */
/*                unsigned short       *restrict o_data  // Output image      */
/*            )                                                               */
/*                                                                            */
/*   DESCRIPTION                                                              */
/*       This kernel performs a 3x3 median filter operation on 16-bit         */
/*       unsigned values.  The median filter comes under the class            */
/*       of non-linear signal processing algorithms.                          */
/*                                                                            */
/*       Rather than replace the grey level at a pixel by a weighted          */
/*       average of the nine pixels including and surrounding it, the         */
/*       grey level at each pixel is replaced by the median of the nine       */
/*       values.  The median of a set of nine numbers is the middle           */
/*       element so that half of the elements in the list are larger and      */
/*       half are smaller.  Median filters remove the effects of extreme      */
/*       values from data, such as salt and pepper noise, although using      */
/*       a wide may result in unacceptable blurring of sharp edges in         */
/*       the original image.                                                  */
/*                                                                            */
/*       The C code of the algorithm has no restrictions.  The optimized      */
/*       implementations have restrictions, as noted under the "ASSUMPTIONS"  */
/*       below.                                                               */
/*                                                                            */
/*                                                                            */
/*   TECHNIQUES                                                               */
/*       This implementation uses an incremental sorting technique to         */
/*       greatly reduce the number of compares and exchanges necessary        */
/*       to sort the image pixels.                                            */
/*                                                                            */
/*       The main loop reads three new pixels from the input image each       */
/*       iteration.  These three pixels form the right edge of the filter     */
/*       mask.  The filter data from the previous iteration is "slid          */
/*       over" by one pixel to form the complete 3x3 mask.                    */
/*                                                                            */
/*       As 3-pixel is read in from the image, the pixels are sorted,         */
/*       resulting in a "lo", "medium" and "hi" pixel value for that          */
/*       column.  The result is that the filter mask is sorted into           */
/*       three rows -- a row of "minimums", a row of "middle values",         */
/*       and a row of "maximums".                                             */
/*                                                                            */
/*       The median filter operates from this partially ordered mask.         */
/*       It finds the smallest element in the row of "maximums",              */
/*       the middle element in the row of "middle values", and                */
/*       the largest element in the row of "minimums".  The median            */
/*       value of these three values is the median for the entire 3x3         */
/*       mask.                                                                */
/*                                                                            */
/*       This process minimizes compares, as the whole mask does not          */
/*       need to be sorted between iterations.  Rather, the partial           */
/*       ordering for two of the three columns from one iteration is          */
/*       used directly for the next iteration.                                */
/*                                                                            */
/*   ASSUMPTIONS                                                              */
/*       The length 'n' must be a multiple of four.                           */
/*                                                                            */
/*    MEMORY NOTE                                                             */
/*       - No bank conflicts occur                                            */ 
/*       - No allignment restrictions on input/output buffers.                */
/*                                                                            */
/*    CYCLES                                                                  */
/*        cycles = n * 14 / 4 + 79                                            */
/*        for n = 256, cycles = 975                                           */
/* -------------------------------------------------------------------------- */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.             */
/*                           All Rights Reserved.                             */
/* ========================================================================== */

#ifndef IMG_MEDIAN_3X3_16
#define IMG_MEDIAN_3X3_16

void IMG_median_3x3_16_cn
(
    const unsigned short *restrict i_data,
    int n,
    unsigned short      *restrict o_data
);

#endif /* IMG_MEDIAN_3X3_16 */

/*========================================================================= */
/*   End of file:  IMG_median_3x3_16_c.c                                    */
/* ------------------------------------------------------------------------ */
/*             Copyright (c) 2008 Texas Instruments, Incorporated.          */
/*                            All Rights Reserved.                          */
/* ======================================================================== */

