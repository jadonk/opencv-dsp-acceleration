/* ======================================================================= */
/* DSPF_sp_w_vec.h -- Vector Weighted Add                                  */
/*              Optimized C Implementation (w/ Intrinsics)                 */
/*                                                                         */
/* Rev 0.0.1                                                               */
/*                                                                         */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */

#ifndef _DSPF_SP_W_VEC_H_
#define _DSPF_SP_W_VEC_H_

void DSPF_sp_w_vec(const float *x1, const float *x2, const float m,
    float *restrict y, const int n);

#endif /* _DSPF_SP_W_VEC_H_ */

/* ======================================================================= */
/*  End of file:  DSPF_sp_w_vec.h                                          */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */

