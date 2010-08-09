/* ======================================================================= */
/* DSPF_sp_lms.h -- LMS Adaptive Filter                                    */
/*              Optimized C Implementation (w/ Intrinsics)                 */
/*                                                                         */
/* Rev 0.0.1                                                               */
/*                                                                         */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */

#ifndef _DSPF_SP_LMS_H_
#define _DSPF_SP_LMS_H_

float DSPF_sp_lms(const float *x, float *restrict h, const float *y_i,
    float *restrict y_o, const float ar, float error, const int nh, const int ny);

#endif /* _DSPF_SP_LMS_H_ */

/* ======================================================================= */
/*  End of file:  DSPF_sp_lms.h                                            */
/* ----------------------------------------------------------------------- */
/*            Copyright (c) 2009 Texas Instruments, Incorporated.          */
/*                           All Rights Reserved.                          */
/* ======================================================================= */

