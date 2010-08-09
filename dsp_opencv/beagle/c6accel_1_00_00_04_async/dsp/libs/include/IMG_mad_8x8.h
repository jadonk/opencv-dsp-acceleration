
/* ######################################################################## */
/* #  TI Disclaimer: This kernel is obsolete. It is provided here for     # */
/* #  backward compatibility.                                             # */
/* ######################################################################## */

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
/*                                                                          */
/*  TEXAS INSTRUMENTS, INC.                                                 */
/*                                                                          */
/*  NAME                                                                    */
/*      mad_8x8 : Minimum absolute Difference                               */
/*                                                                          */
/*  USAGE                                                                   */
/*                                                                          */
/*      This routine is C-callable and can be called as:                    */
/*                                                                          */
/*      void mad_8x8                                                        */
/*      (                                                                   */
/*          const unsigned char *ref_data,  // Ref. image to search //      */
/*          const unsigned char *src_data,  // Source 8x8 block     //      */
/*          int                 pitch,      // Width of ref image   //      */
/*          int sx, int sy,                 // Search window size   //      */
/*          unsigned int        *match      // Result               //      */
/*      );                                                                  */
/*                                                                          */
/*      The reference image is an image of width 'pitch'.  The mad_8x8      */
/*      routine searches a (sx,sy) window within the image.                 */
/*                                                                          */
/*      The source image is an 8x8 block of pixels which contain the        */
/*      image block that is being searched for.                             */
/*                                                                          */
/*      The match[] array is where the best match location and MAD value    */
/*      for the best match are stored.  The best match position and its     */
/*      absolute difference are returned in match, packed as follows:       */
/*                                                                          */
/*                     31             16 15             0                   */
/*                     +----------------+----------------+                  */
/*          match[0]:  |    X offset    |    Y offset    |                  */
/*                     +----------------+----------------+                  */
/*                                                                          */
/*                     31                               0                   */
/*                     +---------------------------------+                  */
/*          match[1]:  |   Sum of absolute differences   |                  */
/*                     +---------------------------------+                  */
/*                                                                          */
/*  DESCRIPTION                                                             */
/*      This routine locates the position in a reference image which        */
/*      most closely matches an 8x8 block from a source image, using        */
/*      the Minimum Absolute Difference metric.  Searches over a range      */
/*      that is 'sx' pixels wide and 'sy' pixels tall within a reference    */
/*      image that is 'pitch' pixels wide.                                  */
/*                                                                          */
/*      The search is performed in top-to-bottom, left-to-right order,      */
/*      with the earliest match taking precedence in the case of ties.      */
/*                                                                          */
/*      The match location as well as the Minimum Absolute Difference       */
/*      metric for the match are returned in the 'match' array.             */
/*      The first element contains the packed horizontal and vertical       */
/*      coordinates of the match, with the X coordinate in the upper        */
/*      halfword and the Y coordinate in the lower halfword.  The           */
/*      second element contains the MAD value for this match location.      */
/*                                                                          */
/*  TECHNIQUES                                                              */
/*      The inner loops that perform the 8x8 mads are completely            */
/*      unrolled and the outer two loops are collpaesd together. In         */
/*      addition all source image data is pre-loaded into registers.        */
/*                                                                          */
/*      The data required for any one row is brought in using               */
/*      non-aligned loads. SUBABS4 and DOTPU4 are used together to do       */
/*      the MAD computation.                                                */
/*                                                                          */
/*      To save instructions and fit within an 8 cycle loop, the            */
/*      precise location of a given match is not stored.  Rather,           */
/*      the loop iteration that it was encountered on is stored.            */
/*      A short divide loop after the search loop converts this             */
/*      value into X and Y coordinates for the location.                    */
/*                                                                          */
/*  ASSUMPTIONS                                                             */
/*      The arrays for the source image data and the result must be         */
/*      word aligned.                                                       */
/*                                                                          */
/*  MEMORY NOTE                                                             */
/*      No bank conflicts occur for this kernel.                            */
/*                                                                          */
/*      No particular memory alignment is required for the reference        */
/*      image, although the source image data and result                    */
/*      must be word aligned.                                               */
/*                                                                          */
/*  NOTES                                                                   */
/*      The search window spans from (0,0) to (sx-1,sy-1), inclusive.       */
/*                                                                          */
/*      The inner loop comprises 64 instructions that are executed in 8     */
/*      cycles, with 64 absolute-differences accumulated in a single        */
/*      iteration.  The source pixels are pre-read into registers and       */
/*      the inner loop is unrolled completely.  This code thus executes     */
/*      8 instructions per cycle, and computes 8 absolute differences       */
/*      per cycle.                                                          */
/*                                                                          */
/*      This benchmark is Little Endian.                                    */
/*                                                                          */
/*      This code suppresses interrupts for the entire duration of the      */
/*      code, hence it is interrupt tolerant but not interruptible.         */
/*                                                                          */
/*  C CODE                                                                  */
/*      The following is the equivalent C code.  The assembly code          */
/*      has restrictions as noted under "ASSUMPTIONS".                      */
/*                                                                          */
/*      void mad_8x8                                                        */
/*      (                                                                   */
/*          const unsigned char *restrict refImg,                           */
/*          const unsigned char *restrict srcImg,                           */
/*          int pitch, int sx, int sy,                                      */
/*          unsigned int *restrict match                                    */
/*      )                                                                   */
/*      {                                                                   */
/*          int i, j, x, y, matx, maty;                                     */
/*          unsigned matpos, matval;                                        */
/*                                                                          */
/*          matval = ~0U;                                                   */
/*          matx   = maty = 0;                                              */
/*                                                                          */
/*          for (x = 0; x < sx; x++)                                        */
/*              for (y = 0; y < sy; y++)                                    */
/*              {                                                           */
/*                  unsigned acc = 0;                                       */
/*                                                                          */
/*                  for (i = 0; i < 8; i++)                                 */
/*                      for (j = 0; j < 8; j++)                             */
/*                          acc += abs(srcImg[i*8 + j] -                    */
/*                                     refImg[(i+y)*pitch + x + j]);        */
/*                                                                          */
/*                  if (acc < matval)                                       */
/*                  {                                                       */
/*                      matval = acc;                                       */
/*                      matx   = x;                                         */
/*                      maty   = y;                                         */
/*                  }                                                       */
/*              }                                                           */
/*                                                                          */
/*          matpos    = (0xffff0000 & (matx << 16)) |                       */
/*                      (0x0000ffff & maty);                                */
/*          match[0] = matpos;                                              */
/*          match[1] = matval;                                              */
/*      }                                                                   */
/*                                                                          */
/*  CYCLES                                                                  */
/*      sx = # of columns of the search space                               */
/*      sy = # of rows of the search space                                  */
/*      cycles = 8 * sx * sy + 66.                                          */
/*                                                                          */
/*      For sx=  4 and sy=  4, cycles = 194.                                */
/*      For sx= 64 and sy= 32, cycles = 16450.                              */
/*                                                                          */
/*      Overall throughput is 7.97 pixels/cycle (0.126 cycles/pixel)        */
/*      over the whole function for the case H=64 and V=32.                 */
/*                                                                          */
/*  CODESIZE                                                                */
/*      788 bytes                                                           */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */
#ifndef MAD_8X8_H_H_
#define MAD_8X8_H_H_ 1

void IMG_mad_8x8
(
    const unsigned char *ref_data,  /* Ref. image to search */
    const unsigned char *src_data,  /* Source 8x8 block     */
    int                 pitch,      /* Width of ref image   */
    int sx, int sy,                 /* Search window size   */
    unsigned int        *match      /* Result               */
);

#endif
/* ======================================================================== */
/*  End of file:  mad_8x8_h.h                                               */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */
