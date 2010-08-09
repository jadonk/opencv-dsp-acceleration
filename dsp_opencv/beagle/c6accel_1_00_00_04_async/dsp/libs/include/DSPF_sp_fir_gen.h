/* ======================================================================= */
/* DSPF_sp_fir_gen.h -- FIR Filter                                         */
/*              Optimized C Implementation (w/ Intrinsics)                 */
/*                                                                         */
/* Rev 0.0.1                                                               */
/*                                                                         */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */

#ifndef _DSPF_SP_FIR_GEN_H_
#define _DSPF_SP_FIR_GEN_H_

void DSPF_sp_fir_gen(const float * restrict x,
    const float * restrict h,
    float * restrict y,
    int nh,
    int ny);

#endif /* _DSPF_SP_FIR_GEN_H_ */

/* ======================================================================= */
/*  End of file:  DSPF_sp_fir_gen.h                                        */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */

