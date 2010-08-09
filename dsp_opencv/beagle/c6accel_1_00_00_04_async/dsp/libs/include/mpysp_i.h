/* ======================================================================== */
/*  TEXAS INSTRUMENTS, INC.                                                 */
/*                                                                          */
/*  NAME                                                                    */
/*      mpysp -- Single Precision Floating Point Multiply                   */
/*                                                                          */
/*  USAGE                                                                   */
/*      This function is C callable, and is called according to this        */
/*      C prototype:                                                        */
/*                                                                          */
/*      float mpysp_i                                                       */
/*      (                                                                   */
/*        float left,   // Argument 1   //                                  */
/*        float right   // Argument 2   //                                  */
/*      );                                                                  */
/*                                                                          */
/*  DESCRIPTION                                                             */
/*      This kernel returns the single precision multiplication of two      */
/*      32-bit single precision inputs.                                     */
/*                                                                          */
/*    Single precision binury floating point number format is 32 bit number */
/*    BIT 31 = SIGN, BIT 30 - 23 = EXPONENT and BIT 22 - 0 = MANTISSA       */
/*                                                                          */
/*  TECHNIQUES                                                              */
/*                                                                          */
/*  SOURCE                                                                  */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */


#ifndef _MPYSP_I_H_
#define _MPYSP_I_H_

#ifdef INLINE_C
    #define _INDEFF static inline float
#else
    #define _INDEFF float
#endif

_INDEFF mpysp_i(float left, float right)
{

#ifdef _TMS320C6700  
   return left * right;
#else 
    unsigned  lfrc_o, rfrc_o, lfrc, rfrc,  pfrc, pfrc_1, pfrc_2;  /* fraction of left, right and product */
    int    lexp, rexp, pexp, pexp_1, flag_zero;    /* exponent of left, right and product */
    unsigned  sign, lfrc_1, rfrc_1, lfrc_2, rfrc_2; /* temp variable */
    #ifndef _TMS320C6400_PLUS
        unsigned temp_resHL, temp_resLH, temp_resLL, temp_resHH;
    #endif

   /*------------------------------------------------------------------------*/
   /* Unpack fraction and exponent, ignoring signs for now.                  */
   /* Reinsert hidden bit or re-normalize fraction as appropriate.           */
   /*------------------------------------------------------------------------*/
       pfrc = 0;
    REALasUNS(lfrc_o,left);
       REALasUNS(rfrc_o, right);
       sign = (lfrc_o ^ rfrc_o) & MINUS; 
       lfrc = lfrc_o & ~MINUS;
       rfrc = rfrc_o & ~MINUS;
    
    flag_zero = (lfrc == 0 | rfrc == 0); /* Set flag_zero = 1 if either operand is zero */

    lexp = (int)(lfrc >> REAL_FRC_MSBs);  /* Extract the exponent */
    rexp = (int)(rfrc >> REAL_FRC_MSBs); 
    lfrc_1 = (lfrc & REAL_FRC_MASK) + REAL_HIDDEN_BIT ; /* Insert hidden bit */
    rfrc_1 = (rfrc & REAL_FRC_MASK) + REAL_HIDDEN_BIT;

   /*------------------------------------------------------------------------*/
   /* Compute product exponent and fraction.                                 */
   /* Round and normalize product if needed.                                 */
   /*------------------------------------------------------------------------*/
       pexp = lexp + rexp; /* Exponent = sum of the exponents of operands */
     lfrc_2 = lfrc_1 * 32; /* Mantissa shift left by 5 */       
    rfrc_2 = rfrc_1 * 32;        
#ifdef _TMS320C6400_PLUS 
    pfrc = _hill(_mpy32u(lfrc_2,rfrc_2)); /* Multiply the fractions of the 2 operands */
#else
    temp_resHL = _mpyhlu(lfrc_2,rfrc_2); 
     temp_resLH = _mpyhlu(rfrc_2,lfrc_2); 
     temp_resLL= _mpyu(lfrc_2,rfrc_2);   
     temp_resHH= _mpyhu(lfrc_2, rfrc_2); 
    temp_resHL = temp_resHL + (temp_resLL >> 16);               
     temp_resLH += temp_resHL;
    temp_resLH = temp_resLH >> 16;
    pfrc = temp_resLH + temp_resHH;
#endif

    pfrc = pfrc + 1;     /* biased rounding */
       if (pfrc >= (REAL_HIDDEN_BIT * 4)) 
       { 
           pfrc >>= 1; 
           pexp += 1; 
       }  

    pfrc_1 = pfrc;
    pexp_1 = pexp - 127;
   /*------------------------------------------------------------------------*/
   /* Check for overflow and underflow.                                      */
   /*------------------------------------------------------------------------*/

   /*TO enable the check, Run in DEBUG mode, or define DEBUG macro*/
#ifdef CHECK_OVERFLOW
       if (pexp_1 <= 0) {        /* underflow */
         pfrc_1 = 0;      /* no significant bits remain */
         pexp_1 = 0;
       }
       else if (pexp_1 >= REAL_EXP_INFNAN) {    /* overflow */
          pexp_1 = REAL_EXP_INFNAN;
          pfrc_1 = 0;
   }
#endif

   /*------------------------------------------------------------------------*/
   /* assemble result and return.                                            */
   /*------------------------------------------------------------------------*/

       pfrc_2 = ((pfrc_1 >> 1) & REAL_FRC_MASK) + (((int )pexp_1) << REAL_FRC_MSBs);

      pfrc_2 = pfrc_2 + sign;

    if(flag_zero == 1) /* Check whether flag_zero is set */
        pfrc_2 = 0;    

    return_UNSasREAL(pfrc_2);

#endif 


}

#endif /* _MPYSP_I_H_ */
/* ======================================================================== */
/*  End of file: mpysp_i.h                                                  */
/* ------------------------------------------------------------------------ */
/*          Copyright (C) 2008 Texas Instruments, Incorporated.             */
/*                          All Rights Reserved.                            */
/* ======================================================================== */
