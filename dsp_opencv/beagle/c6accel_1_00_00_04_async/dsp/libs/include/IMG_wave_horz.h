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
/*==========================================================================S*/
/*                                                                           */
/*                                                                           */
/*    TEXAS INSTRUMENTS, INC.                                                */
/*                                                                           */
/*    NAME                                                                   */
/*          wave_horz : 1D Wavelet Transform                                 */
/*                                                                           */
/*    REVISION HISTORY                                                       */
/*       Date ...Original version written ....................... J.Sankaran */
/*       Date ...Last modified Neatened Code .................... J.Sankaran */
/*                                                                           */
/*    USAGE                                                                  */
/*          This routine is C-callable and can be called as:                 */
/*                                                                           */
/*          void wave_horz_co(short *iptr, short *qmf, short *filter,        */
/*                                            short * optr, int  ish_x_dim ) */
/*                 iptr =  input row of data                                 */
/*                 qmf =  qmf filter-bank for Low-Pass                       */
/*                 filter =  Mirror qmf filter bank for High_pass            */
/*                 optr = output row of detailed/reference decimated outputs */
/*                 ish_x_dim = width of the input row                        */
/*                                                                           */
/*          (See the C compiler reference guide.)                            */
/*                                                                           */
/*    DESCRIPTION                                                            */
/*                                                                           */
/*           This kernel performs a 1D Periodic Orthogonal Wavelet decomp-   */
/*    osition. This also performs athe row decomposition in a 2D wavelet     */
/*    transform. An in put signal x[n] is first low pass and high pass       */
/*    filterd and decimated by two. This results in a reference signal       */
/*    r1[n] which is the decimated output obtained by dropping the odd       */
/*    samples of the low pass filtered output and a detail signal d[n]       */
/*    obtained by dropping the odd samples of the high-pass output. A        */
/*    circular convolution algorithm is implemented and hence the wave-      */
/*    let transform is periodic. The reference signal and the detail         */
/*    signal are half the size of the original signal. The reference         */
/*    signal may then be iterated again to perform another scale of          */
/*    multi-resolution analysis.                                             */
/*                                                                           */
/*                                                                           */
/*    ASSUMPTIONS                                                            */
/*       This kernel uses the Daubechies D4 filter bank for analysis with 4  */
/*    vansishing moments. Hence the length of the analyzing low-pass and     */
/*    high pass filters is 8.                                                */
/*                                                                           */
/*    MEMORY NOTE                                                            */
/*       Serial Assembly kernel should not have any bank conflicts           */
/*                                                                           */
/*    TECHNIQUES                                                             */
/*       The main idea behind the optimized C code is to issue one set of    */
/*    reads to the x array and to perform low-pass and high pass filtering   */
/*    together and to perfrom the filtering operations together to maximize  */
/*    the number of multiplies.  The last 6 elements of the low-pass filter  */
/*    and the first 6 elements of the high pass filter use the same input    */
/*    This is used to appropraitely change the output pointer to the low     */
/*    pass filter after 6 iterations. However for the first six iterations   */
/*    pointer wrap-around can occurr and hence this creates a dependency.    */
/*    Pre-reading those 6 values outside the array prevents the checks that  */
/*    introduce this dependency. In addtion the input data is read as word   */
/*    wide quantities and the low-pass and high-pass filter coefficients     */
/*    are stored in registers allowing for the input loop to be completely   */
/*    unrolled. Thus the intrinsic C code has only one loop. A predication   */
/*    register is used to reset the low-pass output pointer after three      */
/*    iterations. The merging of the loops in this fashion allows for the    */
/*    maximum number of multiplies with the minimum number of reads.         */
/*==========================================================================S*/
/*            Copyright (c) 2008 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/*==========================================================================S*/
#ifndef WAVE_HORZ_P_H_
#define WAVE_HORZ_P_H_ 1

void wave_horz_sa(short *iptr, short *qmf, short *filter,
                                  short * optr, int  ish_x_dim );

#endif
/* ======================================================================== */
/*  End of file:  wave_horz_p.h                                             */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */
