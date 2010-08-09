/* ======================================================================= */
/* DSPF_sp_mat_mul_cplx.h -- Complex Matrix Multiply                       */
/*              Optimized C Implementation (w/ Intrinsics)                 */
/*                                                                         */
/* Rev 0.0.1                                                               */
/*                                                                         */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */

#ifndef _DSPF_SP_MAT_MUL_CPLX_H_
#define _DSPF_SP_MAT_MUL_CPLX_H_

void DSPF_sp_mat_mul_cplx(float* x1, const int r1, const int c1,
    const float* x2, const int c2, float* restrict y);

#endif /* _DSPF_SP_MAT_MUL_CPLX_H_ */

/* ======================================================================= */
/*  End of file:  DSPF_sp_mat_mul_cplx.h                                   */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */

