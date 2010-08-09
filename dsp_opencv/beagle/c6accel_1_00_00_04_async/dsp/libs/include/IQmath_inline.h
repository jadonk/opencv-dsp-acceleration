/* ======================================================================== */
/* IQmath_inline.h : Contains inline version of all IQmath lib functions.   */
/* ======================================================================== */
/*            Copyright (c) 2007 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */

#ifndef __IQMATH_INLINE_H__
#define __IQMATH_INLINE_H__

#include "IQmath.h"


/*;;###########################################################################
;;
;; FILE:    FtoIQN.c
;;
;; TITLE:   C Callable IQ to Float Math Function
;;
;;===========================================================================
;; Function:   _FtoIQN
;;===========================================================================
;;
;; C Usage:    extern int _FtoIQ(float A);   // no round or sat
;;
;;---------------------------------------------------------------------------
;;
;; On Entry:   A4    = IEEE 754 floating-point equivalent of A
;; On Exit:    A4    = A in IQ format 
;;                       
;; Q range:    31 to 1
;;
;;---------------------------------------------------------------------------
;; Algorithm:  This operation converts an IEEE 754 Single-Precision 
;;             floating-point number to equivalent IQ number. This
;;             format is shown below:
;;
;;              31  30        23 22                                 0
;;             +-----------------------------------------------------+
;;             | s |      e     |                 f                  |
;;             +-----------------------------------------------------+
;;
;;             Value = (-1)^s * 2^(e-127) * 1.f
;;
;;             where: e = 1 to 254, f = 0.000000000 to ~1.0
;;                    e = 0, f = 0, s = 0, Value = 0.0
;;                    e = 0 and f != 0 case cannot occur in IQ math
;;                    e = 255 case cannot occur in IQ math
;;          
;;--------------------------------------------------------------------------*/



static inline I32_IQ _FtoIQN(float input, U32_IQ qfmt)
{
#ifdef _TMS320C6X

    U32_IQ al, x, temp1, temp2, sbits, e, imp=1;
    I32_IQ ah;
    x = _ftoi(input);     // Re-interprits the bits in float as an unsigned int
    temp2 = x & 0xc0000000u;        //stores the sign and hidden bit
    if(x == 0)
    {
        imp = 0;
    }

    /* ======================================================================== */
    /*  extract the exponent(sbits) & hidden bit                                */
    /* ======================================================================== */
    
    sbits = _extu(x, 1u, 24u);
    if(sbits < 128u)
    {
        temp2 = temp2 ^ 0x40000000u;
    }

    /* ======================================================================== */
    /*  find the no. of bits by which the final result has to be shiftehd        */
    /* ======================================================================== */
    
    e = 127u + (30u - qfmt);
    sbits = e - sbits;

    /* ======================================================================== */
    /*  arrange for representtion in Q-Format                                   */
    /* ======================================================================== */

    temp1 = x & 0x07fffffu;
    al = temp1;
    al = al << 7u;
    al = al | temp2;

    /* ======================================================================== */
    /*  if then sign is -ve complement the bits except sign bit                 */
    /* ======================================================================== */

    if(((temp2 >> 31u) != 0) && (temp1 != 0u))
    {
        al = al ^ 0x7fffff80u;
    }
    ah = (I32_IQ)al;
    ah =  ((I32_IQ)ah >> sbits);    //shift right by sbits
    ah = ah * (I32_IQ)imp;
    return(ah);

#else /* For host. (since no equivalent of _ftoIQ() exists */
    I32_IQ res;
    res = input * (1 << qfmt);
    return(res);
#endif /* _TMS320C6X */

}

/*###########################################################################
;;
;; FILE:    IQNabs.c
;;
;; TITLE:   C-Model: _IQNabs function
;;
;;===========================================================================
;; Function:   _IQNabs
;;===========================================================================
;;
;; C Usage:    extern long _IQabs(int A);  
;;
;;-------------------------------------------------------------------------
;;
;; On Entry:   A4    = A in IQ format
;; On Exit:    A4    = Absolute value of A in IQ format
;;                       
;; Q range:    31 to 1
;;
;;---------------------------------------------------------------------------*/




static inline I32_IQ _IQNabs(I32_IQ input)        
{   
    I32_IQ temp;
    temp = _abs(input);     
    return(temp);

}

/*###########################################################################
;;
;; FILE:    IQNfrac.c
;;
;; TITLE:   C-Model: _IQNfrac function
;;
;;===========================================================================
;; Function:   _IQNfrac
;;===========================================================================
;;
;; C Usage:    extern int _IQfrac(int A);   // no round or sat
;;
;;---------------------------------------------------------------------------
;;
;; On Entry:   A4    = A in IQ format
;; On Exit:    A4    = fractional portion of A stored as IQ
;;                       
;; Q range:    31 to 1
;;
;;---------------------------------------------------------------------------
;; Algorithm:  The fractional portion of the IQ number is calculated
;;             as follows:
;;
;;                frac = A - ((A >> q_value) << q_value);
;;          
;;---------------------------------------------------------------------------*/




static inline I32_IQ _IQNfrac(I32_IQ f1, U32_IQ var2)
{
    I32_IQ temp1;
    U32_IQ temp;
    U32_IQ reg = 1;
    temp1 = _abs(f1);
    temp= (U32_IQ) temp1;
        
    /* ======================================================================== */
    /*  Find a mask to extract fractional bits                                  */
    /* ======================================================================== */
            
    reg = reg << var2;
    reg = reg - 1;                
    temp = temp & reg;
    temp1 = (I32_IQ)temp;
                
    /* ======================================================================== */
    /*  check if negative                                                       */
    /* ======================================================================== */

    if(f1 < 0)  
    {               
        temp1 = -temp1;
    }
    return(temp1);
}
/*###########################################################################
;;
;; FILE:    IQNint.c
;;
;; TITLE:   C-Model: _IQNint function
;;
;;===========================================================================
;; Function:   _IQNint
;;===========================================================================
;;
;; C Usage:    extern int _IQint(int A);   // no round or sat
;;
;;---------------------------------------------------------------------------
;;
;; On Entry:   A4    = A in IQ format
;; On Exit:    A4    = integer portion of A stored as "long"
;;                       
;; Q range:    29 to 0
;;
;;---------------------------------------------------------------------------
;; Algorithm:  The integer portion of the IQ number is calculated
;;             as follows:
;;
;;                integer = A >> q_value;
;;          
;;---------------------------------------------------------------------------*/




static inline I32_IQ _IQNint(I32_IQ input,U32_IQ qfmt)       
{
    I32_IQ reg;
    U32_IQ temp;
    reg = _abs(input);
    temp= (U32_IQ)reg;

    /* ======================================================================== */
    /*  shift to remove the fractional components                               */
    /* ======================================================================== */

    temp = temp >> qfmt;
    reg= (I32_IQ)temp;

    /* ======================================================================== */
    /*  check if negative                                                       */
    /* ======================================================================== */
    
    if(input < 0)   
    {               
        reg = -reg;
    }
    return(reg);
}


/*###########################################################################
;;
;; FILE:    IQNmag.c
;;
;; TITLE:   C-Model: _IQNmag function
;;
;;===========================================================================
;; Function:   _IQNmag
;;===========================================================================
;;
;; C Usage:    extern long _IQNmag(long A, long B); // with rounding & sat
;;
;;---------------------------------------------------------------------------
;;
;; On Entry:   A4    = A in IQ format
;;             B4    = B in IQ format
;;
;; On Exit:    A4    = sqrt(A*A + B*B) result in IQ format
;;
;; Q range:    30 to 1
;;
;;---------------------------------------------------------------------------
;; Algorithm:  To calculate the magnitute we basically use the sqrt function.
;;             The procedure for calculating "Y = sqrt(X)" is similar
;;             to calculating the inverse sqare root due to the
;;             following relationship:
;;
;;                     sqrt(X) = X * 1/sqrt(X)
;;
;;             The procedure is as follows:
;;
;;             Step 1) Calculate value to square root:
;;
;;                     X = (A*A + B*B)
;;
;;             Step 2) Normalize Input: 
;;
;;                     V = X * 2^n 
;;
;;             Step 3) Obtain initial estimate from "_IQsqrtTable" 
;;                     using the upper 8-bits of the normalized V value.
;;
;;             Step 4) Use Newton-Raphson algorithm to improve accuracy.
;;                     Repeat following equation two times. First iteration
;;                     gives 16-bit accuracy. Second iteration gives 32-bit
;;                     accuracy:
;;
;;                     Yn = Yn(1.5 - Yn*Yn*V/2)
;;
;;                     Yn = 1/sqrt(V)
;;
;;             Step 5) Caculate the square root of the normalized number:
;;
;;                     Yn = V * Yn
;;
;;             Step 6) Denormalize result and round:
;;
;;                     Y = Yn * 2^n / sqrt(2^n)
;;
;;---------------------------------------------------------------------------*/




static inline I32_IQ _IQNmag(I32_IQ x, I32_IQ y, U32_IQ qfmt)     
{
    I64_IQ reg64;
    U32_IQ low1, low2, res,high_new;
    I32_IQ Dnorm=0, yn_new=0, high1, high2;
    I16_IQ index_new;

   U64_IQ tmp1, tmp2, tmp3;
   U16_IQ sbits,n,nn;
   

    /* ======================================   */
    /* Step(1):Calculate value to square root   */
    /* X = (A*A + B*B)                          */
    /* ======================================   */

    reg64 = _mpy32ll(x, x);
    high1 = (I32_IQ)_hill(reg64);
    low1  = _loll(reg64);

    /*  A*A */

    reg64 = _mpy32ll(y, y);
    high2 = (I32_IQ)_hill(reg64);
    low2  = _loll(reg64);

    /* B*B */

    /* ============================================================================= */
    /* code for  _add64(&reg64,&t64)....unsigned addition is performed  on           */
    /* lower 32 bits.An intrinsic _sadd() is used to add upper 32 bits.              */ 
    /* If a carry is generated during lower 32 bit addition,acc is incremented by one*/
    /* ============================================================================= */

     tmp1 = low1;
     tmp2 = low2;
     tmp3 = tmp1 + tmp2;

    low1 = (U32_IQ)tmp3;
    high1 = _sadd(high1, high2);
    res = _hill(tmp3);  // (U32_IQ)((U64_IQ)tmp3 >> 32);
    high1  =  _sadd(high1, (I32_IQ)res);

    /* high1=(A*A + B*B)  */

    /* ========================================= */
    /* Step(2):Normalize Input:                  */
    /*  V = X * 2^n                              */
    /* ========================================= */

    sbits = (U16_IQ)_norm(high1);


    /* =========================================================== */
    /* shifting left a 64 bit number...the number is split into    */
    /* 2 32 bits and shifted.........                              */
    /* =========================================================== */

    res = _SHR(low1, 32 - sbits);   // low1 >> (32 - sbits);
    high_new = high1;
    high1 = (I32_IQ)(high_new << sbits);

    high_new = high1;
    high1 = (I32_IQ)(high_new | res);

    n = sbits;
    sbits = (U16_IQ)_norm(high1);    
    n = (U16_IQ)(n + sbits);

    res = _SHR(low1, 32 - sbits);   // low1 >> (32 - sbits);
    high_new = high1;
    high1 = (I32_IQ)(high_new << sbits);

    high_new = high1;
    high1 = (I32_IQ)(high_new | res);
  


    /* V=high1 */

    /* ===================================================== */
    /* Step(3):Obtain initial estimate from "_IQsqrtTable"   */
    /* using the upper 8-bits of the normalized V value.     */
    /* ===================================================== */
    if(high1 > 0){
        index_new = (I16_IQ)(((U32_IQ)high1) >> 23);
        index_new = (I16_IQ)(index_new - 127);
        yn_new = IQisqrtTable[index_new];
    } 

    /* yn=Initial Estimate */


    /* ============================================================= */
    /* Step(4): Use Newton-Raphson algorithm to improve accuracy.    */
    /*  Repeat following equation two times. First iteration         */
    /*  gives 16-bit accuracy. Second iteration gives 32-bit         */ 
    /*  accuracy:                                                    */ 
    /*  Yn = Yn(1.5 - Yn*Yn*V/2)                                     */
    /*  Yn = 1/sqrt(V)                                               */
    /* ============================================================= */

    Dnorm = high1;
    reg64 = _mpy32ll(yn_new, yn_new);
    high1 = (I32_IQ)_hill(reg64);
    low1 = 0;
    high_new = high1;
    high1 = (I32_IQ)(high_new << 2);

    reg64 = _mpy32ll(Dnorm, high1);
    high1 = (I32_IQ)_hill(reg64);


    high1 = 0x60000000 - high1;   //1.5 - Yn*Yn*V/2
    reg64 = _mpy32ll(yn_new,high1);
    high1 = (I32_IQ)_hill(reg64);
    high_new = high1;
    high1 = (I32_IQ)(high_new << 2);

    yn_new = high1;
    reg64 = _mpy32ll(yn_new, yn_new);
    high1 = (I32_IQ)_hill(reg64);
    high_new = high1;
    high1 = (I32_IQ)(high_new << 2);

    reg64 = _mpy32ll(Dnorm, high1);
    high1 = (I32_IQ)_hill(reg64);


    high1 = 0x60000000 - high1;   //1.5 - Yn*Yn*V/2
    reg64 = _mpy32ll(yn_new, high1);
    high1 = (I32_IQ)_hill(reg64);
    high_new = high1;
    high1 = (I32_IQ)(high_new << 2);



    reg64 = _mpy32ll(high1, Dnorm);
    high1 =(I32_IQ) _hill(reg64);

    nn =(U16_IQ)( n << 15);
    if(nn != 0) //n & 0x01
    {
        reg64 = _mpy32ll(high1, 0x20000000);           // 0.5 .......IQsqrtRoundSatTable[6]
    }

    else
    {
        reg64 = _mpy32ll(high1, 0x2D413CCD);          // 1/sqrt(2) ..IQsqrtRoundSatTable[7]
    }

    high1 = (I32_IQ)_hill(reg64);
    low1 = _loll(reg64);



    /* =================================================== */
    /* Step(5):Denormalize result                          */
    /*        Y = Yn * 2^n / sqrt(2^n)                     */
    /* =================================================== */
    sbits = (U16_IQ)( n >> 1);
    /* shift right by sbits  */
    high_new = high1;
    res = _SHL(high_new, (32 - sbits));   // high_new << (32 - sbits);
    low1 >>= sbits;
    high_new = high1;
    high1 = (I32_IQ)(high_new >> sbits);
    low1  |= res;
    /* shift left by 4  bits */
    res = low1 >> (28);
    low1 <<= 4;
    high_new = high1;
    high_new <<= 4;
    high1 = (I32_IQ)(high_new | res);

    /* code for  _add64(&reg64,&t64);*/
    tmp1 = low1;
    tmp3 = tmp1 + tmp1;
    low1 = (U32_IQ)tmp3;
    high1 = _sadd(high1, high1);
    res = _hill(tmp3);  // (U32_IQ)((U64_IQ)tmp3 >> 32);
    high1  =  _sadd(high1, (I32_IQ)res);



    /* Rounding */
    res  = low1 >> 31;
    high1  =  _sadd(high1, (I32_IQ)res);


    return (high1);
}


/*###########################################################################
;;
;; FILE:    IQNmpy.c
;;
;; TITLE:   C-Model: _IQNmpy function
;;
;;===========================================================================
;; Function:   _IQNmpy
;;===========================================================================
;;
;; C Usage:    extern long _IQmpy(long M, long X);   // no round or sat
;;
;;---------------------------------------------------------------------------
;;
;; On Entry:   A4    = M in IQ format
;;             B4    = X in IQ format
;; On Exit:    A4    = M * X in IQ format
;;
;; Q range:    30 to 1
;;
;;---------------------------------------------------------------------------
;; Algorithm:  The IQmpy operation generates a 64-bit result. The 64-bit
;;             number must then be scaled back to a 32-bit value with the 
;;             same Q value as the original inputs. For example: if Q = 27, 
;;             then the multiplication of two I5Q27 numbers results in the 
;;             following 64-bit value:
;;
;;                          I5Q27 * I5Q27 = I10Q54
;;
;;             The value is then scaled back to an I5Q27 value as follows:
;;
;;                          I5Q27 = I10Q54 >> Q
;;
;;             For some of the operations, the value is either rounded or
;;             saturated (or both) to its maximum value before scaling.
;;          
;;---------------------------------------------------------------------------*/


static inline I32_IQ _IQNmpy(I32_IQ f1, I32_IQ f2, U32_IQ q_format)
{

    I64_IQ res_64;
    U32_IQ res_hi, res_lo, res, res_1;
    const U32_IQ k1 =32u;

    res_64 =  _mpy32ll(f1, f2); //(int)f1 * (int)f2;

    res_lo = _loll(res_64);
    res_hi = _hill(res_64);

    /* Extract the 32 bit result from 64 bit. */
    res = res_lo >> q_format;
    res_1 = (res_hi << (k1 - q_format));

    res |= res_1;

    return((I32_IQ)res);

}

/*###########################################################################
;;
;; FILE:    IQNmpyI32frac.c
;;
;; TITLE:   C-Model: _IQNmpyI32frac function
;;
;;===========================================================================
;; Function:   _IQNmpyI32frac
;;===========================================================================
;;
;; C Usage:    extern long _IQmpyI32frac(long A, long B); // no round or sat
;;
;;---------------------------------------------------------------------------
;;
;; On Entry:   A4    = A in IQ format
;;             B4    = B in 32-bit integer format
;; On Exit:    A4    = fractional portion of A*B stored as IQ
;;                       
;; Q range:    30 to 1
;;
;;---------------------------------------------------------------------------
;; Algorithm:  The fractional portion of the IQ*I32 number is calculated
;;             as follows:
;;
;;                fraction = A*B - ((A*B >> q_value) << q_value)
;;          
;;---------------------------------------------------------------------------*/



static inline I32_IQ _IQNmpyI32frac(I32_IQ x, I32_IQ y, U32_IQ qfmt)      
{


    U32_IQ res_lo,res_hi;
    I32_IQ ax,ay,reg64_hi;
    I64_IQ res_64;
    const U32_IQ k1 = 32U;
    

    ax = _abs(x);
    ay = _abs(y);

    res_64 = _mpy32ll(ax, ay);
    res_lo = _loll(res_64);
    
    res_lo = res_lo << (k1-qfmt);     // Check this.
    res_hi = res_lo >> (k1-qfmt);

    reg64_hi = (I32_IQ) (res_hi); 

//    if( (x<0 && y>0) || (x>0 && y<0))
    if( (x ^ y) >> 31 )   // If the sign bit is different.
    {
        reg64_hi = _ssub(0, reg64_hi);
    }

    return(reg64_hi);
}
/*###########################################################################
;;
;; FILE:    IQNmpyI32int.c
;;
;; TITLE:   C-Model: _IQNmpyI32int function
;;
;;===========================================================================
;; Function:   _IQNmpyI32int
;;===========================================================================
;;
;; C Usage:    extern long _IQmpyI32int(long A, long B); // with sat
;;
;;---------------------------------------------------------------------------
;;
;; On Entry:   A4    = A in IQ format
;;             A3    = B in 32-bit integer (long) format
;; On Exit:    A4    = integer portion of A*B stored as "long"
;;                       
;; Q range:    30 to 1
;;
;;---------------------------------------------------------------------------
;; Algorithm:  The integer portion of the IQ*I32 number is calculated
;;             as follows:
;;
;;                integer = A*B >> q_value;
;;          
;;---------------------------------------------------------------------------*/



static inline I32_IQ _IQNmpyI32int(I32_IQ x, I32_IQ y, U32_IQ qfmt)       
{
   
    U32_IQ res_lo,res_hi,mbits,mask;
    I32_IQ ax,ay,reg64_hi;
    I64_IQ res_64;
    const U32_IQ k1 = 32U;
    
    ax=_abs(x);
    ay=_abs(y);
    

    res_64 = _mpy32ll(ax,ay);
    res_hi = _hill(res_64);
    res_lo = _loll(res_64);

    mask = (1 << qfmt)-1;   // Sada: Why not shift directly by (k1-qfmt)
    mbits = res_hi & mask;
    mbits = mbits << (k1-qfmt);


    res_hi = (res_hi) >> (qfmt-1);
    res_lo = (res_lo) >> qfmt;
    res_lo = mbits | res_lo;

    // Sada: Perform saturation. Probably "if(res_hi < 1)" to consder negative result.
    if((res_hi !=0 ))
    {
    res_lo = 2147483647u;//0x7fffffff;
    }

    reg64_hi = (I32_IQ) res_lo;

//    if( (x<0 && y>0) || (x>0 && y<0))
    if ( (x ^ y) >> 31 )   // If the sign bit is different.
    {
        reg64_hi = _ssub(0,reg64_hi);
    }
  
    return(reg64_hi);
}
/*###########################################################################
;;
;; FILE:    IQNmpyIQx.c
;;
;; TITLE:   C-Model: _IQNmpyIQx function
;;
;;
;; C Usage:    extern static inline I32_IQ _IQNmpyIQx(I32_IQ in1, I32_IQ qfmt1, I32_IQ in2,
;;                                      I32_IQ qfmt2, I32_IQ qfmt)
;;
;;---------------------------------------------------------------------------*/



static inline I32_IQ _IQNmpyIQx(I32_IQ in1, I32_IQ qfmt1, I32_IQ in2, I32_IQ qfmt2, U32_IQ qfmt)      
{
    I32_IQ scnt;
    I64_IQ reg64;
    I32_IQ high, high_new;
    U32_IQ res, low,scnt_new;
    const U32_IQ k1 = 32u;
    I32_IQ result;
    reg64 = _mpy32ll(in1, in2);
    low = _loll(reg64);
    high = (I32_IQ)_hill(reg64);
    scnt = ((I32_IQ)qfmt - qfmt1 - qfmt2);
    if(scnt > 0)
    {
        scnt_new = (U32_IQ)scnt;

        if(high == 0 || high == 0xffffffff)
        {
        low <<= scnt_new;
        }

        else
        {
         low = 2147483647u;//0x7fffffff;
         }
        result = (I32_IQ)low;

    }
    else 
    {

        scnt_new = (U32_IQ)(-scnt);
        high_new = high;
        if (scnt_new > 31) 
        {
            scnt_new =  scnt_new - 32u;
            low = high_new >> scnt_new;

                result = (I32_IQ)low;

        }
        else
        {

            high = (I32_IQ)high >> (scnt_new - 1);
            if(high == 0 || high == 0xffffffff){
                res = (high_new << (k1 - scnt_new));
                low >>= (scnt_new);
                low |=res;
            }
            else
            { low = 2147483647u;//0x7fffffff;

            }

                result = (I32_IQ)low;
        }

    }

    return (result);

}

          




   








/*###########################################################################
;;
;; FILE:    IQNrmpy.c
;;
;; TITLE:   C-Model: _IQNrmpy function
;;
;;===========================================================================
;; Function:   _IQNrmpy
;;===========================================================================
;;
;; (for C64x+ processors )
;; C Usage:    extern int _IQNdiv(int M, int X);    //with rounding
;;
;; (for C28x processors )
;; C Usage:    extern long _IQmpy(long M, long X);
;;


;;---------------------------------------------------------------------------
;;
;; On Entry:   A4    = M in IQ format
;;             A4    = X in IQ format
;; On Exit:    ACC   = M * X in IQ format
;;
;; Q range:    30 to 1
;;
;;---------------------------------------------------------------------------
;; Algorithm:  The IQmpy operation generates a 64-bit result. The 64-bit
;;             number must then be scaled back to a 32-bit value with the 
;;             same Q value as the original inputs. For example: if Q = 27, 
;;             then the multiplication of two I5Q27 numbers results in the 
;;             following 64-bit value:
;;
;;                          I5Q27 * I5Q27 = I10Q54
;;
;;             The value is then scaled back to an I5Q27 value as follows:
;;
;;                          I5Q27 = I10Q54 >> Q
;;
;;             For some of the operations, the value is either rounded or
;;             saturated (or both) to its maximum value before scaling.
;;          
;;---------------------------------------------------------------------------*/


static inline I32_IQ _IQNrmpy(I32_IQ x, I32_IQ y, U32_IQ qfmt)       
{
    I64_IQ reg64;
    I32_IQ hi, lo ,temp;
    Uword40 temp1, temp3 ;
    const U32_IQ k1 = 32u;
    /* =========================================================================*/
    /* multiply the inputs                                                      */          
    /* =========================================================================*/

    reg64 = _mpy32ll(x, y);
    temp1 = _loll(reg64);
    /* =========================================================================*/
    /* rounding of the answer obtained                                                                  */          
    /* =========================================================================*/

    temp3 = temp1 + ( 1 << (qfmt - 1) );  
    lo = (I32_IQ)temp3;
    hi =(I32_IQ)_hill(reg64);
    temp = (I32_IQ)_hill((I64_IQ)temp3);
    hi = _sadd(hi , temp);
    /* =========================================================================*/
    /* finally rescaling so that the q format maintained                        */          
    /* =========================================================================*/

    lo = (I32_IQ)((U32_IQ)lo >> qfmt) ;
    hi = (I32_IQ)hi << (k1 - qfmt) ;
    lo = lo + hi;
    return(lo);
}
/*###########################################################################
;;
;; FILE:    IQNrsmpy.c
;;
;; TITLE:   C-Model: _IQNrsmpy function
;;
;;===========================================================================
;; Function:   _IQNrsmpy
;;===========================================================================
;;
;; C Usage:    extern long _IQrsmpy(long M, long X);   // no round or sat
;;
;;---------------------------------------------------------------------------
;;
;; On Entry:   A4    = M in IQ format
;;             B4    = X in IQ format
;; On Exit:    A4    = M * X in IQ format
;;
;; Q range:    30 to 1
;;
;;---------------------------------------------------------------------------
;; Algorithm:  The IQmpy operation generates a 64-bit result. The 64-bit
;;             number must then be scaled back to a 32-bit value with the 
;;             same Q value as the original inputs. For example: if Q = 27, 
;;             then the multiplication of two I5Q27 numbers results in the 
;;             following 64-bit value:
;;
;;                          I5Q27 * I5Q27 = I10Q54
;;
;;             The value is then scaled back to an I5Q27 value as follows:
;;
;;                          I5Q27 = I10Q54 >> Q
;;
;;             For some of the operations, the value is either rounded or
;;             saturated (or both) to its maximum value before scaling.
;;          
;;---------------------------------------------------------------------------*/




static inline I32_IQ _IQNrsmpy(I32_IQ x, I32_IQ y, U32_IQ qfmt)      
{
    I64_IQ reg64;
    I32_IQ high, acc;
    U32_IQ p, low, res,high_new;
    U64_IQ tmp1, tmp3;
    const U32_IQ k1 = 0xffffffff , k2 =32u;
    
    /* =========================================================================*/
    /* multiply the inputs                                                      */          
    /* =========================================================================*/

    reg64 = _mpy32ll(x, y);
    high = (I32_IQ)_hill(reg64);
    tmp1 = _loll(reg64);



    tmp3 = tmp1 + ( 1 << (qfmt-1) );  
    low = (U32_IQ)tmp3;
    acc = (I32_IQ)_hill((I64_IQ)tmp3);
    high = _sadd(high,acc );

    /* ======================================== */
    /* Checking for minimum of 2 64 bit numbers */
    /* ======================================== */

    acc = ((1 << (qfmt - 1)) - 1);
    p = 0xFFFFFFFF;

    if((high > acc) && (low <=  p) )
    {
        low = p;
        high = acc;
    }


    /* ======================================== */
    /* Checking for maximum of 2 64 bit numbers */
    /* ======================================== */ 
    
    acc = (I32_IQ)(k1 - acc);
    p = 0;

    if( high < acc)
    {
        low = p;
        high = acc;
    }

    /* ======================================================= */
    /* finally rescaling so that the q format maintained       */           
    /* ======================================================  */

    res = low >> (qfmt);
    high_new = high;
    high = (I32_IQ)(high_new << (k2 - qfmt));

    high_new = high;
    high = (I32_IQ)(high_new | res);

    return(high);
}
/*###########################################################################
;;
;; FILE:    _IQNsat.c
;;
;; TITLE:   C-Model: _IQNsat function
;;

;;===========================================================================
;; Function:   _IQNsat
;;===========================================================================
;;
;; C Usage:    _IQsat(int A);
;;
;;---------------------------------------------------------------------------
;;
;; On Entry:   A4    = A in IQ format
;; On Exit:    A4    = Saturated value of A in IQ format
;;                       
;; Q range:    31 to 0
;;
;;---------------------------------------------------------------------------
;; Algorithm:  Saturates the value "A" between the values "Pos" and "Neg".
;;          
;;--------------------------------------------------------------------------*/



static inline I32_IQ _IQNsat(I32_IQ A,U32_IQ qfmt)
{
    U32_IQ Pos;
    I32_IQ Neg;
    const U32_IQ k1 =31u;
    Pos=  ((1<<(k1 - qfmt)) - 1);
    Neg= -(1<<(k1 - qfmt));
    if( A > (I32_IQ)Pos )
    {
        A =(I32_IQ) Pos;
    } 
    if( A < Neg )
    {
        A = Neg;
    }
    return A;
}


/*;;###########################################################################
;;
;; FILE:    IQNtoF.c
;;
;; TITLE:   C Callable IQ to Float Math Function
;;
;;###########################################################################
;;

;;===========================================================================
;; Function:   _IQNtoF
;;===========================================================================
;;
;; C Usage:    extern float _IQtoF(int A);   // no round or sat
;;
;;---------------------------------------------------------------------------
;;
;; On Entry:   A4    = A in IQ format
;; On Exit:    A4    = IEEE 754 floating-point equivalent of A
;;                       
;; Q range:    31 to 0
;;
;;---------------------------------------------------------------------------
;; Algorithm:  This operation converts an IQ number to the equivalent
;;             IEEE 754 Single-Precision floating-point format. This
;;             format is shown below:
;;
;;              31  30        23 22                                 0
;;             +-----------------------------------------------------+
;;             | s |      e     |                 f                  |
;;             +-----------------------------------------------------+
;;
;;             Value = (-1)^s * 2^(e-127) * 1.f
;;
;;             where: e = 1 to 254, f = 0.000000000 to ~1.0
;;                    e = 0, f = 0, s = 0, Value = 0.0
;;                    e = 0 and f != 0 case cannot occur in IQ math
;;                    e = 255 case cannot occur in IQ math
;;          
;;--------------------------------------------------------------------------*/




static inline Fword32 _IQNtoF(I32_IQ input, U32_IQ qfmt)       
{
#ifdef _TMS320C6X 
    U32_IQ ah, al, sign, e, sbits;
     Fword32 output;

    /* ======================================================================== */
    /*  normalize the input                                                     */
    /* ======================================================================== */
    
    sbits = _norm(input);
    ah = (U32_IQ)input;
    ah = ah << sbits;

    /* ======================================================================== */
    /*  use the same value if input is positive else complement it              */
    /* ======================================================================== */
    
    al = ah ^ 0x7fffffffu;
    if((I32_IQ)al >= 0)
    {
        al = ah;
    }
    ah = al;

    /* ======================================================================== */
    /*  find the sbits depending upon the q-format and input                    */
    /* ======================================================================== */
    
    e = 127u + (30u - qfmt);
    sbits =  e - sbits;
    if( ah == 0 )
    {
        sbits = 0;
    }

    /* ======================================================================== */
    /*  arrange the sign,exponent and mantissa(16 bits) in the specified format */
    /* ======================================================================== */
    
    al = al << 2;
    al = al >> 9;
    sign = ah & 0x80000000u;
    ah = sbits;
    ah = ah << 23;
    al = al | ah | sign;
    output = _itof(al);
    return(output);
#else  /* _TMS320C6X */

    Fword32 res = (Fword32) ( (double)input / ((__int64)1 << qfmt) );
    return(res);

#endif /* _TMS320C6X */
}



#endif /* __IQMATH_INLINE_H__ */

/* ======================================================================== */
/*            Copyright (c) 2007 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */

