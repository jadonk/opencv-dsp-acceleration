/****************************************************************************/
/*  syntfi.h         v4.20                                                  */
/*  Copyright (c) 1997-2001  Texas Instruments Incorporated                 */
/****************************************************************************/
 
/* This source automatically generated on Wed Aug  2 15:52:55 CDT 2000 */
 
#ifndef __syntf__
#define __syntf__

#if 0
/*---------------------------------------------------------------------------*/
/* Bind abstract REAL to "float" syntax.                                     */
/* Use ANSI-style function calls for IEEE +,-,*,/, >,<,>=,<=,==,!=, =        */
/* Use ANSI names for float version of math.h routines (sqrtf, sinf, ... etc)*/
/*---------------------------------------------------------------------------*/
#endif

#define REAL float
#define EXPONENT_TYPE int

#if BPLEAN == 0
#undef LEAN /* no next-smaller REAL*/
#else
#define LEAN float16 /*  assume its a size 16-bit IEEE-like float */
#endif

#define CNST(x) (x##f)/*  constants within statements */
#define TSNC
#define KNST(x) x##f  /*  constants within tables     */
#define TSNK

#include "numconst.h" /*  NUMerical CONSTants used by some definitions below */

#if 0
/*---------------------------------------------------------------------------*/
/* Define Compiler Syntax For Basic Operations                               */
/*---------------------------------------------------------------------------*/
/* Names have form: <operation> { F | G } { 1 | 2 | 3 }?                     */
/* Where <operation> is one of:                                              */
/*    CPY  -- simple assignment                                              */
/*    NEG  -- negative                                                       */
/*    ABS  -- absolute value                                                 */
/*    SQR  -- square                                                         */
/*    INV  -- inverse (1/x)                                                  */
/*    ADD  -- plus                                                           */
/*    SUB  -- subtract                                                       */
/*    MPY  -- multiply                                                       */
/*    DIV  -- division                                                       */
/*    EQL  -- ==                                                             */
/*    NEQ  -- !=                                                             */
/*    GTR  -- >                                                              */
/*    GEQ  -- >=                                                             */
/*    LSS  -- <                                                              */
/*    LEQ  -- <=                                                             */
/*    EQZ  -- == 0.0                                                         */
/*    NEZ  -- != 0.0                                                         */
/*    GTZ  -- >  0.0                                                         */
/*    GEZ  -- >= 0.0                                                         */
/*    LSZ  -- <  0.0                                                         */
/*    LEZ  -- <= 0.0                                                         */
/* F means REAL, G means BIGGER_REAL                                         */
/* If no 1,2 or 3 suffix then is function returning result.                  */
/* Optional 1,2 or 3 means 1-addr, 2-addr or 3-addr version:                 */
/*    1 -> right = operation right;                                          */
/*    2 -> dst   = operation right;      ( unary operations)                 */
/*         left  = left operation right; (binary operations)                 */
/*    3 -> dst   = left operation right;                                     */
/*---------------------------------------------------------------------------*/
#endif

#define CPYF(dst,right) dst=(right)

#define NEGF(right) _negf(right)
#define ABSF(right) _absf(right)
#define SQRF(right) MPYF(right,right)
#define INVF(right) DIVF(ONE,right)

#define ADDF(left,right) _addf(left,right)
#define SUBF(left,right) _subf(left,right)
#define MPYF(left,right) _mpyf(left,right)
#define DIVF(left,right) _divf(left,right)

#define NEGF2(dst,right) dst=NEGF(right)
#define ABSF2(dst,right) dst=ABSF(right)
#define SQRF2(dst,right) dst=MPYF(right,right)
#define INVF2(dst,right) dst=INVF(right)

#define ADDF3(dst,left,right) dst=ADDF(left,right)
#define SUBF3(dst,left,right) dst=SUBF(left,right)
#define MPYF3(dst,left,right) dst=MPYF(left,right)
/*#ifdef MPYF3(dst,left,right)
	#error "Int Mutliply"
#endif*/

#define DIVF3(dst,left,right) dst=DIVF(left,right)

#define NEGF1(right) (right)=NEGF(right)
#define ABSF1(right) (right)=ABSF(right)
#define SQRF1(right) (right)=MPYF(right,right)
#define INVF1(right) (right)=INVF(right)

#define ADDF2(left,right) (left)=ADDF(left,right)
#define SUBF2(left,right) (left)=SUBF(left,right)
#define RSBF2(left,right) (left)=SUBF(right,left)
#define MPYF2(left,right) (left)=MPYF(left,right)
#define DIVF2(left,right) (left)=DIVF(left,right)

#define EQLF(left,right) (_eqlf(left,right))
#define NEQF(left,right) (_neqf(left,right))
#define LEQF(left,right) (_leqf(left,right))
#define GEQF(left,right) (_geqf(left,right))
#define LSSF(left,right) (_lssf(left,right))
#define GTRF(left,right) (_gtrf(left,right))

#define CMPF(left,right) _cmpf(left,right) /*  <0 if LT, 0 if EQ, >0 if GT */
#define CMPF3(dst,left,right) dst=CMPF(left,right)

#define EQZF(left) EQLF(left,ZERO)
#define NEZF(left) NEQF(left,ZERO)
#define LEZF(left) LEQF(left,ZERO)
#define GTZF(left) GTRF(left,ZERO)

   #define GEZF(left) GEQF(left,ZERO)
   #define LSZF(left) LSSF(left,ZERO)


#if 0
/*---------------------------------------------------------------------------*/
/* Compose and Decompose REAL                                                */
/* integer <-> REAL conversion operations                                    */
/* REAL whole number conversions, caller KNOWS whole number fits in int      */
/* Exact Remainder                                                           */
/*---------------------------------------------------------------------------*/
/* Names have form: <operation> { F | G } { 1 | 2 | 3 }?                     */
/* Where <operation> is one of:                                              */
/*    SCL  -- f * RADIX**e                                                   */
/*    RIP  -- decompose x into f,e such that x == f*RADIX**e and 1/RADIX<=f<1*/
/*                                                                           */
/*    FIX  -- truncate towards zero and convert to int                       */
/*    FLT  -- convert int  to REAL                                           */
/*    FIXU -- truncate towards zero and convert to unsigned int              */
/*    FLTU -- convert unsigned int  to REAL                                  */
/*                                                                           */
/*    TNC  -- truncate towards 0                (x fits in int)              */
/*    RND  -- round to nearest whole number REAL(x fits in int)              */
/*    FLR  -- truncate towards minus infinity   (x fits in int)              */
/*    CEI  -- truncate towards plus  infinity   (x fits in int)              */
/*                                                                           */
/*    REM  -- *EXACT* remainder (big horrible by-hand divide routine)        */
/*                                                                           */
/* Note that the following whole number approximation rtns exist in math.h:  */
/*    TRUNCF -- truncate towards 0                (x any value)              */
/*    ROUNDF -- round to nearest whole number REAL(x any value)              */
/*    FLOORF -- truncate towards minus infinity   (x any value)              */
/*    CEILF  -- truncate towards plus  infinity   (x any value)              */
/*                                                                           */
/* F means REAL, G means BIGGER_REAL                                         */
/* If no 1,2 or 3 suffix then is function returning result.                  */
/* Optional 1,2 or 3 means 1-addr, 2-addr or 3-addr version:                 */
/*    1 -> right = operation right;                                          */
/*    2 -> dst   = operation right;      ( unary operations)                 */
/*         left  = left operation right; (binary operations)                 */
/*    3 -> dst   = left operation right;                                     */
/*---------------------------------------------------------------------------*/
#endif

#define RIPF(x,f,e) f=FREXPF((x),&e)

#define SCLF(f,e)            LDEXPF((f),(e))
#define SCLF2(f,e)       f = LDEXPF((f),(e))
#define SCLF3(dst,f,e) dst = LDEXPF((f),(e))
 
#define FIXFC(x) _fixfc(x) /* ((char)(x))    */
#define FLTCF(x) _fltcf(x) /* ((REAL)(x))    */

#define FIXFB(x) _fixfb(x) /* ((unsigned char)(x))*/
#define FLTBF(x) _fltbf(x) /* ((REAL)(x))        */

#define FIXFSI(x) _fixfsi(x) /* ((short)(x))   */
#define FLTSIF(x) _fltsif(x) /* ((REAL)(x))    */

#define FIXFUS(x) _fixfus(x) /* ((unsigned short)(x))*/
#define FLTUSF(x) _fltusf(x) /* ((REAL)(x))          */

#define FIXFI(x) _fixfi(x) /* ((int)(x))     */
#define FLTIF(x) _fltif(x) /* ((REAL)(x))    */

#define FIXFU(x) _fixfu(x) /* ((unsigned)(x))*/
#define FLTUF(x) _fltuf(x) /* ((REAL)(x))    */

#define FIXFLI(x) _fixfli(x) /* ((long)(x))    */
#define FLTLIF(x) _fltlif(x) /* ((REAL)(x))    */

#define FIXFUL(x) _fixful(x) /* ((unsigned long)(x))*/
#define FLTULF(x) _fltulf(x) /* ((REAL)(x))         */


#if 0
/* TNCF, RNDF, FLRF, CEIF, may all use FLTIF(FIXFI(f(x))).                */
/* Call sites must be sure to enforce several rules:                      */
/* 1. TRUNCF(f(x)) must not exceed the integer range.                     */
/* 2. if REAL does unbiased round-towards-even,                           */
/*    input x to RNDF must never equal +/- RADIX_to_Mantissa-1.           */
#endif
#ifndef RND_GLITCH
   #error "REAL format .h file must be included before REAL syntax .h file"
#endif
#if (RND_GLITCH)
   #define RND_MAX_P1 RND_P_GLITCH
   #define RND_MIN    RND_M_GLITCH
#else
   #define RND_MAX_P1 INT_MOST_POS_P1
   #define RND_MIN    INT_MOST_NEG
#endif
#ifndef FLT_FIX_Faster_Than_MODF
   #error "target props .h file must be included before REAL syntax .h file"
#endif
#if (FLT_FIX_Faster_Than_MODF)
   #define TNCF(x) (FLTIF(FIXFI(x)))

   #define RNDF(x) (GEZF(x)? \
                    FLTIF(FIXFI((ADDF((x),HALF)))) : \
                    FLTIF(FIXFI((SUBF((x),HALF)))))

   #define FLRF(x) (GEZF(x) ? \
                    (FLTIF(FIXFI(x))) : \
                     (EQLF(x,FLTIF(FIXFI(x))) ? \
                      x : \
                      (FLTIF(NEGF(FIXFI(ADDF(NEGF(x),ONE)))))))

   #define CEIF(x) (LEZF(x) ? \
                    (FLTIF(FIXFI(x))) : \
                     (EQLF(x,FLTIF(FIXFI(x))) ? \
                      x : \
                      (FLTIF(NEGF(FIXFI(SUBF(NEGF(x),ONE)))))))

#else /*  call through to general purpose routines */
   #define TNCF(x) (TRUNCF(x))
   #define RNDF(x) (ROUNDF(x))
   #define FLRF(x) (FLOORF(x))
   #define CEIF(x) (CEILF(x))
#endif

#define FIXFI1(x) x=FIXFI(x)
#define FLTIF1(x) x=FLTIF(x)
#define FIXFU1(x) x=FIXFU(x)
#define FLTUF1(x) x=FLTUF(x)

#define TNCF1(x) x=TNCF(x)
#define RNDF1(x) x=RNDF(x)
#define FLRF1(x) x=FLRF(x)
#define CEIF1(x) x=CEIF(x)

#define FIXFI2(dst,right) dst=FIXFI(right)
#define FLTIF2(dst,right) dst=FLTIF(right)
#define FIXFU2(dst,right) dst=FIXFU(right)
#define FLTUF2(dst,right) dst=FLTUF(right)

#define TNCF2(dst,right) dst=TNCF(right)
#define RNDF2(dst,right) dst=RNDF(right)
#define FLRF2(dst,right) dst=FLRF(right)
#define CEIF2(dst,right) dst=CEIF(right)


#define REMF(right,left)           (FMODF(right,left))
#define REMF1(right,left)     right=FMODF(right,left)
#define REMF2(dst,right,left) dst  =FMODF(right,left)


#if 0
/*---------------------------------------------------------------------------*/
/* IEEE software support specific routines                                   */
/*---------------------------------------------------------------------------*/
#endif
#if (BPREAL == BPbigint)
   #define FRCMPYF(l,r) _frcmpyf(l,r)
   #define FRCDIVF(l,r) _frcdivf(l,r)
#elif (BPREAL == (BPbigint*2))
   #define RENORMF(x) _renormf(x)
   #define FRCADDF(lx,lf,rx,rf) _frcaddf(lx,lf,rx,rf)
   #define FRCMPYF(p,l,r) _frcmpyf(p,l,r)
   #define FRCDIVF(q,l,r) _frcdivf(q,l,r)
#else
   #error "IEEE lowest-level support routines NYI for this size REAL"
#endif
#ifdef LEAN
   #error "GROWF/SLIMF not yet defined for this size LEAN"
#endif

#if 0
/*---------------------------------------------------------------------------*/
/* Polynomial evaluation loops macros.                                       */
/*---------------------------------------------------------------------------*/
/* Loops are used for all polynomials.  It is assumed that this              */
/* is more DSP-friendly given that DSP hardware/software has:                */
/*   . zero-overhead looping,                                                */
/*   . optimizations to software pipeline multiply-accumulate,               */
/*   . the lack of full-precision floating-immediate address mode,           */
/*   . relatively expensive fetch of non-tabular constants,                  */
/*   . the existance of auto-increment addressing for table access.          */
/*---------------------------------------------------------------------------*/

/* form of: r=(((K[0]*x + K[1])*x + K[2])*x); r not same as x */
#endif
#define POLYF(r,x,K,s) {int i; REAL *p=K;                             \
                        MPYF3(r,x,*p++);                              \
                        for (i=s-1; i>0; i--) MPYF3(r,x,ADDF(r,*p++));}

#if 0
/* extra add at the end */
/* form of: r=(((K[0]*x + K[1])*x + K[2])*x) + K[3];  r not same as x */
#endif
#define POLYaF(r,x,K,s) {int i; REAL *p=K;                             \
                         MPYF3(r,x,*p++);                              \
                         for (i=s-2; i>0; i--) MPYF3(r,x,ADDF(r,*p++));\
                         ADDF2(r,*p++);}

#if 0
/* form of: r=((x+K[0])*x + K[1])*x + K[2]) * x) + K[3]; r not same as x */
#endif
#define POLYbF(r,x,K,s) {int i; REAL *p=K;                             \
                         ADDF3(r,x,*p++);                              \
                         for (i=s-1; i>0; i--) ADDF3(r,*p++,MPYF(x,r));}

#if 0
/*---------------------------------------------------------------------------*/
/* Bind The Names Of The Math Library Routines                               */
/*---------------------------------------------------------------------------*/
#endif
#define SQRTF(x)    std::sqrtf(x)
#define RSQRTF(x)   std::rsqrtf(x)
#define EXPF(x)     std::expf(x)
#define EXP2F(x)    std::exp2f(x)
#define EXP10F(x)   std::exp10f(x)
#define LOGF(x)     std::logf(x)
#define LOG2F(x)    std::log2f(x)
#define LOG10F(x)   std::log10f(x)
#define POWF(x,y)   std::powf(x,y)
#define POWIF(x,i)  std::powif(x,i)
#define SINF(x)     std::sinf(x)
#define COSF(x)     std::cosf(x)
#define TANF(x)     std::tanf(x)
#define COTF(x)     std::cotf(x)
#define ASINF(x)    std::asinf(x)
#define ACOSF(x)    std::acosf(x)
#define ATANF(x)    std::atanf(x)
#define ATAN2F(y,x) std::atan2f(y,x)
#define ACOTF(x)    std::acotf(x)
#define ACOT2F(x,y) std::acot2f(x,y)
#define SINHF(x)    std::sinhf(x)
#define COSHF(x)    std::coshf(x)
#define TANHF(x)    std::tanhf(x)
#define COTHF(x)    std::cothf(x)
#define ASINHF(x)   std::asinhf(x)
#define ACOSHF(x)   std::acoshf(x)
#define ATANHF(x)   std::atanhf(x)
#define ACOTHF(x)   std::acothf(x)

#define CEILF(x)    ceilf(x)
#define FLOORF(x)   floorf(x)
#define TRUNCF(x)   std::_truncf(x)
#define ROUNDF(x)   std::_roundf(x)

#define FABSF(x)    fabsf(x)

#define LDEXPF(x,n)   std::ldexpf(x,n)
#define FREXPF(x,exp) std::frexpf(x,exp)
#define MODFF(x,ip)   std::modff(x,ip)
#define FMODF(x,y)    std::fmodf(x,y)

#endif /* __syntf__*/
