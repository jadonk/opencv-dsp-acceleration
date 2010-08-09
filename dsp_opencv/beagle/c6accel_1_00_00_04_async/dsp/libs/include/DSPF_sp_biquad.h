/* ======================================================================= */
/* DSPF_sp_biquad.h -- Biquad Filter                                       */
/*              Optimized C Implementation (w/ Intrinsics)                 */
/*                                                                         */
/* Rev 0.0.1                                                               */
/*                                                                         */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */

#ifndef _DSPF_SP_BIQUAD_H_
#define _DSPF_SP_BIQUAD_H_

void DSPF_sp_biquad(float *restrict x, float *b, float *a,
    float *delay, float *restrict y, const int n);

#endif /* _DSPF_SP_BIQUAD_H_ */

/* ======================================================================= */
/*  End of file:  DSPF_sp_biquad.h                                         */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */

