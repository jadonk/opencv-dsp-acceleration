/* ======================================================================== */
/*  TEXAS INSTRUMENTS, INC.                                                 */
/*                                                                          */
/*  NAME                                                                    */
/*      spint -- 32-bit IEEE-754 SP Floating Point -> 32-bit Signed         */
/*      Integer                                                             */
/*                                                                          */
/*  USAGE                                                                   */
/*      This function is C callable, and is called according to this        */
/*      C prototype:                                                        */
/*                                                                          */
/*      int spint_i                                                         */
/*      (                                                                   */
/*        float a,   // Argument 1   //                                     */
/*      );                                                                  */
/*                                                                          */
/*  DESCRIPTION                                                             */
/*      This kernel returns the integer value of the 32-bit                 */
/*      single precision input.                                             */
/*                                                                          */
/*  TECHNIQUES                                                              */
/*                                                                          */
/*  SOURCE                                                                  */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */

#ifndef _SPINT_I_H_
#define _SPINT_I_H_

#ifdef INLINE_C
    #define _INDEFI static inline int
#else
    #define _INDEFI int
#endif

_INDEFI spint_i(float x)
{

    int  rslt; 
    unsigned  rfrc, rfrc_0, rfrc_1; 
    int rexp, rexp_1;   /* frac and exp of result */
    int flag_zero;

    REALasUNS(rfrc_0,x); 
    rfrc = rfrc_0 & ~MINUS;
    flag_zero = rfrc;
    rexp = (int)(rfrc >> 23 );
    rfrc_1 = (rfrc & REAL_FRC_MASK) + REAL_HIDDEN_BIT;    /* insert hidden bit */
    rexp_1 = 127 + 32 - 1 - rexp;

    rslt = rfrc_1;
    rslt = (unsigned )(rslt << (32  - 23 -1)) >> rexp_1;
    if (rfrc_0 & MINUS) 
        rslt = ~rslt + 1;

    if (rexp_1 <= 0) /* overflow */
           rslt = INT_MAX ;

    if ((rfrc_0 & MINUS) && (rexp_1 <= 0)) 
        rslt = ~rslt ; // INT_MIN = -INT_MAX-1
       
    if(flag_zero == 0 | rexp_1 >= 32)   /* underflow */
        rslt = 0;

    return rslt;
}

#endif /* _SPINT_I_H_ */
/* ======================================================================== */
/*  End of file: spint_i.h                                                  */
/* ------------------------------------------------------------------------ */
/*          Copyright (C) 2008 Texas Instruments, Incorporated.             */
/*                          All Rights Reserved.                            */
/* ======================================================================== */
