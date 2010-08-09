/* ======================================================================== */
/*  TEXAS INSTRUMENTS, INC.                                                 */
/*                                                                          */
/*  NAME                                                                    */
/*      divsp -- Single Precision Floating Point Divide                     */
/*                                                                          */
/*  USAGE                                                                   */
/*      This function is C callable, and is called according to this        */
/*      C prototype:                                                        */
/*                                                                          */
/*      float divsp_i                                                       */
/*      (                                                                   */
/*        float left,   // Argument 1   //                                  */
/*        float right   // Argument 2   //                                  */
/*      );                                                                  */
/*                                                                          */
/*  DESCRIPTION                                                             */
/*      This kernel returns the single precision division of two            */
/*      32-bit single precision inputs.                                     */
/*                                                                          */
/*	Single precision binury floating point number format is 32 bit number   */
/*	BIT 31 = SIGN, BIT 30 - 23 = EXPONENT and BIT 22 - 0 = MANTISSA         */
/*                                                                          */
/*                                                                          */
/*  TECHNIQUES                                                              */
/*                                                                          */
/*  SOURCE                                                                  */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */



#ifndef _DIVSP_I_H_
#define _DIVSP_I_H_

#ifdef INLINE_C
	#define _INDEFF static inline float
#else
	#define _INDEFF float
#endif

_INDEFF divsp_i(float left, float right)
{

	unsigned  lfrc, rfrc, pfrc, pfrc_1, W;     /* fraction of left, right and product */
    int    lexp, rexp, pexp, pexp_1, Q, Q_1, N;	/* exponent of left, right and product */
    unsigned  sign, flag_zero, flag_infinity, lfrc_o, rfrc_o, temp_res;    
    unsigned W_1, lfrc_1, rfrc_1, lfrc_2, rfrc_2;	
   /*------------------------------------------------------------------------*/
   /* Unpack fraction and exponent, ignoring signs for now.                  */
   /* Reinsert hidden bit or re-normalize fraction as appropriate.           */
   /*------------------------------------------------------------------------*/

	 pfrc = 0;
	 REALasUNS(lfrc,left);
	 REALasUNS(rfrc, right);
	 sign = (lfrc ^ rfrc) & MINUS; /*Sign of the result = EXOR of the sign of the operands */
	 lfrc_o = lfrc & ~MINUS;
	 rfrc_o = rfrc & ~MINUS;

     flag_zero = lfrc_o; /* Set Flag_zero = 0 if the dividend is zero */
	 flag_infinity = rfrc_o; /* Set flag_infinity = 0 if divisor is zero */

	 lexp = lfrc_o >> 23;  /* Extract the exponent */
	 rexp = rfrc_o >> 23;

	 lfrc_1 = ((lfrc_o & REAL_FRC_MASK) + REAL_HIDDEN_BIT);
	 rfrc_1 = ((rfrc_o & REAL_FRC_MASK) + REAL_HIDDEN_BIT);
	 lfrc_2 = lfrc_1 * 128; /* Shift left by 7 */
	 rfrc_2 = rfrc_1 * 128; /* Shift left by 7 */
	 Q = 0;  /* Set partial quotient to zero */

   /*------------------------------------------------------------------------*/
   /* Compute quotient exponent and fraction.                                */
   /* Round and normalize quotient if needed.                                */
   /*------------------------------------------------------------------------*/
	 pexp = lexp - rexp;  /* Exponent of the result = difference of exponents of operands */

	/* 0.5 < quotient < 2.0, hence the 25 iterations */
	 for(N = 0; N < 5; N++){
		W = lfrc_2;   /* Copy the Q bits into temp W */
		lfrc_2 = _clr(lfrc_2,0,4); /* clear the lower 5 bits */

		lfrc_2 = _subc(lfrc_2,rfrc_2); /* repeated subtraction to get the partial quotient */
		W = _extu(W,27,27);

		lfrc_2 = _subc(lfrc_2,rfrc_2);
		Q = Q + W;
		
		lfrc_2 = _subc(lfrc_2,rfrc_2);
		Q = Q * 32;		

		lfrc_2 = _subc(lfrc_2,rfrc_2);

		lfrc_2 = _subc(lfrc_2,rfrc_2);
	 }
	 pexp_1 = pexp + 133;

	 W_1 = _norm(Q);  /* normalize the quotient */
	 temp_res = _extu(lfrc_2,27,27);  /* Extract the final quotient bits */
	 Q_1 = Q + temp_res;
	 pexp_1 = pexp_1 - W_1; /* Adjust the exponent based on the result */
	 Q_1 = Q_1 << W_1;

     pfrc = Q_1 + 1; 

   /*------------------------------------------------------------------------*/
   /* check for overflow and underflow                                       */
   /*------------------------------------------------------------------------*/

/*TO enable the check, Run in DEBUG mode, or define DEBUG macro*/

#ifdef CHECK_OVERFLOW
    if (pexp_1 <= 0) {        /* underflow */
    	pfrc = 0;      /* no significant bits remain */
        pexp_1 = 0;
    }
    else if (pexp_1 >= REAL_EXP_INFNAN) {    /* overflow */
        pexp_1 = REAL_EXP_INFNAN; 
        pfrc = 0;
    }
#endif
   
   /*------------------------------------------------------------------------*/
   /* assemble result and return                                             */
   /*------------------------------------------------------------------------*/

	pfrc_1 = ((pfrc >> 7) & REAL_FRC_MASK) + (((int )pexp_1) << 23 );
	
	if(flag_zero == 0) /* If flag_zero = 0, dividend is zero, so result = 0 */
		pfrc_1 = 0;

    if(flag_infinity == 0) /* If flag_infinity = 0, divisor is zero, so result = infinity */
	 	pfrc_1 = REAL_INFNAN;
	
	if(flag_zero == 0 & flag_infinity == 0) /* If both opernads are zero, result = NAN */
		pfrc_1 = REAL_NAN;

    pfrc_1 += sign;

    return_UNSasREAL(pfrc_1);

/* endif_IEEE_Hardware */


}

#endif /* _DIVSP_I_H_ */
/* ======================================================================== */
/*  End of file: divsp_i.h                                                  */
/* ------------------------------------------------------------------------ */
/*          Copyright (C) 2008 Texas Instruments, Incorporated.             */
/*                          All Rights Reserved.                            */
/* ======================================================================== */
