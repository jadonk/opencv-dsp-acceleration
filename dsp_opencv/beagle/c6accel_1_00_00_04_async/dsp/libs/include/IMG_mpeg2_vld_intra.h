
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
/*  TEXAS INSTRUMENTS, INC.                                                 */
/*                                                                          */
/*  NAME                                                                    */
/*      mpeg2_vld_intra                                                     */
/*                                                                          */
/*  PLATFORM                                                                */
/*      C6400                                                               */
/*                                                                          */
/*  DESCRIPTION                                                             */
/*      This routine takes a bitstream of an MPEG-2 intra coded macroblock  */
/*      and returns the decoded IDCT coefficients. The routine is           */
/*      implemented as specified in the MPEG-2 standard text (ISO/IEC       */
/*      13818-2). The routine checks the coded block pattern (cbp),         */
/*      performs DC and AC decoding inlcuding, variable length decode,      */
/*      run-length expansion, inverse zigzag, dequantization, saturation    */
/*      and mismatch control.                                               */
/*                                                                          */
/*  USAGE                                                                   */
/*      This routine is C callable, and has the following C prototype:      */
/*                                                                          */
/*      void mpeg2_vld_intra                                                */
/*      (                                                                   */
/*          const short    *restrict Wptr,                                  */
/*          short          *restrict outi,                                  */
/*          IMG_mpeg2_vld  *restrict Mpeg2v,                                */
/*          int            dc_pred[3],                                      */
/*          int            mode_12Q4,                                       */
/*          int            num_blocks,                                      */
/*          int            bsbuf_words                                      */
/*      );                                                                  */
/*                                                                          */
/*        Wptr:   Pointer to array that contains quantization matrix. The   */
/*                elements of the quantization matrix in *Wptr must be      */
/*                ordered according to the scan pattern used (zigzag or     */
/*                alternate scan). Video format 4:2:0 requires one          */
/*                quantization matrix (64 array elements).  For formats     */
/*                4:2:2 and 4:4:4 two quantization matrices (one for luma   */
/*                and one for chroma) must specified in the array (128      */
/*                array elements).                                          */
/*                                                                          */
/*        outi:   Pointer to the IDCT coefficients output array             */
/*                (6*64 elements), elements must be set to zero prior to    */
/*                function call.                                            */
/*                                                                          */
/*        Mpeg2v: Pointer to the context object containing the coding       */
/*                parameters of the MB to be decoded and the current state  */
/*                of the bitstream buffer. The structure is described       */
/*                below.                                                    */
/*                                                                          */
/*       dc_pred: Intra DC prediction array, the first element of dc_pred   */
/*                is the DC prediction for Y, the second for Cr and the     */
/*                third for Cb.                                             */
/*                                                                          */
/*     mode_12Q4: 0: Coefficients are returned in normal 16-bit integer     */
/*                format.                                                   */
/*                Otherwise: Coefficients are returned in 12Q4 format       */
/*                (normal 16-bit integer format left shifted by 4). This    */
/*                mode is useful for directly passing the coefficients      */
/*                into the IMG_idct_8x8 routine.                            */
/*                                                                          */
/*    num_blocks: Number of blocks that the MB contains. Valid values are   */
/*                6 for 4:2:0, 8 for 4:2:2 and 12 for 4:4:4 format.         */
/*                                                                          */
/*   bsbuf_words: Size of bitstream buffer in words. Must be a power of 2.  */
/*                Bitstream buffer must be aligned at an address boundary   */
/*                equal to its size in bytes (bitstream buffer is           */
/*                addressed circularly by this routine.)                    */
/*                                                                          */
/*      The structure Mpeg2v is defined as follows:                         */
/*                                                                          */
/*        #ifndef IMG_MPEG2_VLD_STRUCT_                                     */
/*        #define IMG_MPEG2_VLD_STRUCT_ 1                                   */
/*                                                                          */
/*        typedef struct {                                                  */
/*            unsigned int  *bsbuf;      // pointer to bitstream buffer     */
/*            unsigned int  next_wptr;   // next word to read from buffer   */
/*            unsigned int  bptr;        // bit position within word        */
/*            unsigned int  word1;       // word aligned buffer             */
/*            unsigned int  word2;       // word aligned buffer             */
/*            unsigned int  top0;        // top 32 bits of bitstream        */
/*            unsigned int  top1;        // next 32 bits of bitstream       */
/*            const unsigned char *scan; // inverse zigzag scan matrix      */
/*            unsigned int  intravlc;    // intra_vlc_format                */
/*            unsigned int  quant_scale; // quant_scale                     */
/*            unsigned int  dc_prec;     // intra_dc_precision              */
/*            unsigned int  cbp;         // coded_block_pattern             */
/*            unsigned int  fault;       // fault condition (returned)      */
/*            unsigned int  reserved;    // reserved                        */
/*        } IMG_mpeg2_vld;                                                  */
/*                                                                          */
/*        #endif                                                            */
/*                                                                          */
/*      The Mpeg2v variables should have a fixed layout since they are      */
/*      accessed by this routine.  If the layout is changed, the            */
/*      corresponding changes have to be made in the assembly code too.     */
/*                                                                          */
/*      The routine sets the fault flag Mpeg2v.fault to 1 if an invalid     */
/*      VLC code was encountered or the total run went beyond 63. In        */
/*      theses cases the decoder has to resynchronize.                      */
/*                                                                          */
/*      The required lookup tables for this routine are provided in         */
/*      IMGLIB and are linked in automatically when linking against         */
/*      IMGLIB.                                                             */
/*                                                                          */
/*      Before calling the routine the bitstream variables in Mpeg2v        */
/*      have to be initialized. If bsbuf is a circular buffer and bsptr     */
/*      contains the number of bits in the buffer that already have         */
/*      been consumed, then next_wptr, bptr, word1, word2, top0 and         */
/*      top1 are initialized as follows:                                    */
/*                                                                          */
/*      1. nextwptr: bsptr may not be a multiple of 32, therefore obtain    */
/*      the next lower multiple of 32.                                      */
/*                                                                          */
/*          next_wptr = (bsptr >> 5);                                       */
/*                                                                          */
/*      2. bptr: bptr is the bit pointer which points to the current        */
/*      bit WITHIN the word pointed to by next_wptr.                        */
/*                                                                          */
/*          bptr = bsptr & 31;                                              */
/*          bptr_cmpl = 32 - bptr;                                          */
/*                                                                          */
/*      3. word1 and word2: read next 3 words from the bitstream buffer     */
/*      (word0 is a temporary variable). bsbuf_words is the size of the     */
/*      bitstream buffer in words.                                          */
/*                                                                          */
/*          word0 = bsbuf[next_wptr];                                       */
/*          next_wptr = (next_wptr + 1) & (bsbuf_words-1);                  */
/*                                                                          */
/*          word1 = bsbuf[next_wptr];                                       */
/*          next_wptr = (next_wptr + 1) & (bsbuf_words-1);                  */
/*                                                                          */
/*          word2 = bsbuf[next_wptr];                                       */
/*          next_wptr = (next_wptr + 1) & (bsbuf_words-1);                  */
/*                                                                          */
/*      4. top0 and top1: Shift words word0, word1, word2 by bptr to the    */
/*      left so that the current bit becomes the MSB in word0. word0 can    */
/*      simply be shifted by bptr; the then empty LSBs of word0 have to be  */
/*      filled with the MSBs of word1. To do that the required MSBs are     */
/*      brought into the position of empty LSBs of word0 by shifting word1  */
/*      to the right by (32-bptr). The result is then copied into word0 by  */
/*      an addition. Rather than overwriting word0, top0 is used to hold    */
/*      the new bit aligned word. The same procedure is used to obtain      */
/*      top1. top0 and top1 contain the next 64 bits of the bitstream.      */
/*                                                                          */
/*          s1 = word0 << bptr;                                             */
/*          s2 = word1 >> bptr_cmpl;  // unsigned right-shift //            */
/*          top0 = s1 + s2;                                                 */
/*                                                                          */
/*          s3 = word1 << bptr;                                             */
/*          s4 = word2 >> bptr_cmpl;  // unsigned right-shift //            */
/*          top1 = s3 + s4;                                                 */
/*                                                                          */
/*      Note that the routine returns the updated state of the bitstream    */
/*      buffer variables, top0, top1, word1, word2, bptr and next_wptr. If  */
/*      all other functions which access the bitstream in a decoder system  */
/*      maintain the buffer variables in the same way, then the above       */
/*      initialization procedure has to be performed only once at the       */
/*      beginning.                                                          */
/*                                                                          */
/*  TECHNIQUES                                                              */
/*      The instruction NORM is used to detect the number of leading zeros  */
/*      or ones in a code word. This value together with additional bits    */
/*      extracted from the codeword is then used as an index into look-up   */
/*      tables to determine the length, run, level and sign. Escape code    */
/*      sequences are directly extracted from the code word.                */
/*                                                                          */
/*      DC coefficients are decoded without lookup tables by exploiting     */
/*      the relatively simple relationship between the number of leading    */
/*      zeros and dc_size and the length of the code word.                  */
/*                                                                          */
/*  ASSUMPTIONS                                                             */
/*      The bitstream must be stored in memory in 32-bit words which are    */
/*      in little endian byte order.                                        */
/*                                                                          */
/*      Wptr is allowed to overrun once (to detect total run overrun), so   */
/*      maximum overrun that can occur is 66 (Error mark). Therefore,       */
/*      in memory 66+1 halfwords behind the weighting matrix should be      */
/*      valid (e.g. peripherals). No memory is overwritten,                 */
/*      only loads occurr.                                                  */
/*                                                                          */
/*      Note that the AMR register is set to zero on exit.                  */
/*                                                                          */
/*  NOTES                                                                   */
/*      This code is little ENDIAN.                                         */
/*      This code is interrupt-tolerant but not interruptible.              */
/*                                                                          */
/*  MEMORY NOTES                                                            */
/*      No bank conflicts                                                   */
/*                                                                          */
/*  CYCLES                                                                  */
/*      10 * (S - CB) + 55 * CB + 15 * NCB + 35                             */
/*      where S:   Number of symbols in MB, CB: Number of coded blocks,     */
/*            NCB: Number of not-coded blocks, and CB+NCB=6                 */
/*                                                                          */
/*  CODE SIZE                                                               */
/*      1508 bytes                                                          */
/*                                                                          */
/*  DATA SIZE                                                               */
/*      3584 bytes for the lookup tables                                    */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */
#ifndef MPEG2_VLD_INTRA_H_H_
#define MPEG2_VLD_INTRA_H_H_ 1

