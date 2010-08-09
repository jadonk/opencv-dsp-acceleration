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
/*       IMG_thr_gt2thr_16: Unsigned 16-bit thresholding to constant          */
/*                                                                            */
/*   USAGE                                                                    */
/*       This routine has the following C prototype:                          */
/*                                                                            */
/*       void IMG_thr_gt2thr_16                                               */
/*       (                                                                    */
/*           const unsigned short *in_data,     //  Input image data          */
/*           unsigned short *restrict out_data, //  Output image data         */
/*           short cols, short rows,            //  Image dimensions          */
/*           unsigned short       threshold     //  Threshold value           */
/*       )                                                                    */
/*                                                                            */
/*       This routine performs a thresholding operation on an input           */
/*       image in in_data[] whose dimensions are given in the arguments       */
/*       'cols' and 'rows'.  The thresholded pixels are written to the        */
/*       output image pointed to by out_data[].  The input and output         */
/*       are exactly the same dimensions.                                     */
/*                                                                            */
/*   DESCRIPTION                                                              */
/*       Pixels that are below the threshold value are written to the         */
/*       output unmodified.  Pixels that are greater than the threshold       */
/*       are set to the threshold value in the output image.                  */
/*                                                                            */
/*       The exact thresholding function performed is described by            */
/*       the following transfer function diagram:                             */
/*                                                                            */
/*                                                                            */
/*                65535_|                                                     */
/*                      |                                                     */
/*                      |                                                     */
/*             O        |                                                     */
/*             U        |                                                     */
/*             T    th _|. . . . . _________                                  */
/*             P        |        /.                                           */
/*             U        |      /  .                                           */
/*             T        |    /    .                                           */
/*                      |  /      .                                           */
/*                    0_|/________.__________                                 */
/*                      |         |        |                                  */
/*                      0        th       65535                               */
/*                                                                            */
/*                              INPUT                                         */
/*                                                                            */
/*       See the IMGLIB functions IMG_thr_le2thr_16, IMG_thr_gt2max_16        */
/*       and IMG_thr_le2min_16 for other thresholding operations.             */
/*                                                                            */
/*   ASSUMPTIONS                                                              */
/*       - The input and output buffers do not alias.                         */
/*       - The input and output buffers must be double-word aligned.          */
/*       - The total number of pixels rows*cols must be at least              */
/*         16 and a multiple of 16.                                           */
/*                                                                            */
/*   TECHNIQUES                                                               */
/*       The loop is unrolled 16x.  Packed-data processing techniques         */
/*       allow us to process all 16 pixels in parallel.                       */
/*                                                                            */
/*       We compare using MIN2, but have to change the unsigned values to     */
/*       signed values first, using XOR 0x8000.                               */
/*                                                                            */
/*       Four LDDW instructions load in 16 pixels.                            */
/*       The 16 results are then written with two STDWs.                      */
/*                                                                            */
/*   MEMORY NOTE                                                              */
/*       This code is ENDIAN NEUTRAL.                                         */
/*                                                                            */
/*       No bank conflicts occur, regardless of the relative alignment of     */
/*       in_data[] and out_data[].                                            */
/*                                                                            */
/*   CYCLES                                                                   */
/*       cycles = 0.375 * rows * cols + 26.                                   */
/*       For rows = 256 and cols = 3, cycles = 314.                           */
/*                                                                            */
/* -------------------------------------------------------------------------- */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.             */
/*                           All Rights Reserved.                             */
/* ========================================================================== */

void IMG_thr_gt2thr_16
(
    const unsigned short *in_data,                /*  Input image data    */
    unsigned short       *restrict out_data,      /*  Output image data   */
    short cols, short rows,                       /*  Image dimensions    */
    unsigned short       threshold                /*  Threshold value     */
);

/*========================================================================= */
/*   End of file:  IMG_thr_gt2thr_16.h                                      */
/* ------------------------------------------------------------------------ */
/*             Copyright (c) 2008 Texas Instruments, Incorporated.          */
/*                            All Rights Reserved.                          */
/* ======================================================================== */

