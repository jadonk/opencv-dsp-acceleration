/* ======================================================================= */
/* DSPF_sp_iir.h -- IIR Filter                                             */
/*              Optimized C Implementation (w/ Intrinsics)                 */
/*                                                                         */
/* Rev 0.0.1                                                               */
/*                                                                         */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */

#ifndef _DSPF_SP_IIR_H_
#define _DSPF_SP_IIR_H_

void DSPF_sp_iir (float* restrict y1,
    const float* x,
    float* restrict y2,
    const float* hb,
    const float* ha,
    int n);

#endif /* _DSPF_SP_IIR_H_ */

/* ======================================================================= */
/*  End of file:  DSPF_sp_iir.h                                            */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */

