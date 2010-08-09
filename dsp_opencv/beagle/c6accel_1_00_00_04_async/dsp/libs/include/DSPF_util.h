/* ======================================================================= */
/* DSPF_util.h -- General purpose utility module for C674x DSPLIB          */
/*                                                                         */
/* Rev 0.0.1                                                               */
/*                                                                         */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */

#ifndef _DSPF_UTIL_H_
#define _DSPF_UTIL_H_ 1

void UTIL_fillRandSP(float *ptr_x, int N, float factor);
void UTIL_fillRandUint(void *ptr_x, int N);
void UTIL_fillRandFilterSP(float *ptr_h, int N);
void UTIL_compareResultsSP(float *ptr_y_cn, float *ptr_y_lib, float *ptr_y_ex,
    int N, int strict, int verbose);
void UTIL_compareResultsUint(unsigned int *ptr_y_cn, unsigned int *ptr_y_lib,
    unsigned int *ptr_y_ex, int N, int strict, int verbose);

#endif /* _DSPF_UTIL_H_ */

/* ======================================================================= */
/*  End of file:  DSPF_util.h                                              */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */
