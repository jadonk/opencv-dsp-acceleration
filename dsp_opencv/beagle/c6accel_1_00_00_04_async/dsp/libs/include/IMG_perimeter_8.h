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
/*      IMG_perimeter_8                                                     */
/*                                                                          */
/*  USAGE                                                                   */
/*      This routine is C-callable and can be called as:                    */
/*                                                                          */
/*          int IMG_perimeter_8                                             */
/*          (                                                               */
/*              const unsigned char *restrict in,  // Input image    //     */
/*              int cols,                          // Width of input //     */
/*              unsigned char       *restrict out  // Output image   //     */
/*          );                                                              */
/*                                                                          */
/*  DESCRIPTION                                                             */
/*      This routine produces the boundary of a binary image.  It           */
/*      echoes the boundary pixels with a value of 0xFF and sets the        */
/*      other pisels as 0.  Detection of the boundary of a binary image     */
/*      is a segmentation problem and is done by examining spatial          */
/*      locality of the neighboring pixels.  This is done by using the      */
/*      four connectivity algorithm                                         */
/*                                                                          */
/*                                                                          */
/*                      pix_up                                              */
/*             pix_lft pix_cent pix_rgt                                     */
/*                      pix_dn                                              */
/*                                                                          */
/*      The output pixel at location pix_cent is echoed as a boundary       */
/*      pixel if pix_cent is non-zero and any one of its four               */
/*      neighbors is zero.  The four neighbors are shown and stand          */
/*      for the following:                                                  */
/*                                                                          */
/*            pix_up:  top pixel                                            */
/*            pix_lft: left pixel                                           */
/*            pix_rgt: right pixel                                          */
/*            pix_dn:  bottom pixel                                         */
/*                                                                          */
/*  ASSUMPTIONS                                                             */
/*      The input must be a multiple of 16 pixels across, and be            */
/*      on a double-word boundary.                                          */
/*                                                                          */
/*  MEMORY NOTE                                                             */
/*      No bank conflicts are expected for this kernel.                     */
/*                                                                          */
/*  TECHNIQUES                                                              */
/*      The loop is unrolled 16 times.  Split-compares are used to          */
/*      find boundary pixels, with comparison results reused between        */
/*      adjacent comparisons.  Also, multiplies replace some of the         */
/*      conditional operations to reduce the bottleneck on the              */
/*      predication registers as well as on the L, S, and D units.          */
/*                                                                          */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */
#ifndef PERIMETER_I_H_
#define PERIMETER_I_H_ 1

int IMG_perimeter_8
(
    const unsigned char *restrict img_in,  /* Input image    */
    int cols,                              /* Width of input */
    unsigned char       *restrict img_out  /* Output image   */
);

#endif
/* ======================================================================== */
/*  End of file:  IMG_perimeter_8_i.h                                       */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */
