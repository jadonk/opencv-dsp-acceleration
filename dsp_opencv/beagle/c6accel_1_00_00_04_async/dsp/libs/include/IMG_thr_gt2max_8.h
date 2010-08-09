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
/*      IMG_thr_gt2max_8                                                    */
/*                                                                          */
/*  USAGE                                                                   */
/*      This routine has the following C prototype:                         */
/*                                                                          */
/*      void IMG_thr_gt2max_8                                               */
/*      (                                                                   */
/*          const unsigned char *in_data,     //  Input image data  //      */
/*          unsigned char *restrict out_data, //  Output image data //      */
/*          short cols, short rows,           //  Image dimensions  //      */
/*          unsigned char       threshold     //  Threshold value   //      */
/*      )                                                                   */
/*                                                                          */
/*      This routine performs a thresholding operation on an input          */
/*      image in in_data[] whose dimensions are given in the arguments      */
/*      'cols' and 'rows'.  The thresholded pixels are written to the       */
/*      output image pointed to by out_data[].  The input and output        */
/*      are exactly the same dimensions.                                    */
/*                                                                          */
/*  DESCRIPTION                                                             */
/*      Pixels that are below or equal to the threshold value are           */
/*      written to the output unmodified.  Pixels that are greater          */
/*      than the threshold are set to 255 in the output image.              */
/*                                                                          */
/*      The exact thresholding function performed is described by           */
/*      the following transfer function diagram:                            */
/*                                                                          */
/*                                                                          */
/*                 255_|          _________                                 */
/*                     |         |                                          */
/*                     |         |                                          */
/*            O        |         |                                          */
/*            U        |         |                                          */
/*            T    th _|. . . . .|                                          */
/*            P        |        /.                                          */
/*            U        |      /  .                                          */
/*            T        |    /    .                                          */
/*                     |  /      .                                          */
/*                   0_|/________.__________                                */
/*                     |         |        |                                 */
/*                     0        th       255                                */
/*                                                                          */
/*                             INPUT                                        */
/*                                                                          */
/*      Please see the IMGLIB functions IMG_thr_gt2thr, IMG_thr_le2thr      */
/*      and IMG_thr_le2min for other thresholding functions.                */
/*                                                                          */
/*  ASSUMPTIONS                                                             */
/*      The input and output buffers do not alias.                          */
/*                                                                          */
/*      The input and output buffers must be double-word aligned.           */
/*                                                                          */
/*      The total number of pixels rows*cols must be at least               */
/*      16 and a multiple of 16.                                            */
/*                                                                          */
/*  TECHNIQUES                                                              */
/*      The loop is manually unrolled 4x.  We ask the compiler to unroll    */
/*      the loop an additional factor of 4x, yielding a total unroll        */
/*      factor of 16x.  Packed-data processing techniques allow us to       */
/*      process all 4 pixels in parallel.                                   */
/*                                                                          */
/*      A single _amem4_const() intrinsic brings in four pixels,            */
/*      designated p0 thru p3.  These pixels are packed into the unsigned   */
/*      integer variable p3p2p1p0 as follows:                               */
/*                                                                          */
/*                            31  24   16    8    0                         */
/*                             +----+----+----+----+                        */
/*                    p3p2p1p0 | p3 | p2 | p1 | p0 |                        */
/*                             +----+----+----+----+                        */
/*                                                                          */
/*      (Note that this diagram assumes a little endian memory              */
/*      configuration, although this kernel works equally well in           */
/*      either endian mode.)                                                */
/*      We compare this packed word against a packed copy of the threshold  */
/*      value.  The packed threshold contains four copies of the threshold  */
/*      value, one in each byte, like so:                                   */
/*                                                                          */
/*                             31  24   16    8    0                        */
/*                              +----+----+----+----+                       */
/*                     thththth | th | th | th | th |                       */
/*                              +----+----+----+----+                       */
/*                                                                          */
/*      We compare using _cmpgtu4().  The comparison results are then       */
/*      expanded to masks using _xpnd4().  The result is a four-byte mask   */
/*      (x3210) which contains 0xFF in bytes that are greater than the      */
/*      threshold, and 0x00 in bytes that are less than or equal to the     */
/*      threshold.                                                          */
/*                                                                          */
/*      To complete the thresholding process, we compute the logical OR     */
/*      between our original pixel values and the mask.  This forces        */
/*      values above the threshold to 0xFF, and leaves the other values     */
/*      unmodified.                                                         */
/*                                                                          */
/*      The four pixels are then written with a single _amem4().            */
/*                                                                          */
/*      In this version of the code, we rely on the compiler to unroll      */
/*      the loop 4x (as noted above), and convert the _amem4_const() and    */
/*      _amem4() calls into _amemd8_const()/_amemd8() as part of its        */
/*      automatic optimizations.                                            */
/*                                                                          */
/*  MEMORY NOTE                                                             */
/*      This code is ENDIAN NEUTRAL.                                        */
/*                                                                          */
/*      The input and output arrays must be double-word aligned.            */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */
#ifndef IMG_THR_GT2MAX_8_I_H_
#define IMG_THR_GT2MAX_8_I_H_ 1

void IMG_thr_gt2max_8
(
    const unsigned char *in_data,                /*  Input image data    */
    unsigned char       *restrict out_data,      /*  Output image data   */
    short cols, short rows,                      /*  Image dimensions    */
    unsigned char       threshold                /*  Threshold value     */
);

#endif
/* ======================================================================== */
/*  End of file:  IMG_thr_gt2max_8_i.h                                      */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */
