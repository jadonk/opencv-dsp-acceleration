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
/*      IMG_sobel_7x7_16s  Sobel with 7x7 masks                             */
/*                                                                          */
/*  USAGE                                                                   */
/*      This routine is C-callable and can be called as:                    */
/*                                                                          */
/*      void IMG_sobel_7x7_16s                                              */
/*      (                                                                   */
/*          const short restrict  *in,     // Input image data  //          */
/*                short restrict *out,     // Output image data //          */
/*                short          cols,     // image columns     //          */
/*                short          rows      // Image rows        //          */
/*      )                                                                   */
/*      The sobel_7x7 filter is applied to the input image.  The input      */
/*      image dimensions are given by the arguments 'cols' and 'rows'.      */
/*      The output image is 'cols' pixels wide and 'rows - 6' pixels        */
/*      tall.                                                               */
/*                                                                          */
/*                                                                          */
/*  DESCRIPTION                                                             */
/*                                                                          */
/*      To see how the implementation is going to work on the input         */
/*      buffer, lets imagine we have the following input buffer:            */
/*      lets imagine we have the following input buffer:                    */
/*                                                                          */
/*              yyyyyyyyyyyyyyyy                                            */
/*              yyyyyyyyyyyyyyyy                                            */
/*              yyyyyyyyyyyyyyyy                                            */
/*              yyyxxxxxxxxxxyyy                                            */
/*              yyyxxxxxxxxxxyyy                                            */
/*              yyyxxxxxxxxxxyyy                                            */
/*              yyyxxxxxxxxxxyyy                                            */
/*              yyyyyyyyyyyyyyyy                                            */
/*              yyyyyyyyyyyyyyyy                                            */
/*              yyyyyyyyyyyyyyyy                                            */
/*                                                                          */
/*      The output buffer would be:                                         */
/*                                                                          */
/*              tttXXXXXXXXXXzzz                                            */
/*              zzzXXXXXXXXXXzzz                                            */
/*              zzzXXXXXXXXXXzzz                                            */
/*              zzzXXXXXXXXXXttt                                            */
/*                                                                          */
/*      Where:                                                              */
/*                                                                          */
/*          X = sobel(x)    The algorithm is applied to that pixel.         */
/*                          The correct output is obtained, the data        */
/*                          around the pixels we work on is used            */
/*                                                                          */
/*          t               Whatever was in the output buffer in that       */
/*                          position is kept there.                         */
/*                                                                          */
/*          z = sobel(y)    The algorithm is applied to that pixel.         */
/*                          The output is not meaningful, because the       */
/*                          necessary data to process the pixel is not      */
/*                          available.  This is because for each output     */
/*                          pixel we need input pixels from the right and   */
/*                          from the left of the output pixel.  But this    */
/*                          data doesn't exist.                             */
/*                                                                          */
/*      This means that we will only process (rows-6) lines.  Then, we      */
/*      will process all the pixels inside each line. Even though the       */
/*      results for the first 3 and last 3 pixels in each line will not     */
/*      be relevant, it makes the control much simpler and ends up          */
/*      saving cycles.                                                      */
/*                                                                          */
/*      Also the first 3 pixels in the first processed line and the         */
/*      last 3 pixels in the last processed line will not be computed.      */
/*      It is not necessary, since the results would be bogus.              */
/*                                                                          */
/*      The following horizontal and vertical masks that are                */
/*      applied to the input buffer to obtain one output pixel.             */
/*                                                                          */
/*          Horizontal:                                                     */
/*              -1 -1 -1 -2 -1 -1 -1                                        */
/*              -1 -1 -1 -2 -1 -1 -1                                        */
/*              -1 -1 -1 -2 -1 -1 -1                                        */
/*               0  0  0  0  0  0  0                                        */
/*               1  1  1  2  1  1  1                                        */
/*               1  1  1  2  1  1  1                                        */
/*               1  1  1  2  1  1  1                                        */
/*                                                                          */
/*          Vertical:                                                       */
/*              -1 -1 -1  0  1  1  1                                        */
/*              -1 -1 -1  0  1  1  1                                        */
/*              -1 -1 -1  0  1  1  1                                        */
/*              -2 -2 -2  0  2  2  2                                        */
/*              -1 -1 -1  0  1  1  1                                        */
/*              -1 -1 -1  0  1  1  1                                        */
/*              -1 -1 -1  0  1  1  1                                        */
/*                                                                          */
/*      The natural C implementation has no restrictions. The Optimized     */
/*      and IntrinsiC codes have restrictions as noted in the ASSUMPTIONS   */
/*      below.                                                              */
/*                                                                          */
/*  ASSUMPTIONS                                                             */
/*      1. cols :     'cols' >  7 and multiple of 2.                        */
/*      2. rows :     'rows' >= 7.                                          */
/*      3. cols * (rows - 6) - 6 should be greater than or equal to 2.      */
/*      4. Input and output arrays do not overlap.                          */
/*      5. Input and output arrays should be half-word aligned.             */
/*                                                                          */
/*  TECHNIQUE                                                               */
/*      The loop is unrolled twice and 2 outputs are calculated every       */
/*      iteration. The horizontal and vertical masks have sub-blocks.       */
/*      where the absolute value of the mask elements are same. These       */
/*      sub-blocks are applied on image only once and are added/subtracted  */
/*      to both horizontal and vertical components,thus improving           */
/*      performance.                                                        */
/*                                                                          */
/*  NOTES                                                                   */
/*      The values of the three left-most and three right-most pixels       */
/*      on each line of the output are not well-defined.                    */
/*                                                                          */
/*  COMPATIBILITY                                                           */
/*      This code is compatible with C64x+ processors only.                 */
/*                                                                          */
/*  SOURCE                                                                  */
/*      The algorithm is taken from sobel in IMGLIB                         */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */
#ifndef IMG_SOBEL_7x7_16s_I_H_
#define IMG_SOBEL_7x7_16s_I_H_ 1

void IMG_sobel_7x7_16s
(
    const short *restrict  in,   /* Input image data   */
          short *restrict out,   /* Output image data  */
          short          cols,   /* image columns      */
          short          rows    /* Image rows         */
);

#endif
/* ======================================================================== */
/*  End of file:  IMG_sobel_7x7_16s_i.h                                     */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */
