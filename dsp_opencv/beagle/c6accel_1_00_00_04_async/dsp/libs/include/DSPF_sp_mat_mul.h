/* ======================================================================= */
/* DSPF_sp_mat_mul.h -- Matrix Multiply                                    */
/*              Optimized C Implementation (w/ Intrinsics)                 */
/*                                                                         */
/* Rev 0.0.1                                                               */
/*                                                                         */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */

#ifndef _DSPF_SP_MAT_MUL_H_
#define _DSPF_SP_MAT_MUL_H_

void DSPF_sp_mat_mul(float *x1, const int r1, const int c1,
    float *x2, const int c2, float *restrict y);

#endif /* _DSPF_SP_MAT_MUL_H_ */

/* ======================================================================= */
/*  End of file:  DSPF_sp_mat_mul.h                                        */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */

