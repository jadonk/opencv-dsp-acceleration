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
/*============================================================================*/
/*                                                                            */
/*     TEXAS INSTRUMENTS, INC.                                                */
/*                                                                            */
/*     NAME                                                                   */
/*        histogram                                                           */
/*                                                                            */
/*     REVISION HISTORY                                                       */
/*        8/4/99..Original version written .......................... D.Hoyle */
/*        Date ...Last modified Neatened Code ....................... D.Hoyle */
/*                                                                            */
/*     USAGE                                                                  */
/*        This routine is C-callable and can be called as:                    */
/*                                                                            */
/*        void histogram_8(unsigned char * image, int  n, int  accumulate,    */
/*                         unsigned short * t_hist, unsigned short * hist )   */
/*                                                                            */
/*                  image      =  input image data                            */
/*                  n          =  number of points                            */
/*                  accumulate =  defines add/subtract from existing          */
/*                                histogram: takes values 1, -1               */
/*                  t_hist     =  temporary histogram bins (1024)             */
/*                  hist       =  running histogram bins (256)                */
/*                                                                            */
/*           (See the C compiler reference guide.)                            */
/*                                                                            */
/*     DESCRIPTION                                                            */
/*        This code takes a histogram of an array of n, 8 bit inputs. It      */
/*        returns the histogram of 256 bins at 16 bit precision. It can       */
/*        either add  or subtract to an existing histogram, using             */
/*        the 'accumulate' control.                                           */
/*        It requires some temporary storage for 4 temporary histograms,      */
/*        which are later summed together.                                    */
/*                                                                            */
/*     ASSUMPTIONS                                                            */
/*        It is assumed that the temporary array of data, t_hist is           */
/*        initialised to zero. The input array of image data must be aligned  */
/*        to a 4 byte boundary and n must be a multiple of 8. The maximum     */
/*        number of pixels that can be profiled in each bin is 65535 in       */
/*        the main histogram and the maximum n is 262143.                     */
/*                                                                            */
/*     MEMORY NOTE                                                            */
/*        No bank conflicts should occur in this code in the optimized        */
/*        forms of hand assembly and serial assembly.                         */
/*                                                                            */
/*     TECHNIQUES                                                             */
/*        This code operates on four interleaved histogram bins. The loop     */
/*        is divided into two halves. The even half operates on even words    */
/*        full of pixels and the odd half operates on odd words. Both         */
/*        halves operate on the same 4 histogram bins. This introduces a      */
/*        memory dependency which ordinarily would degrade performance. To    */
/*        break the memory depenencies, the two halves forward their results  */
/*        to each other.                                                      */
/*        Exact memory access ordering obviates the need to predicate stores  */
/*         The algorithm is ordered as follows:                               */
/*                                                                            */
/*          1. Load from histogram for even half                              */
/*          2. Store odd_bin to histogram for odd half (previous itn.)        */
/*          3. if data_even == previous data_odd increment even_bin by 2      */
/*             else increment even_bin by 1, forward to odd                   */
/*          4. Load from histogram for odd half (current itn.)                */
/*          5. Store even_bin to histogram for even half                      */
/*          6. if data_odd == previous data_even increment odd_bin by 2       */
/*             else increment odd_bin by 1, forward to even                   */
/*          7. goto 1.                                                        */
/*                                                                            */
/*        With this particular ordering, forwarding is necessary between      */
/*        even/odd halves when pixels in adjacent halves need to be           */
/*        placed in the same bin.                                             */
/*        The store is never predicated and occurs speculatively              */
/*        as it will be overwritten by the next value containing the          */
/*        extra forwarded value.                                              */
/*                                                                            */
/*        The four histograms are interleaved with each bin spaced four       */
/*        half-words apart and each histogram starting in a different         */
/*        memory bank. This allows the four histogram accesses to             */
/*        proceed in any order without worrying about bank conflicts.         */
/*        The diagram below illustrates this:  (addresses are halfword        */
/*        offsets)                                                            */
/*                                                                            */
/*             0       1       2       3       4       5       6   ...        */
/*         | hst 0 | hst 1 | hst 2 | hst 3 | hst 0 | hst 1 | ...   ...        */
/*         | bin 0 | bin 0 | bin 0 | bin 0 | bin 1 | bin 1 | ...   ...        */
/*                                                                            */
/*     SOURCE                                                                 */
/*         Optimized c code based on assembly code loop developed by          */
/*         J. Zbiciak, 1998                                                   */
/*                                                                            */
/*============================================================================*/
/*      Copyright (C) 1997-1999 Texas Instruments Incorporated.               */
/*                      All Rights Reserved                                   */
/*============================================================================*/
void IMG_histogram_8
(
    const unsigned char *restrict image,
    int   n,
    short accumulate,
    short *restrict t_hist,
    short *restrict hist
);
/*============================================================================*/
/*      Copyright (C) 1997-1999 Texas Instruments Incorporated.               */
/*                      All Rights Reserved                                   */
/*============================================================================*/
