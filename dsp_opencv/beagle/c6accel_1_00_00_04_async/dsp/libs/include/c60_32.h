/****************************************************************************/
/*  c60_32.h         v4.20                                                  */
/*  Copyright (c) 1997-2001  Texas Instruments Incorporated                 */
/****************************************************************************/
 
/* This source automatically generated on Wed Aug  2 15:54:33 CDT 2000 */
 
#ifndef __c60_32__
#define __c60_32__
#if 0
/*---------------------------------------------------------------------------*/
/* C60-specific 32-bit REAL (and other) behavior descriptions.               */
/*---------------------------------------------------------------------------*/
#endif

#ifndef _TMS320C6x
  #define _TMS320C6x 1
#endif

#define Adds_Per_Add  1  /* ratio of REAL add  to add */
#define Adds_Per_Mult 10 /* ratio of REAL mult to add */
#define Adds_Per_Div  50 /* ratio of REAL div  to add */

#define FLT_FIX_Faster_Than_MODF 0 /* (double)((int)(x)) faster than MODFF */

#define Floating_Sub_Has_Guard_Digit 0 /* safe value is 0 */

#undef Support_DENORM     /* do/not support denormalized numbers            */
#undef Support_INFNAN	  /* do/not support INFINITY's and NAN's            */
#undef Support_SATURATION /* do/not saturate overflows to MOST_POS,MOST_NEG */

#define INT_MOST_POS_P1 CNST( 2147483648.0) /* most pos int +1 as REAL */
#define INT_MOST_NEG    CNST(-2147483648.0) /* most neg int    as REAL */

#define BPAU 8    /* bits per Addressable Unit                  */
#define BPchar 8  /* bits per char                              */
#define BPshrt 16 /* bits per short                             */
#define BPint 32  /* bits per int                               */
#define BPlong 40 /* bits per long (0 if not supported)         */
#define BPREAL 32 /* bits per REAL when in MEMORY               */
#define BPLEAN 0  /* bits per next smallest REAL when in MEMORY */

#ifndef _LITTLE_ENDIAN
   #ifdef _BIG_ENDIAN
      #define _LITTLE_ENDIAN !(_BIG_ENDIAN)
   #endif
#endif

#define MSB_In_Hi_Addr _LITTLE_ENDIAN /* ENDIANness. 0=big, 1=little */

#define IEEE_Hardware _TMS320C6700 /* 1 if IEEE hardware present */

#if 0
/*---------------------------------------------------------------------------*/
/* Pick the integer size on top of which REAL is to be implemented.          */
/*---------------------------------------------------------------------------*/
#endif

#if ((1) || (BPint==BPREAL)) /* for c60, always pick int */
   #define BPbigint BPint
   #define bigint int
   #define biguns unsigned
#elif (BPint >= BPlong)
   #define BPbigint BPint
   #define bigint int
   #define biguns unsigned
#elif (BPlong==BPREAL)
   #define BPbigint BPlong
   #define bigint long
   #define biguns unsigned long
#else
   #define BPbigint BPlong
   #define bigint long
   #define biguns unsigned long
#endif

biguns _lmbd (biguns, biguns);
#define COUNT_LZERO(nnn) _lmbd(1, nnn)

#define Pointers_Might_Not_Work 1

#endif

