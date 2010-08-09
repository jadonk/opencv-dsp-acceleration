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
/*       IMG_errdif_bin_16: Floyd-Steinberg Error Diffusion for 16 bit data   */
/*                                                                            */
/*   USAGE                                                                    */
/*       This routine is C-callable and can be called as:                     */
/*                                                                            */
/*   void IMG_errdif_bin_16                                                   */
/*   (                                                                        */
/*       unsigned short *restrict errdif_data,                                */
/*       int                      cols,                                       */
/*       int                      rows,                                       */
/*       short          *restrict err_buf,                                    */
/*       unsigned short           thresh                                      */
/*   )                                                                        */
/*                                                                            */
/*      errdif_data:     Input/Output image ptr                               */
/*      cols:            Number of columns (Width)                            */
/*      rows:            Number of rows    (Height)                           */
/*      err_buf[cols+1]: Buffer where one row of errors is to be saved        */
/*      thresh:          Threshold in the range [0x00, 0xFF]                  */
/*                                                                            */
/*   DESCRIPTION                                                              */
/*                                                                            */
/*      The code implements the Binary Floyd-Steinberg error diffusion        */
/*      filter.  The filter kernel used is this one:                          */
/*                                                                            */
/*                                  +---+                                     */
/*                                P | 7 |                                     */
/*                          +---+---+---+                                     */
/*                          | 3 | 5 | 1 |                                     */
/*                          +---+---+---+                                     */
/*                                                                            */
/*                                                                            */
/*      Pixels are processed from left-to-right, top-to-bottom.  Each         */
/*      pixel is compared against a user-defined threshold.  Pixels           */
/*      that are larger than the threshold are set to 255, and pixels         */
/*      that are smaller or equal to the threshold are set to 0.  The         */
/*      error value for the pixel (eg. the difference between the             */
/*      thresholded pixel and its original grey level) is propagated to       */
/*      the neighboring pixels according to the filter above.  This           */
/*      error propagation diffuses the error over a larger area, hence        */
/*      the term "error diffusion."                                           */
/*                                                                            */
/*   MEMORY NOTE                                                              */
/*      This kernel places no restrictions on the alignment of its input.     */
/*                                                                            */
/*      No bank conflicts occur.                                              */
/*                                                                            */
/*      This code assumes a LITTLE ENDIAN configuration.                      */
/*                                                                            */
/*   TECHNIQUES                                                               */
/*      The processing of the filter itself is inverted so that the           */
/*      errors from previous pixels "propagate into" a given pixel at         */
/*      the time the pixel is processed, rather than "accumulate into"        */
/*      a pixel as its neighbors are processed.  This allows us to            */
/*      keep our image as an 8-bit image, and reduces the number of           */
/*      accesses to the image array.  The inverted filter kernel              */
/*      performs identically to the kernel's original form.  In this          */
/*      form, the weights specify the weighting assigned to the errors        */
/*      coming from the neighboring pixels.                                   */
/*                                                                            */
/*                          +---+---+---+                                     */
/*                          | 1 | 5 | 3 |                                     */
/*                          +---+---+---+                                     */
/*                          | 7 | P                                           */
/*                          +---+                                             */
/*                                                                            */
/*   ASSUMPTIONS                                                              */
/*      - Input snd Output buffers do not alias.                              */
/*      - 'cols' should be even                                               */
/*      - 'err_buf[]' must be initialized to zeros for the first call and     */
/*        the returned err_buf should be provided for the subsequent calls.   */
/*                                                                            */
/*  SOURCE                                                                    */
/*      Floyd-Steinberg Error Diffusion.                                      */
/*                                                                            */
/* -------------------------------------------------------------------------- */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.             */
/*                           All Rights Reserved.                             */
/* ========================================================================== */

#ifndef IMG_ERRDIF_BIN_16
#define IMG_ERRDIF_BIN_16

void IMG_errdif_bin_16
(
    unsigned short *restrict errdif_data, /* Input/Output image ptr       */
    int                      cols,        /* Number of columns (Width)    */
    int                      rows,        /* Number of rows    (Height)   */
    short          *restrict err_buf,     /* row-to-row error buffer.     */
    unsigned short           thresh       /* Threshold from [0x00, 0xFF]  */
);

#endif /* IMG_ERRDIF_BIN_16 */

/* ======================================================================== */
/*  End of file:  errdif_bin_16_c.c                                         */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */
