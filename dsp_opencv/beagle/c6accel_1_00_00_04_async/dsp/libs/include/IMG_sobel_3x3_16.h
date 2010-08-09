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
/* ========================================================================   */
/*   NAME                                                                     */
/*       IMG_sobel_3x3_16: Little Endian, sobel 3x3 for 16-bit unsigned data. */
/*                                                                            */
/*   USAGE                                                                    */
/*       This routine is C-callable and can be called as:                     */
/*                                                                            */
/*       void IMG_sobel_3x3_16                                                */
/*       (                                                                    */
/*           const unsigned short *in_data,    // Input image data            */
/*           unsigned short       *out_data,   // Output image data           */
/*           short cols, short rows     	     // Image dimensions            */
/*       )                                                                    */
/*                                                                            */
/*       The IMG_sobel filter is applied to the input image. The input image  */
/*       dimensions are given by the arguments 'cols' and 'rows'.  The        */
/*       output image is 'cols' pixels wide and 'rows - 2' pixels tall.       */
/*                                                                            */
/*   DESCRIPTION                                                              */
/*                                                                            */
/*       To see how the implementation is going to work on the input          */
/*       buffer, lets imagine we have the following input buffer:             */
/*                                                                            */
/*               yyyyyyyyyyyyyyyy                                             */
/*               yxxxxxxxxxxxxxxy                                             */
/*               yxxxxxxxxxxxxxxy                                             */
/*               yxxxxxxxxxxxxxxy                                             */
/*               yxxxxxxxxxxxxxxy                                             */
/*               yyyyyyyyyyyyyyyy                                             */
/*                                                                            */
/*       The output buffer would be:                                          */
/*                                                                            */
/*               tXXXXXXXXXXXXXXz                                             */
/*               zXXXXXXXXXXXXXXz                                             */
/*               zXXXXXXXXXXXXXXz                                             */
/*               zXXXXXXXXXXXXXXt                                             */
/*                                                                            */
/*       Where:                                                               */
/*                                                                            */
/*           X = IMG_sobel(x)    The algorithm is applied to that pixel.      */
/*                           The correct output is obtained, the data         */
/*                           around the pixels we work on is used             */
/*                                                                            */
/*           t               Whatever was in the output buffer in that        */
/*                           position is kept there.                          */
/*                                                                            */
/*           z = IMG_sobel(y)    The algorithm is applied to that pixel.      */
/*                           The output is not meaningful, because the        */
/*                           necessary data to process the pixel is not       */
/*                           available.  This is because for each output      */
/*                           pixel we need input pixels from the right and    */
/*                           from the left of the output pixel.  But this     */
/*                           data doesn't exist.                              */
/*                                                                            */
/*       This means that we will only process (rows-2) lines.  Then, we       */
/*       will process all the pixels inside each line. Even though the        */
/*       results for the first and last pixels in each line will not          */
/*       be relevant, it makes the control much simpler and ends up           */
/*       saving cycles.                                                       */
/*                                                                            */
/*       Also the first pixel in the first processed line and the             */
/*       last pixel in the last processed line will not be computed.          */
/*       It is not necessary, since the results would be bogus.               */
/*                                                                            */
/*       The following horizontal and vertical masks that are                 */
/*       applied to the input buffer to obtain one output pixel.              */
/*                                                                            */
/*           Horizontal:                                                      */
/*               -1 -2 -1                                                     */
/*                0  0  0                                                     */
/*                1  2  1                                                     */
/*                                                                            */
/*           Vertical:                                                        */
/*               -1  0  1                                                     */
/*               -2  0  2                                                     */
/*               -1  0  1                                                     */
/*                                                                            */
/*       The C code is functionally equivalent to the serial-assembly code    */
/*       without restrictions. The assembly code may impose additional        */
/*       restrictions.                                                        */
/*                                                                            */
/*   TECHNIQUES                                                               */
/*       Four output pixels are computed per iteration using loop unrolling   */
/*       and packed operations.                                               */
/*                                                                            */
/*   ASSUMPTIONS                                                              */
/*       At least four output pixels must be processed.                       */
/*       The input image width must be even (eg. 'cols' must be even).        */
/*       rows >= 3                                                            */
/*                                                                            */
/*   MEMORY NOTE                                                              */
/*       No bank conflicts occur.                                             */
/*       There are no alignment restrictions.                                 */
/*                                                                            */
/*   NOTES                                                                    */
/*       This is a LITTLE ENDIAN implementation.                              */
/*       The image arrays do not need to be aligned.                          */
/*                                                                            */
/*       The values of the left-most and right-most pixels on each line       */
/*       of the output are not well-defined.                                  */
/*                                                                            */
/*   CYCLES                                                                   */
/*       cycles = 15 * cols * (rows-2)/4 + 32,                                */
/*       For cols = 128 and rows = 8, cycles = 2912.                          */
/* -------------------------------------------------------------------------- */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.             */
/*                           All Rights Reserved.                             */
/* ========================================================================== */

void IMG_sobel_3x3_16
(
    const unsigned short *restrict in,   /* Input image data   */
    unsigned short       *restrict out,  /* Output image data  */
    short cols, short rows               /* Image dimensions   */
);

/* ======================================================================== */
/*   End of file, "IMG_sobel_3x3_16.h"                                      */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */

