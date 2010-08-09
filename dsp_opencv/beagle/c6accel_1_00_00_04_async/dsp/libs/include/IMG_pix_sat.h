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
/*      IMG_pix_sat -- 16 to 8 bit conversion                               */
/*                                                                          */
/*  USAGE                                                                   */
/*      This routine is C-callable and can be called as:                    */
/*                                                                          */
/*      void IMG_pix_sat                                                    */
/*      (                                                                   */
/*          int Size,                         // Number of pixels //        */
/*          const short   *restrict in_data,  // Incoming data    //        */
/*          unsigned char *restrict out_data  // Outgoing data    //        */
/*      )                                                                   */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/*  DESCRIPTION                                                             */
/*                                                                          */
/*      The function IMG_pix_sat() takes signed 16-bit input pixels and     */
/*      saturates them to unsigned 8-bit results.  Pixel values above       */
/*      255 are clamped to 255, and values below 0 are clamped to 0.        */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/*   ASSUMPTIONS                                                            */
/*      - The input must be a multiple of 32 pixels long.                   */
/*		  - All pointers must be double word aligned.													*/
/*                                                                          */
/*                                                                          */
/*                                                                          */
/*   MEMORY NOTE                                                            */
/*            The input and output data must be double-word aligned.        */
/*                                                                          */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */
#ifndef PIX_SAT_I_H_
#define PIX_SAT_I_H_ 1

void IMG_pix_sat
(
    int                     n,
    const short   *restrict in_data,
    unsigned char *restrict out_data
);

#endif
/* ======================================================================== */
/*  End of file:  IMG_pix_sat_i.h                                           */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */
