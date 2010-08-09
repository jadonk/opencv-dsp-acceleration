
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
/*  NAME                                                                    */
/*      idct_8x8_12q4 -- IEEE-1180/1990 Compliant IDCT, Little Endian.      */
/*                                                                          */
/*  USAGE                                                                   */
/*      This routine is C callable, and has the following C prototype:      */
/*                                                                          */
/*          void idct_8x8_12q4(short idct_data[], unsigned num_idcts)       */
/*                                                                          */
/*      The idct_8x8_12q4 routine accepts a list of 8x8 DCT coeffient blocks*/
/*      and performs IDCTs on each.  The array should be aligned to a       */
/*      64-bit boundary, and be laid out equivalently to the C array        */
/*      idct_data[num_idcts][8][8].  The input data should be in 12Q4       */
/*      format.                                                             */
/*                                                                          */
/*      The routine operates entirely in-place, requiring no additional     */
/*      storage for intermediate results.                                   */
/*                                                                          */
/*      This code requires '62 + 92 * num_idcts' cycles to process          */
/*      'num_idcts' blocks, including 6 cycles of function call overhead.   */
/*                                                                          */
/*  DESCRIPTION                                                             */
/*      The idct_8x8_12q4 algorithm performs an IEEE-1180 compliant IDCT,   */
/*      complete with rounding and saturation to signed 9-bit quantities.   */
/*      The input coefficients are assumed to be signed 16-bit DCT          */
/*      coefficients in 12Q4 format.                                        */
/*                                                                          */
/*      void idct_8x8_12q4(short idct_data[], unsigned num_idcts)           */
/*      {                                                                   */
/*          // -------------------------------------------------------- //  */
/*          //  Cosine Constants (Q16, scaled down by sqrt(2)).         //  */
/*          // -------------------------------------------------------- //  */
/*          const unsigned short C0 = 0xB505;                               */
/*          const unsigned short C1 = 0xB18B, C2 = 0xA73D;                  */
/*          const unsigned short C3 = 0x9683, C5 = 0x6492;                  */
/*          const unsigned short C6 = 0x4546, C7 = 0x2351;                  */
/*                                                                          */
/*          // -------------------------------------------------------- //  */
/*          //  Intermediate values (used in both loops).               //  */
/*          // -------------------------------------------------------- //  */
/*          short F0, F1, F2, F3, F4, F5, F6, F7;  // stage 0           //  */
/*          short P0, P1, R0, R1, Q0, Q1, S0, S1;  // stage 1           //  */
/*          short p0, p1, r0, r1, q0, q1, s0, s1;  // stage 2           //  */
/*          short g0, g1, g2, g3, h0, h1, h2, h3;  // stage 3           //  */
/*          short f0, f1, f2, f3, f4, f5, f6, f7;  // stage 4           //  */
/*          short f0r,f1r,f2r,f3r,f4r,f5r,f6r,f7r; // rounded           //  */
/*          int   f0s,f1s,f2s,f3s,f4s,f5s,f6s,f7s; // saturated         //  */
/*          int   f0t,f1t,f2t,f3t,f4t,f5t,f6t,f7t; // truncated         //  */
/*          int   i, j;                            // loop counts       //  */
/*          short (*idct)[8][8] = (short (*)[8][8])idct_data;               */
/*                                                                          */
/*          // -------------------------------------------------------- //  */
/*          //  Vertical Pass                                           //  */
/*          //                                                          //  */
/*          //  This pass performs a single 8-pt IDCT per iteration.    //  */
/*          //  Inputs are in 12Q4 format, and results of this pass     //  */
/*          //  are in 11Q5 format. (Actually, the results are halfway  //  */
/*          //  between 11Q5 and 12Q4 due to the scaling by sqrt(2).)   //  */
/*          //                                                          //  */
/*          //  The outer loop steps between IDCT blocks, whereas the   //  */
/*          //  inner loop focuses on columns within each IDCT block.   //  */
/*          // -------------------------------------------------------- //  */
/*          for (i = 0; i < num_idcts; i++)                                 */
/*          {                                                               */
/*              for (j = 0; j < 8; j++)                                     */
/*              {                                                           */
/*                  // ------------------------------------------------ //  */
/*                  //  Stage 0:  Load in freq-domain coefficients.     //  */
/*                  // ------------------------------------------------ //  */
/*                  F0 = idct[i][0][j];                                     */
/*                  F1 = idct[i][1][j];                                     */
/*                  F2 = idct[i][2][j];                                     */
/*                  F3 = idct[i][3][j];                                     */
/*                  F4 = idct[i][4][j];                                     */
/*                  F5 = idct[i][5][j];                                     */
/*                  F6 = idct[i][6][j];                                     */
/*                  F7 = idct[i][7][j];                                     */
/*                                                                          */
/*                  // ------------------------------------------------ //  */
/*                  //  Stage 1 of signal flow graph.                   //  */
/*                  // ------------------------------------------------ //  */
/*                  P0 = F0;                P1 = F4;                        */
/*                  R1 = F2;                R0 = F6;                        */
/*                                                                          */
/*                  Q1 = (F1*C7 - F7*C1 + 0x8000) >> 16;                    */
/*                  Q0 = (F5*C3 - F3*C5 + 0x8000) >> 16;                    */
/*                  S0 = (F5*C5 + F3*C3 + 0x8000) >> 16;                    */
/*                  S1 = (F1*C1 + F7*C7 + 0x8000) >> 16;                    */
/*                                                                          */
/*                  // ------------------------------------------------ //  */
/*                  //  Stage 2 of signal flow graph.                   //  */
/*                  // ------------------------------------------------ //  */
/*                  p0 = ((int)P0 + (int)P1 + 1 ) >> 1;                     */
/*                  p1 = ((int)P0 - (int)P1     ) >> 1;                     */
/*                  r1 = (R1*C6 - R0*C2 + 0x8000) >> 16;                    */
/*                  r0 = (R1*C2 + R0*C6 + 0x8000) >> 16;                    */
/*                                                                          */
/*                  s1 = (S1 + S0);         q1 = (Q1 + Q0);                 */
/*                  s0 = (S1 - S0);         q0 = (Q1 - Q0);                 */
/*                                                                          */
/*                  // ------------------------------------------------ //  */
/*                  //  Stage 3 of signal flow graph.                   //  */
/*                  // ------------------------------------------------ //  */
/*                  g0 = (p0 + r0);         g1 = (p1 + r1);                 */
/*                  h0 = (p0 - r0);         h1 = (p1 - r1);                 */
/*                                                                          */
/*                  h2 = s1;                g2 = q1;                        */
/*                  g3 = (s0*C0 - q0*C0 + 0x8000) >> 16;                    */
/*                  h3 = (s0*C0 + q0*C0 + 0x8000) >> 16;                    */
/*                                                                          */
/*                  // ------------------------------------------------ //  */
/*                  //  Stage 4 of signal flow graph.                   //  */
/*                  // ------------------------------------------------ //  */
/*                  f0 = (g0 + h2);         f7 = (g0 - h2);                 */
/*                  f1 = (g1 + h3);         f6 = (g1 - h3);                 */
/*                  f2 = (h1 + g3);         f5 = (h1 - g3);                 */
/*                  f3 = (h0 + g2);         f4 = (h0 - g2);                 */
/*                                                                          */
/*                  // ------------------------------------------------ //  */
/*                  //  Stage 5:  Write sample-domain results.          //  */
/*                  // ------------------------------------------------ //  */
/*                  idct[i][0][j] = f0;                                     */
/*                  idct[i][1][j] = f1;                                     */
/*                  idct[i][2][j] = f2;                                     */
/*                  idct[i][3][j] = f3;                                     */
/*                  idct[i][4][j] = f4;                                     */
/*                  idct[i][5][j] = f5;                                     */
/*                  idct[i][6][j] = f6;                                     */
/*                  idct[i][7][j] = f7;                                     */
/*              }                                                           */
/*          }                                                               */
/*                                                                          */
/*          // -------------------------------------------------------- //  */
/*          //  Horizontal Pass                                         //  */
/*          //                                                          //  */
/*          //  This performs one IDCT per iteration on the 11Q5        //  */
/*          //  results from the previous pass.  Both horizontal and    //  */
/*          //  vertical passes are scaled down by sqrt(2) -- the net   //  */
/*          //  effect of which is that the IDCT results generated by   //  */
/*          //  this pass (prior to saturation) are also 11Q5 results,  //  */
/*          //  only with no sqrt(2) factors remaining.                 //  */
/*          //                                                          //  */
/*          //  The IDCT butterflies in this pass are identical to the  //  */
/*          //  ones in the vertical pass, except for an additional     //  */
/*          //  rounding value which is added into the DC term early    //  */
/*          //  in the flow graph.                                      //  */
/*          //                                                          //  */
/*          //  The 11Q5 sample-domain terms are saturated to 9Q7       //  */
/*          //  values, and then truncated to 9Q0 results before        //  */
/*          //  storing.                                                //  */
/*          //                                                          //  */
/*          //  The outer loop steps between IDCT blocks, whereas the   //  */
/*          //  inner loop focuses on rows within each IDCT block.      //  */
/*          // -------------------------------------------------------- //  */
/*          for (i = num_idcts - 1; i >= 0; i--)                            */
/*          {                                                               */
/*              for (j = 0; j < 8; j++)                                     */
/*              {                                                           */
/*                  // ------------------------------------------------ //  */
/*                  //  Stage 0:  Load in freq.-domain coefficients.    //  */
/*                  // ------------------------------------------------ //  */
/*                  F0 = idct[i][j][0];                                     */
/*                  F1 = idct[i][j][1];                                     */
/*                  F2 = idct[i][j][2];                                     */
/*                  F3 = idct[i][j][3];                                     */
/*                  F4 = idct[i][j][4];                                     */
/*                  F5 = idct[i][j][5];                                     */
/*                  F6 = idct[i][j][6];                                     */
/*                  F7 = idct[i][j][7];                                     */
/*                                                                          */
/*                  // ------------------------------------------------ //  */
/*                  //  Stage 1 of signal flow graph.                   //  */
/*                  // ------------------------------------------------ //  */
/*                  P0 = F0;                P1 = F4;                        */
/*                  R1 = F2;                R0 = F6;                        */
/*                                                                          */
/*                  Q1 = (F1*C7 - F7*C1 + 0x8000) >> 16;                    */
/*                  Q0 = (F5*C3 - F3*C5 + 0x8000) >> 16;                    */
/*                  S0 = (F5*C5 + F3*C3 + 0x8000) >> 16;                    */
/*                  S1 = (F1*C1 + F7*C7 + 0x8000) >> 16;                    */
/*                                                                          */
/*                  // ------------------------------------------------ //  */
/*                  //  Stage 2 of signal flow graph.                   //  */
/*                  // ------------------------------------------------ //  */
/*                  p0 = (((int)P0 + (int)P1 + 1) >> 1) + 15;               */
/*                  p1 = (((int)P0 - (int)P1    ) >> 1) + 16;               */
/*                  r1 = (R1*C6 - R0*C2 + 0x8000) >> 16;                    */
/*                  r0 = (R1*C2 + R0*C6 + 0x8000) >> 16;                    */
/*                                                                          */
/*                  s1 = (S1 + S0);         q1 = (Q1 + Q0);                 */
/*                  s0 = (S1 - S0);         q0 = (Q1 - Q0);                 */
/*                                                                          */
/*                  // ------------------------------------------------ //  */
/*                  //  Stage 3 of signal flow graph.                   //  */
/*                  // ------------------------------------------------ //  */
/*                  g0 = (p0 + r0);         g1 = (p1 + r1);                 */
/*                  h0 = (p0 - r0);         h1 = (p1 - r1);                 */
/*                                                                          */
/*                  h2 = s1;                g2 = q1;                        */
/*                  g3 = (s0*C0 - q0*C0 + 0x8000) >> 16;                    */
/*                  h3 = (s0*C0 + q0*C0 + 0x8000) >> 16;                    */
/*                                                                          */
/*                  // ------------------------------------------------ //  */
/*                  //  Stage 4 of signal flow graph.                   //  */
/*                  // ------------------------------------------------ //  */
/*                  f0 = (g0 + h2);         f7 = (g0 - h2);                 */
/*                  f1 = (g1 + h3);         f6 = (g1 - h3);                 */
/*                  f2 = (h1 + g3);         f5 = (h1 - g3);                 */
/*                  f3 = (h0 + g2);         f4 = (h0 - g2);                 */
/*                                                                          */
/*                  // ------------------------------------------------ //  */
/*                  //  Stage 4.1:  Q-pt adjust: Bit 15 is don't-care.  //  */
/*                  // ------------------------------------------------ //  */
/*                  f0r = f0 + f0;          f7r = f7 + f7;                  */
/*                  f1r = f1 + f1;          f6r = f6 + f6;                  */
/*                  f2r = f2 + f2;          f5r = f5 + f5;                  */
/*                  f3r = f3 + f3;          f4r = f4 + f4;                  */
/*                                                                          */
/*                  // ------------------------------------------------ //  */
/*                  //  Stage 4.2:  Saturate results to 9Q6.            //  */
/*                  // ------------------------------------------------ //  */
/*                  f0s = f0r>0x3FFF?0x3FFF: f0r<-0x4000?-0x4000 : f0r;     */
/*                  f1s = f1r>0x3FFF?0x3FFF: f1r<-0x4000?-0x4000 : f1r;     */
/*                  f2s = f2r>0x3FFF?0x3FFF: f2r<-0x4000?-0x4000 : f2r;     */
/*                  f3s = f3r>0x3FFF?0x3FFF: f3r<-0x4000?-0x4000 : f3r;     */
/*                  f4s = f4r>0x3FFF?0x3FFF: f4r<-0x4000?-0x4000 : f4r;     */
/*                  f5s = f5r>0x3FFF?0x3FFF: f5r<-0x4000?-0x4000 : f5r;     */
/*                  f6s = f6r>0x3FFF?0x3FFF: f6r<-0x4000?-0x4000 : f6r;     */
/*                  f7s = f7r>0x3FFF?0x3FFF: f7r<-0x4000?-0x4000 : f7r;     */
/*                                                                          */
/*                  // ------------------------------------------------ //  */
/*                  //  Stage 4.3:  Truncate results to 9Q0.            //  */
/*                  // ------------------------------------------------ //  */
/*                  f0t = f0s >> 6;         f7t = f7s >> 6;                 */
/*                  f1t = f1s >> 6;         f6t = f6s >> 6;                 */
/*                  f2t = f2s >> 6;         f5t = f5s >> 6;                 */
/*                  f3t = f3s >> 6;         f4t = f4s >> 6;                 */
/*                                                                          */
/*                  // ------------------------------------------------ //  */
/*                  //  Stage 5:  Store sample-domain results.          //  */
/*                  // ------------------------------------------------ //  */
/*                  idct[i][j][0] = f0t;                                    */
/*                  idct[i][j][1] = f1t;                                    */
/*                  idct[i][j][2] = f2t;                                    */
/*                  idct[i][j][3] = f3t;                                    */
/*                  idct[i][j][4] = f4t;                                    */
/*                  idct[i][j][5] = f5t;                                    */
/*                  idct[i][j][6] = f6t;                                    */
/*                  idct[i][j][7] = f7t;                                    */
/*              }                                                           */
/*          }                                                               */
/*                                                                          */
/*          return;                                                         */
/*      }                                                                   */
/*                                                                          */
/*      Note:  This code guarantees correct operation, even in the case     */
/*      that 'num_idcts == 0'.  In that case, the function runs for only    */
/*      13 cycles (counting 6 cycles of function-call overhead), due to     */
/*      early-exit code.  Also, the assembly code imposes additional data   */
/*      alignment restrictions that are not present in the C code above.    */
/*                                                                          */
/*  TECHNIQUES                                                              */
/*      All levels of looping are collapsed into single loops which are     */
/*      pipelined.  The outer loop focuses on 8-pt IDCTs, whereas the       */
/*      inner loop controls the column-pointer to handle jumps between      */
/*      IDCT blocks.  (The column-pointer adjustment is handled by a        */
/*      four-phase rotating "fixup" constant which takes the place of       */
/*      the original inner-loop.)                                           */
/*                                                                          */
/*      For performance, portions of the outer-loop code have been          */
/*      inter-scheduled with the prologs and epilogs of both loops.         */
/*      Finally, cosine term registers are reused between the horizontal    */
/*      and vertical loops to save the need for reinitialization.           */
/*                                                                          */
/*      To save codesize, prolog and epilog collapsing have been performed  */
/*      to the extent that performance is not affected.  The remaining      */
/*      prolog and epilog code has been interscheduled with code outside    */
/*      the loops to improve performance.                                   */
/*                                                                          */
/*      Additional section-specific optimization notes are provided below.  */
/*                                                                          */
/*  ASSUMPTIONS                                                             */
/*      This is a LITTLE ENDIAN implementation.                             */
/*                                                                          */
/*      The input array must be aligned on a double-word boundary.          */
/*                                                                          */
/*  MEMORY NOTE                                                             */
/*      No bank conflicts occur.                                            */
/*      The input array must be aligned on a double-word boundary.          */
/*                                                                          */
/*      Bank usage for N 32-bit banks:                                      */
/*                                                                          */
/*          Vert loop accesses:  2 of N banks for 54% of cycles             */
/*                               1 of N banks for 36% of cycles             */
/*                               0 of N banks for  9% of cycles             */
/*                                                                          */
/*          Horiz loop accesses: 4 of N banks for 16% of cycles             */
/*                               2 of N banks for 33% of cycles             */
/*                               0 of N banks for 50% of cycles             */
/*                                                                          */
/*      The code may perform speculative reads of up to 128 bytes           */
/*      beyond the end or before the start of the IDCT array.  The          */
/*      speculatively accessed data is ignored.                             */
/*                                                                          */
/*  NOTES                                                                   */
/*      This is fully interruptable and fully reentrant.                    */
/*                                                                          */
/*      The cosine terms have all been scaled by sqrt(2), so that the       */
/*      "c4" term is basically an even power of 2.                          */
/*                                                                          */
/*  CYCLES                                                                  */
/*      cycles = 62 + 92 * num_idcts, for num_idcts > 0                     */
/*      cycles = 13,                  for num_idcts == 0.                   */
/*                                                                          */
/*      For num_idcts =  6, cycles = 614.                                   */
/*      For num_idcts = 24, cycles = 2270.                                  */
/*                                                                          */
/*  CODESIZE                                                                */
/*      968 bytes                                                           */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */
#ifndef IDCT_8X8_H_H_
#define IDCT_8X8_H_H_ 1

void IMG_idct_8x8_12q4(short idct_data[], unsigned num_idcts);

#endif
/* ======================================================================== */
/*  End of file:  idct_8x8_12q4_h.h                                         */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */
