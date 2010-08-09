/* ======================================================================= */
/* DSP_irr.h -- IIR Filter                                                 */
/*              Optimized C Implementation (w/ Intrinsics)                 */
/*                                                                         */
/* Rev 0.0.1                                                               */
/*                                                                         */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2007 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */

#ifndef _DSP_IIR_H_
#define _DSP_IIR_H_

short DSP_iir (
    short                 Input,
    const short *restrict Coefs,
    int                   nCoefs,
    short       *restrict State
);

#endif /* _DSP_IIR_H_ */

/* ======================================================================= */
/*  End of file:  DSP_iir.h                                                */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2007 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */

