/* ======================================================================= */
/* DSPF_sp_fir_cplx.h -- Complex FIR Filter                                */
/*              Optimized C Implementation (w/ Intrinsics)                 */
/*                                                                         */
/* Rev 0.0.1                                                               */
/*                                                                         */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */

#ifndef _DSPF_SP_FIR_CPLX_H_
#define _DSPF_SP_FIR_CPLX_H_

void DSPF_sp_fir_cplx(const float * x, const float * h,
    float * restrict y, int nh, int ny);

#endif /* _DSPF_SP_FIR_CPLX_H_ */

/* ======================================================================= */
/*  End of file:  DSPF_sp_fir_cplx.h                                       */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */

