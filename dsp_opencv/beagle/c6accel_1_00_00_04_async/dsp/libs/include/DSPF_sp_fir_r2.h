/* ======================================================================= */
/* DSPF_sp_fir_r2.h -- FIR Filter                                          */
/*              Optimized C Implementation (w/ Intrinsics)                 */
/*                                                                         */
/* Rev 0.0.1                                                               */
/*                                                                         */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */

#ifndef _DSPF_SP_FIR_R2_H_
#define _DSPF_SP_FIR_R2_H_

void DSPF_sp_fir_r2(const float * x, const float * h,
    float *restrict y, const int nh, const int ny);

#endif /* _DSPF_SP_FIR_R2_H_ */

/* ======================================================================= */
/*  End of file:  DSPF_sp_fir_r2.h                                         */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */

