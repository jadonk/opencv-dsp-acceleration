/* ======================================================================== */
/*  TEXAS INSTRUMENTS, INC.                                                 */
/*                                                                          */
/*  NAME                                                                    */
/*      spuint -- 32-bit IEEE-754 SP Floating Point -> 32-bit Unsigned      */
/*      Integer                                                             */
/*                                                                          */
/*  USAGE                                                                   */
/*      This function is C callable, and is called according to this        */
/*      C prototype:                                                        */
/*                                                                          */
/*      unsigned int spuint_i                                               */
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

#ifndef _SPUINT_I_H_
#define _SPUINT_I_H_

#ifdef INLINE_C
    #define _INDEFUI static inline unsigned int
#else
    #define _INDEFUI unsigned int
#endif

_INDEFUI spuint_i(float x)
{

    unsigned  rslt; 
    unsigned  rfrc, rfrc_0, rfrc_1; 
    int rexp, rexp_1;   /* frac and exp of result */

    REALasUNS(rfrc_0,x);
 
    rfrc = rfrc_0 & ~MINUS;
    rexp = (int) (rfrc >> 23);
    
    rfrc_1 = (rfrc & REAL_FRC_MASK) + REAL_HIDDEN_BIT;    /* insert hidden bit */
    rexp_1 = 127  + 32 - 1 - rexp;

    rslt = (rfrc_1 << (32 - 23 -1)) >> rexp_1;
 
    if (rexp_1 < 0) 
    { /* overflow  */
        rslt = UINT_MAX ;
    }

    if (rfrc_0 & MINUS) 
    {
        rslt = ~rslt + 1;            
        if((rexp_1 < 0))
            rslt = (unsigned )INT_MIN ;
        if((int)rslt < 0)
            rslt = (unsigned )INT_MIN ;
    }    
      if((rexp == 0) | (rexp_1 >= 32)) /* underflow */
        rslt = (unsigned )0;
    
    return rslt;


}

#endif /* _SPUINT_I_H */
/* ======================================================================== */
/*  End of file: spuint_i.h                                                 */
/* ------------------------------------------------------------------------ */
/*          Copyright (C) 2008 Texas Instruments, Incorporated.             */
/*                          All Rights Reserved.                            */
/* ======================================================================== */
