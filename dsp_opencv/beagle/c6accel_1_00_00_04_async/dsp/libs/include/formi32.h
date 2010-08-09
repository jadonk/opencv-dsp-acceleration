/****************************************************************************/
/*  formi32.h        v4.20                                                 */
/*  Copyright (c) 1997-2001  Texas Instruments Incorporated                 */
/****************************************************************************/
 
/* This source automatically generated on Wed Aug  2 15:54:34 CDT 2000 */
 
#ifndef __formi32__
#define __formi32__

#if 0
/*---------------------------------------------------------------------------*/
/* Description of the IEEE 32-bit float format.                              */
/*                                                                           */
/* This file is used in conjunction with the c pre-processor to build        */
/* customized product sources.  It is carefully constructed so that comments */
/* are removed by the preprocessor and all expansions result in a minimum    */
/* disruption of surrounding code.                                           */
/*                                                                           */
/* All floating-point constants in this file must be expressed either        */
/* exactly or to *MORE* places than REAL_DIGITS (REAL_DIGITS+3 is suggested).*/
/*---------------------------------------------------------------------------*/
#endif

#if 0
/*---------------------------------------------------------------------------*/
/* REAL is described in terms of the canonical form:  +/- f * r**e           */
/* where: r is "radix",                                                      */
/*        e is "exponent" (an integer),                                      */
/*        f is fractional part (mantissa) and  1/r <= f < 1,                 */
/*---------------------------------------------------------------------------*/
#endif

#define REAL_RADIX     2  /* radix (r)                                        */
#define REAL_MANTISSA 24  /* radix digits in mantissa                         */
#define REAL_EMAX    128  /* max e for which +/- f has MANTISSA digits        */
#define REAL_EMIN  (-125) /* min e for which +/- f has MANTISSA digits        */
#define REAL_DIGITS    6  /* FLRF(REAL_MANTISSA-1)*Ln_RADIX/Ln_10)            */

#define Round_To_Even

#define IEEE /* don't define "IEEE" at all if not ieee format */

#if defined(Generate_Sources)
   #define if_IEEE_Hardware #ifdef IEEE_Hardware
   #define elsif_not_IEEE_Hardware #else
   #define endif_IEEE_Hardware #endif
#elif defined(IEEE_Hardware)
   #define if_IEEE_Hardware if (IEEE_Hardware) {
   #define elsif_not_IEEE_Hardware } else {
   #define endif_IEEE_Hardware }
#else
   #define if_IEEE_Hardware if (0) {
   #define elsif_not_IEEE_Hardware } else {
   #define endif_IEEE_Hardware }
#endif

#define REAL_EXP_BITS 8   /* bits in unsigned_exp_field                    */
#define REAL_FRC_BITS 23  /* bits in fraction_field                        */
#define REAL_EXP_BIAS 127 /* exponent = unsigned_exp_field - REAL_EXP_BIAS */


#if BPLEAN==0 /* no next-smaller REAL? */
   #undef LEAN_DIGITS
   #undef LEAN_EXP_BITS
   #undef LEAN_FRC_BITS
   #undef LEAN_EXP_BIAS
#else
   #error "LEAN_* not yet defined for this size LEAN"
#endif

#if 0
/*---------------------------------------------------------------------------*/
/* Absolute extremes of floating point range.                                */
/* f need not have full MANTISSA digits, but do not                          */
/* include consideration of NAN's, ZEROs, infinities.                        */
/* Express to *MORE* places than REAL_DIGITS.                                */
/*---------------------------------------------------------------------------*/
#endif
#define REAL_MOST_POS  CNST( 3.402823466E+38)
#define REAL_MOST_NEG  CNST(-3.402823466E+38)

#ifdef Support_DENORM
   #define REAL_LEAST_POS CNST( 1.401298464E-45)  
   #define REAL_LEAST_NEG CNST(-1.401298464E-45)  
   #define LEAST_POS_INVERTABLE_NOT_LEAST_POS 1
#else
   #define REAL_LEAST_POS CNST( 1.175494351e-38)
   #define REAL_LEAST_NEG CNST(-1.175494351e-38)
   #define LEAST_POS_INVERTABLE_NOT_LEAST_POS 0
#endif

#define REAL_LEAST_POS_INVERTABLE CNST(1.1754943508e-38)
/* usually (-EMIN<EMAX) ? RADIX**(EMIN-1) : 1/MOST_POS */

#if 0
/*---------------------------------------------------------------------------*/
/* Constants derived from RADIX & MANTISSA needed for math.h routines.       */
/* Express to *MORE* places than REAL_DIGITS.                                */
/*---------------------------------------------------------------------------*/
#endif

#define RADIX                    CNST( 2.0) /* FLTIF(REAL_RADIX   )*/
#define MANTISSA                 CNST(24.0) /* FLTIF(REAL_MANTISSA)*/

#define Ln_RADIX                 Ln_2   /* ln(RADIX)  */
#define Sqrt_RADIX               Sqrt_2 /* sqrt(RADIX)*/

#define RADIX_to(x)              SCLF(ONE,x) /* RADIX**x, x an integer       */

#define RADIX_to_Hlf_MANTISSA_M2 CNST(1024.0)     /*    RADIX**((MANTISSA/2)-2)*/
#define RADIX_to_Hlf_MANTISSA_M1 CNST(2048.0) 	  /*    RADIX**((MANTISSA/2)-1)*/
#define RADIX_to_Hlf_MANTISSA    CNST(4096.0) 	  /*    RADIX**( MANTISSA/2)   */
#define RADIX_to_MANTISSA        CNST(16777216.0) /* RADIX**( MANTISSA     )   */

#define One_Over_RADIX          CNST(0.5)               /* RADIX**(-1         )*/
#define RADIX_to_M_Hlf_MANTISSA CNST(2.44140625e-4)	/* RADIX**(-MANTISSA/2) */
#define RADIX_to_One_M_MANTISSA CNST(1.1920928955E-7)	/* RADIX**(1-MANTISSA ) */
#define RADIX_to_M_MANTISSA     CNST(5.9604644753E-8)	/* RADIX**( -MANTISSA ) */
#define RADIX_to_EMIN           CNST(2.35098870164E-38)	/* RADIX**(EMIN       ) */

#ifndef BPint
   #error "target props .h file must be included before REAL format .h file"
#endif

#define RND_GLITCH ((REAL_MANTISSA+1) <= BPint)
#define RND_P_GLITCH CNST( 16777216.0) /*\_RNDF glitch points. see fltsyntx.h */
#define RND_M_GLITCH CNST(-16777215.0) /*/ if no round-to-even dont need these*/

#define RSQRT_Sz_Newtons 3 /* (int)FLOOR(LOG2((MANTISSA-1)/5))+1*/
#define SQRT_Sz_Newtons  2 /* (int)FLOOR(LOG2((MANTISSA-1)/5))  */
#define SQRT_CW_Newtons  3 /* (int)CEIL (LOG2( MANTISSA     ))-2*/

#define EXP_Max CNST( 88.72283382) /* ln(MOST_POS   )*(1-RADIX**-MANTISSA)   */
#define EXP_Min CNST(-86.64339241) /* ln(RADIX**EMIN)*(1-RADIX**-MANTISSA)   */

#define POW_Ovfl CNST( 1418.0) /* FLOOR(16*ln(MOST_POS   )-1)*/
#define POW_Unfl CNST(-1386.0) /* FLOOR(16*ln(RADIX**EMIN)+1)*/

#define TANH_Max CNST(9.01091334725) /* (Ln_2+(MANTISSA+1)*Ln_RADIX)/2        */

#endif
