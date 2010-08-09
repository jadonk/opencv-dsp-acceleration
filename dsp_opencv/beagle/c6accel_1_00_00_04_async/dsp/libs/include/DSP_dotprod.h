/* ======================================================================= */
/* DSP_dotprod.h --   Dot product implementation                           */
/*                    Intrinsic C Implementation                           */
/*                                                                         */
/* Rev 0.0.1                                                               */
/*                                                                         */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2007 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */

#ifndef DSP_DOTPROD_H_
#define DSP_DOTPROD_H_

int DSP_dotprod (
    const short * restrict m,    /* Pointer to first vector  */
    const short * restrict n,    /* Pointer to second vector */
    int count                    /* Length of vectors.       */
);

#endif /* DSP_DOTPROD_H_ */

/* ======================================================================== */
/*  End of file:  DSP_dotprod.h                                             */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2007 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */

