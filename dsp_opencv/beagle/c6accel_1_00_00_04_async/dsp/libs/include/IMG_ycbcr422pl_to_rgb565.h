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
/*      IMG_ycbcr422pl_to_rgb565 -- Planarized YCbCr 4:2:2/4:2:0 to 16-bit  */
/*                              RGB 5:6:5 color space conversion.           */
/*                                                                          */
/*  USAGE                                                                   */
/*      This function is C callable, and is called according to this        */
/*      C prototype:                                                        */
/*                                                                          */
/*      void IMG_ycbcr422pl_to_rgb565                                       */
/*      (                                                                   */
/*        const short         coeff[5],  // Matrix coefficients.        //  */
/*        const unsigned char *y_data,   // Luminence data  (Y')        //  */
/*        const unsigned char *cb_data,  // Blue color-diff (B'-Y')     //  */
/*        const unsigned char *cr_data,  // Red color-diff  (R'-Y')     //  */
/*        unsigned short                                                    */
/*                   *restrict rgb_data, // RGB 5:6:5 packed pixel out. //  */
/*        unsigned            num_pixels // # of luma pixels to process //  */
/*      );                                                                  */
/*                                                                          */
/*      The 'coeff[]' array contains the color-space-conversion matrix      */
/*      coefficients.  The 'y_data', 'cb_data' and 'cr_data' pointers       */
/*      point to the separate input image planes.  The 'rgb_data' pointer   */
/*      points to the output image buffer.  See below for alignment         */
/*      constraints on these buffers.                                       */
/*                                                                          */
/*      The kernel is designed to process arbitrary amounts of 4:2:2        */
/*      image data, although 4:2:0 image data may be processed as well.     */
/*      For 4:2:2 input data, the 'y_data', 'cb_data' and 'cr_data'         */
/*      arrays may hold an arbitrary amount of image data, including        */
/*      multiple scan lines of image data.                                  */
/*                                                                          */
/*      For 4:2:0 input data, only a single scan-line (or portion           */
/*      thereof) may be processed at a time.  This is achieved by           */
/*      calling the function twice using the same row data for              */
/*      'cr_data' and 'cb_data', and providing new row data for             */
/*      'y_data'.  This is numerically equivalent to replicating the Cr     */
/*      and Cb pixels vertically.                                           */
/*                                                                          */
/*      The coefficients in the coeff array must be in signed Q13 form.     */
/*      These coefficients correspond to the following matrix equation:     */
/*                                                                          */
/*          [ coeff[0] 0.0000   coeff[1] ]   [ Y' -  16 ]     [ R']         */
/*          [ coeff[0] coeff[2] coeff[3] ] * [ Cb - 128 ]  =  [ G']         */
/*          [ coeff[0] coeff[4] 0.0000   ]   [ Cr - 128 ]     [ B']         */
/*                                                                          */
/*      The output from this kernel is 16-bit RGB in 5:6:5 format.          */
/*      The RGB components are packed into halfwords as shown below.        */
/*                                                                          */
/*                     15      11 10       5 4        0                     */
/*                    +----------+----------+----------+                    */
/*                    |   Red    |  Green   |   Blue   |                    */
/*                    +----------+----------+----------+                    */
/*                                                                          */
/*      This kernel can also return the red, green, and blue values in      */
/*      the opposite order if a particular application requires it.         */
/*      This is achieved by exchanging the 'cb_data' and 'cr_data'          */
/*      arguments when calling the function, and by reversing the order     */
/*      of coefficients in coeff[1] through coeff[4].  This essentially     */
/*      implements the following matrix multiply:                           */
/*                                                                          */
/*          [ coeff[0] 0.0000   coeff[4] ]   [ Y' -  16 ]     [ B']         */
/*          [ coeff[0] coeff[3] coeff[2] ] * [ Cr - 128 ]  =  [ G']         */
/*          [ coeff[0] coeff[1] 0.0000   ]   [ Cb - 128 ]     [ R']         */
/*                                                                          */
/*      The reversed RGB ordering output by this mode is as follows:        */
/*                                                                          */
/*                     15      11 10       5 4        0                     */
/*                    +----------+----------+----------+                    */
/*                    |   Blue   |  Green   |   Red    |                    */
/*                    +----------+----------+----------+                    */
/*                                                                          */
/*  DESCRIPTION                                                             */
/*      This kernel performs Y'CbCr to RGB conversion.  From the Color      */
/*      FAQ, http://home.inforamp.net/~poynton/ColorFAQ.html :              */
/*                                                                          */
/*          Various scale factors are applied to (B'-Y') and (R'-Y')        */
/*          for different applications.  The Y'PbPr scale factors are       */
/*          optimized for component analog video.  The Y'CbCr scaling       */
/*          is appropriate for component digital video, JPEG and MPEG.      */
/*          Kodak's PhotoYCC(tm) uses scale factors optimized for the       */
/*          gamut of film colors.  Y'UV scaling is appropriate as an        */
/*          intermediate step in the formation of composite NTSC or PAL     */
/*          video signals, but is not appropriate when the components       */
/*          are keps separate.  Y'UV nomenclature is now used rather        */
/*          loosely, and it sometimes denotes any scaling of (B'-Y')        */
/*          and (R'-Y').  Y'IQ coding is obsolete.                          */
/*                                                                          */
/*      This code can perform various flavors of Y'CbCr to RGB              */
/*      conversion as long as the offsets on Y, Cb, and Cr are -16,         */
/*      -128, and -128, respectively, and the coefficients match the        */
/*      pattern shown.                                                      */
/*                                                                          */
/*      The kernel implements the following matrix form, which involves 5   */
/*      unique coefficients:                                                */
/*                                                                          */
/*          [ coeff[0] 0.0000   coeff[1] ]   [ Y' -  16 ]     [ R']         */
/*          [ coeff[0] coeff[2] coeff[3] ] * [ Cb - 128 ]  =  [ G']         */
/*          [ coeff[0] coeff[4] 0.0000   ]   [ Cr - 128 ]     [ B']         */
/*                                                                          */
/*                                                                          */
/*      Below are some common coefficient sets, along with the matrix       */
/*      equation that they correspond to.   Coefficients are in signed      */
/*      Q13 notation, which gives a suitable balance between precision      */
/*      and range.                                                          */
/*                                                                          */
/*      1.  Y'CbCr -> RGB conversion with RGB levels that correspond to     */
/*          the 219-level range of Y'.  Expected ranges are [16..235] for   */
/*          Y' and [16..240] for Cb and Cr.                                 */
/*                                                                          */
/*          coeff[] = { 0x2000, 0x2BDD, -0x0AC5, -0x1658, 0x3770 };         */
/*                                                                          */
/*          [ 1.0000    0.0000    1.3707 ]   [ Y' -  16 ]     [ R']         */
/*          [ 1.0000   -0.3365   -0.6982 ] * [ Cb - 128 ]  =  [ G']         */
/*          [ 1.0000    1.7324    0.0000 ]   [ Cr - 128 ]     [ B']         */
/*                                                                          */
/*      2.  Y'CbCr -> RGB conversion with the 219-level range of Y'         */
/*          expanded to fill the full RGB dynamic range.  (The matrix       */
/*          has been scaled by 255/219.)  Expected ranges are [16..235]     */
/*          for Y' and [16..240] for Cb and Cr.                             */
/*                                                                          */
/*          coeff[] = { 0x2543, 0x3313, -0x0C8A, -0x1A04, 0x408D };         */
/*                                                                          */
/*          [ 1.1644    0.0000    1.5960 ]   [ Y' -  16 ]     [ R']         */
/*          [ 1.1644   -0.3918   -0.8130 ] * [ Cb - 128 ]  =  [ G']         */
/*          [ 1.1644    2.0172    0.0000 ]   [ Cr - 128 ]     [ B']         */
/*                                                                          */
/*      3.  Y'CbCr -> BGR conversion with RGB levels that correspond to     */
/*          the 219-level range of Y'.  This is equivalent to #1 above,     */
/*          except that the R, G, and B output order in the packed          */
/*          pixels is reversed.  Note:  The 'cr_data' and 'cb_data'         */
/*          input arguments must be exchanged for this example as           */
/*          indicated under USAGE above.                                    */
/*                                                                          */
/*          coeff[] = { 0x2000, 0x3770, -0x1658, -0x0AC5, 0x2BDD };         */
/*                                                                          */
/*          [ 1.0000    0.0000    1.7324 ]   [ Y' -  16 ]     [ B']         */
/*          [ 1.0000   -0.6982   -0.3365 ] * [ Cr - 128 ]  =  [ G']         */
/*          [ 1.0000    1.3707    0.0000 ]   [ Cb - 128 ]     [ R']         */
/*                                                                          */
/*      4.  Y'CbCr -> BGR conversion with the 219-level range of Y'         */
/*          expanded to fill the full RGB dynamic range.  This is           */
/*          equivalent to #2 above, except that the R, G, and B output      */
/*          order in the packed pixels is reversed.  Note:  The             */
/*          'cr_data' and 'cb_data' input arguments must be exchanged       */
/*          for this example as indicated under USAGE above.                */
/*                                                                          */
/*          coeff[] = { 0x2000, 0x408D, -0x1A04, -0x0C8A, 0x3313 };         */
/*                                                                          */
/*          [ 1.0000    0.0000    2.0172 ]   [ Y' -  16 ]     [ B']         */
/*          [ 1.0000   -0.8130   -0.3918 ] * [ Cr - 128 ]  =  [ G']         */
/*          [ 1.0000    1.5960    0.0000 ]   [ Cb - 128 ]     [ R']         */
/*                                                                          */
/*      Other scalings of the color differences (B'-Y') and (R'-Y')         */
/*      (sometimes incorrectly referred to as U and V) are supported, as    */
/*      long as the color differences are unsigned values centered around   */
/*      128 rather than signed values centered around 0, as noted above.    */
/*                                                                          */
/*      In addition to performing plain color-space conversion, color       */
/*      saturation can be adjusted by scaling coeff[1] through coeff[4].    */
/*      Similarly, brightness can be adjusted by scaling coeff[0].          */
/*      General hue adjustment can not be performed, however, due to the    */
/*      two zeros hard-coded in the matrix.                                 */
/*                                                                          */
/*  TECHNIQUES                                                              */
/*      Pixel replication is performed implicitly on chroma data to         */
/*      reduce the total number of multiplies required.  The chroma         */
/*      portion of the matrix is calculated once for each Cb, Cr pair,      */
/*      and the result is added to both Y' samples.                         */
/*                                                                          */
/*      Matrix Multiplication is performed as a combination of MPY2s and    */
/*      DOTP2s.  Saturation to 8bit values is performed using SPACKU4       */
/*      which takes in 4 signed 16-bit values and saturates them to         */
/*      unsigned 8-bit values.  The output of Matrix Multiplication would   */
/*      ideally be in a Q13 format.  This however, cannot be fed directly   */
/*      to SPACKU4.                                                         */
/*                                                                          */
/*      This implies a shift left by 3 bits, which could be pretty          */
/*      expensive in terms of the number of shifts to be performed.  Thus,  */
/*      to avoid being bottlenecked by so many shifts, the Y, Cr & Cb data  */
/*      are shifted left by 3 before multiplication.  This is possible      */
/*      because they are 8-bit unsigned data.  Due to this, the output of   */
/*      Matrix Multiplication is in a Q16 format, which can be directly     */
/*      fed to SPACKU4.                                                     */
/*                                                                          */
/*  ASSUMPTIONS                                                             */
/*      The number of luma samples to be processed needs to be a multiple   */
/*      of 8.                                                               */
/*      The input Y array needs to be double-word aligned.                  */
/*      The input Cr and Cb arrays need to be word aligned                  */
/*      The output image must be double-word aligned.                       */
/*                                                                          */
/*  SOURCE                                                                  */
/*      Poynton, Charles et al.  "The Color FAQ,"  1999.                    */
/*          http://home.inforamp.net/~poynton/ColorFAQ.html                 */
/*                                                                          */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */
#ifndef YCBCR422PL_TO_RGB565_I_H_
#define YCBCR422PL_TO_RGB565_I_H_ 1

void IMG_ycbcr422pl_to_rgb565
(
    const short                   coeff[5],  /* Matrix coefficients.          */
    const unsigned char *restrict y_data,    /* Luminence data        (Y')    */
    const unsigned char *restrict cb_data,   /* Blue color-difference (B'-Y') */
    const unsigned char *restrict cr_data,   /* Red color-difference  (R'-Y') */
    unsigned short      *restrict rgb_data,  /* RGB 5:6:5 packed pixel output */
    unsigned                      num_pixels /* # of luma pixels to process.  */
);

#endif
/* ======================================================================== */
/*  End of file:  IMG_ycbcr422pl_to_rgb565_i.h                              */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */
