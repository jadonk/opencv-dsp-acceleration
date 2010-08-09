/* ======================================================================= */
/* DSPF_sp_fircirc.h -- Circular FIR Filter                                */
/*              Optimized C Implementation (w/ Intrinsics)                 */
/*                                                                         */
/* Rev 0.0.1                                                               */
/*                                                                         */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */

#ifndef _DSPF_SP_FIRCIRC_H_
#define _DSPF_SP_FIRCIRC_H_

void DSPF_sp_fircirc(const float *x, float *h, float *restrict y,
    const int index, const int csize, const int nh, const int ny);

#endif /* _DSPF_SP_FIRCIRC_H_ */

/* ======================================================================= */
/*  End of file:  DSPF_sp_fircirc.h                                        */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */

