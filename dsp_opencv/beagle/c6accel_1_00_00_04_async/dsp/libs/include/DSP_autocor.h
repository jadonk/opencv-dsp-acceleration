/* ======================================================================== */
/*  NAME                                                                    */
/*      DSP_autocor -- Autocorrelation                                      */
/*                                                                          */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2007 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */

#ifndef DSP_AUTOCOR_H_
#define DSP_AUTOCOR_H_ 1

void DSP_autocor (
    short *restrict r,
    const short *restrict x,
    int nx,
    int nr
);

#endif

/* ======================================================================== */
/*  End of file:  DSP_autocor.h                                             */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2007 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */

