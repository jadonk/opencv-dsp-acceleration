/* ======================================================================= */
/* DSP_dotp_sqr.h --  Vector Dot Product and Square implementation         */
/*                    Intrinsic C Implementation                           */
/*                                                                         */
/* Rev 0.0.1                                                               */
/*                                                                         */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2007 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */

#ifndef DSP_DOTP_SQR_H_
#define DSP_DOTP_SQR_H_

int DSP_dotp_sqr (
    int G,            /* Sum-of-y-squared initial value */
    const short  *x,  /* First input array              */
    const short  *y,  /* Second input array.            */
    int *restrict d,  /* Accumulation of x[] * y[].     */
    int nx            /* Length of the input vector.    */
);

#endif /* DSP_DOTP_SQR_H_ */

/* ======================================================================== */
/*  End of file:  DSP_dotp_sqr.h                                            */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2007 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */

