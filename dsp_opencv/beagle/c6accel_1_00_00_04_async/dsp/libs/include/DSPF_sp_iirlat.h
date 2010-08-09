/* ======================================================================= */
/* DSPF_sp_iirlat.h -- Lattice IIR Filter                                  */
/*              Optimized C Implementation (w/ Intrinsics)                 */
/*                                                                         */
/* Rev 0.0.1                                                               */
/*                                                                         */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */

#ifndef _DSPF_SP_IIRLAT_H_
#define _DSPF_SP_IIRLAT_H_

void DSPF_sp_iirlat(const float *x, const int nx,
    const float *restrict k, const int nk, float *restrict b,
    float *restrict y);

#endif /* _DSPF_SP_IIRLAT_H_ */

/* ======================================================================= */
/*  End of file:  DSPF_sp_iirlat.h                                         */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */

