
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
/*      quantize -- Matrix Quantization w/ Rounding, Little Endian          */
/*                                                                          */
/*  USAGE                                                                   */
/*      This routine is C callable, and has the following C prototype:      */
/*                                                                          */
/*      void quantize                                                       */
/*      (                                                                   */
/*          short           *data,      // Data to be quantized.        //  */
/*          unsigned short  num_blks,   // Number of blocks to process. //  */
/*          unsigned short  blk_size,   // Block size (multiple of 16). //  */
/*          const short     *recip_tbl, // Quant. values (reciprocals). //  */
/*          int             q_pt        // Q-point of Quant values.     //  */
/*      )                                                                   */
/*                                                                          */
/*      The number of blocks, num_blks, may be zero.  The block             */
/*      size (number of elements in each block) must be at least 32,        */
/*      and a multiple of 16.  The Q-point, q_pt, controls rounding and     */
/*      final truncation; it must be in the range from 0 <= q_pt <= 31.     */
/*                                                                          */
/*      Both input arrays, data[] and recip_tbl[], must be double-word      */
/*      aligned.  The data[] array must be 'num_blks * blk_size' elements,  */
/*      and the recip_tbl[] array must be 'blk_size' elements.              */
/*                                                                          */
/*  DESCRIPTION                                                             */
/*      The function quantize() quantizes matrices by multiplying their     */
/*      contents with a second matrix that contains reciprocals of the      */
/*      quantization terms.  This step corresponds to the quantization      */
/*      that is performed in 2-D DCT-based compression techniques,          */
/*      although quantize() may be used on any signed 16-bit data using     */
/*      signed 16-bit quantization terms.                                   */
/*                                                                          */
/*      quantize() multiplies the contents of the quantization matrix       */
/*      with the data being quantized.  Therefore, it may be used for       */
/*      inverse quantization as well, by setting the Q-point                */
/*      appropriately.                                                      */
/*                                                                          */
/*      The assembly code requires 26 + (blk_size/16) * num_blks * 8        */
/*      cycles to execute, including function call overhead.  This code     */
/*      does not explicitly disable interrupts.  On entry to the function,  */
/*      interrupts are masked by branch delay slots for a maximum of 16     */
/*      cycles.                                                             */
/*                                                                          */
/*      The following C code describes the general implementation of        */
/*      quantize().                                                         */
/*                                                                          */
/*      void quantize                                                       */
/*      (                                                                   */
/*          short           *data,      // Data to be quantized.        //  */
/*          unsigned short  num_blks,   // Number of 64-element blocks. //  */
/*          unsigned short  blk_size,   // Block size (multiple of 16). //  */
/*          const short     *recip_tbl, // Quant. values (reciprocals). //  */
/*          int             q_pt        // Q-point of Quant values.     //  */
/*      )                                                                   */
/*      {                                                                   */
/*          short recip;                                                    */
/*          int i, j, k, quot, round;                                       */
/*                                                                          */
/*          round = q_pt ? 1 << (q_pt - 1) : 0;                             */
/*                                                                          */
/*          for (i = 0; i < blk_size; i++)                                  */
/*          {                                                               */
/*              recip   = recip_tbl[i];                                     */
/*              k       = i;                                                */
/*              for (j = 0; j < num_blks; j++)                              */
/*              {                                                           */
/*                  quot    = data[k] * recip + round;                      */
/*                  data[k] = quot >> q_pt;                                 */
/*                  k      += blk_size;                                     */
/*              }                                                           */
/*          }                                                               */
/*      }                                                                   */
/*                                                                          */
/*      The above C code is a general implementation without                */
/*      restrictions.  The assembly code has restrictions, as noted above.  */
/*                                                                          */
/*  TECHNIQUES                                                              */
/*      The outer loop is unrolled 16 times to allow greater amounts        */
/*      of work to be performed in the inner loop.  The resulting loop-     */
/*      nest was then collapsed and pipelined as a single loop, since       */
/*      the code is not bottlenecked on bandwidth.                          */
/*                                                                          */
/*      Reciprocals and data terms are loaded in groups of four using       */
/*      double-word loads, making best use of the available memory          */
/*      bandwidth.                                                          */
/*                                                                          */
/*      SSHVR is used in the M-unit to avoid an S-unit bottleneck.          */
/*                                                                          */
/*      Twin stack pointers have been used to speed up stack accesses.      */
/*                                                                          */
/*  ASSUMPTIONS                                                             */
/*      The recip_tbl[] and data[] must be double-word aligned.             */
/*      The block size, blk_size, must be a multiple of 16 and at least 32. */
/*      The number of blocks, num_blks, may be 0.                           */
/*      The Q-point, q_pt, must be in the range 0 <= q_pt <= 31.            */
/*                                                                          */
/*  MEMORY NOTE                                                             */
/*      No bank conflicts occur, regardless of the relative orientation     */
/*      of recip_tbl[] and data[].                                          */
/*                                                                          */
/*      A total of 6 words of stack frame are used for saving the           */
/*      Save-On-Entry registers.                                            */
/*                                                                          */
/*  NOTES                                                                   */
/*      This code is fully interruptible, with a maximum interrupt latency  */
/*      due to branch delay slots of 16.                                    */
/*                                                                          */
/*      Input arrays must be double-word-aligned for correct operation.     */
/*                                                                          */
/*      No checking is performed on the input arguments for correctness.    */
/*                                                                          */
/*  CYCLES                                                                  */
/*      cycles = 26 + (blk_size/16) * num_blks * 8                          */
/*                                                                          */
/*      For blk_size =  64 and num_blks =  8, cycles =  282.                */
/*      For blk_size = 256 and num_blks = 24, cycles = 3098.                */
/*                                                                          */
/*  CODESIZE                                                                */
/*      580 bytes                                                           */
/*                                                                          */
/*  SOURCE                                                                  */
/*      n/a                                                                 */
/*                                                                          */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */
#ifndef QUANT_H_H_
#define QUANT_H_H_ 1

void IMG_quantize
(
    short           *data,      /* Data to be quantized.        */
    unsigned short  num_blks,   /* Number of blocks to process. */
    unsigned short  blk_size,   /* Block size (multiple of 16);. */
    const short     *recip_tbl, /* Quant. values (reciprocals);. */
    int             q_pt        /* Q-point of Quant values.     */
);

#endif
/* ======================================================================== */
/*  End of file:  quantize_h.h                                                 */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */
