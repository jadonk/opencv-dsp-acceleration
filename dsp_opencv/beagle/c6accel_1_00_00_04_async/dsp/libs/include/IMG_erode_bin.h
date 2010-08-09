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
/*      IMG_erode_bin-- This code performs 3x3 binary dilation              */
/*                                                                          */
/*  USAGE                                                                   */
/*      This routine is C callable, and has the following C prototype:      */
/*                                                                          */
/*      void IMG_erode_bin                                                  */
/*      (                                                                   */
/*          const unsigned char *restrict in_data,   // Incoming image  //  */
/*          unsigned char       *restrict out_data,  // Filtered output //  */
/*          const char          *restrict mask,      // Filter mask     //  */
/*          int cols  // Number of columns to process, in bytes.        //  */
/*      );                                                                  */
/*                                                                          */
/*  DESCRIPTION                                                             */
/*      The function IMG_erode_bin() implements binary erosion using an     */
/*      arbitrary 3x3 mask.  The erosion operator generates output pixels   */
/*      by ANDing the pixels under the input mask together to generate      */
/*      the output pixel.  The input mask specifies whether one or more     */
/*      pixels from the input are to be ignored.                            */
/*                                                                          */
/*      In pseudo-code, the filtering operation for a pixel at (x, y)       */
/*      works like so:                                                      */
/*                                                                          */
/*          result = 1;                                                     */
/*          if (mask[0][0] != DONT_CARE) result &= input[y + 0][x + 0];     */
/*          if (mask[0][1] != DONT_CARE) result &= input[y + 1][x + 1];     */
/*          if (mask[0][2] != DONT_CARE) result &= input[y + 2][x + 2];     */
/*          if (mask[1][0] != DONT_CARE) result &= input[y + 0][x + 0];     */
/*          if (mask[1][1] != DONT_CARE) result &= input[y + 1][x + 1];     */
/*          if (mask[1][2] != DONT_CARE) result &= input[y + 2][x + 2];     */
/*          if (mask[2][0] != DONT_CARE) result &= input[y + 0][x + 0];     */
/*          if (mask[2][1] != DONT_CARE) result &= input[y + 1][x + 1];     */
/*          if (mask[2][2] != DONT_CARE) result &= input[y + 2][x + 2];     */
/*          output[y][x] = result;                                          */
/*                                                                          */
/*      For this code, "DONT_CARE" is specified by a negative value         */
/*      in the input mask.  Non-negative values in the mask cause the       */
/*      corresponding pixel to be included in the erosion operation.        */
/*                                                                          */
/*      Note that this code operates on a bitmap where each pixel is        */
/*      represented as a single bit within a byte or word.  Although        */
/*      the pseudo-code above operates only on one pixel at a time,         */
/*      with a single pixel in each array element, this implementation      */
/*      operates on a bitmap which contains 8 pixels in each byte.          */
/*                                                                          */
/*      Pixels are organized within each byte such that the pixel with      */
/*      the smallest index is in the LSB position, and the pixel with       */
/*      the largest index is in the MSB position.  (That is, the code       */
/*      assumes a LITTLE ENDIAN bit ordering.)                              */
/*                                                                          */
/*      Note that the "cols" argument actually specifies the number of      */
/*      BYTES in the output, not the number of columns.  The number of      */
/*      columns is 8 times this argument.                                   */
/*                                                                          */
/*  TECHNIQUES                                                              */
/*      The 3x3 erosion mask is applied to 32 output pixels                 */
/*      simultaneously.  This is done with 32-bit-wide bitwise              */
/*      operators in the register file.  In order to do this, the code      */
/*      reads in a 34-bit-wide input window, and 40-bit operations          */
/*      are used to manipulate the pixels initially.                        */
/*                                                                          */
/*      Because the code reads a 34-bit context for each 32-bits of         */
/*      output, the input needs to be one byte longer than the output       */
/*      in order to make the rightmost two pixels well-defined.             */
/*                                                                          */
/*  ASSUMPTIONS                                                             */
/*      1.  Negative values in the mask specify "DONT_CARE", and non-       */
/*          negative values specify that pixels are included in the         */
/*          erosion operation.                                              */
/*                                                                          */
/*      2.  The input image needs to have a multiple of 64 pixels(bits)     */
/*          per row.  Therefore, "cols" must be a multiple of 8.            */
/*                                                                          */
/*  NOTES                                                                   */
/*      Little Endian                                                       */
/*                                                                          */
/*      "Digital Image Processing: Principles and Applications"             */
/*      by Gregory A. Baxes, Chapter 5                                      */
/*                                                                          */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */
#ifndef ERODE_BIN_I_H_
#define ERODE_BIN_I_H_ 1

void IMG_erode_bin
(
    const unsigned char *restrict in_data,
    unsigned char       *restrict out_data,
    const char          *restrict mask,
    int cols
);

#endif
/* ======================================================================== */
/*  End of file:  IMG_erode_bin_i.h                                         */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */
