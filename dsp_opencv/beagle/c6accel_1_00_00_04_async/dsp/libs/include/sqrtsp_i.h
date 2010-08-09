/****************************************************************************/
/*  sqrtsp_i.h          v6.0.8                                              */
/*  Copyright (c) 1997-2006  Texas Instruments Incorporated                 */
/****************************************************************************/
 

#ifndef _SQRTSP_I_H_
#define _SQRTSP_I_H_


#ifdef INLINE_C
    #define _INDEFF static inline float
#else 
    #define _INDEFF float 
#endif

_INDEFF sqrtsp_i(float x)
{
    float    f, y, z, y1;
    int      e;
    float a, b;
    int uy, ux, e1, n;
    int *exp = 0;
    
//    zero_flag = x;   

    CPYF(b, (x));
    ux = _ftoi(b); /* Required to extract the exponent */
    e = ((ux >> (32 - (8 +1) )) & ((1<< 8 )-1) ); /* get the exponent */
    if (e == 0) /* +/- ZERO or denormalized? */
    {
  
        b = mpysp_i(x,CNST(16777216.0) );  /* its not denormalized now! */
       
        ux = _ftoi(b);
       
        e = ((ux >> (32 - (8 +1) )) & ((1<< 8 )-1) ) - 24 ;
           
        if EQZF(b) { 
            *exp = 0; 
            f = x; 
        } /* +/- ZERO */

    }
    else if (e == ((1<< 8 )-1) ) { /* infinities and NAN's */
        if LSSF(b,CNST(-3.402823466E+38) ) {
             errno = EDOM; 
             CPYF(b,CNST(-3.402823466E+38) );
        }
        else if GTRF(b,CNST( 3.402823466E+38) ) { 
             errno = EDOM; 
            CPYF(b,CNST( 3.402823466E+38) );
         }
         else{ 
             errno = EDOM; 
             *exp = 0; 
             b = ZERO;
         }
         
     }
     e -= (127 -1) ;
     *exp = e;
     b = _itof((ux & ((1<<(32 -1))+(1<<(23 % 32 ))-1) ) | /* out with the old ...*/
                ((127 -1)  << (32 - (8 +1) )));       /* and in with the new */
    
     f = b;

    /* First form an estimate of the inverse-square-root.                */
    /* The accuracy of the estimate determines the number of required    */
    /* Newton iterations to compute the required n-bit mantissa result.  */
    /* Given a number expressed as x = f* 2**e, an estimate of 1/SQRT(x),*/
    /* may be computed as:                                               */
    /*   1/SQRT(x)                                                       */
    /* = 1/SQRT(f * 2**e)                                                */
    /* = 1/SQRT(f)      * 1/SQRT(2**e)                                   */
    /*~= (C1*f + C2)    * 1/SQRT(2**(e & 1)) * 1/SQRT(2**(e-(e & 1)))    */
    /* = (C1*f + C2)    * 1/SQRT(2**(e & 1)) * (2**-((e-(e & 1))/2))     */
    /* = (C1*f + C2)    * 1/SQRT(2**(e & 1)) * (2**-((e-(e & 1))/2)-1)*2 */
    /* = (C1*f + C2)    * 1/SQRT(2**(e & 1)) * (2**-((e-(e & 1))/2)-1)*2 */
    /* = 2*(C1*f + C2)  * 1/SQRT(2**(e & 1)) * (2**-((e-(e & 1))/2)-1)   */
    /* =(2*C1*f + 2*C2) * 1/SQRT(2**(e & 1)) * (2**-((e-(e & 1))/2)-1)   */
    /* =(2*C1*f + 2*C2) * {1 or 1/SQRT(2)}   * (2**-((e-(e & 1))/2)-1)   */
    /* .5 <= f < 1, we need 1 <= f < 2, so... */
    /* e = e - 1  ; -- moved below.  Maybe CPU has LSB test as byproduct. */
    /* f = f * 2.0; -- subsumed into calculation below */

    f = mpysp_i(CNST(-1.145508) ,f);
    f = addsp_i(f,CNST( 2.528320));
    e -= 1;
    if (e & 1) 
    {
        e -= 1;
        f = mpysp_i(f, One_Over_Sqrt_2);
    }
 /* note: last use of "e" */
    n = ~(e >> 1); 
    CPYF(a,f);
    uy = (int)_ftoi(a);
    e1 = ((uy >> (32 - (8 +1) )) & ((1<< 8 )-1) );
    a = _itof((uy & ((1<<(32 -1))+(1<<(23 % 32 ))-1) ) |              /* out with the old ...*/
        ((e1+n) << (32 - (8 +1) )));    /* and in with the new */
    if (n > 128 + (127 - 1) - e1)
    {
        errno = ERANGE;
        CPYF(a,(GEZF(f) ? CNST( 3.402823466E+38)  : CNST(-3.402823466E+38) ));

        y = a;
    }
    
    if (n < (-125) + (127 -1) - e1) { /* underflow or denormalized? */
                  /* resulting number will be denormalized */
        a = _itof((uy & ((1<<(32 -1))+(1<<(23 % 32 ))-1) ) |                /* out with the old...*/
             ((e1+n+ 24 )<<(32 - (8 +1) )));/* in with (some) new.*/
        y = mpysp_i(a,CNST(5.9604644753E-8));
        if (n < (-125) -(24 -1)+ (127 -1) - e1) /* underflow? */
             a = ZERO;
         /* MPYF will underflow if target does not support denormalized nos. */
    }
    y = a;
    if (e1 == 0)
    {
        if EQZF(f) 
            y = f;
        y = mpysp_i(f,CNST(16777216.0));
        uy = (int) _ftoi(y);
        e1 = ((uy >> (32 -(8+1))) & ((1 << 8)-1 )) - 24;
    }
    else if (e1 == ((1<<8)-1)) 
        y = f;

    /* Estimate of 1/sqrt in y now good to about 5 mantissa bits. */
    /* Refine estimate with Newton iterations.                    */
    /* Do one less newton iterations than is required.            */
    /* Accuracy roughly doubles each time through the loop.       */
    /* Haven't determined actual accuracy vs. iterations yet.     */
    /* Thresholds on loop choices are thus overly conservative.   */
    /* Note: Cannot square y below for reasons of over/under-flow.*/
    /* y = y+y*(0.5-((x*y)*0.5)*y); */
        y1 = mpysp_i(x,y);
        y1 = mpysp_i(HALF,y1);
        y1 = mpysp_i(y1,y);
        y1 = subsp_i(HALF,y1);
        y1 = mpysp_i(y1,y);
        y = addsp_i(y,y1);

        y1 = mpysp_i(x,y);
        y1 = mpysp_i(HALF,y1);
        y1 = mpysp_i(y1,y);
        y1 = subsp_i(HALF,y1);
        y1 = mpysp_i(y1,y);
        y = addsp_i(y,y1);

    z = mpysp_i(x,y);

       /* Here's the last iteration! */
       /* return z+z*(0.5-((x*y)*0.5)*y); */

    y1 = mpysp_i(x,y);
    y1 = mpysp_i(HALF,y1);
    y1 = mpysp_i(y1,y);
    y1 = subsp_i(HALF,y1);
    y1 = mpysp_i(y1,z);
    z = addsp_i(z,y1); 
    if(x <= 0)
        z = 0;  
    return z;
  
} /* SQRTF */

#endif /* _SQRTSP_I_H */

/* ======================================================================== */
/*  End of file: sqrtsp_i.h                                                 */
/* ------------------------------------------------------------------------ */
/*          Copyright (C) 2002 Texas Instruments, Incorporated.             */
/*                          All Rights Reserved.                            */
/* ======================================================================== */

