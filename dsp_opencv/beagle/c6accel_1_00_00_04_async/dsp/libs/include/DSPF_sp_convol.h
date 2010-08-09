/* ======================================================================= */
/* DSPF_sp_convol.h -- Convolution                                         */
/*              Optimized C Implementation (w/ Intrinsics)                 */
/*                                                                         */
/* Rev 0.0.1                                                               */
/*                                                                         */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */

#ifndef _DSPF_SP_CONVOL_H_
#define _DSPF_SP_CONVOL_H_

void DSPF_sp_convol(const float *x, const float *h,
    float *restrict y, const short nh, const short ny);

#endif /* _DSPF_SP_CONVOL_H_ */

/* ======================================================================= */
/*  End of file:  DSPF_sp_convol.h                                         */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */

