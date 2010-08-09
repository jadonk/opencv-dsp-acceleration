/* ======================================================================== */
/*  TEXAS INSTRUMENTS, INC.                                                 */
/*                                                                          */
/*  NAME                                                                    */
/*      intsp -- 32-bit Signed Integer -> 32-bit IEEE-754 SP Floating       */
/*      Point                                                               */
/*                                                                          */
/*  USAGE                                                                   */
/*      This function is C callable, and is called according to this        */
/*      C prototype:                                                        */
/*                                                                          */
/*      float intsp_i                                                       */
/*      (                                                                   */
/*        int a,   // Argument 1   //                                       */
/*      );                                                                  */
/*                                                                          */
/*  DESCRIPTION                                                             */
/*      This kernel returns the single precision floating point             */
/*      representation of the inpt integer.                                 */
/*                                                                          */
/*  TECHNIQUES                                                              */
/*                                                                          */
/*  SOURCE                                                                  */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */


#ifndef _INTSP_I_H_
#define _INTSP_I_H_

#ifdef INLINE_C
    #define _INDEFF static inline float
#else
    #define _INDEFF float
#endif

_INDEFF intsp_i(int x)
{

    unsigned  rfrc, rfrc_1; 
    int rexp, rexp_1; /* frc and exp of result */
    unsigned  rslt, rslt_1;

    rfrc = x;
    if ((int )rfrc < 0) 
        rfrc = ~rfrc + 1;  /* *WANT* -most_neg == most_neg */

    rexp = _lmbd(1,  rfrc ) ; /* no of consecutive 0 MSBs */

    rfrc_1 = rfrc << rexp;
    rexp_1 = 127  + 32 - 2 - rexp;

   /* align for mantissa field and round */

    #define SHIFT_COUNT (32 - 23 -1)
        rslt = (unsigned )(((rfrc_1 >> (SHIFT_COUNT - 1)) +1) >> 1);
    #undef SHIFT_COUNT

   /*-----------------------------------------------------------------------*/
   /* The exponent field in rfrc now holds 1 or maybe 2 if the fraction     */
   /* rounded up.  ADD in rexp to simultaneously combine exponent/fraction  */
   /* and also increment the exponent by the appropriate amount.            */
   /*-----------------------------------------------------------------------*/
    rslt_1 = rslt + (((unsigned )rexp_1) << 23) ;

    if (x < 0) 
        rslt_1 += MINUS;

    if(x == 0)
        rslt_1 = 0;

    return _itof(rslt_1);


}

#endif /* _INTSP_I_H_ */
/* ======================================================================== */
/*  End of file: intsp_i.h                                                  */
/* ------------------------------------------------------------------------ */
/*          Copyright (C) 2008 Texas Instruments, Incorporated.             */
/*                          All Rights Reserved.                            */
/* ======================================================================== */
