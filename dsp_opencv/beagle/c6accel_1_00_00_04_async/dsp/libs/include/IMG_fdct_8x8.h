
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
/*      fdct_8x8 -- 8x8 Block FDCT With Rounding, Little Endian             */
/*                                                                          */
/*  USAGE                                                                   */
/*      This routine is C callable, and has the following C prototype:      */
/*                                                                          */
/*          void fdct_8x8(short fdct_data[], unsigned num_fdcts)            */
/*                                                                          */
/*      The fdct routine accepts a list of 8x8 pixel blocks and performs    */
/*      FDCTs on each.  The array should be laid out identically to         */
/*      "fdct_data[num_fdcts][8][8]".  All operations in this array are     */
/*      performed entirely in-place.                                        */
/*                                                                          */
/*      Input values are stored in shorts, and may be in the range          */
/*      [-512,511].  Larger input values may result in overflow.            */
/*                                                                          */
/*      This code requires '50 + 76 * num_fdcts' cycles to process          */
/*      'num_fdcts' blocks, including function call overhead.  When         */
/*      'num_fdcts' is zero, an early exit is taken and the function        */
/*      runs for only 13 cycles (again, including call overhead).           */
/*                                                                          */
/*  DESCRIPTION                                                             */
/*      The fdct_8x8 function implements a Chen FDCT.  Output values are    */
/*      rounded, providing improved accuracy.  Input terms are expected     */
/*      to be signed 11Q0 values, producing signed 15Q0 results.  (A        */
/*      smaller dynamic range may be used on the input, producing a         */
/*      correspondingly smaller output range.  Typical applications         */
/*      include processing signed 9Q0 and unsigned 8Q0 pixel data,          */
/*      producing signed 13Q0 or 12Q0 outputs, respectively.)  No           */
/*      saturation is performed.                                            */
/*                                                                          */
/*      Note:  This code guarantees correct operation, even in the case     */
/*      that 'num_fdcts == 0'.  In this case, the function runs for only    */
/*      13 cycles (counting 6 cycles of function-call overhead), due to     */
/*      early-exit code.  The early-exit case performs one access to the    */
/*      fdct_data[] array and no access to the stack.                       */
/*                                                                          */
/*  C CODE                                                                  */
/*                                                                          */
/*      void fdct_8x8(short *dct_data, unsigned num_fdcts)                  */
/*      {                                                                   */
/*          // ---------------------------------------------------- //      */
/*          //  Set up the cosine coefficients.                     //      */
/*          // ---------------------------------------------------- //      */
/*          const unsigned short c1 = 0x1F62;   // Q13   coeff      //      */
/*          const unsigned short c3 = 0x1A9B;   // Q13   coeff      //      */
/*          const unsigned short c5 = 0x11C7;   // Q13   coeff      //      */
/*          const unsigned short c7 = 0x063E;   // Q13   coeff      //      */
/*          const unsigned short c2 = 0x29CF;   // Q13.5 coeff      //      */
/*          const unsigned short c6 = 0x1151;   // Q13.5 coeff      //      */
/*          const unsigned short C1 = 0xFB15;   // Q16   coeff      //      */
/*          const unsigned short C3 = 0xD4DB;   // Q16   coeff      //      */
/*          const unsigned short C5 = 0x8E3A;   // Q16   coeff      //      */
/*          const unsigned short C7 = 0x31F1;   // Q16   coeff      //      */
/*          const unsigned short C2 = 0xA73D;   // Q15.5 coeff      //      */
/*          const unsigned short C6 = 0x4546;   // Q15.5 coeff      //      */
/*          const unsigned short C4 = 0xB505;   // Q16   coeff      //      */
/*                                                                          */
/*          // ---------------------------------------------------- //      */
/*          //  Intermediate calculations.                          //      */
/*          // ---------------------------------------------------- //      */
/*          short f0, f1, f2, f3;   // Spatial domain samples.      //      */
/*          short f4, f5, f6, f7;   // Spatial domain samples.      //      */
/*          short g0, g1, h0, h1;   // Even-half intermediate.      //      */
/*          short p0, p1;           // Even-half intermediate.      //      */
/*          short r0, r1, r0_,r1_;  // Even-half intermediate.      //      */
/*          short P0, P1, R0, R1;   // Even-half intermediate.      //      */
/*          short g2, g3, h2, h3;   // Odd-half intermediate.       //      */
/*          short q1a,s1a,q0, q1;   // Odd-half intermediate.       //      */
/*          short s0, s1;           // Odd-half intermediate.       //      */
/*          short Q0, Q1, S0, S1;   // Odd-half intermediate.       //      */
/*          short F0, F1, F2, F3;   // Freq. domain results.        //      */
/*          short F4, F5, F6, F7;   // Freq. domain results.        //      */
/*                                                                          */
/*          // ---------------------------------------------------- //      */
/*          //  Input and output pointers, loop control.            //      */
/*          // ---------------------------------------------------- //      */
/*          unsigned i, j;                                                  */
/*          short    *dct_io_ptr;                                           */
/*                                                                          */
/*          // ---------------------------------------------------- //      */
/*          //  Outer vertical loop -- Process each 8x8 block.      //      */
/*          // ---------------------------------------------------- //      */
/*          dct_io_ptr = dct_data;                                          */
/*          for (i = 0; i < num_fdcts; i++)                                 */
/*          {                                                               */
/*              // ------------------------------------------------ //      */
/*              //  Perform Vertical 1-D FDCT on columns within     //      */
/*              //  each block.  The inputs to this pass are in Q0  //      */
/*              //  and the outputs are in Q1.5.                    //      */
/*              // ------------------------------------------------ //      */
/*              for (j = 0; j < 8; j++)                                     */
/*              {                                                           */
/*                  // -------------------------------------------- //      */
/*                  //  Load the spatial-domain samples.            //      */
/*                  //  The incoming terms start at Q0 precision.   //      */
/*                  // -------------------------------------------- //      */
/*                  f0 = dct_io_ptr[ 0];                                    */
/*                  f1 = dct_io_ptr[ 8];                                    */
/*                  f2 = dct_io_ptr[16];                                    */
/*                  f3 = dct_io_ptr[24];                                    */
/*                  f4 = dct_io_ptr[32];                                    */
/*                  f5 = dct_io_ptr[40];                                    */
/*                  f6 = dct_io_ptr[48];                                    */
/*                  f7 = dct_io_ptr[56];                                    */
/*                                                                          */
/*                  // -------------------------------------------- //      */
/*                  //  Stage 1:  Separate into even & odd halves.  //      */
/*                  //                                              //      */
/*                  //  The results of this stage are implicitly    //      */
/*                  //  in Q1, since we do not explicitly multiply  //      */
/*                  //  by 0.5.                                     //      */
/*                  // -------------------------------------------- //      */
/*                  g0 = f0 + f7;               //  Results in Q1   //      */
/*                  g1 = f1 + f6;               //  Results in Q1   //      */
/*                  h1 = f2 + f5;               //  Results in Q1   //      */
/*                  h0 = f3 + f4;               //  Results in Q1   //      */
/*                  g2 = f3 - f4;               //  Results in Q1   //      */
/*                  g3 = f2 - f5;               //  Results in Q1   //      */
/*                  h3 = f1 - f6;               //  Results in Q1   //      */
/*                  h2 = f0 - f7;               //  Results in Q1   //      */
/*                                                                          */
/*                  // -------------------------------------------- //      */
/*                  //  Stage 2                                     //      */
/*                  //                                              //      */
/*                  //  Note, on the odd-half, the results are in   //      */
/*                  //  Q1.5 since those values are scaled upwards  //      */
/*                  //  by sqrt(2) at this point.                   //      */
/*                  // -------------------------------------------- //      */
/*                  p0 = g0 + h0;               //  Results in Q1   //      */
/*                  p1 = g1 + h1;               //  Results in Q1   //      */
/*                  r0 = g0 - h0;               //  Results in Q1   //      */
/*                  r1 = g1 - h1;               //  Results in Q1   //      */
/*                                                                          */
/*                  q1a = g2 + g2;              //  q1a is now Q2   //      */
/*                  s1a = h2 + h2;              //  s1a is now Q2   //      */
/*                  q1  = (q1a * C4 + 0x8000) >> 16; //  .. in Q1.5 //      */
/*                  s1  = (s1a * C4 + 0x8000) >> 16; //  .. in Q1.5 //      */
/*                                                                          */
/*                  s0 = h3 + g3;               //  Results in Q1.5 //      */
/*                  q0 = h3 - g3;               //  Results in Q1.5 //      */
/*                                                                          */
/*                  // -------------------------------------------- //      */
/*                  //  Stage 3                                     //      */
/*                  //                                              //      */
/*                  //  Now, the even-half ends up in Q1.5.  On P0  //      */
/*                  //  and P1, this happens because the multiply-  //      */
/*                  //  by-C4 was canceled with an upward scaling   //      */
/*                  //  by sqrt(2).  On R0 and R1, this happens     //      */
/*                  //  because C2 and C6 are at Q15.5, and we      //      */
/*                  //  scale r0 and r1 to Q2 before we multiply.   //      */
/*                  // -------------------------------------------- //      */
/*                  P0 = p0 + p1;               //  Results in Q1.5 //      */
/*                  P1 = p0 - p1;               //  Results in Q1.5 //      */
/*                                                                          */
/*                  r0_= r0 + r0;               //  r0_ is now Q2   //      */
/*                  r1_= r1 + r1;               //  r1_ is now Q2   //      */
/*                  R1 = (C6 * r1_+ C2 * r0_+ 0x8000) >>16; // Q1.5 //      */
/*                  R0 = (C6 * r0_- C2 * r1_+ 0x8000) >>16; // Q1.5 //      */
/*                                                                          */
/*                  Q1 = q1 + q0;               //  Results in Q1.5 //      */
/*                  S1 = s1 + s0;               //  Results in Q1.5 //      */
/*                  Q0 = q1 - q0;               //  Results in Q1.5 //      */
/*                  S0 = s1 - s0;               //  Results in Q1.5 //      */
/*                                                                          */
/*                  // -------------------------------------------- //      */
/*                  //  Stage 4                                     //      */
/*                  //  No further changes in Q-point happen here.  //      */
/*                  // -------------------------------------------- //      */
/*                  F0 = P0;                    //  Results in Q1.5 //      */
/*                  F4 = P1;                    //  Results in Q1.5 //      */
/*                  F2 = R1;                    //  Results in Q1.5 //      */
/*                  F6 = R0;                    //  Results in Q1.5 //      */
/*                                                                          */
/*                  F1 = (C7 * Q1 + C1 * S1 + 0x8000) >>16; // Q1.5 //      */
/*                  F7 = (C7 * S1 - C1 * Q1 + 0x8000) >>16; // Q1.5 //      */
/*                  F5 = (C3 * Q0 + C5 * S0 + 0x8000) >>16; // Q1.5 //      */
/*                  F3 = (C3 * S0 - C5 * Q0 + 0x8000) >>16; // Q1.5 //      */
/*                                                                          */
/*                  // -------------------------------------------- //      */
/*                  //  Store the frequency domain results.         //      */
/*                  //  These values are all at Q1.5 precision.     //      */
/*                  // -------------------------------------------- //      */
/*                  dct_io_ptr[ 0] = F0;                                    */
/*                  dct_io_ptr[ 8] = F1;                                    */
/*                  dct_io_ptr[16] = F2;                                    */
/*                  dct_io_ptr[24] = F3;                                    */
/*                  dct_io_ptr[32] = F4;                                    */
/*                  dct_io_ptr[40] = F5;                                    */
/*                  dct_io_ptr[48] = F6;                                    */
/*                  dct_io_ptr[56] = F7;                                    */
/*                                                                          */
/*                  dct_io_ptr++;                                           */
/*              }                                                           */
/*              // ------------------------------------------------ //      */
/*              //  Update pointer to next 8x8 FDCT block.          //      */
/*              // ------------------------------------------------ //      */
/*              dct_io_ptr += 56;                                           */
/*          }                                                               */
/*                                                                          */
/*          // ---------------------------------------------------- //      */
/*          //  Perform Horizontal 1-D FDCT on each 8x8 block.      //      */
/*          // ---------------------------------------------------- //      */
/*          dct_io_ptr = dct_data;                                          */
/*          for (i = 0; i < 8 * num_fdcts; i++)                             */
/*          {                                                               */
/*              // ------------------------------------------------ //      */
/*              //  Load the spatial-domain samples.                //      */
/*              //  The incoming terms are at Q1.5 precision from   //      */
/*              //  the first pass.                                 //      */
/*              // ------------------------------------------------ //      */
/*              f0 = dct_io_ptr[0];                                         */
/*              f1 = dct_io_ptr[1];                                         */
/*              f2 = dct_io_ptr[2];                                         */
/*              f3 = dct_io_ptr[3];                                         */
/*              f4 = dct_io_ptr[4];                                         */
/*              f5 = dct_io_ptr[5];                                         */
/*              f6 = dct_io_ptr[6];                                         */
/*              f7 = dct_io_ptr[7];                                         */
/*                                                                          */
/*              // ------------------------------------------------ //      */
/*              //  Stage 1:  Separate into even and odd halves.    //      */
/*              //                                                  //      */
/*              //  The results of this stage are implicitly in     //      */
/*              //  Q2.5, since we do not explicitly multiply by    //      */
/*              //  0.5.                                            //      */
/*              // ------------------------------------------------ //      */
/*              g0 = f0 + f7;                   //  Results in Q2.5 //      */
/*              g1 = f1 + f6;                   //  Results in Q2.5 //      */
/*              h1 = f2 + f5;                   //  Results in Q2.5 //      */
/*              h0 = f3 + f4;                   //  Results in Q2.5 //      */
/*              g2 = f3 - f4;                   //  Results in Q2.5 //      */
/*              g3 = f2 - f5;                   //  Results in Q2.5 //      */
/*              h3 = f1 - f6;                   //  Results in Q2.5 //      */
/*              h2 = f0 - f7;                   //  Results in Q2.5 //      */
/*                                                                          */
/*              // ------------------------------------------------ //      */
/*              //  Stage 2                                         //      */
/*              //                                                  //      */
/*              //  Note, on the odd-half, the results are in Q3    //      */
/*              //  since those values are scaled upwards by        //      */
/*              //  sqrt(2) at this point.  The order of operations //      */
/*              //  differs in this pass as compared to the first   //      */
/*              //  due to overflow concerns.                       //      */
/*              //                                                  //      */
/*              //  We also inject a rounding term into the DC      //      */
/*              //  term which will also round the Nyquist term,    //      */
/*              //  F4.  This trick works despite the fact that we  //      */
/*              //  are technically still at Q2.5 here, since       //      */
/*              //  the step from Q2.5 to Q3 later is done          //      */
/*              //  implicitly, rather than with a multiply. (This  //      */
/*              //  is due to the sqrt(2) terms cancelling on the   //      */
/*              //  P0/P1 butterfly.)                               //      */
/*              // ------------------------------------------------ //      */
/*              p0 = g0 + h0 + 4;               //  Results in Q2.5 //      */
/*              p1 = g1 + h1;                   //  Results in Q2.5 //      */
/*              r0 = g0 - h0;                   //  Results in Q2.5 //      */
/*              r1 = g1 - h1;                   //  Results in Q2.5 //      */
/*                                                                          */
/*              q1a= (g2 * C4 + 0x8000) >> 16;  //  q1a now in Q2   //      */
/*              s1a= (h2 * C4 + 0x8000) >> 16;  //  s1a now in Q2   //      */
/*              q1 = q1a + q1a;                 //  Results in Q3   //      */
/*              s1 = s1a + s1a;                 //  Results in Q3   //      */
/*                                                                          */
/*              s0 = h3 + g3;                   //  Results in Q3   //      */
/*              q0 = h3 - g3;                   //  Results in Q3   //      */
/*                                                                          */
/*              // ------------------------------------------------ //      */
/*              //  Stage 3                                         //      */
/*              //                                                  //      */
/*              //  Now, the even-half becomes Q0.  This happens    //      */
/*              //  on P0 and P1 because the multiply-by-c4 was     //      */
/*              //  canceled with an upward scaling by sqrt(2),     //      */
/*              //  yielding Q3 intermediate values.  The final     //      */
/*              //  >> 3 leaves these at Q0.  On R0 and R1, this    //      */
/*              //  happens because c2 and c6 are at Q13.5,         //      */
/*              //  yielding Q16 intermediate values.  The final    //      */
/*              //  >> 16 then leaves those values at Q0.           //      */
/*              // ------------------------------------------------ //      */
/*              P0 = ((short)(p0 + p1)) >> 3;   //  Results in Q0   //      */
/*              P1 = ((short)(p0 - p1)) >> 3;   //  Results in Q0   //      */
/*              R1 = (c6 * r1 + c2 * r0 + 0x8000) >> 16; // .. Q0   //      */
/*              R0 = (c6 * r0 - c2 * r1 + 0x8000) >> 16; // .. Q0   //      */
/*                                                                          */
/*              Q1 = q1 + q0;                   //  Results in Q3   //      */
/*              Q0 = q1 - q0;                   //  Results in Q3   //      */
/*              S1 = s1 + s0;                   //  Results in Q3   //      */
/*              S0 = s1 - s0;                   //  Results in Q3   //      */
/*                                                                          */
/*              // ------------------------------------------------ //      */
/*              //  Stage 4                                         //      */
/*              //                                                  //      */
/*              //  Next, the odd-half ends up in Q0. This happens  //      */
/*              //  because our values are in Q3 and our cosine     //      */
/*              //  terms are in Q13, giving us Q16 intermediate    //      */
/*              //  values. The final >> 16 leaves us a Q0 result.  //      */
/*              // ------------------------------------------------ //      */
/*              F0 = P0;                        //  Results in Q0   //      */
/*              F4 = P1;                        //  Results in Q0   //      */
/*              F2 = R1;                        //  Results in Q0   //      */
/*              F6 = R0;                        //  Results in Q0   //      */
/*                                                                          */
/*              F1 = (c7 * Q1 + c1 * S1 + 0x8000) >> 16; // .. Q0   //      */
/*              F7 = (c7 * S1 - c1 * Q1 + 0x8000) >> 16; // .. Q0   //      */
/*              F5 = (c3 * Q0 + c5 * S0 + 0x8000) >> 16; // .. Q0   //      */
/*              F3 = (c3 * S0 - c5 * Q0 + 0x8000) >> 16; // .. Q0   //      */
/*                                                                          */
/*              // ------------------------------------------------ //      */
/*              //  Store the results                               //      */
/*              // ------------------------------------------------ //      */
/*              dct_io_ptr[0] = F0;                                         */
/*              dct_io_ptr[1] = F1;                                         */
/*              dct_io_ptr[2] = F2;                                         */
/*              dct_io_ptr[3] = F3;                                         */
/*              dct_io_ptr[4] = F4;                                         */
/*              dct_io_ptr[5] = F5;                                         */
/*              dct_io_ptr[6] = F6;                                         */
/*              dct_io_ptr[7] = F7;                                         */
/*                                                                          */
/*              // ------------------------------------------------ //      */
/*              //  Update pointer to next FDCT row.                //      */
/*              // ------------------------------------------------ //      */
/*              dct_io_ptr += 8;                                            */
/*          }                                                               */
/*                                                                          */
/*          return;                                                         */
/*      }                                                                   */
/*                                                                          */
/*                                                                          */
/*  TECHNIQUES                                                              */
/*      The loop nest in the vertical pass has been collapsed into a        */
/*      single-level loop.  Both vertical and horizontal loops have         */
/*      been software pipelined.                                            */
/*                                                                          */
/*      For performance, portions of the code outside the loops have been   */
/*      inter-scheduled with the prolog and epilog code of the loops.       */
/*      Also, twin stack-pointers are used to accelerate stack accesses.    */
/*      Finally, pointer values and cosine term registers are reused        */
/*      between the horizontal and vertical loops to reduce the impact of   */
/*      pointer and constant reinitialization.                              */
/*                                                                          */
/*      To save codesize, prolog and epilog collapsing have been performed  */
/*      to the extent that it does not impact performance.                  */
/*                                                                          */
/*      To reduce register pressure and save some code, the horizontal      */
/*      loop uses the same pair of pointer register for both reading and    */
/*      writing.  The pointer increments are on the LDs to permit prolog    */
/*      and epilog collapsing, since LDs can be speculated.                 */
/*                                                                          */
/*  ASSUMPTIONS                                                             */
/*      Stack is aligned to a double-word boundary.                         */
/*                                                                          */
/*  MEMORY NOTE                                                             */
/*      No bank conflicts occur.                                            */
/*                                                                          */
/*      The "fdct_data[]" array must be aligned on a double-word (8 byte)   */
/*      boundary.                                                           */
/*                                                                          */
/*      The code requires 4 words of stack space to save Save-On-Entry      */
/*      (SOE) registers.                                                    */
/*                                                                          */
/*      Nearly every cycle of this function performs at least one           */
/*      memory access.                                                      */
/*                                                                          */
/*  NOTES                                                                   */
/*      This code is fully interruptible.  Interrupts are blocked only      */
/*      branch delay slots.                                                 */
/*                                                                          */
/*      The cosine terms have all been scaled by sqrt(2), so that the       */
/*      "c4" term is basically an even power of 2.                          */
/*                                                                          */
/*      The code is LITTLE ENDIAN.                                          */
/*                                                                          */
/*  CYCLES                                                                  */
/*      cycles = 50 + 76 * num_fdcts                                        */
/*                                                                          */
/*      For num_fdcts =  6, cycles = 506.                                   */
/*      For num_fdcts = 24, cycles = 1848.                                  */
/*                                                                          */
/*  CODESIZE                                                                */
/*      980 bytes.                                                          */
/*                                                                          */
/*  SOURCE                                                                  */
/*      Chen FDCT.                                                          */
/*                                                                          */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */
#ifndef FDCT_8X8_H_H_
#define FDCT_8X8_H_H_ 1

void IMG_fdct_8x8(short fdct_data[], unsigned num_fdcts);

#endif
/* ======================================================================== */
/*  End of file:  fdct_8x8_h.h                                              */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */
