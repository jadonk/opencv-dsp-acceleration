/* ======================================================================= */
/* DSPF_sp_dotp_cplx.h -- Complex Dot Product                              */
/*              Optimized C Implementation (w/ Intrinsics)                 */
/*                                                                         */
/* Rev 0.0.1                                                               */
/*                                                                         */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */

#ifndef _DSPF_SP_DOTP_CPLX_H_
#define _DSPF_SP_DOTP_CPLX_H_

void DSPF_sp_dotp_cplx(const float * x, const float * y, int n,
    float * restrict re, float * restrict im);

#endif /* _DSPF_SP_DOTP_CPLX_H_ */

/* ======================================================================= */
/*  End of file:  DSPF_sp_dotp_cplx.h                                      */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */

