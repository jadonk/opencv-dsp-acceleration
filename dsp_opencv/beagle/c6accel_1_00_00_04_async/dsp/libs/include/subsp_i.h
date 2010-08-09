/* ======================================================================== */
/*  TEXAS INSTRUMENTS, INC.                                                 */
/*                                                                          */
/*  NAME                                                                    */
/*      subsp -- Single Precision Floating Point Subtraction                */
/*                                                                          */
/*                                                                          */   
/*  USAGE                                                                   */
/*      This function is C callable, and is called according to this        */
/*      C prototype:                                                        */
/*                                                                          */
/*      float subsp_i                                                       */
/*      (                                                                   */
/*        float left,   // Argument 1   //                                  */
/*        float right   // Argument 2   //                                  */
/*      );                                                                  */
/*                                                                          */
/*  DESCRIPTION                                                             */
/*      This kernel returns the single precision subtraction of two         */
/*      32-bit single precision inputs. Underflow returns zero,             */
/*      overflow return + or - infinity, they are placed in the DEBUG macro */
/*                                                                          */    
/*    Single precision binury floating point number format is 32 bit number */
/*    BIT 31 = SIGN, BIT 30 - 23 = EXPONENT and BIT 22 - 0 = MANTISSA       */
/*                                                                          */
/*  Subtraction is performed by taking the 2's complement of the right      */
/*               operand and performing Addition                            */ 
/*                                                                          */
/*  TECHNIQUES                                                              */
/*                                                                          */
/*  SOURCE                                                                  */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */

#ifndef _SUBSP_I_H_
#define _SUBSP_I_H_

#ifdef INLINE_C
    #define _INDEFF static inline float
#else
    #define _INDEFF float
#endif

_INDEFF subsp_i(float left, float right)
{

#ifdef _TMS320C6700
   return left - right;
#else
    int  lft , rgt, rgt1, lft1 ; /* int register versions of left,right REAL params   */
    int  lfrc, rfrc, temp_res, temp_exp, res; /* fraction of left/dest operand and right operand   */
    int  lexp, rexp, pexp; /* exponent of left/dest operand and right operand   */
    int  sign = 0, rgt_sign; /* sign of the result and sign of the right operand */
    int lfrc_1, rfrc_1, flag_zero; /* zero flag */
    int temp_res_1, temp_exp_1, temp_res_2, temp_exp_2;

   /*------------------------------------------------------------------------*/
   /* Transfer the left,right bits to lft,rgt.                               */
   /* REALasINT may force left and right to memory if dumb compiler.         */
   /* This is OK since they won't be needed again except for unusual cases.  */
   /* lft,rgt will hopefully wind up in integer registers.                   */
   /*------------------------------------------------------------------------*/

       REALasINT(lft,left); 
    REALasINT(rgt,right);

    /*------------------------------------------------------------------------*/
    /* Unpack fraction and exponent, ignoring signs for now.                  */
    /* Reinsert hidden bit or re-normalize fraction as appropriate.           */
    /*------------------------------------------------------------------------*/
    lfrc = lft & ~MINUS;
    rfrc = rgt & ~MINUS;
    flag_zero = lfrc | rfrc;
    rgt_sign = (rgt >> (BPint - 1));    /* take 2's complement of the right operand */
    rgt1 = ((~rgt_sign) << (BPint - 1)) | rfrc ;

    lexp = lfrc >> REAL_FRC_MSBs; /* Extract the exponent */
    lfrc_1 = (lfrc & REAL_FRC_MASK) + REAL_HIDDEN_BIT; /*insert hidden bit*/

    rexp = rfrc >> REAL_FRC_MSBs;  /* Extract the exponent */
    rfrc_1 = (rfrc & REAL_FRC_MASK) + REAL_HIDDEN_BIT; /*insert hidden bit*/

    if ((lft1 = lft) < 0) 
        lfrc_1 = (~lfrc_1 + 1) * 2; /* make 2's complement */
    if (lft1 >= 0) 
        lfrc_1 = lfrc_1 * 2;   /* Shift left by 1 */
    
    if (rgt1 < 0) 
        rfrc_1 = (~rfrc_1 + 1) * 2;  /* make 2's complement */
    if (rgt1 >= 0) 
        rfrc_1 = rfrc_1 * 2;   /* Shift left by 1 */

    /*------------------------------------------------------------------------*/
    /* Shift fractions to align radix point and add.                          */
    /* Result has one rounding bit.                                           */
    /*------------------------------------------------------------------------*/
    pexp = lexp - rexp;
    if (pexp >= 0) {  /* |left| >= |right| */
        temp_res = lfrc_1 + (rfrc_1 >> pexp);
        temp_exp = lexp;
    }
    else {/* |left| <  |right| */    
        temp_res = rfrc_1 + (lfrc_1 >> -pexp);
        temp_exp = rexp;
    }
 /* addition result stored in temp_res and temp_exp */

    if (temp_res < 0) { /* back to sign-magnitude */
        temp_res_1 = ~temp_res+1;  /* Take 2's complement */
        sign = (int )MINUS;  /* Set the sign flag */
    }
    else 
        temp_res_1 = temp_res;
    temp_exp_1 = temp_exp;

    /*------------------------------------------------------------------------*/
    /* Round and normalize the result                                         */
    /*------------------------------------------------------------------------*/
    
    if (temp_res_1 < REAL_HIDDEN_BIT*2) {
        RENORM2F(temp_res_1, temp_exp_1);
    }
    else {
        temp_res_1 += 1;             /* biased rounding */
        if (temp_res_1 >= (REAL_HIDDEN_BIT * 4)) {
            temp_res_1 = ((temp_res_1 -1) >> 1) +1;
            temp_exp_1 += 1;
        }    
    }
    temp_res_2 = temp_res_1;
    temp_exp_2 = temp_exp_1;

    /*------------------------------------------------------------------------*/
    /* Check for overflow and underflow                                       */
    /*------------------------------------------------------------------------*/

    /*TO enable the check, Run in DEBUG mode, or define DEBUG macro*/
    #ifdef CHECK_OVERFLOW
        if (temp_exp_2 <= 0) { /* underflow? */
            temp_res_2 = 0;  /* no significant bits remain */
            temp_exp_2 = 0;
        }
        else if (temp_exp_2 >= REAL_EXP_INFNAN) { /* overflow? */
            temp_exp_2 = REAL_EXP_INFNAN; temp_res_2 = 0;
        }
    #endif
    /*------------------------------------------------------------------------*/
    /* Assemble result and return                                             */
    /*------------------------------------------------------------------------*/
    res = ((temp_res_2>>1)& REAL_FRC_MASK) + (((int )temp_exp_2) << REAL_FRC_MSBs) +sign;

    if(flag_zero == 0)
        res = 0;

    return_INTasREAL(res);
#endif


}

#endif /* _SUBSP_I_H_ */
/* ======================================================================== */
/*  End of file: subsp_i.h                                                  */
/* ------------------------------------------------------------------------ */
/*          Copyright (C) 2008 Texas Instruments, Incorporated.             */
/*                          All Rights Reserved.                            */
/* ======================================================================== */