#ifndef IMG_MPEG2_VLD_STRUCT_
#define IMG_MPEG2_VLD_STRUCT_ 1

typedef struct {
    unsigned int  *bsbuf;      // pointer to bitstream buffer
    unsigned int  next_wptr;   // next word to read from buffer
    unsigned int  bptr;        // bit position within word
    unsigned int  word1;       // word aligned buffer
    unsigned int  word2;       // word aligned buffer
    unsigned int  top0;        // top 32 bits of bitstream
    unsigned int  top1;        // next 32 bits of bitstream
    const unsigned char *scan; // inverse zigzag scan matrix
    unsigned int  intravlc;    // intra_vlc_format
    unsigned int  quant_scale; // quant_scale
    unsigned int  dc_prec;     // intra_dc_precision
    unsigned int  cbp;         // coded_block_pattern
    unsigned int  fault;       // fault condition (returned)
    unsigned int  reserved;    // reserved
} IMG_mpeg2_vld;

#endif

void IMG_mpeg2_vld_intra
(
    const short    *restrict Wptr,
    short          *restrict outi,
    IMG_mpeg2_vld  *restrict Mpeg2v,
    int            dc_pred[3],
    int            mode_12Q4,
    int            num_blocks,
    int            bsbuf_words
);

#endif
/* ======================================================================== */
/*  End of file:  mpeg2_vld_intra_h.h                                       */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */
