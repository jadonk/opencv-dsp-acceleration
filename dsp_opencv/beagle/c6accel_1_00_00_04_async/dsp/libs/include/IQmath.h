//###########################################################################
//
// FILE:    IQmath.h
//
// TITLE:   IQ Math library functions definitions.
//
// 
// User needs to configure "MATH_TYPE" and "GLOBAL_Q" values:
//
//---------------------------------------------------------------------------
// Select math type, IQ_MATH or FLOAT_MATH:
//
#ifndef __IQMATH_H_INCLUDED__
#define __IQMATH_H_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef _VC_HOST_BUILD
# include "C6xSimulator.h"
#endif /* _VC_HOST_BUILD */

#include "typedef.h"

#define   FLOAT_MATH (1)
#define   IQ_MATH    (0)

#ifndef   MATH_TYPE
#define   MATH_TYPE  (IQ_MATH)
#endif

//---------------------------------------------------------------------------
// Select global Q value and scaling.
//
#ifndef   GLOBAL_Q
#define   GLOBAL_Q       (24)
#endif

#if (GLOBAL_Q < 0) || (GLOBAL_Q > 31)
#error "GLOBAL_Q : valid values are 0 to 31 (both inclusive)"
#endif

//---------------------------------------------------------------------------
// Various Usefull Constant Definitions:
//
#define   QG          (GLOBAL_Q)
#define   Q31         (31)
#define   Q30         (30)
#define   Q29         (29)
#define   Q28         (28)
#define   Q27         (27)
#define   Q26         (26)
#define   Q25         (25)
#define   Q24         (24)
#define   Q23         (23)
#define   Q22         (22)
#define   Q21         (21)
#define   Q20         (20)
#define   Q19         (19)
#define   Q18         (18)
#define   Q17         (17)
#define   Q16         (16)
#define   Q15         (15)
#define   Q14         (14)
#define   Q13         (13)
#define   Q12         (12)
#define   Q11         (11)
#define   Q10         (10)
#define   Q9          (9)
#define   Q8          (8)
#define   Q7          (7)
#define   Q6          (6)
#define   Q5          (5)
#define   Q4          (4)
#define   Q3          (3)
#define   Q2          (2)
#define   Q1          (1)
#define   Q0          (0)

#define   MAX_IQ_POS  (2147483647)
#define   MAX_IQ_NEG  (-2147483648)
#define   MIN_IQ_POS  (1)
#define   MIN_IQ_NEG  (-1)

//###########################################################################
#if MATH_TYPE == IQ_MATH
//###########################################################################
// If IQ_MATH is used, the following IQmath library function definitions
// are used:
//===========================================================================
typedef   I32_IQ    _iq;
typedef   I32_IQ    _iq31;
typedef   I32_IQ    _iq30;
typedef   I32_IQ    _iq29;
typedef   I32_IQ    _iq28;
typedef   I32_IQ    _iq27;
typedef   I32_IQ    _iq26;
typedef   I32_IQ    _iq25;
typedef   I32_IQ    _iq24;
typedef   I32_IQ    _iq23;
typedef   I32_IQ    _iq22;
typedef   I32_IQ    _iq21;
typedef   I32_IQ    _iq20;
typedef   I32_IQ    _iq19;
typedef   I32_IQ    _iq18;
typedef   I32_IQ    _iq17;
typedef   I32_IQ    _iq16;
typedef   I32_IQ    _iq15;
typedef   I32_IQ    _iq14;
typedef   I32_IQ    _iq13;
typedef   I32_IQ    _iq12;
typedef   I32_IQ    _iq11;
typedef   I32_IQ    _iq10;
typedef   I32_IQ    _iq9;
typedef   I32_IQ    _iq8;
typedef   I32_IQ    _iq7;
typedef   I32_IQ    _iq6;
typedef   I32_IQ    _iq5;
typedef   I32_IQ    _iq4;
typedef   I32_IQ    _iq3;
typedef   I32_IQ    _iq2;
typedef   I32_IQ    _iq1;
typedef   I32_IQ    _iq0;

//===========================================================================
// Tables used:
//===========================================================================
extern const I32_IQ IQsinTable[];
extern const I32_IQ IQdivTable[];
extern const I32_IQ IQisqrtTable[];
extern const I32_IQ IQatan2HalfPITable[];
extern const I32_IQ IQatan2Table[];
extern const I32_IQ IQexpTableMinMax[];
extern const I32_IQ IQexpTableCoeff[];
extern const I32_IQ IQlog_TableCoeff[];
extern const I32_IQ IQlogTableCoeff[];
extern const I32_IQ IQasinTable[];
extern const I32_IQ IQsininverseMinMax[];

//---------------------------------------------------------------------------


#define   _IQ31(A)      ((I32_IQ) ((A) * 2147483648u))
#define   _IQ30(A)      ((I32_IQ) ((A) * 1073741824u))
#define   _IQ29(A)      ((I32_IQ) ((A) * 536870912u))
#define   _IQ28(A)      ((I32_IQ) ((A) * 268435456u))
#define   _IQ27(A)      ((I32_IQ) ((A) * 134217728u))
#define   _IQ26(A)      ((I32_IQ) ((A) * 67108864u))
#define   _IQ25(A)      ((I32_IQ) ((A) * 33554432u))
#define   _IQ24(A)      ((I32_IQ) ((A) * 16777216u))
#define   _IQ23(A)      ((I32_IQ) ((A) * 8388608u))
#define   _IQ22(A)      ((I32_IQ) ((A) * 4194304u))
#define   _IQ21(A)      ((I32_IQ) ((A) * 2097152u))
#define   _IQ20(A)      ((I32_IQ) ((A) * 1048576u))
#define   _IQ19(A)      ((I32_IQ) ((A) * 524288u))
#define   _IQ18(A)      ((I32_IQ) ((A) * 262144u))
#define   _IQ17(A)      ((I32_IQ) ((A) * 131072u))
#define   _IQ16(A)      ((I32_IQ) ((A) * 65536u))
#define   _IQ15(A)      ((I32_IQ) ((A) * 32768u))
#define   _IQ14(A)      ((I32_IQ) ((A) * 16384u))
#define   _IQ13(A)      ((I32_IQ) ((A) * 8192u))
#define   _IQ12(A)      ((I32_IQ) ((A) * 4096u))
#define   _IQ11(A)      ((I32_IQ) ((A) * 2048u))
#define   _IQ10(A)      ((I32_IQ) ((A) * 1024u))
#define   _IQ9(A)       ((I32_IQ) ((A) * 512u))
#define   _IQ8(A)       ((I32_IQ) ((A) * 256u))
#define   _IQ7(A)       ((I32_IQ) ((A) * 128u))
#define   _IQ6(A)       ((I32_IQ) ((A) * 64u))
#define   _IQ5(A)       ((I32_IQ) ((A) * 32u))
#define   _IQ4(A)       ((I32_IQ) ((A) * 16u))
#define   _IQ3(A)       ((I32_IQ) ((A) * 8u))
#define   _IQ2(A)       ((I32_IQ) ((A) * 4u))
#define   _IQ1(A)       ((I32_IQ) ((A) * 2u))
#define   _IQ0(A)       ((I32_IQ) ((A) * 1u))


#if GLOBAL_Q == 31
#define   _IQ(A)  (_IQ31(A))
#endif
#if GLOBAL_Q == 30
#define   _IQ(A)  (_IQ30(A))
#endif
#if GLOBAL_Q == 29
#define   _IQ(A)  (_IQ29(A))
#endif
#if GLOBAL_Q == 28
#define   _IQ(A)  (_IQ28(A))
#endif
#if GLOBAL_Q == 27
#define   _IQ(A)  (_IQ27(A))
#endif
#if GLOBAL_Q == 26
#define   _IQ(A)  (_IQ26(A))
#endif
#if GLOBAL_Q == 25
#define   _IQ(A)  (_IQ25(A))
#endif
#if GLOBAL_Q == 24
#define   _IQ(A)  (_IQ24(A))
#endif
#if GLOBAL_Q == 23
#define   _IQ(A)  (_IQ23(A))
#endif
#if GLOBAL_Q == 22
#define   _IQ(A)  (_IQ22(A))
#endif
#if GLOBAL_Q == 21
#define   _IQ(A)  (_IQ21(A))
#endif
#if GLOBAL_Q == 20
#define   _IQ(A)  (_IQ20(A))
#endif
#if GLOBAL_Q == 19
#define   _IQ(A)  (_IQ19(A))
#endif
#if GLOBAL_Q == 18
#define   _IQ(A)  (_IQ18(A))
#endif
#if GLOBAL_Q == 17
#define   _IQ(A)  (_IQ17(A))
#endif
#if GLOBAL_Q == 16
#define   _IQ(A)  (_IQ16(A))
#endif
#if GLOBAL_Q == 15
#define   _IQ(A)  (_IQ15(A))
#endif
#if GLOBAL_Q == 14
#define   _IQ(A)  (_IQ14(A))
#endif
#if GLOBAL_Q == 13
#define   _IQ(A)  (_IQ13(A))
#endif
#if GLOBAL_Q == 12
#define   _IQ(A)  (_IQ12(A))
#endif
#if GLOBAL_Q == 11
#define   _IQ(A)  (_IQ11(A))
#endif
#if GLOBAL_Q == 10
#define   _IQ(A)  (_IQ10(A))
#endif
#if GLOBAL_Q == 9
#define   _IQ(A)  (_IQ9(A))
#endif
#if GLOBAL_Q == 8
#define   _IQ(A)  (_IQ8(A))
#endif
#if GLOBAL_Q == 7
#define   _IQ(A)  (_IQ7(A))
#endif
#if GLOBAL_Q == 6
#define   _IQ(A)  (_IQ6(A))
#endif
#if GLOBAL_Q == 5
#define   _IQ(A)  (_IQ5(A))
#endif
#if GLOBAL_Q == 4
#define   _IQ(A)  (_IQ4(A))
#endif
#if GLOBAL_Q == 3
#define   _IQ(A)  (_IQ3(A))
#endif
#if GLOBAL_Q == 2
#define   _IQ(A)  (_IQ2(A))
#endif
#if GLOBAL_Q == 1
#define   _IQ(A)  (_IQ1(A))
#endif
#if GLOBAL_Q == 0
#define   _IQ(A)  (_IQ0(A))
#endif
//========================================================================

#ifdef _INLINE_IQMATH
static inline I32_IQ _FtoIQN(float input, U32_IQ qfmt);
#else
I32_IQ _FtoIQN(float input, U32_IQ qfmt);
#endif
#define _FtoIQ31(A)     (_FtoIQN((A),31))
#define _FtoIQ30(A)     (_FtoIQN((A),30))
#define _FtoIQ29(A)     (_FtoIQN((A),29))
#define _FtoIQ28(A)     (_FtoIQN((A),28))
#define _FtoIQ27(A)     (_FtoIQN((A),27))
#define _FtoIQ26(A)     (_FtoIQN((A),26))
#define _FtoIQ25(A)     (_FtoIQN((A),25))
#define _FtoIQ24(A)     (_FtoIQN((A),24))
#define _FtoIQ23(A)     (_FtoIQN((A),23))
#define _FtoIQ22(A)     (_FtoIQN((A),22))
#define _FtoIQ21(A)     (_FtoIQN((A),21))
#define _FtoIQ20(A)     (_FtoIQN((A),20))
#define _FtoIQ19(A)     (_FtoIQN((A),19))
#define _FtoIQ18(A)     (_FtoIQN((A),18))
#define _FtoIQ17(A)     (_FtoIQN((A),17))
#define _FtoIQ16(A)     (_FtoIQN((A),16))
#define _FtoIQ15(A)     (_FtoIQN((A),15))
#define _FtoIQ14(A)     (_FtoIQN((A),14))
#define _FtoIQ13(A)     (_FtoIQN((A),13))
#define _FtoIQ12(A)     (_FtoIQN((A),12))
#define _FtoIQ11(A)     (_FtoIQN((A),11))
#define _FtoIQ10(A)     (_FtoIQN((A),10))
#define _FtoIQ9(A)      (_FtoIQN((A),9))
#define _FtoIQ8(A)      (_FtoIQN((A),8))
#define _FtoIQ7(A)      (_FtoIQN((A),7))
#define _FtoIQ6(A)      (_FtoIQN((A),6))
#define _FtoIQ5(A)      (_FtoIQN((A),5))
#define _FtoIQ4(A)      (_FtoIQN((A),4))
#define _FtoIQ3(A)      (_FtoIQN((A),3))
#define _FtoIQ2(A)      (_FtoIQN((A),2))
#define _FtoIQ1(A)      (_FtoIQN((A),1))
#define _FtoIQ0(A)      (_FtoIQN((A),0))


#if GLOBAL_Q == 31
#define   _FtoIQ(A)  (_FtoIQ31(A))
#endif
#if GLOBAL_Q == 30
#define   _FtoIQ(A)  (_FtoIQ30(A))
#endif
#if GLOBAL_Q == 29
#define   _FtoIQ(A)  (_FtoIQ29(A))
#endif
#if GLOBAL_Q == 28
#define   _FtoIQ(A)  (_FtoIQ28(A))
#endif
#if GLOBAL_Q == 27
#define   _FtoIQ(A)  (_FtoIQ27(A))
#endif
#if GLOBAL_Q == 26
#define   _FtoIQ(A)  (_FtoIQ26(A))
#endif
#if GLOBAL_Q == 25
#define   _FtoIQ(A)  (_FtoIQ25(A))
#endif
#if GLOBAL_Q == 24
#define   _FtoIQ(A)  (_FtoIQ24(A))
#endif
#if GLOBAL_Q == 23
#define   _FtoIQ(A)  (_FtoIQ23(A))
#endif
#if GLOBAL_Q == 22
#define   _FtoIQ(A)  (_FtoIQ22(A))
#endif
#if GLOBAL_Q == 21
#define   _FtoIQ(A)  (_FtoIQ21(A))
#endif
#if GLOBAL_Q == 20
#define   _FtoIQ(A)  (_FtoIQ20(A))
#endif
#if GLOBAL_Q == 19
#define   _FtoIQ(A)  (_FtoIQ19(A))
#endif
#if GLOBAL_Q == 18
#define   _FtoIQ(A)  (_FtoIQ18(A))
#endif
#if GLOBAL_Q == 17
#define   _FtoIQ(A)  (_FtoIQ17(A))
#endif
#if GLOBAL_Q == 16
#define   _FtoIQ(A)  (_FtoIQ16(A))
#endif
#if GLOBAL_Q == 15
#define   _FtoIQ(A)  (_FtoIQ15(A))
#endif
#if GLOBAL_Q == 14
#define   _FtoIQ(A)  (_FtoIQ14(A))
#endif
#if GLOBAL_Q == 13
#define   _FtoIQ(A)  (_FtoIQ13(A))
#endif
#if GLOBAL_Q == 12
#define   _FtoIQ(A)  (_FtoIQ12(A))
#endif
#if GLOBAL_Q == 11
#define   _FtoIQ(A)  (_FtoIQ11(A))
#endif
#if GLOBAL_Q == 10
#define   _FtoIQ(A)  (_FtoIQ10(A))
#endif
#if GLOBAL_Q == 9
#define   _FtoIQ(A)  (_FtoIQ9(A))
#endif
#if GLOBAL_Q == 8
#define   _FtoIQ(A)  (_FtoIQ8(A))
#endif
#if GLOBAL_Q == 7
#define   _FtoIQ(A)  (_FtoIQ7(A))
#endif
#if GLOBAL_Q == 6
#define   _FtoIQ(A)  (_FtoIQ6(A))
#endif
#if GLOBAL_Q == 5
#define   _FtoIQ(A)  (_FtoIQ5(A))
#endif
#if GLOBAL_Q == 4
#define   _FtoIQ(A)  (_FtoIQ4(A))
#endif
#if GLOBAL_Q == 3
#define   _FtoIQ(A)  (_FtoIQ3(A))
#endif
#if GLOBAL_Q == 2
#define   _FtoIQ(A)  (_FtoIQ2(A))
#endif
#if GLOBAL_Q == 1
#define   _FtoIQ(A)  (_FtoIQ1(A))
#endif
#if GLOBAL_Q == 0
#define   _FtoIQ(A)  (_FtoIQ0(A))
#endif


//========================================================================
//---------------------------------------------------------------------------
#ifdef _INLINE_IQMATH
static inline float _IQNtoF(I32_IQ input, U32_IQ qfmt);
#else
float _IQNtoF(I32_IQ input, U32_IQ qfmt);
#endif
#define _IQ31toF(A)     (_IQNtoF((A),31))
#define _IQ30toF(A)     (_IQNtoF((A),30))
#define _IQ29toF(A)     (_IQNtoF((A),29))
#define _IQ28toF(A)     (_IQNtoF((A),28))
#define _IQ27toF(A)     (_IQNtoF((A),27))
#define _IQ26toF(A)     (_IQNtoF((A),26))
#define _IQ25toF(A)     (_IQNtoF((A),25))
#define _IQ24toF(A)     (_IQNtoF((A),24))
#define _IQ23toF(A)     (_IQNtoF((A),23))
#define _IQ22toF(A)     (_IQNtoF((A),22))
#define _IQ21toF(A)     (_IQNtoF((A),21))
#define _IQ20toF(A)     (_IQNtoF((A),20))
#define _IQ19toF(A)     (_IQNtoF((A),19))
#define _IQ18toF(A)     (_IQNtoF((A),18))
#define _IQ17toF(A)     (_IQNtoF((A),17))
#define _IQ16toF(A)     (_IQNtoF((A),16))
#define _IQ15toF(A)     (_IQNtoF((A),15))
#define _IQ14toF(A)     (_IQNtoF((A),14))
#define _IQ13toF(A)     (_IQNtoF((A),13))
#define _IQ12toF(A)     (_IQNtoF((A),12))
#define _IQ11toF(A)     (_IQNtoF((A),11))
#define _IQ10toF(A)     (_IQNtoF((A),10))
#define _IQ9toF(A)      (_IQNtoF((A),9))
#define _IQ8toF(A)      (_IQNtoF((A),8))
#define _IQ7toF(A)      (_IQNtoF((A),7))
#define _IQ6toF(A)      (_IQNtoF((A),6))
#define _IQ5toF(A)      (_IQNtoF((A),5))
#define _IQ4toF(A)      (_IQNtoF((A),4))
#define _IQ3toF(A)      (_IQNtoF((A),3))
#define _IQ2toF(A)      (_IQNtoF((A),2))
#define _IQ1toF(A)      (_IQNtoF((A),1))
#define _IQ0toF(A)      (_IQNtoF((A),0))

#if GLOBAL_Q == 31
#define   _IQtoF(A)  (_IQ31toF(A))
#endif
#if GLOBAL_Q == 30
#define   _IQtoF(A)  (_IQ30toF(A))
#endif
#if GLOBAL_Q == 29
#define   _IQtoF(A)  (_IQ29toF(A))
#endif
#if GLOBAL_Q == 28
#define   _IQtoF(A)  (_IQ28toF(A))
#endif
#if GLOBAL_Q == 27
#define   _IQtoF(A)  (_IQ27toF(A))
#endif
#if GLOBAL_Q == 26
#define   _IQtoF(A)  (_IQ26toF(A))
#endif
#if GLOBAL_Q == 25
#define   _IQtoF(A)  (_IQ25toF(A))
#endif
#if GLOBAL_Q == 24
#define   _IQtoF(A)  (_IQ24toF(A))
#endif
#if GLOBAL_Q == 23
#define   _IQtoF(A)  (_IQ23toF(A))
#endif
#if GLOBAL_Q == 22
#define   _IQtoF(A)  (_IQ22toF(A))
#endif
#if GLOBAL_Q == 21
#define   _IQtoF(A)  (_IQ21toF(A))
#endif
#if GLOBAL_Q == 20
#define   _IQtoF(A)  (_IQ20toF(A))
#endif
#if GLOBAL_Q == 19
#define   _IQtoF(A)  (_IQ19toF(A))
#endif
#if GLOBAL_Q == 18
#define   _IQtoF(A)  (_IQ18toF(A))
#endif
#if GLOBAL_Q == 17
#define   _IQtoF(A)  (_IQ17toF(A))
#endif
#if GLOBAL_Q == 16
#define   _IQtoF(A)  (_IQ16toF(A))
#endif
#if GLOBAL_Q == 15
#define   _IQtoF(A)  (_IQ15toF(A))
#endif
#if GLOBAL_Q == 14
#define   _IQtoF(A)  (_IQ14toF(A))
#endif
#if GLOBAL_Q == 13
#define   _IQtoF(A)  (_IQ13toF(A))
#endif
#if GLOBAL_Q == 12
#define   _IQtoF(A)  (_IQ12toF(A))
#endif
#if GLOBAL_Q == 11
#define   _IQtoF(A)  (_IQ11toF(A))
#endif
#if GLOBAL_Q == 10
#define   _IQtoF(A)  (_IQ10toF(A))
#endif
#if GLOBAL_Q == 9
#define   _IQtoF(A)  (_IQ9toF(A))
#endif
#if GLOBAL_Q == 8
#define   _IQtoF(A)  (_IQ8toF(A))
#endif
#if GLOBAL_Q == 7
#define   _IQtoF(A)  (_IQ7toF(A))
#endif
#if GLOBAL_Q == 6
#define   _IQtoF(A)  (_IQ6toF(A))
#endif
#if GLOBAL_Q == 5
#define   _IQtoF(A)  (_IQ5toF(A))
#endif
#if GLOBAL_Q == 4
#define   _IQtoF(A)  (_IQ4toF(A))
#endif
#if GLOBAL_Q == 3
#define   _IQtoF(A)  (_IQ3toF(A))
#endif
#if GLOBAL_Q == 2
#define   _IQtoF(A)  (_IQ2toF(A))
#endif
#if GLOBAL_Q == 1
#define   _IQtoF(A)  (_IQ1toF(A))
#endif
#if GLOBAL_Q == 0
#define   _IQtoF(A)  (_IQ0toF(A))
#endif
//---------------------------------------------------------------------------
//I32_IQ _IQsat(I32_IQ A, I32_IQ Pos, I32_IQ Neg);
#ifdef _INLINE_IQMATH
static inline I32_IQ _IQNsat(I32_IQ A, U32_IQ qfmt);
#else
I32_IQ _IQNsat(I32_IQ A, U32_IQ qfmt);
#endif
#if GLOBAL_Q == 30
#define   _IQsat(A)  (_IQ30sat(A))
#endif
#if GLOBAL_Q == 29
#define   _IQsat(A)  (_IQ29sat(A))
#endif
#if GLOBAL_Q == 28
#define   _IQsat(A)  (_IQ28sat(A))
#endif
#if GLOBAL_Q == 27
#define   _IQsat(A)  (_IQ27sat(A))
#endif
#if GLOBAL_Q == 26
#define   _IQsat(A)  (_IQ26sat(A))
#endif
#if GLOBAL_Q == 25
#define   _IQsat(A)  (_IQ25sat(A))
#endif
#if GLOBAL_Q == 24
#define   _IQsat(A)  (_IQ24sat(A))
#endif
#if GLOBAL_Q == 23
#define   _IQsat(A)  (_IQ23sat(A))
#endif
#if GLOBAL_Q == 22
#define   _IQsat(A)  (_IQ22sat(A))
#endif
#if GLOBAL_Q == 21
#define   _IQsat(A)  (_IQ21sat(A))
#endif
#if GLOBAL_Q == 20
#define   _IQsat(A)  (_IQ20sat(A))
#endif
#if GLOBAL_Q == 19
#define   _IQsat(A)  (_IQ19sat(A))
#endif
#if GLOBAL_Q == 18
#define   _IQsat(A)  (_IQ18sat(A))
#endif
#if GLOBAL_Q == 17
#define   _IQsat(A)  (_IQ17sat(A))
#endif
#if GLOBAL_Q == 16
#define   _IQsat(A)  (_IQ16sat(A))
#endif
#if GLOBAL_Q == 15
#define   _IQsat(A)  (_IQ15sat(A))
#endif
#if GLOBAL_Q == 14
#define   _IQsat(A)  (_IQ14sat(A))
#endif
#if GLOBAL_Q == 13
#define   _IQsat(A)  (_IQ13sat(A))
#endif
#if GLOBAL_Q == 12
#define   _IQsat(A)  (_IQ12sat(A))
#endif
#if GLOBAL_Q == 11
#define   _IQsat(A)  (_IQ11sat(A))
#endif
#if GLOBAL_Q == 10
#define   _IQsat(A)  (_IQ10sat(A))
#endif
#if GLOBAL_Q == 9
#define   _IQsat(A)  (_IQ9sat(A))
#endif
#if GLOBAL_Q == 8
#define   _IQsat(A)  (_IQ8sat(A))
#endif
#if GLOBAL_Q == 7
#define   _IQsat(A)  (_IQ7sat(A))
#endif
#if GLOBAL_Q == 6
#define   _IQsat(A)  (_IQ6sat(A))
#endif
#if GLOBAL_Q == 5
#define   _IQsat(A)  (_IQ5sat(A))
#endif
#if GLOBAL_Q == 4
#define   _IQsat(A)  (_IQ4sat(A))
#endif
#if GLOBAL_Q == 3
#define   _IQsat(A)  (_IQ3sat(A))
#endif
#if GLOBAL_Q == 2
#define   _IQsat(A)  (_IQ2sat(A))
#endif
#if GLOBAL_Q == 1
#define   _IQsat(A)  (_IQ1sat(A))
#endif
#if GLOBAL_Q == 0
#define   _IQsat(A)  (_IQ0sat(A))
#endif
#if GLOBAL_Q == 31
#define   _IQsat(A)  (_IQ31sat(A))
#endif
//---------------------------------------------------------------------------
//extern  float _satf(float A, float Pos, float Neg);
//I32_IQ _IQNsat(I32_IQ A, I32_IQ Pos, I32_IQ Neg)


I32_IQ _IQNsat(I32_IQ A, U32_IQ qfmt);

#define _IQ31sat(A)    (_IQNsat((A),31))
#define _IQ30sat(A)    (_IQNsat((A),30))
#define _IQ29sat(A)    (_IQNsat((A),29))
#define _IQ28sat(A)    (_IQNsat((A),28))
#define _IQ27sat(A)    (_IQNsat((A),27))
#define _IQ26sat(A)    (_IQNsat((A),26))
#define _IQ25sat(A)    (_IQNsat((A),25))
#define _IQ24sat(A)    (_IQNsat((A),24))
#define _IQ23sat(A)    (_IQNsat((A),23))
#define _IQ22sat(A)    (_IQNsat((A),22))
#define _IQ21sat(A)    (_IQNsat((A),21))
#define _IQ20sat(A)    (_IQNsat((A),20))
#define _IQ19sat(A)    (_IQNsat((A),19))
#define _IQ18sat(A)    (_IQNsat((A),18))
#define _IQ17sat(A)    (_IQNsat((A),17))
#define _IQ16sat(A)    (_IQNsat((A),16))
#define _IQ15sat(A)    (_IQNsat((A),15))
#define _IQ14sat(A)    (_IQNsat((A),14))
#define _IQ13sat(A)    (_IQNsat((A),13))
#define _IQ12sat(A)    (_IQNsat((A),12))
#define _IQ11sat(A)    (_IQNsat((A),11))
#define _IQ10sat(A)    (_IQNsat((A),10))
#define _IQ9sat(A)     (_IQNsat((A),9))
#define _IQ8sat(A)     (_IQNsat((A),8))
#define _IQ7sat(A)     (_IQNsat((A),7))
#define _IQ6sat(A)     (_IQNsat((A),6))
#define _IQ5sat(A)     (_IQNsat((A),5))
#define _IQ4sat(A)     (_IQNsat((A),4))
#define _IQ3sat(A)     (_IQNsat((A),3))
#define _IQ2sat(A)     (_IQNsat((A),2))
#define _IQ1sat(A)     (_IQNsat((A),1))
#define _IQ0sat(A)     (_IQNsat((A),0))

//---------------------------------------------------------------------------
#define   _IQXtoIQY(A, X, Y) ( (X)<(Y) ? ( (A)<<((Y)-(X))) : (A>>((X)-(Y))) )

//---------------------------------------------------------------------------

#define   _IQtoIQ31(A)  ((I32_IQ) (A) << (31 - GLOBAL_Q))
#define   _IQ31toIQ(A)  ((I32_IQ) (A) >> (31 - GLOBAL_Q))

#define   _IQtoIQ30(A)  ((GLOBAL_Q >= 30) ? ((I32_IQ) (A) >> (GLOBAL_Q - 30)):((I32_IQ) (A) << (30 - GLOBAL_Q)))
#define   _IQ30toIQ(A)  ((GLOBAL_Q >= 30) ? ((I32_IQ) (A) << (GLOBAL_Q - 30)):((I32_IQ) (A) >> (30 - GLOBAL_Q)))

#define   _IQtoIQ29(A)  ((GLOBAL_Q >= 29) ? ((I32_IQ) (A) >> (GLOBAL_Q - 29)):((I32_IQ) (A) << (29 - GLOBAL_Q)))
#define   _IQ29toIQ(A)  ((GLOBAL_Q >= 29) ? ((I32_IQ) (A) << (GLOBAL_Q - 29)):((I32_IQ) (A) >> (29 - GLOBAL_Q)))

#define   _IQtoIQ28(A)  ((GLOBAL_Q >= 28) ? ((I32_IQ) (A) >> (GLOBAL_Q - 28)):((I32_IQ) (A) << (28 - GLOBAL_Q)))
#define   _IQ28toIQ(A)  ((GLOBAL_Q >= 28) ? ((I32_IQ) (A) << (GLOBAL_Q - 28)):((I32_IQ) (A) >> (28 - GLOBAL_Q)))

#define   _IQtoIQ27(A)  ((GLOBAL_Q >= 27) ? ((I32_IQ) (A) >> (GLOBAL_Q - 27)):((I32_IQ) (A) << (27 - GLOBAL_Q)))
#define   _IQ27toIQ(A)  ((GLOBAL_Q >= 27) ? ((I32_IQ) (A) << (GLOBAL_Q - 27)):((I32_IQ) (A) >> (27 - GLOBAL_Q)))

#define   _IQtoIQ26(A)  ((GLOBAL_Q >= 26) ? ((I32_IQ) (A) >> (GLOBAL_Q - 26)):((I32_IQ) (A) << (26 - GLOBAL_Q)))
#define   _IQ26toIQ(A)  ((GLOBAL_Q >= 26) ? ((I32_IQ) (A) << (GLOBAL_Q - 26)):((I32_IQ) (A) >> (26 - GLOBAL_Q)))

#define   _IQtoIQ25(A)  ((GLOBAL_Q >= 25) ? ((I32_IQ) (A) >> (GLOBAL_Q - 25)):((I32_IQ) (A) << (25 - GLOBAL_Q)))
#define   _IQ25toIQ(A)  ((GLOBAL_Q >= 25) ? ((I32_IQ) (A) << (GLOBAL_Q - 25)):((I32_IQ) (A) >> (25 - GLOBAL_Q)))

#define   _IQtoIQ24(A)  ((GLOBAL_Q >= 24) ? ((I32_IQ) (A) >> (GLOBAL_Q - 24)):((I32_IQ) (A) << (24 - GLOBAL_Q)))
#define   _IQ24toIQ(A)  ((GLOBAL_Q >= 24) ? ((I32_IQ) (A) << (GLOBAL_Q - 24)):((I32_IQ) (A) >> (24 - GLOBAL_Q)))

#define   _IQtoIQ23(A)  ((GLOBAL_Q >= 23) ? ((I32_IQ) (A) >> (GLOBAL_Q - 23)):((I32_IQ) (A) << (23 - GLOBAL_Q)))
#define   _IQ23toIQ(A)  ((GLOBAL_Q >= 23) ? ((I32_IQ) (A) << (GLOBAL_Q - 23)):((I32_IQ) (A) >> (23 - GLOBAL_Q)))

#define   _IQtoIQ22(A)  ((GLOBAL_Q >= 22) ? ((I32_IQ) (A) >> (GLOBAL_Q - 22)):((I32_IQ) (A) << (22 - GLOBAL_Q)))
#define   _IQ22toIQ(A)  ((GLOBAL_Q >= 22) ? ((I32_IQ) (A) << (GLOBAL_Q - 22)):((I32_IQ) (A) >> (22 - GLOBAL_Q)))

#define   _IQtoIQ21(A)  ((GLOBAL_Q >= 21) ? ((I32_IQ) (A) >> (GLOBAL_Q - 21)):((I32_IQ) (A) << (21 - GLOBAL_Q)))
#define   _IQ21toIQ(A)  ((GLOBAL_Q >= 21) ? ((I32_IQ) (A) << (GLOBAL_Q - 21)):((I32_IQ) (A) >> (21 - GLOBAL_Q)))

#define   _IQtoIQ20(A)  ((GLOBAL_Q >= 20) ? ((I32_IQ) (A) >> (GLOBAL_Q - 20)):((I32_IQ) (A) << (20 - GLOBAL_Q)))
#define   _IQ20toIQ(A)  ((GLOBAL_Q >= 20) ? ((I32_IQ) (A) << (GLOBAL_Q - 20)):((I32_IQ) (A) >> (20 - GLOBAL_Q)))

#define   _IQtoIQ19(A)  ((GLOBAL_Q >= 19) ? ((I32_IQ) (A) >> (GLOBAL_Q - 19)):((I32_IQ) (A) << (19 - GLOBAL_Q)))
#define   _IQ19toIQ(A)  ((GLOBAL_Q >= 19) ? ((I32_IQ) (A) << (GLOBAL_Q - 19)):((I32_IQ) (A) >> (19 - GLOBAL_Q)))

#define   _IQtoIQ18(A)  ((GLOBAL_Q >= 18) ? ((I32_IQ) (A) >> (GLOBAL_Q - 18)):((I32_IQ) (A) << (18 - GLOBAL_Q)))
#define   _IQ18toIQ(A)  ((GLOBAL_Q >= 18) ? ((I32_IQ) (A) << (GLOBAL_Q - 18)):((I32_IQ) (A) >> (18 - GLOBAL_Q)))

#define   _IQtoIQ17(A)  ((GLOBAL_Q >= 17) ? ((I32_IQ) (A) >> (GLOBAL_Q - 17)):((I32_IQ) (A) << (17 - GLOBAL_Q)))
#define   _IQ17toIQ(A)  ((GLOBAL_Q >= 17) ? ((I32_IQ) (A) << (GLOBAL_Q - 17)):((I32_IQ) (A) >> (17 - GLOBAL_Q)))

#define   _IQtoIQ16(A)  ((GLOBAL_Q >= 16) ? ((I32_IQ) (A) >> (GLOBAL_Q - 16)):((I32_IQ) (A) << (16 - GLOBAL_Q)))
#define   _IQ16toIQ(A)  ((GLOBAL_Q >= 16) ? ((I32_IQ) (A) << (GLOBAL_Q - 16)):((I32_IQ) (A) >> (16 - GLOBAL_Q)))

#define   _IQtoIQ15(A)  ((GLOBAL_Q >= 15) ? ((I32_IQ) (A) >> (GLOBAL_Q - 15)):((I32_IQ) (A) << (15 - GLOBAL_Q)))
#define   _IQ15toIQ(A)  ((GLOBAL_Q >= 15) ? ((I32_IQ) (A) << (GLOBAL_Q - 15)):((I32_IQ) (A) >> (15 - GLOBAL_Q)))

#define   _IQtoIQ14(A)  ((GLOBAL_Q >= 14) ? ((I32_IQ) (A) >> (GLOBAL_Q - 14)):((I32_IQ) (A) << (14 - GLOBAL_Q)))
#define   _IQ14toIQ(A)  ((GLOBAL_Q >= 14) ? ((I32_IQ) (A) << (GLOBAL_Q - 14)):((I32_IQ) (A) >> (14 - GLOBAL_Q)))

#define   _IQtoIQ13(A)  ((GLOBAL_Q >= 13) ? ((I32_IQ) (A) >> (GLOBAL_Q - 13)):((I32_IQ) (A) << (13 - GLOBAL_Q)))
#define   _IQ13toIQ(A)  ((GLOBAL_Q >= 13) ? ((I32_IQ) (A) << (GLOBAL_Q - 13)):((I32_IQ) (A) >> (13 - GLOBAL_Q)))

#define   _IQtoIQ12(A)  ((GLOBAL_Q >= 12) ? ((I32_IQ) (A) >> (GLOBAL_Q - 12)):((I32_IQ) (A) << (12 - GLOBAL_Q)))
#define   _IQ12toIQ(A)  ((GLOBAL_Q >= 12) ? ((I32_IQ) (A) << (GLOBAL_Q - 12)):((I32_IQ) (A) >> (12 - GLOBAL_Q)))

#define   _IQtoIQ11(A)  ((GLOBAL_Q >= 11) ? ((I32_IQ) (A) >> (GLOBAL_Q - 11)):((I32_IQ) (A) << (11 - GLOBAL_Q)))
#define   _IQ11toIQ(A)  ((GLOBAL_Q >= 11) ? ((I32_IQ) (A) << (GLOBAL_Q - 11)):((I32_IQ) (A) >> (11 - GLOBAL_Q)))

#define   _IQtoIQ10(A)  ((GLOBAL_Q >= 10) ? ((I32_IQ) (A) >> (GLOBAL_Q - 10)):((I32_IQ) (A) << (10 - GLOBAL_Q)))
#define   _IQ10toIQ(A)  ((GLOBAL_Q >= 10) ? ((I32_IQ) (A) << (GLOBAL_Q - 10)):((I32_IQ) (A) >> (10 - GLOBAL_Q)))

#define   _IQtoIQ9(A)   ((GLOBAL_Q >= 9) ? ((I32_IQ) (A) >> (GLOBAL_Q - 9)):((I32_IQ) (A) << (9 - GLOBAL_Q)))
#define   _IQ9toIQ(A)   ((GLOBAL_Q >= 9) ? ((I32_IQ) (A) << (GLOBAL_Q - 9)):((I32_IQ) (A) >> (9 - GLOBAL_Q)))

#define   _IQtoIQ8(A)   ((GLOBAL_Q >= 8) ? ((I32_IQ) (A) >> (GLOBAL_Q - 8)):((I32_IQ) (A) << (8 - GLOBAL_Q)))
#define   _IQ8toIQ(A)   ((GLOBAL_Q >= 8) ? ((I32_IQ) (A) << (GLOBAL_Q - 8)):((I32_IQ) (A) >> (8 - GLOBAL_Q)))

#define   _IQtoIQ7(A)   ((GLOBAL_Q >= 7) ? ((I32_IQ) (A) >> (GLOBAL_Q - 7)):((I32_IQ) (A) << (7 - GLOBAL_Q)))
#define   _IQ7toIQ(A)   ((GLOBAL_Q >= 7) ? ((I32_IQ) (A) << (GLOBAL_Q - 7)):((I32_IQ) (A) >> (7 - GLOBAL_Q)))

#define   _IQtoIQ6(A)   ((GLOBAL_Q >= 6) ? ((I32_IQ) (A) >> (GLOBAL_Q - 6)):((I32_IQ) (A) << (6 - GLOBAL_Q)))
#define   _IQ6toIQ(A)   ((GLOBAL_Q >= 6) ? ((I32_IQ) (A) << (GLOBAL_Q - 6)):((I32_IQ) (A) >> (6 - GLOBAL_Q)))

#define   _IQtoIQ5(A)   ((GLOBAL_Q >= 5) ? ((I32_IQ) (A) >> (GLOBAL_Q - 5)):((I32_IQ) (A) << (5 - GLOBAL_Q)))
#define   _IQ5toIQ(A)   ((GLOBAL_Q >= 5) ? ((I32_IQ) (A) << (GLOBAL_Q - 5)):((I32_IQ) (A) >> (5 - GLOBAL_Q)))

#define   _IQtoIQ4(A)   ((GLOBAL_Q >= 4) ? ((I32_IQ) (A) >> (GLOBAL_Q - 4)):((I32_IQ) (A) << (4 - GLOBAL_Q)))
#define   _IQ4toIQ(A)   ((GLOBAL_Q >= 4) ? ((I32_IQ) (A) << (GLOBAL_Q - 4)):((I32_IQ) (A) >> (4 - GLOBAL_Q)))

#define   _IQtoIQ3(A)   ((GLOBAL_Q >= 3) ? ((I32_IQ) (A) >> (GLOBAL_Q - 3)):((I32_IQ) (A) << (3 - GLOBAL_Q)))
#define   _IQ3toIQ(A)   ((GLOBAL_Q >= 3) ? ((I32_IQ) (A) << (GLOBAL_Q - 3)):((I32_IQ) (A) >> (3 - GLOBAL_Q)))

#define   _IQtoIQ2(A)   ((GLOBAL_Q >= 2) ? ((I32_IQ) (A) >> (GLOBAL_Q - 2)):((I32_IQ) (A) << (2 - GLOBAL_Q)))
#define   _IQ2toIQ(A)   ((GLOBAL_Q >= 2) ? ((I32_IQ) (A) << (GLOBAL_Q - 2)):((I32_IQ) (A) >> (2 - GLOBAL_Q)))

#define   _IQtoIQ1(A)    ((GLOBAL_Q >= 1) ? ((I32_IQ) (A) >> (GLOBAL_Q - 1)):((I32_IQ) (A) << (1 - GLOBAL_Q)))
#define   _IQ1toIQ(A)    ((GLOBAL_Q >= 1) ? ((I32_IQ) (A) << (GLOBAL_Q - 1)):((I32_IQ) (A) >> (1 - GLOBAL_Q)))

#define   _IQtoIQ0(A)   ((I32_IQ) (A) >> (GLOBAL_Q ))
#define   _IQ0toIQ(A)   ((I32_IQ) (A) << (GLOBAL_Q ))

//---------------------------------------------------------------------------

#ifdef _INLINE_IQMATH
static inline  I32_IQ _IQNmpy(I32_IQ f1, I32_IQ f2, U32_IQ q_format);
#else
I32_IQ _IQNmpy(I32_IQ f1, I32_IQ f2, U32_IQ q_format);
#endif
#define _IQ31mpy(A,B)   (_IQNmpy((A),(B),31))
#define _IQ30mpy(A,B)   (_IQNmpy((A),(B),30))
#define _IQ29mpy(A,B)   (_IQNmpy((A),(B),29))
#define _IQ28mpy(A,B)   (_IQNmpy((A),(B),28))
#define _IQ27mpy(A,B)   (_IQNmpy((A),(B),27))
#define _IQ26mpy(A,B)   (_IQNmpy((A),(B),26))
#define _IQ25mpy(A,B)   (_IQNmpy((A),(B),25))
#define _IQ24mpy(A,B)   (_IQNmpy((A),(B),24))
#define _IQ23mpy(A,B)   (_IQNmpy((A),(B),23))
#define _IQ22mpy(A,B)   (_IQNmpy((A),(B),22))
#define _IQ21mpy(A,B)   (_IQNmpy((A),(B),21))
#define _IQ20mpy(A,B)   (_IQNmpy((A),(B),20))
#define _IQ19mpy(A,B)   (_IQNmpy((A),(B),19))
#define _IQ18mpy(A,B)   (_IQNmpy((A),(B),18))
#define _IQ17mpy(A,B)   (_IQNmpy((A),(B),17))
#define _IQ16mpy(A,B)   (_IQNmpy((A),(B),16))
#define _IQ15mpy(A,B)   (_IQNmpy((A),(B),15))
#define _IQ14mpy(A,B)   (_IQNmpy((A),(B),14))
#define _IQ13mpy(A,B)   (_IQNmpy((A),(B),13))
#define _IQ12mpy(A,B)   (_IQNmpy((A),(B),12))
#define _IQ11mpy(A,B)   (_IQNmpy((A),(B),11))
#define _IQ10mpy(A,B)   (_IQNmpy((A),(B),10))
#define _IQ9mpy(A,B)    (_IQNmpy((A),(B),9))
#define _IQ8mpy(A,B)    (_IQNmpy((A),(B),8))
#define _IQ7mpy(A,B)    (_IQNmpy((A),(B),7))
#define _IQ6mpy(A,B)    (_IQNmpy((A),(B),6))
#define _IQ5mpy(A,B)    (_IQNmpy((A),(B),5))
#define _IQ4mpy(A,B)    (_IQNmpy((A),(B),4))
#define _IQ3mpy(A,B)    (_IQNmpy((A),(B),3))
#define _IQ2mpy(A,B)    (_IQNmpy((A),(B),2))
#define _IQ1mpy(A,B)    (_IQNmpy((A),(B),1))
#define _IQ0mpy(A,B)    (_IQNmpy((A),(B),0))

#if GLOBAL_Q == 31
#define   _IQmpy(A,B)  (_IQ31mpy((A),(B)))
#endif
#if GLOBAL_Q == 30
#define   _IQmpy(A,B)  (_IQ30mpy((A),(B)))
#endif
#if GLOBAL_Q == 29
#define   _IQmpy(A,B)  (_IQ29mpy((A),(B)))
#endif
#if GLOBAL_Q == 28
#define   _IQmpy(A,B)  (_IQ28mpy((A),(B)))
#endif
#if GLOBAL_Q == 27
#define   _IQmpy(A,B)  (_IQ27mpy((A),(B)))
#endif
#if GLOBAL_Q == 26
#define   _IQmpy(A,B)  (_IQ26mpy((A),(B)))
#endif
#if GLOBAL_Q == 25
#define   _IQmpy(A,B)  (_IQ25mpy((A),(B)))
#endif
#if GLOBAL_Q == 24
#define   _IQmpy(A,B)  (_IQ24mpy((A),(B)))
#endif
#if GLOBAL_Q == 23
#define   _IQmpy(A,B)  (_IQ23mpy((A),(B)))
#endif
#if GLOBAL_Q == 22
#define   _IQmpy(A,B)  (_IQ22mpy((A),(B)))
#endif
#if GLOBAL_Q == 21
#define   _IQmpy(A,B)  (_IQ21mpy((A),(B)))
#endif
#if GLOBAL_Q == 20
#define   _IQmpy(A,B)  (_IQ20mpy((A),(B)))
#endif
#if GLOBAL_Q == 19
#define   _IQmpy(A,B)  (_IQ19mpy((A),(B)))
#endif
#if GLOBAL_Q == 18
#define   _IQmpy(A,B)  (_IQ18mpy((A),(B)))
#endif
#if GLOBAL_Q == 17
#define   _IQmpy(A,B)  (_IQ17mpy((A),(B)))
#endif
#if GLOBAL_Q == 16
#define   _IQmpy(A,B)  (_IQ16mpy((A),(B)))
#endif
#if GLOBAL_Q == 15
#define   _IQmpy(A,B)  (_IQ15mpy((A),(B)))
#endif
#if GLOBAL_Q == 14
#define   _IQmpy(A,B)  (_IQ14mpy((A),(B)))
#endif
#if GLOBAL_Q == 13
#define   _IQmpy(A,B)  (_IQ13mpy((A),(B)))
#endif
#if GLOBAL_Q == 12
#define   _IQmpy(A,B)  (_IQ12mpy((A),(B)))
#endif
#if GLOBAL_Q == 11
#define   _IQmpy(A,B)  (_IQ11mpy((A),(B)))
#endif
#if GLOBAL_Q == 10
#define   _IQmpy(A,B)  (_IQ10mpy((A),(B)))
#endif
#if GLOBAL_Q == 9
#define   _IQmpy(A,B)  (_IQ9mpy((A),(B)))
#endif
#if GLOBAL_Q == 8
#define   _IQmpy(A,B)  (_IQ8mpy((A),(B)))
#endif
#if GLOBAL_Q == 7
#define   _IQmpy(A,B)  (_IQ7mpy((A),(B)))
#endif
#if GLOBAL_Q == 6
#define   _IQmpy(A,B)  (_IQ6mpy((A),(B)))
#endif
#if GLOBAL_Q == 5
#define   _IQmpy(A,B)  (_IQ5mpy((A),(B)))
#endif
#if GLOBAL_Q == 4
#define   _IQmpy(A,B)  (_IQ4mpy((A),(B)))
#endif
#if GLOBAL_Q == 3
#define   _IQmpy(A,B)  (_IQ3mpy((A),(B)))
#endif
#if GLOBAL_Q == 2
#define   _IQmpy(A,B)  (_IQ2mpy((A),(B)))
#endif
#if GLOBAL_Q == 1
#define   _IQmpy(A,B)  (_IQ1mpy((A),(B)))
#endif
#if GLOBAL_Q == 0
#define   _IQmpy(A,B)  (_IQ0mpy((A),(B)))
#endif
//---------------------------------------------------------------------------
#ifdef _INLINE_IQMATH
static inline I32_IQ _IQNrmpy(I32_IQ x, I32_IQ y, Uword32 qfmt);
#else
I32_IQ _IQNrmpy(I32_IQ x, I32_IQ y, Uword32 qfmt);
#endif
#define _IQ31rmpy(A,B)  (_IQNrmpy((A),(B),31))
#define _IQ30rmpy(A,B)  (_IQNrmpy((A),(B),30))
#define _IQ29rmpy(A,B)  (_IQNrmpy((A),(B),29))
#define _IQ28rmpy(A,B)  (_IQNrmpy((A),(B),28))
#define _IQ27rmpy(A,B)  (_IQNrmpy((A),(B),27))
#define _IQ26rmpy(A,B)  (_IQNrmpy((A),(B),26))
#define _IQ25rmpy(A,B)  (_IQNrmpy((A),(B),25))
#define _IQ24rmpy(A,B)  (_IQNrmpy((A),(B),24))
#define _IQ23rmpy(A,B)  (_IQNrmpy((A),(B),23))
#define _IQ22rmpy(A,B)  (_IQNrmpy((A),(B),22))
#define _IQ21rmpy(A,B)  (_IQNrmpy((A),(B),21))
#define _IQ20rmpy(A,B)  (_IQNrmpy((A),(B),20))
#define _IQ19rmpy(A,B)  (_IQNrmpy((A),(B),19))
#define _IQ18rmpy(A,B)  (_IQNrmpy((A),(B),18))
#define _IQ17rmpy(A,B)  (_IQNrmpy((A),(B),17))
#define _IQ16rmpy(A,B)  (_IQNrmpy((A),(B),16))
#define _IQ15rmpy(A,B)  (_IQNrmpy((A),(B),15))
#define _IQ14rmpy(A,B)  (_IQNrmpy((A),(B),14))
#define _IQ13rmpy(A,B)  (_IQNrmpy((A),(B),13))
#define _IQ12rmpy(A,B)  (_IQNrmpy((A),(B),12))
#define _IQ11rmpy(A,B)  (_IQNrmpy((A),(B),11))
#define _IQ10rmpy(A,B)  (_IQNrmpy((A),(B),10))
#define _IQ9rmpy(A,B)   (_IQNrmpy((A),(B),9))
#define _IQ8rmpy(A,B)   (_IQNrmpy((A),(B),8))
#define _IQ7rmpy(A,B)   (_IQNrmpy((A),(B),7))
#define _IQ6rmpy(A,B)   (_IQNrmpy((A),(B),6))
#define _IQ5rmpy(A,B)   (_IQNrmpy((A),(B),5))
#define _IQ4rmpy(A,B)   (_IQNrmpy((A),(B),4))
#define _IQ3rmpy(A,B)   (_IQNrmpy((A),(B),3))
#define _IQ2rmpy(A,B)   (_IQNrmpy((A),(B),2))
#define _IQ1rmpy(A,B)   (_IQNrmpy((A),(B),1))
#define _IQ0rmpy(A,B)   (_IQNrmpy((A),(B),0))



#if GLOBAL_Q == 31
#define   _IQrmpy(A,B)  (_IQ31rmpy((A),(B)))
#endif
#if GLOBAL_Q == 30
#define   _IQrmpy(A,B)  (_IQ30rmpy((A),(B)))
#endif
#if GLOBAL_Q == 29
#define   _IQrmpy(A,B)  (_IQ29rmpy((A),(B)))
#endif
#if GLOBAL_Q == 28
#define   _IQrmpy(A,B)  (_IQ28rmpy((A),(B)))
#endif
#if GLOBAL_Q == 27
#define   _IQrmpy(A,B)  (_IQ27rmpy((A),(B)))
#endif
#if GLOBAL_Q == 26
#define   _IQrmpy(A,B)  (_IQ26rmpy((A),(B)))
#endif
#if GLOBAL_Q == 25
#define   _IQrmpy(A,B)  (_IQ25rmpy((A),(B)))
#endif
#if GLOBAL_Q == 24
#define   _IQrmpy(A,B)  (_IQ24rmpy((A),(B)))
#endif
#if GLOBAL_Q == 23
#define   _IQrmpy(A,B)  (_IQ23rmpy((A),(B)))
#endif
#if GLOBAL_Q == 22
#define   _IQrmpy(A,B)  (_IQ22rmpy((A),(B)))
#endif
#if GLOBAL_Q == 21
#define   _IQrmpy(A,B)  (_IQ21rmpy((A),(B)))
#endif
#if GLOBAL_Q == 20
#define   _IQrmpy(A,B)  (_IQ20rmpy((A),(B)))
#endif
#if GLOBAL_Q == 19
#define   _IQrmpy(A,B)  (_IQ19rmpy((A),(B)))
#endif
#if GLOBAL_Q == 18
#define   _IQrmpy(A,B)  (_IQ18rmpy((A),(B)))
#endif
#if GLOBAL_Q == 17
#define   _IQrmpy(A,B)  (_IQ17rmpy((A),(B)))
#endif
#if GLOBAL_Q == 16
#define   _IQrmpy(A,B)  (_IQ16rmpy((A),(B)))
#endif
#if GLOBAL_Q == 15
#define   _IQrmpy(A,B)  (_IQ15rmpy((A),(B)))
#endif
#if GLOBAL_Q == 14
#define   _IQrmpy(A,B)  (_IQ14rmpy((A),(B)))
#endif
#if GLOBAL_Q == 13
#define   _IQrmpy(A,B)  (_IQ13rmpy((A),(B)))
#endif
#if GLOBAL_Q == 12
#define   _IQrmpy(A,B)  (_IQ12rmpy((A),(B)))
#endif
#if GLOBAL_Q == 11
#define   _IQrmpy(A,B)  (_IQ11rmpy((A),(B)))
#endif
#if GLOBAL_Q == 10
#define   _IQrmpy(A,B)  (_IQ10rmpy((A),(B)))
#endif
#if GLOBAL_Q == 9
#define   _IQrmpy(A,B)  (_IQ9rmpy((A),(B)))
#endif
#if GLOBAL_Q == 8
#define   _IQrmpy(A,B)  (_IQ8rmpy((A),(B)))
#endif
#if GLOBAL_Q == 7
#define   _IQrmpy(A,B)  (_IQ7rmpy((A),(B)))
#endif
#if GLOBAL_Q == 6
#define   _IQrmpy(A,B)  (_IQ6rmpy((A),(B)))
#endif
#if GLOBAL_Q == 5
#define   _IQrmpy(A,B)  (_IQ5rmpy((A),(B)))
#endif
#if GLOBAL_Q == 4
#define   _IQrmpy(A,B)  (_IQ4rmpy((A),(B)))
#endif
#if GLOBAL_Q == 3
#define   _IQrmpy(A,B)  (_IQ3rmpy((A),(B)))
#endif
#if GLOBAL_Q == 2
#define   _IQrmpy(A,B)  (_IQ2rmpy((A),(B)))
#endif
#if GLOBAL_Q == 1
#define   _IQrmpy(A,B)  (_IQ1rmpy((A),(B)))
#endif
#if GLOBAL_Q == 0
#define   _IQrmpy(A,B)  (_IQ0rmpy((A),(B)))
#endif

//---------------------------------------------------------------------------
#ifdef _INLINE_IQMATH
static inline I32_IQ _IQNrsmpy(I32_IQ x, I32_IQ y, U32_IQ qfmt);
#else
I32_IQ _IQNrsmpy(I32_IQ x, I32_IQ y, U32_IQ qfmt);
#endif
#define _IQ31rsmpy(A,B)    ( _IQNrsmpy((A),(B),31))
#define _IQ30rsmpy(A,B)    ( _IQNrsmpy((A),(B),30))
#define _IQ29rsmpy(A,B)    ( _IQNrsmpy((A),(B),29))
#define _IQ28rsmpy(A,B)    ( _IQNrsmpy((A),(B),28))
#define _IQ27rsmpy(A,B)    ( _IQNrsmpy((A),(B),27))
#define _IQ26rsmpy(A,B)    ( _IQNrsmpy((A),(B),26))
#define _IQ25rsmpy(A,B)    ( _IQNrsmpy((A),(B),25))
#define _IQ24rsmpy(A,B)    ( _IQNrsmpy((A),(B),24))
#define _IQ23rsmpy(A,B)    ( _IQNrsmpy((A),(B),23))
#define _IQ22rsmpy(A,B)    ( _IQNrsmpy((A),(B),22))
#define _IQ21rsmpy(A,B)    ( _IQNrsmpy((A),(B),21))
#define _IQ20rsmpy(A,B)    ( _IQNrsmpy((A),(B),20))
#define _IQ19rsmpy(A,B)    ( _IQNrsmpy((A),(B),19))
#define _IQ18rsmpy(A,B)    ( _IQNrsmpy((A),(B),18))
#define _IQ17rsmpy(A,B)    ( _IQNrsmpy((A),(B),17))
#define _IQ16rsmpy(A,B)    ( _IQNrsmpy((A),(B),16))
#define _IQ15rsmpy(A,B)    ( _IQNrsmpy((A),(B),15))
#define _IQ14rsmpy(A,B)    ( _IQNrsmpy((A),(B),14))
#define _IQ13rsmpy(A,B)    ( _IQNrsmpy((A),(B),13))
#define _IQ12rsmpy(A,B)    ( _IQNrsmpy((A),(B),12))
#define _IQ11rsmpy(A,B)    ( _IQNrsmpy((A),(B),11))
#define _IQ10rsmpy(A,B)    ( _IQNrsmpy((A),(B),10))
#define _IQ9rsmpy(A,B) ( _IQNrsmpy((A),(B),9))
#define _IQ8rsmpy(A,B) ( _IQNrsmpy((A),(B),8))
#define _IQ7rsmpy(A,B) ( _IQNrsmpy((A),(B),7))
#define _IQ6rsmpy(A,B) ( _IQNrsmpy((A),(B),6))
#define _IQ5rsmpy(A,B) ( _IQNrsmpy((A),(B),5))
#define _IQ4rsmpy(A,B) ( _IQNrsmpy((A),(B),4))
#define _IQ3rsmpy(A,B) ( _IQNrsmpy((A),(B),3))
#define _IQ2rsmpy(A,B) ( _IQNrsmpy((A),(B),2))
#define _IQ1rsmpy(A,B) ( _IQNrsmpy((A),(B),1))
#define _IQ0rsmpy(A,B) ( _IQNrsmpy((A),(B),0))

#if GLOBAL_Q == 31
#define   _IQrsmpy(A,B)  (_IQ31rsmpy((A),(B)))
#endif
#if GLOBAL_Q == 30
#define   _IQrsmpy(A,B)  (_IQ30rsmpy((A),(B)))
#endif
#if GLOBAL_Q == 29
#define   _IQrsmpy(A,B)  (_IQ29rsmpy((A),(B)))
#endif
#if GLOBAL_Q == 28
#define   _IQrsmpy(A,B)  (_IQ28rsmpy((A),(B)))
#endif
#if GLOBAL_Q == 27
#define   _IQrsmpy(A,B)  (_IQ27rsmpy((A),(B)))
#endif
#if GLOBAL_Q == 26
#define   _IQrsmpy(A,B)  (_IQ26rsmpy((A),(B)))
#endif
#if GLOBAL_Q == 25
#define   _IQrsmpy(A,B)  (_IQ25rsmpy((A),(B)))
#endif
#if GLOBAL_Q == 24
#define   _IQrsmpy(A,B)  (_IQ24rsmpy((A),(B)))
#endif
#if GLOBAL_Q == 23
#define   _IQrsmpy(A,B)  (_IQ23rsmpy((A),(B)))
#endif
#if GLOBAL_Q == 22
#define   _IQrsmpy(A,B)  (_IQ22rsmpy((A),(B)))
#endif
#if GLOBAL_Q == 21
#define   _IQrsmpy(A,B)  (_IQ21rsmpy((A),(B)))
#endif
#if GLOBAL_Q == 20
#define   _IQrsmpy(A,B)  (_IQ20rsmpy((A),(B)))
#endif
#if GLOBAL_Q == 19
#define   _IQrsmpy(A,B)  (_IQ19rsmpy((A),(B)))
#endif
#if GLOBAL_Q == 18
#define   _IQrsmpy(A,B)  (_IQ18rsmpy((A),(B)))
#endif
#if GLOBAL_Q == 17
#define   _IQrsmpy(A,B)  (_IQ17rsmpy((A),(B)))
#endif
#if GLOBAL_Q == 16
#define   _IQrsmpy(A,B)  (_IQ16rsmpy((A),(B)))
#endif
#if GLOBAL_Q == 15
#define   _IQrsmpy(A,B)  (_IQ15rsmpy((A),(B)))
#endif
#if GLOBAL_Q == 14
#define   _IQrsmpy(A,B)  (_IQ14rsmpy((A),(B)))
#endif
#if GLOBAL_Q == 13
#define   _IQrsmpy(A,B)  (_IQ13rsmpy((A),(B)))
#endif
#if GLOBAL_Q == 12
#define   _IQrsmpy(A,B)  (_IQ12rsmpy((A),(B)))
#endif
#if GLOBAL_Q == 11
#define   _IQrsmpy(A,B)  (_IQ11rsmpy((A),(B)))
#endif
#if GLOBAL_Q == 10
#define   _IQrsmpy(A,B)  (_IQ10rsmpy((A),(B)))
#endif
#if GLOBAL_Q == 9
#define   _IQrsmpy(A,B)  (_IQ9rsmpy((A),(B)))
#endif
#if GLOBAL_Q == 8
#define   _IQrsmpy(A,B)  (_IQ8rsmpy((A),(B)))
#endif
#if GLOBAL_Q == 7
#define   _IQrsmpy(A,B)  (_IQ7rsmpy((A),(B)))
#endif
#if GLOBAL_Q == 6
#define   _IQrsmpy(A,B)  (_IQ6rsmpy((A),(B)))
#endif
#if GLOBAL_Q == 5
#define   _IQrsmpy(A,B)  (_IQ5rsmpy((A),(B)))
#endif
#if GLOBAL_Q == 4
#define   _IQrsmpy(A,B)  (_IQ4rsmpy((A),(B)))
#endif
#if GLOBAL_Q == 3
#define   _IQrsmpy(A,B)  (_IQ3rsmpy((A),(B)))
#endif
#if GLOBAL_Q == 2
#define   _IQrsmpy(A,B)  (_IQ2rsmpy((A),(B)))
#endif
#if GLOBAL_Q == 1
#define   _IQrsmpy(A,B)  (_IQ1rsmpy((A),(B)))
#endif
#if GLOBAL_Q == 0
#define   _IQrsmpy(A,B)  (_IQ0rsmpy((A),(B)))
#endif

//---------------------------------------------------------------------------
#ifdef _INLINE_IQMATH
static inline I32_IQ _IQNdiv(I32_IQ num, I32_IQ den, Uword32 qfmt);
#else
I32_IQ _IQNdiv(I32_IQ num, I32_IQ den, Uword32 qfmt);
#endif
#define _IQ31div(A,B)   (_IQNdiv((A),(B),31))
#define _IQ30div(A,B)   (_IQNdiv((A),(B),30))

#define _IQ29div(A,B)   (_IQNdiv((A),(B),29))
#define _IQ28div(A,B)   (_IQNdiv((A),(B),28))
#define _IQ27div(A,B)   (_IQNdiv((A),(B),27))
#define _IQ26div(A,B)   (_IQNdiv((A),(B),26))
#define _IQ25div(A,B)   (_IQNdiv((A),(B),25))
#define _IQ24div(A,B)   (_IQNdiv((A),(B),24))
#define _IQ23div(A,B)   (_IQNdiv((A),(B),23))
#define _IQ22div(A,B)   (_IQNdiv((A),(B),22))
#define _IQ21div(A,B)   (_IQNdiv((A),(B),21))
#define _IQ20div(A,B)   (_IQNdiv((A),(B),20))
#define _IQ19div(A,B)   (_IQNdiv((A),(B),19))
#define _IQ18div(A,B)   (_IQNdiv((A),(B),18))
#define _IQ17div(A,B)   (_IQNdiv((A),(B),17))
#define _IQ16div(A,B)   (_IQNdiv((A),(B),16))
#define _IQ15div(A,B)   (_IQNdiv((A),(B),15))
#define _IQ14div(A,B)   (_IQNdiv((A),(B),14))
#define _IQ13div(A,B)   (_IQNdiv((A),(B),13))
#define _IQ12div(A,B)   (_IQNdiv((A),(B),12))
#define _IQ11div(A,B)   (_IQNdiv((A),(B),11))
#define _IQ10div(A,B)   (_IQNdiv((A),(B),10))
#define _IQ9div(A,B)    (_IQNdiv((A),(B),9))
#define _IQ8div(A,B)    (_IQNdiv((A),(B),8))
#define _IQ7div(A,B)    (_IQNdiv((A),(B),7))
#define _IQ6div(A,B)    (_IQNdiv((A),(B),6))
#define _IQ5div(A,B)    (_IQNdiv((A),(B),5))
#define _IQ4div(A,B)    (_IQNdiv((A),(B),4))
#define _IQ3div(A,B)    (_IQNdiv((A),(B),3))
#define _IQ2div(A,B)    (_IQNdiv((A),(B),2))
#define _IQ1div(A,B)    (_IQNdiv((A),(B),1))
#define _IQ0div(A,B)    (_IQNdiv((A),(B),0))

#if GLOBAL_Q == 31
#define   _IQdiv(A,B)  (_IQ31div((A),(B)))
#endif
#if GLOBAL_Q == 30
#define   _IQdiv(A,B)  (_IQ30div((A),(B)))
#endif
#if GLOBAL_Q == 29
#define   _IQdiv(A,B)  (_IQ29div((A),(B)))
#endif
#if GLOBAL_Q == 28
#define   _IQdiv(A,B)  (_IQ28div((A),(B)))
#endif
#if GLOBAL_Q == 27
#define   _IQdiv(A,B)  (_IQ27div((A),(B)))
#endif
#if GLOBAL_Q == 26
#define   _IQdiv(A,B)  (_IQ26div((A),(B)))
#endif
#if GLOBAL_Q == 25
#define   _IQdiv(A,B)  (_IQ25div((A),(B)))
#endif
#if GLOBAL_Q == 24
#define   _IQdiv(A,B)  (_IQ24div((A),(B)))
#endif
#if GLOBAL_Q == 23
#define   _IQdiv(A,B)  (_IQ23div((A),(B)))
#endif
#if GLOBAL_Q == 22
#define   _IQdiv(A,B)  (_IQ22div((A),(B)))
#endif
#if GLOBAL_Q == 21
#define   _IQdiv(A,B)  (_IQ21div((A),(B)))
#endif
#if GLOBAL_Q == 20
#define   _IQdiv(A,B)  (_IQ20div((A),(B)))
#endif
#if GLOBAL_Q == 19
#define   _IQdiv(A,B)  (_IQ19div((A),(B)))
#endif
#if GLOBAL_Q == 18
#define   _IQdiv(A,B)  (_IQ18div((A),(B)))
#endif
#if GLOBAL_Q == 17
#define   _IQdiv(A,B)  (_IQ17div((A),(B)))
#endif
#if GLOBAL_Q == 16
#define   _IQdiv(A,B)  (_IQ16div((A),(B)))
#endif
#if GLOBAL_Q == 15
#define   _IQdiv(A,B)  (_IQ15div((A),(B)))
#endif
#if GLOBAL_Q == 14
#define   _IQdiv(A,B)  (_IQ14div((A),(B)))
#endif
#if GLOBAL_Q == 13
#define   _IQdiv(A,B)  (_IQ13div((A),(B)))
#endif
#if GLOBAL_Q == 12
#define   _IQdiv(A,B)  (_IQ12div((A),(B)))
#endif
#if GLOBAL_Q == 11
#define   _IQdiv(A,B)  (_IQ11div((A),(B)))
#endif
#if GLOBAL_Q == 10
#define   _IQdiv(A,B)  (_IQ10div((A),(B)))
#endif
#if GLOBAL_Q == 9
#define   _IQdiv(A,B)  (_IQ9div((A),(B)))
#endif
#if GLOBAL_Q == 8
#define   _IQdiv(A,B)  (_IQ8div((A),(B)))
#endif
#if GLOBAL_Q == 7
#define   _IQdiv(A,B)  (_IQ7div((A),(B)))
#endif
#if GLOBAL_Q == 6
#define   _IQdiv(A,B)  (_IQ6div((A),(B)))
#endif
#if GLOBAL_Q == 5
#define   _IQdiv(A,B)  (_IQ5div((A),(B)))
#endif
#if GLOBAL_Q == 4
#define   _IQdiv(A,B)  (_IQ4div((A),(B)))
#endif
#if GLOBAL_Q == 3
#define   _IQdiv(A,B)  (_IQ3div((A),(B)))
#endif
#if GLOBAL_Q == 2
#define   _IQdiv(A,B)  (_IQ2div((A),(B)))
#endif
#if GLOBAL_Q == 1
#define   _IQdiv(A,B)  (_IQ1div((A),(B)))
#endif
#if GLOBAL_Q == 0
#define   _IQdiv(A,B)  (_IQ0div((A),(B)))
#endif
//--------------------------------------------------------------------------

#ifdef _INLINE_IQMATH
static inline  I32_IQ _IQNexp(I32_IQ a, U32_IQ qfmt);
#else
I32_IQ _IQNexp(I32_IQ a, U32_IQ qfmt);
#endif
#define _IQ30exp(A)     (_IQNexp((A),30))
#define _IQ29exp(A)     (_IQNexp((A),29))
#define _IQ28exp(A)     (_IQNexp((A),28))
#define _IQ27exp(A)     (_IQNexp((A),27))
#define _IQ26exp(A)     (_IQNexp((A),26))
#define _IQ25exp(A)     (_IQNexp((A),25))
#define _IQ24exp(A)     (_IQNexp((A),24))
#define _IQ23exp(A)     (_IQNexp((A),23))
#define _IQ22exp(A)     (_IQNexp((A),22))
#define _IQ21exp(A)     (_IQNexp((A),21))
#define _IQ20exp(A)     (_IQNexp((A),20))
#define _IQ19exp(A)     (_IQNexp((A),19))
#define _IQ18exp(A)     (_IQNexp((A),18))
#define _IQ17exp(A)     (_IQNexp((A),17))
#define _IQ16exp(A)     (_IQNexp((A),16))
#define _IQ15exp(A)     (_IQNexp((A),15))
#define _IQ14exp(A)     (_IQNexp((A),14))
#define _IQ13exp(A)     (_IQNexp((A),13))
#define _IQ12exp(A)     (_IQNexp((A),12))
#define _IQ11exp(A)     (_IQNexp((A),11))
#define _IQ10exp(A)     (_IQNexp((A),10))
#define _IQ9exp(A)  (_IQNexp((A),9))
#define _IQ8exp(A)  (_IQNexp((A),8))
#define _IQ7exp(A)  (_IQNexp((A),7))
#define _IQ6exp(A)  (_IQNexp((A),6))
#define _IQ5exp(A)  (_IQNexp((A),5))
#define _IQ4exp(A)  (_IQNexp((A),4))
#define _IQ3exp(A)  (_IQNexp((A),3))
#define _IQ2exp(A)  (_IQNexp((A),2))
#define _IQ1exp(A)  (_IQNexp((A),1))
#define _IQ0exp(A)  (_IQNexp((A),0))

#if GLOBAL_Q == 30
#define   _IQexp(A)  (_IQ30exp(A))
#endif
#if GLOBAL_Q == 29
#define   _IQexp(A)  (_IQ29exp(A))
#endif
#if GLOBAL_Q == 28
#define   _IQexp(A)  (_IQ28exp(A))
#endif
#if GLOBAL_Q == 27
#define   _IQexp(A)  (_IQ27exp(A))
#endif
#if GLOBAL_Q == 26
#define   _IQexp(A)  (_IQ26exp(A))
#endif
#if GLOBAL_Q == 25
#define   _IQexp(A)  (_IQ25exp(A))
#endif
#if GLOBAL_Q == 24
#define   _IQexp(A)  (_IQ24exp(A))
#endif
#if GLOBAL_Q == 23
#define   _IQexp(A)  (_IQ23exp(A))
#endif
#if GLOBAL_Q == 22
#define   _IQexp(A)  (_IQ22exp(A))
#endif
#if GLOBAL_Q == 21
#define   _IQexp(A)  (_IQ21exp(A))
#endif
#if GLOBAL_Q == 20
#define   _IQexp(A)  (_IQ20exp(A))
#endif
#if GLOBAL_Q == 19
#define   _IQexp(A)  (_IQ19exp(A))
#endif
#if GLOBAL_Q == 18
#define   _IQexp(A)  (_IQ18exp(A))
#endif
#if GLOBAL_Q == 17
#define   _IQexp(A)  (_IQ17exp(A))
#endif
#if GLOBAL_Q == 16
#define   _IQexp(A)  (_IQ16exp(A))
#endif
#if GLOBAL_Q == 15
#define   _IQexp(A)  (_IQ15exp(A))
#endif
#if GLOBAL_Q == 14
#define   _IQexp(A)  (_IQ14exp(A))
#endif
#if GLOBAL_Q == 13
#define   _IQexp(A)  (_IQ13exp(A))
#endif
#if GLOBAL_Q == 12
#define   _IQexp(A)  (_IQ12exp(A))
#endif
#if GLOBAL_Q == 11
#define   _IQexp(A)  (_IQ11exp(A))
#endif
#if GLOBAL_Q == 10
#define   _IQexp(A)  (_IQ10exp(A))
#endif
#if GLOBAL_Q == 9
#define   _IQexp(A)  (_IQ9exp(A))
#endif
#if GLOBAL_Q == 8
#define   _IQexp(A)  (_IQ8exp(A))
#endif
#if GLOBAL_Q == 7
#define   _IQexp(A)  (_IQ7exp(A))
#endif
#if GLOBAL_Q == 6
#define   _IQexp(A)  (_IQ6exp(A))
#endif
#if GLOBAL_Q == 5
#define   _IQexp(A)  (_IQ5exp(A))
#endif
#if GLOBAL_Q == 4
#define   _IQexp(A)  (_IQ4exp(A))
#endif
#if GLOBAL_Q == 3
#define   _IQexp(A)  (_IQ3exp(A))
#endif
#if GLOBAL_Q == 2
#define   _IQexp(A)  (_IQ2exp(A))
#endif
#if GLOBAL_Q == 1
#define   _IQexp(A)  (_IQ1exp(A))
#endif
#if GLOBAL_Q == 0
#define   _IQexp(A)  (_IQ0exp(A))
#endif
// -------------------------------------------------------------------------

#ifdef _INLINE_IQMATH
static inline I32_IQ _IQNlog(I32_IQ a, U32_IQ qfmt);
#else
I32_IQ _IQNlog(I32_IQ a, U32_IQ qfmt);
#endif
#define _IQ31log(A)     (_IQNlog((A),31))
#define _IQ30log(A)     (_IQNlog((A),30))
#define _IQ29log(A)     (_IQNlog((A),29))
#define _IQ28log(A)     (_IQNlog((A),28))
#define _IQ27log(A)     (_IQNlog((A),27))
#define _IQ26log(A)     (_IQNlog((A),26))
#define _IQ25log(A)     (_IQNlog((A),25))
#define _IQ24log(A)     (_IQNlog((A),24))
#define _IQ23log(A)     (_IQNlog((A),23))
#define _IQ22log(A)     (_IQNlog((A),22))
#define _IQ21log(A)     (_IQNlog((A),21))
#define _IQ20log(A)     (_IQNlog((A),20))
#define _IQ19log(A)     (_IQNlog((A),19))
#define _IQ18log(A)     (_IQNlog((A),18))
#define _IQ17log(A)     (_IQNlog((A),17))
#define _IQ16log(A)     (_IQNlog((A),16))
#define _IQ15log(A)     (_IQNlog((A),15))
#define _IQ14log(A)     (_IQNlog((A),14))
#define _IQ13log(A)     (_IQNlog((A),13))
#define _IQ12log(A)     (_IQNlog((A),12))
#define _IQ11log(A)     (_IQNlog((A),11))
#define _IQ10log(A)     (_IQNlog((A),10))
#define _IQ9log(A)  (_IQNlog((A),9))
#define _IQ8log(A)  (_IQNlog((A),8))
#define _IQ7log(A)  (_IQNlog((A),7))
#define _IQ6log(A)  (_IQNlog((A),6))
#define _IQ5log(A)  (_IQNlog((A),5))
#define _IQ4log(A)  (_IQNlog((A),4))
#define _IQ3log(A)  (_IQNlog((A),3))
#define _IQ2log(A)  (_IQNlog((A),2))
#define _IQ1log(A)  (_IQNlog((A),1))
#define _IQ0log(A)  (_IQNlog((A),0))

#if GLOBAL_Q == 31
#define   _IQlog(A)  (_IQ31log(A))
#endif
#if GLOBAL_Q == 30
#define   _IQlog(A)  (_IQ30log(A))
#endif
#if GLOBAL_Q == 29
#define   _IQlog(A)  (_IQ29log(A))
#endif
#if GLOBAL_Q == 28
#define   _IQlog(A)  (_IQ28log(A))
#endif
#if GLOBAL_Q == 27
#define   _IQlog(A)  (_IQ27log(A))
#endif
#if GLOBAL_Q == 26
#define   _IQlog(A)  (_IQ26log(A))
#endif
#if GLOBAL_Q == 25
#define   _IQlog(A)  (_IQ25log(A))
#endif
#if GLOBAL_Q == 24
#define   _IQlog(A)  (_IQ24log(A))
#endif
#if GLOBAL_Q == 23
#define   _IQlog(A)  (_IQ23log(A))
#endif
#if GLOBAL_Q == 22
#define   _IQlog(A)  (_IQ22log(A))
#endif
#if GLOBAL_Q == 21
#define   _IQlog(A)  (_IQ21log(A))
#endif
#if GLOBAL_Q == 20
#define   _IQlog(A)  (_IQ20log(A))
#endif
#if GLOBAL_Q == 19
#define   _IQlog(A)  (_IQ19log(A))
#endif
#if GLOBAL_Q == 18
#define   _IQlog(A)  (_IQ18log(A))
#endif
#if GLOBAL_Q == 17
#define   _IQlog(A)  (_IQ17log(A))
#endif
#if GLOBAL_Q == 16
#define   _IQlog(A)  (_IQ16log(A))
#endif
#if GLOBAL_Q == 15
#define   _IQlog(A)  (_IQ15log(A))
#endif
#if GLOBAL_Q == 14
#define   _IQlog(A)  (_IQ14log(A))
#endif
#if GLOBAL_Q == 13
#define   _IQlog(A)  (_IQ13log(A))
#endif
#if GLOBAL_Q == 12
#define   _IQlog(A)  (_IQ12log(A))
#endif
#if GLOBAL_Q == 11
#define   _IQlog(A)  (_IQ11log(A))
#endif
#if GLOBAL_Q == 10
#define   _IQlog(A)  (_IQ10log(A))
#endif
#if GLOBAL_Q == 9
#define   _IQlog(A)  (_IQ9log(A))
#endif
#if GLOBAL_Q == 8
#define   _IQlog(A)  (_IQ8log(A))
#endif
#if GLOBAL_Q == 7
#define   _IQlog(A)  (_IQ7log(A))
#endif
#if GLOBAL_Q == 6
#define   _IQlog(A)  (_IQ6log(A))
#endif
#if GLOBAL_Q == 5
#define   _IQlog(A)  (_IQ5log(A))
#endif
#if GLOBAL_Q == 4
#define   _IQlog(A)  (_IQ4log(A))
#endif
#if GLOBAL_Q == 3
#define   _IQlog(A)  (_IQ3log(A))
#endif
#if GLOBAL_Q == 2
#define   _IQlog(A)  (_IQ2log(A))
#endif
#if GLOBAL_Q == 1
#define   _IQlog(A)  (_IQ1log(A))
#endif 
#if GLOBAL_Q == 0
#define   _IQlog(A)  (_IQ0log(A))
#endif 
// -------------------------------------------------------------------------
#ifdef _INLINE_IQMATH
static inline I32_IQ _IQNpow(I32_IQ x, I32_IQ y, U32_IQ qfmt);
#else
I32_IQ _IQNpow(I32_IQ x, I32_IQ y, U32_IQ qfmt);
#endif
#define _IQ31pow(A,B)     (_IQNpow((A),(B),31))
#define _IQ30pow(A,B)     (_IQNpow((A),(B),30))
#define _IQ29pow(A,B)     (_IQNpow((A),(B),29))
#define _IQ28pow(A,B)     (_IQNpow((A),(B),28))
#define _IQ27pow(A,B)     (_IQNpow((A),(B),27))
#define _IQ26pow(A,B)     (_IQNpow((A),(B),26))
#define _IQ25pow(A,B)     (_IQNpow((A),(B),25))
#define _IQ24pow(A,B)     (_IQNpow((A),(B),24))
#define _IQ23pow(A,B)     (_IQNpow((A),(B),23))
#define _IQ22pow(A,B)     (_IQNpow((A),(B),22))
#define _IQ21pow(A,B)     (_IQNpow((A),(B),21))
#define _IQ20pow(A,B)     (_IQNpow((A),(B),20))
#define _IQ19pow(A,B)     (_IQNpow((A),(B),19))
#define _IQ18pow(A,B)     (_IQNpow((A),(B),18))
#define _IQ17pow(A,B)     (_IQNpow((A),(B),17))
#define _IQ16pow(A,B)     (_IQNpow((A),(B),16))
#define _IQ15pow(A,B)     (_IQNpow((A),(B),15))
#define _IQ14pow(A,B)     (_IQNpow((A),(B),14))
#define _IQ13pow(A,B)     (_IQNpow((A),(B),13))
#define _IQ12pow(A,B)     (_IQNpow((A),(B),12))
#define _IQ11pow(A,B)     (_IQNpow((A),(B),11))
#define _IQ10pow(A,B)     (_IQNpow((A),(B),10))
#define _IQ9pow(A,B)      (_IQNpow((A),(B),9))
#define _IQ8pow(A,B)      (_IQNpow((A),(B),8))
#define _IQ7pow(A,B)      (_IQNpow((A),(B),7))
#define _IQ6pow(A,B)      (_IQNpow((A),(B),6))
#define _IQ5pow(A,B)      (_IQNpow((A),(B),5))
#define _IQ4pow(A,B)      (_IQNpow((A),(B),4))
#define _IQ3pow(A,B)      (_IQNpow((A),(B),3))
#define _IQ2pow(A,B)      (_IQNpow((A),(B),2))
#define _IQ1pow(A,B)      (_IQNpow((A),(B),1))
#define _IQ0pow(A,B)      (_IQNpow((A),(B),0))

#if GLOBAL_Q == 31
#define   _IQpow(A,B)  (_IQ31pow((A),(B)))
#endif
#if GLOBAL_Q == 30
#define   _IQpow(A,B)  (_IQ30pow((A),(B)))
#endif
#if GLOBAL_Q == 29
#define   _IQpow(A,B)  (_IQ29pow((A),(B)))
#endif
#if GLOBAL_Q == 28
#define   _IQpow(A,B)  (_IQ28pow((A),(B)))
#endif
#if GLOBAL_Q == 27
#define   _IQpow(A,B)  (_IQ27pow((A),(B)))
#endif
#if GLOBAL_Q == 26
#define   _IQpow(A,B)  (_IQ26pow((A),(B)))
#endif
#if GLOBAL_Q == 25
#define   _IQpow(A,B)  (_IQ25pow((A),(B)))
#endif
#if GLOBAL_Q == 24
#define   _IQpow(A,B)  (_IQ24pow((A),(B)))
#endif
#if GLOBAL_Q == 23
#define   _IQpow(A,B)  (_IQ23pow((A),(B)))
#endif
#if GLOBAL_Q == 22
#define   _IQpow(A,B)  (_IQ22pow((A),(B)))
#endif
#if GLOBAL_Q == 21
#define   _IQpow(A,B)  (_IQ21pow((A),(B)))
#endif
#if GLOBAL_Q == 20
#define   _IQpow(A,B)  (_IQ20pow((A),(B)))
#endif
#if GLOBAL_Q == 19
#define   _IQpow(A,B)  (_IQ19pow((A),(B)))
#endif
#if GLOBAL_Q == 18
#define   _IQpow(A,B)  (_IQ18pow((A),(B)))
#endif
#if GLOBAL_Q == 17
#define   _IQpow(A,B)  (_IQ17pow((A),(B)))
#endif
#if GLOBAL_Q == 16
#define   _IQpow(A,B)  (_IQ16pow((A),(B)))
#endif
#if GLOBAL_Q == 15
#define   _IQpow(A,B)  (_IQ15pow((A),(B)))
#endif
#if GLOBAL_Q == 14
#define   _IQpow(A,B)  (_IQ14pow((A),(B)))
#endif
#if GLOBAL_Q == 13
#define   _IQpow(A,B)  (_IQ13pow((A),(B)))
#endif
#if GLOBAL_Q == 12
#define   _IQpow(A,B)  (_IQ12pow((A),(B)))
#endif
#if GLOBAL_Q == 11
#define   _IQpow(A,B)  (_IQ11pow((A),(B)))
#endif
#if GLOBAL_Q == 10
#define   _IQpow(A,B)  (_IQ10pow((A),(B)))
#endif
#if GLOBAL_Q == 9
#define   _IQpow(A,B)  (_IQ9pow((A),(B)))
#endif
#if GLOBAL_Q == 8
#define   _IQpow(A,B)  (_IQ8pow((A),(B)))
#endif
#if GLOBAL_Q == 7
#define   _IQpow(A,B)  (_IQ7pow((A),(B)))
#endif
#if GLOBAL_Q == 6
#define   _IQpow(A,B)  (_IQ6pow((A),(B)))
#endif
#if GLOBAL_Q == 5
#define   _IQpow(A,B)  (_IQ5pow((A),(B)))
#endif
#if GLOBAL_Q == 4
#define   _IQpow(A,B)  (_IQ4pow((A),(B)))
#endif
#if GLOBAL_Q == 3
#define   _IQpow(A,B)  (_IQ3pow((A),(B)))
#endif
#if GLOBAL_Q == 2
#define   _IQpow(A,B)  (_IQ2pow((A),(B)))
#endif
#if GLOBAL_Q == 1
#define   _IQpow(A,B)  (_IQ1pow((A),(B)))
#endif 
#if GLOBAL_Q == 0
#define   _IQpow(A,B)  (_IQ0pow((A),(B)))
#endif 
// -------------------------------------------------------------------------


#ifdef _INLINE_IQMATH
static inline I32_IQ _IQNsin(I32_IQ in, U32_IQ qfmt);
#else
I32_IQ _IQNsin(I32_IQ in, U32_IQ qfmt);
#endif
#define _IQ30sin(A)     (_IQNsin((A),30))
#define _IQ29sin(A)     (_IQNsin((A),29))
#define _IQ28sin(A)     (_IQNsin((A),28))
#define _IQ27sin(A)     (_IQNsin((A),27))
#define _IQ26sin(A)     (_IQNsin((A),26))
#define _IQ25sin(A)     (_IQNsin((A),25))
#define _IQ24sin(A)     (_IQNsin((A),24))
#define _IQ23sin(A)     (_IQNsin((A),23))
#define _IQ22sin(A)     (_IQNsin((A),22))
#define _IQ21sin(A)     (_IQNsin((A),21))
#define _IQ20sin(A)     (_IQNsin((A),20))
#define _IQ19sin(A)     (_IQNsin((A),19))
#define _IQ18sin(A)     (_IQNsin((A),18))
#define _IQ17sin(A)     (_IQNsin((A),17))
#define _IQ16sin(A)     (_IQNsin((A),16))
#define _IQ15sin(A)     (_IQNsin((A),15))
#define _IQ14sin(A)     (_IQNsin((A),14))
#define _IQ13sin(A)     (_IQNsin((A),13))
#define _IQ12sin(A)     (_IQNsin((A),12))
#define _IQ11sin(A)     (_IQNsin((A),11))
#define _IQ10sin(A)     (_IQNsin((A),10))
#define _IQ9sin(A)      (_IQNsin((A),9))
#define _IQ8sin(A)      (_IQNsin((A),8))
#define _IQ7sin(A)      (_IQNsin((A),7))
#define _IQ6sin(A)      (_IQNsin((A),6))
#define _IQ5sin(A)      (_IQNsin((A),5))
#define _IQ4sin(A)      (_IQNsin((A),4))
#define _IQ3sin(A)      (_IQNsin((A),3))
#define _IQ2sin(A)      (_IQNsin((A),2))
#define _IQ1sin(A)      (_IQNsin((A),1))


#if GLOBAL_Q == 30
#define   _IQsin(A)  (_IQ30sin(A))
#endif
#if GLOBAL_Q == 29
#define   _IQsin(A)  (_IQ29sin(A))
#endif
#if GLOBAL_Q == 28
#define   _IQsin(A)  (_IQ28sin(A))
#endif
#if GLOBAL_Q == 27
#define   _IQsin(A)  (_IQ27sin(A))
#endif
#if GLOBAL_Q == 26
#define   _IQsin(A)  (_IQ26sin(A))
#endif
#if GLOBAL_Q == 25
#define   _IQsin(A)  (_IQ25sin(A))
#endif
#if GLOBAL_Q == 24
#define   _IQsin(A)  (_IQ24sin(A))
#endif
#if GLOBAL_Q == 23
#define   _IQsin(A)  (_IQ23sin(A))
#endif
#if GLOBAL_Q == 22
#define   _IQsin(A)  (_IQ22sin(A))
#endif
#if GLOBAL_Q == 21
#define   _IQsin(A)  (_IQ21sin(A))
#endif
#if GLOBAL_Q == 20
#define   _IQsin(A)  (_IQ20sin(A))
#endif
#if GLOBAL_Q == 19
#define   _IQsin(A)  (_IQ19sin(A))
#endif
#if GLOBAL_Q == 18
#define   _IQsin(A)  (_IQ18sin(A))
#endif
#if GLOBAL_Q == 17
#define   _IQsin(A)  (_IQ17sin(A))
#endif
#if GLOBAL_Q == 16
#define   _IQsin(A)  (_IQ16sin(A))
#endif
#if GLOBAL_Q == 15
#define   _IQsin(A)  (_IQ15sin(A))
#endif
#if GLOBAL_Q == 14
#define   _IQsin(A)  (_IQ14sin(A))
#endif
#if GLOBAL_Q == 13
#define   _IQsin(A)  (_IQ13sin(A))
#endif
#if GLOBAL_Q == 12
#define   _IQsin(A)  (_IQ12sin(A))
#endif
#if GLOBAL_Q == 11
#define   _IQsin(A)  (_IQ11sin(A))
#endif
#if GLOBAL_Q == 10
#define   _IQsin(A)  (_IQ10sin(A))
#endif
#if GLOBAL_Q == 9
#define   _IQsin(A)  (_IQ9sin(A))
#endif
#if GLOBAL_Q == 8
#define   _IQsin(A)  (_IQ8sin(A))
#endif
#if GLOBAL_Q == 7
#define   _IQsin(A)  (_IQ7sin(A))
#endif
#if GLOBAL_Q == 6
#define   _IQsin(A)  (_IQ6sin(A))
#endif
#if GLOBAL_Q == 5
#define   _IQsin(A)  (_IQ5sin(A))
#endif
#if GLOBAL_Q == 4
#define   _IQsin(A)  (_IQ4sin(A))
#endif
#if GLOBAL_Q == 3
#define   _IQsin(A)  (_IQ3sin(A))
#endif
#if GLOBAL_Q == 2
#define   _IQsin(A)  (_IQ2sin(A))
#endif
#if GLOBAL_Q == 1
#define   _IQsin(A)  (_IQ1sin(A))
#endif

//---------------------------------------------------------------------------
#ifdef _INLINE_IQMATH
static inline I32_IQ _IQNasin(I32_IQ in, U32_IQ qfmt);
#else
I32_IQ _IQNasin(I32_IQ in, U32_IQ qfmt);
#endif

#define _IQ30asin(A)    (_IQNasin((A),30))
#define _IQ29asin(A)    (_IQNasin((A),29))
#define _IQ28asin(A)    (_IQNasin((A),28))
#define _IQ27asin(A)    (_IQNasin((A),27))
#define _IQ26asin(A)    (_IQNasin((A),26))
#define _IQ25asin(A)    (_IQNasin((A),25))
#define _IQ24asin(A)    (_IQNasin((A),24))
#define _IQ23asin(A)    (_IQNasin((A),23))
#define _IQ22asin(A)    (_IQNasin((A),22))
#define _IQ21asin(A)    (_IQNasin((A),21))
#define _IQ20asin(A)    (_IQNasin((A),20))
#define _IQ19asin(A)    (_IQNasin((A),19))
#define _IQ18asin(A)    (_IQNasin((A),18))
#define _IQ17asin(A)    (_IQNasin((A),17))
#define _IQ16asin(A)    (_IQNasin((A),16))
#define _IQ15asin(A)    (_IQNasin((A),15))
#define _IQ14asin(A)    (_IQNasin((A),14))
#define _IQ13asin(A)    (_IQNasin((A),13))
#define _IQ12asin(A)    (_IQNasin((A),12))
#define _IQ11asin(A)    (_IQNasin((A),11))
#define _IQ10asin(A)    (_IQNasin((A),10))
#define _IQ9asin(A)         (_IQNasin((A),9))
#define _IQ8asin(A)         (_IQNasin((A),8))
#define _IQ7asin(A)         (_IQNasin((A),7))
#define _IQ6asin(A)         (_IQNasin((A),6))
#define _IQ5asin(A)         (_IQNasin((A),5))
#define _IQ4asin(A)         (_IQNasin((A),4))
#define _IQ3asin(A)         (_IQNasin((A),3))
#define _IQ2asin(A)         (_IQNasin((A),2))
#define _IQ1asin(A)         (_IQNasin((A),1))



#if GLOBAL_Q == 30
#define   _IQasin(A)  (_IQ30asin(A))
#endif
#if GLOBAL_Q == 29
#define   _IQasin(A)  (_IQ29asin(A))
#endif
#if GLOBAL_Q == 28
#define   _IQasin(A)  (_IQ28asin(A))
#endif
#if GLOBAL_Q == 27
#define   _IQasin(A)  (_IQ27asin(A))
#endif
#if GLOBAL_Q == 26
#define   _IQasin(A)  (_IQ26asin(A))
#endif
#if GLOBAL_Q == 25
#define   _IQasin(A)  (_IQ25asin(A))
#endif
#if GLOBAL_Q == 24
#define   _IQasin(A)  (_IQ24asin(A))
#endif
#if GLOBAL_Q == 23
#define   _IQasin(A)  (_IQ23asin(A))
#endif
#if GLOBAL_Q == 22
#define   _IQasin(A)  (_IQ22asin(A))
#endif
#if GLOBAL_Q == 21
#define   _IQasin(A)  (_IQ21asin(A))
#endif
#if GLOBAL_Q == 20
#define   _IQasin(A)  (_IQ20asin(A))
#endif
#if GLOBAL_Q == 19
#define   _IQasin(A)  (_IQ19asin(A))
#endif
#if GLOBAL_Q == 18
#define   _IQasin(A)  (_IQ18asin(A))
#endif
#if GLOBAL_Q == 17
#define   _IQasin(A)  (_IQ17asin(A))
#endif
#if GLOBAL_Q == 16
#define   _IQasin(A)  (_IQ16asin(A))
#endif
#if GLOBAL_Q == 15
#define   _IQasin(A)  (_IQ15asin(A))
#endif
#if GLOBAL_Q == 14
#define   _IQasin(A)  (_IQ14asin(A))
#endif
#if GLOBAL_Q == 13
#define   _IQasin(A)  (_IQ13asin(A))
#endif
#if GLOBAL_Q == 12
#define   _IQasin(A)  (_IQ12asin(A))
#endif
#if GLOBAL_Q == 11
#define   _IQasin(A)  (_IQ11asin(A))
#endif
#if GLOBAL_Q == 10
#define   _IQasin(A)  (_IQ10asin(A))
#endif
#if GLOBAL_Q == 9
#define   _IQasin(A)  (_IQ9asin(A))
#endif
#if GLOBAL_Q == 8
#define   _IQasin(A)  (_IQ8asin(A))
#endif
#if GLOBAL_Q == 7
#define   _IQasin(A)  (_IQ7asin(A))
#endif
#if GLOBAL_Q == 6
#define   _IQasin(A)  (_IQ6asin(A))
#endif
#if GLOBAL_Q == 5
#define   _IQasin(A)  (_IQ5asin(A))
#endif
#if GLOBAL_Q == 4
#define   _IQasin(A)  (_IQ4asin(A))
#endif
#if GLOBAL_Q == 3
#define   _IQasin(A)  (_IQ3asin(A))
#endif
#if GLOBAL_Q == 2
#define   _IQasin(A)  (_IQ2asin(A))
#endif
#if GLOBAL_Q == 1
#define   _IQasin(A)  (_IQ1asin(A))
#endif

//---------------------------------------------------------------------------
#ifdef _INLINE_IQMATH
static inline I32_IQ _IQNacos(I32_IQ in, U32_IQ qfmt);
#else
I32_IQ _IQNacos(I32_IQ in, U32_IQ qfmt);
#endif
#define _IQ30acos(A)    (_IQNacos((A),30))
#define _IQ29acos(A)    (_IQNacos((A),29))
#define _IQ28acos(A)    (_IQNacos((A),28))
#define _IQ27acos(A)    (_IQNacos((A),27))
#define _IQ26acos(A)    (_IQNacos((A),26))
#define _IQ25acos(A)    (_IQNacos((A),25))
#define _IQ24acos(A)    (_IQNacos((A),24))
#define _IQ23acos(A)    (_IQNacos((A),23))
#define _IQ22acos(A)    (_IQNacos((A),22))
#define _IQ21acos(A)    (_IQNacos((A),21))
#define _IQ20acos(A)    (_IQNacos((A),20))
#define _IQ19acos(A)    (_IQNacos((A),19))
#define _IQ18acos(A)    (_IQNacos((A),18))
#define _IQ17acos(A)    (_IQNacos((A),17))
#define _IQ16acos(A)    (_IQNacos((A),16))
#define _IQ15acos(A)    (_IQNacos((A),15))
#define _IQ14acos(A)    (_IQNacos((A),14))
#define _IQ13acos(A)    (_IQNacos((A),13))
#define _IQ12acos(A)    (_IQNacos((A),12))
#define _IQ11acos(A)    (_IQNacos((A),11))
#define _IQ10acos(A)    (_IQNacos((A),10))
#define _IQ9acos(A)         (_IQNacos((A),9))
#define _IQ8acos(A)         (_IQNacos((A),8))
#define _IQ7acos(A)         (_IQNacos((A),7))
#define _IQ6acos(A)         (_IQNacos((A),6))
#define _IQ5acos(A)         (_IQNacos((A),5))
#define _IQ4acos(A)         (_IQNacos((A),4))
#define _IQ3acos(A)         (_IQNacos((A),3))
#define _IQ2acos(A)         (_IQNacos((A),2))
#define _IQ1acos(A)         (_IQNacos((A),1))




#if GLOBAL_Q == 30
#define   _IQacos(A)  (_IQ30acos(A))
#endif
#if GLOBAL_Q == 29
#define   _IQacos(A)  (_IQ29acos(A))
#endif
#if GLOBAL_Q == 28
#define   _IQacos(A)  (_IQ28acos(A))
#endif
#if GLOBAL_Q == 27
#define   _IQacos(A)  (_IQ27acos(A))
#endif
#if GLOBAL_Q == 26
#define   _IQacos(A)  (_IQ26acos(A))
#endif
#if GLOBAL_Q == 25
#define   _IQacos(A)  (_IQ25acos(A))
#endif
#if GLOBAL_Q == 24
#define   _IQacos(A)  (_IQ24acos(A))
#endif
#if GLOBAL_Q == 23
#define   _IQacos(A)  (_IQ23acos(A))
#endif
#if GLOBAL_Q == 22
#define   _IQacos(A)  (_IQ22acos(A))
#endif
#if GLOBAL_Q == 21
#define   _IQacos(A)  (_IQ21acos(A))
#endif
#if GLOBAL_Q == 20
#define   _IQacos(A)  (_IQ20acos(A))
#endif
#if GLOBAL_Q == 19
#define   _IQacos(A)  (_IQ19acos(A))
#endif
#if GLOBAL_Q == 18
#define   _IQacos(A)  (_IQ18acos(A))
#endif
#if GLOBAL_Q == 17
#define   _IQacos(A)  (_IQ17acos(A))
#endif
#if GLOBAL_Q == 16
#define   _IQacos(A)  (_IQ16acos(A))
#endif
#if GLOBAL_Q == 15
#define   _IQacos(A)  (_IQ15acos(A))
#endif
#if GLOBAL_Q == 14
#define   _IQacos(A)  (_IQ14acos(A))
#endif
#if GLOBAL_Q == 13
#define   _IQacos(A)  (_IQ13acos(A))
#endif
#if GLOBAL_Q == 12
#define   _IQacos(A)  (_IQ12acos(A))
#endif
#if GLOBAL_Q == 11
#define   _IQacos(A)  (_IQ11acos(A))
#endif
#if GLOBAL_Q == 10
#define   _IQacos(A)  (_IQ10acos(A))
#endif
#if GLOBAL_Q == 9
#define   _IQacos(A)  (_IQ9acos(A))
#endif
#if GLOBAL_Q == 8
#define   _IQacos(A)  (_IQ8acos(A))
#endif
#if GLOBAL_Q == 7
#define   _IQacos(A)  (_IQ7acos(A))
#endif
#if GLOBAL_Q == 6
#define   _IQacos(A)  (_IQ6acos(A))
#endif
#if GLOBAL_Q == 5
#define   _IQacos(A)  (_IQ5acos(A))
#endif
#if GLOBAL_Q == 4
#define   _IQacos(A)  (_IQ4acos(A))
#endif
#if GLOBAL_Q == 3
#define   _IQacos(A)  (_IQ3acos(A))
#endif
#if GLOBAL_Q == 2
#define   _IQacos(A)  (_IQ2acos(A))
#endif
#if GLOBAL_Q == 1
#define   _IQacos(A)  (_IQ1acos(A))
#endif

#if GLOBAL_Q == 0
#define   _IQacos(A)  (_IQ0acos(A))
#endif
//---------------------------------------------------------------------------
#ifdef _INLINE_IQMATH
static inline I32_IQ _IQNsinPU(I32_IQ in, U32_IQ qfmt);
#else
I32_IQ _IQNsinPU(I32_IQ in, U32_IQ qfmt);
#endif

#define _IQ30sinPU(A)   (_IQNsinPU((A),30))
#define _IQ29sinPU(A)   (_IQNsinPU((A),29))
#define _IQ28sinPU(A)   (_IQNsinPU((A),28))
#define _IQ27sinPU(A)   (_IQNsinPU((A),27))
#define _IQ26sinPU(A)   (_IQNsinPU((A),26))
#define _IQ25sinPU(A)   (_IQNsinPU((A),25))
#define _IQ24sinPU(A)   (_IQNsinPU((A),24))
#define _IQ23sinPU(A)   (_IQNsinPU((A),23))
#define _IQ22sinPU(A)   (_IQNsinPU((A),22))
#define _IQ21sinPU(A)   (_IQNsinPU((A),21))
#define _IQ20sinPU(A)   (_IQNsinPU((A),20))
#define _IQ19sinPU(A)   (_IQNsinPU((A),19))
#define _IQ18sinPU(A)   (_IQNsinPU((A),18))
#define _IQ17sinPU(A)   (_IQNsinPU((A),17))
#define _IQ16sinPU(A)   (_IQNsinPU((A),16))
#define _IQ15sinPU(A)   (_IQNsinPU((A),15))
#define _IQ14sinPU(A)   (_IQNsinPU((A),14))
#define _IQ13sinPU(A)   (_IQNsinPU((A),13))
#define _IQ12sinPU(A)   (_IQNsinPU((A),12))
#define _IQ11sinPU(A)   (_IQNsinPU((A),11))
#define _IQ10sinPU(A)   (_IQNsinPU((A),10))
#define _IQ9sinPU(A)    (_IQNsinPU((A),9))
#define _IQ8sinPU(A)    (_IQNsinPU((A),8))
#define _IQ7sinPU(A)    (_IQNsinPU((A),7))
#define _IQ6sinPU(A)    (_IQNsinPU((A),6))
#define _IQ5sinPU(A)    (_IQNsinPU((A),5))
#define _IQ4sinPU(A)    (_IQNsinPU((A),4))
#define _IQ3sinPU(A)    (_IQNsinPU((A),3))
#define _IQ2sinPU(A)    (_IQNsinPU((A),2))
#define _IQ1sinPU(A)    (_IQNsinPU((A),1))



#if GLOBAL_Q == 30
#define   _IQsinPU(A)  (_IQ30sinPU(A))
#endif
#if GLOBAL_Q == 29
#define   _IQsinPU(A)  (_IQ29sinPU(A))
#endif
#if GLOBAL_Q == 28
#define   _IQsinPU(A)  (_IQ28sinPU(A))
#endif
#if GLOBAL_Q == 27
#define   _IQsinPU(A)  (_IQ27sinPU(A))
#endif
#if GLOBAL_Q == 26
#define   _IQsinPU(A)  (_IQ26sinPU(A))
#endif
#if GLOBAL_Q == 25
#define   _IQsinPU(A)  (_IQ25sinPU(A))
#endif
#if GLOBAL_Q == 24
#define   _IQsinPU(A)  (_IQ24sinPU(A))
#endif
#if GLOBAL_Q == 23
#define   _IQsinPU(A)  (_IQ23sinPU(A))
#endif
#if GLOBAL_Q == 22
#define   _IQsinPU(A)  (_IQ22sinPU(A))
#endif
#if GLOBAL_Q == 21
#define   _IQsinPU(A)  (_IQ21sinPU(A))
#endif
#if GLOBAL_Q == 20
#define   _IQsinPU(A)  (_IQ20sinPU(A))
#endif
#if GLOBAL_Q == 19
#define   _IQsinPU(A)  (_IQ19sinPU(A))
#endif
#if GLOBAL_Q == 18
#define   _IQsinPU(A)  (_IQ18sinPU(A))
#endif
#if GLOBAL_Q == 17
#define   _IQsinPU(A)  (_IQ17sinPU(A))
#endif
#if GLOBAL_Q == 16
#define   _IQsinPU(A)  (_IQ16sinPU(A))
#endif
#if GLOBAL_Q == 15
#define   _IQsinPU(A)  (_IQ15sinPU(A))
#endif
#if GLOBAL_Q == 14
#define   _IQsinPU(A)  (_IQ14sinPU(A))
#endif
#if GLOBAL_Q == 13
#define   _IQsinPU(A)  (_IQ13sinPU(A))
#endif
#if GLOBAL_Q == 12
#define   _IQsinPU(A)  (_IQ12sinPU(A))
#endif
#if GLOBAL_Q == 11
#define   _IQsinPU(A)  (_IQ11sinPU(A))
#endif
#if GLOBAL_Q == 10
#define   _IQsinPU(A)  (_IQ10sinPU(A))
#endif
#if GLOBAL_Q == 9
#define   _IQsinPU(A)  (_IQ9sinPU(A))
#endif
#if GLOBAL_Q == 8
#define   _IQsinPU(A)  (_IQ8sinPU(A))
#endif
#if GLOBAL_Q == 7
#define   _IQsinPU(A)  (_IQ7sinPU(A))
#endif
#if GLOBAL_Q == 6
#define   _IQsinPU(A)  (_IQ6sinPU(A))
#endif
#if GLOBAL_Q == 5
#define   _IQsinPU(A)  (_IQ5sinPU(A))
#endif
#if GLOBAL_Q == 4
#define   _IQsinPU(A)  (_IQ4sinPU(A))
#endif
#if GLOBAL_Q == 3
#define   _IQsinPU(A)  (_IQ3sinPU(A))
#endif
#if GLOBAL_Q == 2
#define   _IQsinPU(A)  (_IQ2sinPU(A))
#endif
#if GLOBAL_Q == 1
#define   _IQsinPU(A)  (_IQ1sinPU(A))
#endif

//---------------------------------------------------------------------------

#ifdef _INLINE_IQMATH
static inline I32_IQ _IQNcos(I32_IQ in, U32_IQ qfmt);
#else
I32_IQ _IQNcos(I32_IQ in, U32_IQ qfmt);
#endif
#define _IQ30cos(A)     (_IQNcos((A),30))
#define _IQ29cos(A)     (_IQNcos((A),29))
#define _IQ28cos(A)     (_IQNcos((A),28))
#define _IQ27cos(A)     (_IQNcos((A),27))
#define _IQ26cos(A)     (_IQNcos((A),26))
#define _IQ25cos(A)     (_IQNcos((A),25))
#define _IQ24cos(A)     (_IQNcos((A),24))
#define _IQ23cos(A)     (_IQNcos((A),23))
#define _IQ22cos(A)     (_IQNcos((A),22))
#define _IQ21cos(A)     (_IQNcos((A),21))
#define _IQ20cos(A)     (_IQNcos((A),20))
#define _IQ19cos(A)     (_IQNcos((A),19))
#define _IQ18cos(A)     (_IQNcos((A),18))
#define _IQ17cos(A)     (_IQNcos((A),17))
#define _IQ16cos(A)     (_IQNcos((A),16))
#define _IQ15cos(A)     (_IQNcos((A),15))
#define _IQ14cos(A)     (_IQNcos((A),14))
#define _IQ13cos(A)     (_IQNcos((A),13))
#define _IQ12cos(A)     (_IQNcos((A),12))
#define _IQ11cos(A)     (_IQNcos((A),11))
#define _IQ10cos(A)     (_IQNcos((A),10))
#define _IQ9cos(A)      (_IQNcos((A),9))
#define _IQ8cos(A)      (_IQNcos((A),8))
#define _IQ7cos(A)      (_IQNcos((A),7))
#define _IQ6cos(A)      (_IQNcos((A),6))
#define _IQ5cos(A)      (_IQNcos((A),5))
#define _IQ4cos(A)      (_IQNcos((A),4))
#define _IQ3cos(A)      (_IQNcos((A),3))
#define _IQ2cos(A)      (_IQNcos((A),2))
#define _IQ1cos(A)      (_IQNcos((A),1))

#if GLOBAL_Q == 30
#define   _IQcos(A)  (_IQ30cos(A))
#endif
#if GLOBAL_Q == 29
#define   _IQcos(A)  (_IQ29cos(A))
#endif
#if GLOBAL_Q == 28
#define   _IQcos(A)  (_IQ28cos(A))
#endif
#if GLOBAL_Q == 27
#define   _IQcos(A)  (_IQ27cos(A))
#endif
#if GLOBAL_Q == 26
#define   _IQcos(A)  (_IQ26cos(A))
#endif
#if GLOBAL_Q == 25
#define   _IQcos(A)  (_IQ25cos(A))
#endif
#if GLOBAL_Q == 24
#define   _IQcos(A)  (_IQ24cos(A))
#endif
#if GLOBAL_Q == 23
#define   _IQcos(A)  (_IQ23cos(A))
#endif
#if GLOBAL_Q == 22
#define   _IQcos(A)  (_IQ22cos(A))
#endif
#if GLOBAL_Q == 21
#define   _IQcos(A)  (_IQ21cos(A))
#endif
#if GLOBAL_Q == 20
#define   _IQcos(A)  (_IQ20cos(A))
#endif
#if GLOBAL_Q == 19
#define   _IQcos(A)  (_IQ19cos(A))
#endif
#if GLOBAL_Q == 18
#define   _IQcos(A)  (_IQ18cos(A))
#endif
#if GLOBAL_Q == 17
#define   _IQcos(A)  (_IQ17cos(A))
#endif
#if GLOBAL_Q == 16
#define   _IQcos(A)  (_IQ16cos(A))
#endif
#if GLOBAL_Q == 15
#define   _IQcos(A)  (_IQ15cos(A))
#endif
#if GLOBAL_Q == 14
#define   _IQcos(A)  (_IQ14cos(A))
#endif
#if GLOBAL_Q == 13
#define   _IQcos(A)  (_IQ13cos(A))
#endif
#if GLOBAL_Q == 12
#define   _IQcos(A)  (_IQ12cos(A))
#endif
#if GLOBAL_Q == 11
#define   _IQcos(A)  (_IQ11cos(A))
#endif
#if GLOBAL_Q == 10
#define   _IQcos(A)  (_IQ10cos(A))
#endif
#if GLOBAL_Q == 9
#define   _IQcos(A)  (_IQ9cos(A))
#endif
#if GLOBAL_Q == 8
#define   _IQcos(A)  (_IQ8cos(A))
#endif
#if GLOBAL_Q == 7
#define   _IQcos(A)  (_IQ7cos(A))
#endif
#if GLOBAL_Q == 6
#define   _IQcos(A)  (_IQ6cos(A))
#endif
#if GLOBAL_Q == 5
#define   _IQcos(A)  (_IQ5cos(A))
#endif
#if GLOBAL_Q == 4
#define   _IQcos(A)  (_IQ4cos(A))
#endif
#if GLOBAL_Q == 3
#define   _IQcos(A)  (_IQ3cos(A))
#endif
#if GLOBAL_Q == 2
#define   _IQcos(A)  (_IQ2cos(A))
#endif
#if GLOBAL_Q == 1
#define   _IQcos(A)  (_IQ1cos(A))
#endif

//---------------------------------------------------------------------------
#ifdef _INLINE_IQMATH
static inline I32_IQ _IQNcosPU(I32_IQ in, U32_IQ qfmt);
#else
I32_IQ _IQNcosPU(I32_IQ in, U32_IQ qfmt);
#endif
#define _IQ30cosPU(A)   (_IQNcosPU((A),30))
#define _IQ29cosPU(A)   (_IQNcosPU((A),29))
#define _IQ28cosPU(A)   (_IQNcosPU((A),28))
#define _IQ27cosPU(A)   (_IQNcosPU((A),27))
#define _IQ26cosPU(A)   (_IQNcosPU((A),26))
#define _IQ25cosPU(A)   (_IQNcosPU((A),25))
#define _IQ24cosPU(A)   (_IQNcosPU((A),24))
#define _IQ23cosPU(A)   (_IQNcosPU((A),23))
#define _IQ22cosPU(A)   (_IQNcosPU((A),22))
#define _IQ21cosPU(A)   (_IQNcosPU((A),21))
#define _IQ20cosPU(A)   (_IQNcosPU((A),20))
#define _IQ19cosPU(A)   (_IQNcosPU((A),19))
#define _IQ18cosPU(A)   (_IQNcosPU((A),18))
#define _IQ17cosPU(A)   (_IQNcosPU((A),17))
#define _IQ16cosPU(A)   (_IQNcosPU((A),16))
#define _IQ15cosPU(A)   (_IQNcosPU((A),15))
#define _IQ14cosPU(A)   (_IQNcosPU((A),14))
#define _IQ13cosPU(A)   (_IQNcosPU((A),13))
#define _IQ12cosPU(A)   (_IQNcosPU((A),12))
#define _IQ11cosPU(A)   (_IQNcosPU((A),11))
#define _IQ10cosPU(A)   (_IQNcosPU((A),10))
#define _IQ9cosPU(A)    (_IQNcosPU((A),9))
#define _IQ8cosPU(A)    (_IQNcosPU((A),8))
#define _IQ7cosPU(A)    (_IQNcosPU((A),7))
#define _IQ6cosPU(A)    (_IQNcosPU((A),6))
#define _IQ5cosPU(A)    (_IQNcosPU((A),5))
#define _IQ4cosPU(A)    (_IQNcosPU((A),4))
#define _IQ3cosPU(A)    (_IQNcosPU((A),3))
#define _IQ2cosPU(A)    (_IQNcosPU((A),2))
#define _IQ1cosPU(A)    (_IQNcosPU((A),1))

#if GLOBAL_Q == 30
#define   _IQcosPU(A)  (_IQ30cosPU(A))
#endif
#if GLOBAL_Q == 29
#define   _IQcosPU(A)  (_IQ29cosPU(A))
#endif
#if GLOBAL_Q == 28
#define   _IQcosPU(A)  (_IQ28cosPU(A))
#endif
#if GLOBAL_Q == 27
#define   _IQcosPU(A)  (_IQ27cosPU(A))
#endif
#if GLOBAL_Q == 26
#define   _IQcosPU(A)  (_IQ26cosPU(A))
#endif
#if GLOBAL_Q == 25
#define   _IQcosPU(A)  (_IQ25cosPU(A))
#endif
#if GLOBAL_Q == 24
#define   _IQcosPU(A)  (_IQ24cosPU(A))
#endif
#if GLOBAL_Q == 23
#define   _IQcosPU(A)  (_IQ23cosPU(A))
#endif
#if GLOBAL_Q == 22
#define   _IQcosPU(A)  (_IQ22cosPU(A))
#endif
#if GLOBAL_Q == 21
#define   _IQcosPU(A)  (_IQ21cosPU(A))
#endif
#if GLOBAL_Q == 20
#define   _IQcosPU(A)  (_IQ20cosPU(A))
#endif
#if GLOBAL_Q == 19
#define   _IQcosPU(A)  (_IQ19cosPU(A))
#endif
#if GLOBAL_Q == 18
#define   _IQcosPU(A)  (_IQ18cosPU(A))
#endif
#if GLOBAL_Q == 17
#define   _IQcosPU(A)  (_IQ17cosPU(A))
#endif
#if GLOBAL_Q == 16
#define   _IQcosPU(A)  (_IQ16cosPU(A))
#endif
#if GLOBAL_Q == 15
#define   _IQcosPU(A)  (_IQ15cosPU(A))
#endif
#if GLOBAL_Q == 14
#define   _IQcosPU(A)  (_IQ14cosPU(A))
#endif
#if GLOBAL_Q == 13
#define   _IQcosPU(A)  (_IQ13cosPU(A))
#endif
#if GLOBAL_Q == 12
#define   _IQcosPU(A)  (_IQ12cosPU(A))
#endif
#if GLOBAL_Q == 11
#define   _IQcosPU(A)  (_IQ11cosPU(A))
#endif
#if GLOBAL_Q == 10
#define   _IQcosPU(A)  (_IQ10cosPU(A))
#endif
#if GLOBAL_Q == 9
#define   _IQcosPU(A)  (_IQ9cosPU(A))
#endif
#if GLOBAL_Q == 8
#define   _IQcosPU(A)  (_IQ8cosPU(A))
#endif
#if GLOBAL_Q == 7
#define   _IQcosPU(A)  (_IQ7cosPU(A))
#endif
#if GLOBAL_Q == 6
#define   _IQcosPU(A)  (_IQ6cosPU(A))
#endif
#if GLOBAL_Q == 5
#define   _IQcosPU(A)  (_IQ5cosPU(A))
#endif
#if GLOBAL_Q == 4
#define   _IQcosPU(A)  (_IQ4cosPU(A))
#endif
#if GLOBAL_Q == 3
#define   _IQcosPU(A)  (_IQ3cosPU(A))
#endif
#if GLOBAL_Q == 2
#define   _IQcosPU(A)  (_IQ2cosPU(A))
#endif
#if GLOBAL_Q == 1
#define   _IQcosPU(A)  (_IQ1cosPU(A))
#endif

//---------------------------------------------------------------------------
#ifdef _INLINE_IQMATH
static inline I32_IQ _IQNatan2(I32_IQ y, I32_IQ x, U32_IQ qfmt);
#else
I32_IQ _IQNatan2(I32_IQ y, I32_IQ x, U32_IQ qfmt);
#endif
#define _IQ30atan2(A,B)     (_IQNatan2((A),(B),30))
#define _IQ29atan2(A,B)     (_IQNatan2((A),(B),29))
#define _IQ28atan2(A,B)     (_IQNatan2((A),(B),28))
#define _IQ27atan2(A,B)     (_IQNatan2((A),(B),27))
#define _IQ26atan2(A,B)     (_IQNatan2((A),(B),26))
#define _IQ25atan2(A,B)     (_IQNatan2((A),(B),25))
#define _IQ24atan2(A,B)     (_IQNatan2((A),(B),24))
#define _IQ23atan2(A,B)     (_IQNatan2((A),(B),23))
#define _IQ22atan2(A,B)     (_IQNatan2((A),(B),22))
#define _IQ21atan2(A,B)     (_IQNatan2((A),(B),21))
#define _IQ20atan2(A,B)     (_IQNatan2((A),(B),20))
#define _IQ19atan2(A,B)     (_IQNatan2((A),(B),19))
#define _IQ18atan2(A,B)     (_IQNatan2((A),(B),18))
#define _IQ17atan2(A,B)     (_IQNatan2((A),(B),17))
#define _IQ16atan2(A,B)     (_IQNatan2((A),(B),16))
#define _IQ15atan2(A,B)     (_IQNatan2((A),(B),15))
#define _IQ14atan2(A,B)     (_IQNatan2((A),(B),14))
#define _IQ13atan2(A,B)     (_IQNatan2((A),(B),13))
#define _IQ12atan2(A,B)     (_IQNatan2((A),(B),12))
#define _IQ11atan2(A,B)     (_IQNatan2((A),(B),11))
#define _IQ10atan2(A,B)     (_IQNatan2((A),(B),10))
#define _IQ9atan2(A,B)  (_IQNatan2((A),(B),9))
#define _IQ8atan2(A,B)  (_IQNatan2((A),(B),8))
#define _IQ7atan2(A,B)  (_IQNatan2((A),(B),7))
#define _IQ6atan2(A,B)  (_IQNatan2((A),(B),6))
#define _IQ5atan2(A,B)  (_IQNatan2((A),(B),5))
#define _IQ4atan2(A,B)  (_IQNatan2((A),(B),4))
#define _IQ3atan2(A,B)  (_IQNatan2((A),(B),3))
#define _IQ2atan2(A,B)  (_IQNatan2((A),(B),2))
#define _IQ1atan2(A,B)  (_IQNatan2((A),(B),1))

#if GLOBAL_Q == 30
#define   _IQatan2(A,B)  (_IQ30atan2((A),(B)))
#endif
#if GLOBAL_Q == 29
#define   _IQatan2(A,B)  (_IQ29atan2((A),(B)))
#endif
#if GLOBAL_Q == 28
#define   _IQatan2(A,B)  (_IQ28atan2((A),(B)))
#endif
#if GLOBAL_Q == 27
#define   _IQatan2(A,B)  (_IQ27atan2((A),(B)))
#endif
#if GLOBAL_Q == 26
#define   _IQatan2(A,B)  (_IQ26atan2((A),(B)))
#endif
#if GLOBAL_Q == 25
#define   _IQatan2(A,B)  (_IQ25atan2((A),(B)))
#endif
#if GLOBAL_Q == 24
#define   _IQatan2(A,B)  (_IQ24atan2((A),(B)))
#endif
#if GLOBAL_Q == 23
#define   _IQatan2(A,B)  (_IQ23atan2((A),(B)))
#endif
#if GLOBAL_Q == 22
#define   _IQatan2(A,B)  (_IQ22atan2((A),(B)))
#endif
#if GLOBAL_Q == 21
#define   _IQatan2(A,B)  (_IQ21atan2((A),(B)))
#endif
#if GLOBAL_Q == 20
#define   _IQatan2(A,B)  (_IQ20atan2((A),(B)))
#endif
#if GLOBAL_Q == 19
#define   _IQatan2(A,B)  (_IQ19atan2((A),(B)))
#endif
#if GLOBAL_Q == 18
#define   _IQatan2(A,B)  (_IQ18atan2((A),(B)))
#endif
#if GLOBAL_Q == 17
#define   _IQatan2(A,B)  (_IQ17atan2((A),(B)))
#endif
#if GLOBAL_Q == 16
#define   _IQatan2(A,B)  (_IQ16atan2((A),(B)))
#endif
#if GLOBAL_Q == 15
#define   _IQatan2(A,B)  (_IQ15atan2((A),(B)))
#endif
#if GLOBAL_Q == 14
#define   _IQatan2(A,B)  (_IQ14atan2((A),(B)))
#endif
#if GLOBAL_Q == 13
#define   _IQatan2(A,B)  (_IQ13atan2((A),(B)))
#endif
#if GLOBAL_Q == 12
#define   _IQatan2(A,B)  (_IQ12atan2((A),(B)))
#endif
#if GLOBAL_Q == 11
#define   _IQatan2(A,B)  (_IQ11atan2((A),(B)))
#endif
#if GLOBAL_Q == 10
#define   _IQatan2(A,B)  (_IQ10atan2((A),(B)))
#endif
#if GLOBAL_Q == 9
#define   _IQatan2(A,B)  (_IQ9atan2((A),(B)))
#endif
#if GLOBAL_Q == 8
#define   _IQatan2(A,B)  (_IQ8atan2((A),(B)))
#endif
#if GLOBAL_Q == 7
#define   _IQatan2(A,B)  (_IQ7atan2((A),(B)))
#endif
#if GLOBAL_Q == 6
#define   _IQatan2(A,B)  (_IQ6atan2((A),(B)))
#endif
#if GLOBAL_Q == 5
#define   _IQatan2(A,B)  (_IQ5atan2((A),(B)))
#endif
#if GLOBAL_Q == 4
#define   _IQatan2(A,B)  (_IQ4atan2((A),(B)))
#endif
#if GLOBAL_Q == 3
#define   _IQatan2(A,B)  (_IQ3atan2((A),(B)))
#endif
#if GLOBAL_Q == 2
#define   _IQatan2(A,B)  (_IQ2atan2((A),(B)))
#endif
#if GLOBAL_Q == 1
#define   _IQatan2(A,B)  (_IQ1atan2((A),(B)))
#endif

//---------------------------------------------------------------------------
#ifdef _INLINE_IQMATH
static inline I32_IQ _IQNatan2PU(I32_IQ y, I32_IQ x, U32_IQ qfmt);
#else
I32_IQ _IQNatan2PU(I32_IQ y, I32_IQ x, U32_IQ qfmt);
#endif
#define _IQ30atan2PU(A,B)   (_IQNatan2PU((A),(B),30))
#define _IQ29atan2PU(A,B)   (_IQNatan2PU((A),(B),29))
#define _IQ28atan2PU(A,B)   (_IQNatan2PU((A),(B),28))
#define _IQ27atan2PU(A,B)   (_IQNatan2PU((A),(B),27))
#define _IQ26atan2PU(A,B)   (_IQNatan2PU((A),(B),26))
#define _IQ25atan2PU(A,B)   (_IQNatan2PU((A),(B),25))
#define _IQ24atan2PU(A,B)   (_IQNatan2PU((A),(B),24))
#define _IQ23atan2PU(A,B)   (_IQNatan2PU((A),(B),23))
#define _IQ22atan2PU(A,B)   (_IQNatan2PU((A),(B),22))
#define _IQ21atan2PU(A,B)   (_IQNatan2PU((A),(B),21))
#define _IQ20atan2PU(A,B)   (_IQNatan2PU((A),(B),20))
#define _IQ19atan2PU(A,B)   (_IQNatan2PU((A),(B),19))
#define _IQ18atan2PU(A,B)   (_IQNatan2PU((A),(B),18))
#define _IQ17atan2PU(A,B)   (_IQNatan2PU((A),(B),17))
#define _IQ16atan2PU(A,B)   (_IQNatan2PU((A),(B),16))
#define _IQ15atan2PU(A,B)   (_IQNatan2PU((A),(B),15))
#define _IQ14atan2PU(A,B)   (_IQNatan2PU((A),(B),14))
#define _IQ13atan2PU(A,B)   (_IQNatan2PU((A),(B),13))
#define _IQ12atan2PU(A,B)   (_IQNatan2PU((A),(B),12))
#define _IQ11atan2PU(A,B)   (_IQNatan2PU((A),(B),11))
#define _IQ10atan2PU(A,B)   (_IQNatan2PU((A),(B),10))
#define _IQ9atan2PU(A,B)    (_IQNatan2PU((A),(B),9))
#define _IQ8atan2PU(A,B)    (_IQNatan2PU((A),(B),8))
#define _IQ7atan2PU(A,B)    (_IQNatan2PU((A),(B),7))
#define _IQ6atan2PU(A,B)    (_IQNatan2PU((A),(B),6))
#define _IQ5atan2PU(A,B)    (_IQNatan2PU((A),(B),5))
#define _IQ4atan2PU(A,B)    (_IQNatan2PU((A),(B),4))
#define _IQ3atan2PU(A,B)    (_IQNatan2PU((A),(B),3))
#define _IQ2atan2PU(A,B)    (_IQNatan2PU((A),(B),2))
#define _IQ1atan2PU(A,B)    (_IQNatan2PU((A),(B),1))

#if GLOBAL_Q == 30
#define   _IQatan2PU(A,B)  (_IQ30atan2PU((A),(B)))
#endif
#if GLOBAL_Q == 29
#define   _IQatan2PU(A,B)  (_IQ29atan2PU((A),(B)))
#endif
#if GLOBAL_Q == 28
#define   _IQatan2PU(A,B)  (_IQ28atan2PU((A),(B)))
#endif
#if GLOBAL_Q == 27
#define   _IQatan2PU(A,B)  (_IQ27atan2PU((A),(B)))
#endif
#if GLOBAL_Q == 26
#define   _IQatan2PU(A,B)  (_IQ26atan2PU((A),(B)))
#endif
#if GLOBAL_Q == 25
#define   _IQatan2PU(A,B)  (_IQ25atan2PU((A),(B)))
#endif
#if GLOBAL_Q == 24
#define   _IQatan2PU(A,B)  (_IQ24atan2PU((A),(B)))
#endif
#if GLOBAL_Q == 23
#define   _IQatan2PU(A,B)  (_IQ23atan2PU((A),(B)))
#endif
#if GLOBAL_Q == 22
#define   _IQatan2PU(A,B)  (_IQ22atan2PU((A),(B)))
#endif
#if GLOBAL_Q == 21
#define   _IQatan2PU(A,B)  (_IQ21atan2PU((A),(B)))
#endif
#if GLOBAL_Q == 20
#define   _IQatan2PU(A,B)  (_IQ20atan2PU((A),(B)))
#endif
#if GLOBAL_Q == 19
#define   _IQatan2PU(A,B)  (_IQ19atan2PU((A),(B)))
#endif
#if GLOBAL_Q == 18
#define   _IQatan2PU(A,B)  (_IQ18atan2PU((A),(B)))
#endif
#if GLOBAL_Q == 17
#define   _IQatan2PU(A,B)  (_IQ17atan2PU((A),(B)))
#endif
#if GLOBAL_Q == 16
#define   _IQatan2PU(A,B)  (_IQ16atan2PU((A),(B)))
#endif
#if GLOBAL_Q == 15
#define   _IQatan2PU(A,B)  (_IQ15atan2PU((A),(B)))
#endif
#if GLOBAL_Q == 14
#define   _IQatan2PU(A,B)  (_IQ14atan2PU((A),(B)))
#endif
#if GLOBAL_Q == 13
#define   _IQatan2PU(A,B)  (_IQ13atan2PU((A),(B)))
#endif
#if GLOBAL_Q == 12
#define   _IQatan2PU(A,B)  (_IQ12atan2PU((A),(B)))
#endif
#if GLOBAL_Q == 11
#define   _IQatan2PU(A,B)  (_IQ11atan2PU((A),(B)))
#endif
#if GLOBAL_Q == 10
#define   _IQatan2PU(A,B)  (_IQ10atan2PU((A),(B)))
#endif
#if GLOBAL_Q == 9
#define   _IQatan2PU(A,B)  (_IQ9atan2PU((A),(B)))
#endif
#if GLOBAL_Q == 8
#define   _IQatan2PU(A,B)  (_IQ8atan2PU((A),(B)))
#endif
#if GLOBAL_Q == 7
#define   _IQatan2PU(A,B)  (_IQ7atan2PU((A),(B)))
#endif
#if GLOBAL_Q == 6
#define   _IQatan2PU(A,B)  (_IQ6atan2PU((A),(B)))
#endif
#if GLOBAL_Q == 5
#define   _IQatan2PU(A,B)  (_IQ5atan2PU((A),(B)))
#endif
#if GLOBAL_Q == 4
#define   _IQatan2PU(A,B)  (_IQ4atan2PU((A),(B)))
#endif
#if GLOBAL_Q == 3
#define   _IQatan2PU(A,B)  (_IQ3atan2PU((A),(B)))
#endif
#if GLOBAL_Q == 2
#define   _IQatan2PU(A,B)  (_IQ2atan2PU((A),(B)))
#endif
#if GLOBAL_Q == 1
#define   _IQatan2PU(A,B)  (_IQ1atan2PU((A),(B)))
#endif
//---------------------------------------------------------------------------
#if GLOBAL_Q == 30
#define   _IQatan(A)  (_IQ30atan2((A),_IQ(1)))
#endif
#if GLOBAL_Q == 29
#define   _IQatan(A)  (_IQ29atan2((A),_IQ(1)))
#endif
#if GLOBAL_Q == 28
#define   _IQatan(A)  (_IQ28atan2((A),_IQ(1)))
#endif
#if GLOBAL_Q == 27
#define   _IQatan(A)  (_IQ27atan2((A),_IQ(1)))
#endif
#if GLOBAL_Q == 26
#define   _IQatan(A)  (_IQ26atan2((A),_IQ(1)))
#endif
#if GLOBAL_Q == 25
#define   _IQatan(A)  (_IQ25atan2((A),_IQ(1)))
#endif
#if GLOBAL_Q == 24
#define   _IQatan(A)  (_IQ24atan2((A),_IQ(1)))
#endif
#if GLOBAL_Q == 23
#define   _IQatan(A)  (_IQ23atan2((A),_IQ(1)))
#endif
#if GLOBAL_Q == 22
#define   _IQatan(A)  (_IQ22atan2((A),_IQ(1)))
#endif
#if GLOBAL_Q == 21
#define   _IQatan(A)  (_IQ21atan2((A),_IQ(1)))
#endif
#if GLOBAL_Q == 20
#define   _IQatan(A)  (_IQ20atan2((A),_IQ(1)))
#endif
#if GLOBAL_Q == 19
#define   _IQatan(A)  (_IQ19atan2((A),_IQ(1)))
#endif
#if GLOBAL_Q == 18
#define   _IQatan(A)  (_IQ18atan2((A),_IQ(1)))
#endif
#if GLOBAL_Q == 17
#define   _IQatan(A)  (_IQ17atan2((A),_IQ(1)))
#endif
#if GLOBAL_Q == 16
#define   _IQatan(A)  (_IQ16atan2((A),_IQ(1)))
#endif
#if GLOBAL_Q == 15
#define   _IQatan(A)  (_IQ15atan2((A),_IQ(1)))
#endif
#if GLOBAL_Q == 14
#define   _IQatan(A)  (_IQ14atan2((A),_IQ(1)))
#endif
#if GLOBAL_Q == 13
#define   _IQatan(A)  (_IQ13atan2((A),_IQ(1)))
#endif
#if GLOBAL_Q == 12
#define   _IQatan(A)  (_IQ12atan2((A),_IQ(1)))
#endif
#if GLOBAL_Q == 11
#define   _IQatan(A)  (_IQ11atan2((A),_IQ(1)))
#endif
#if GLOBAL_Q == 10
#define   _IQatan(A)  (_IQ10atan2((A),_IQ(1)))
#endif
#if GLOBAL_Q == 9
#define   _IQatan(A)  (_IQ9atan2((A),_IQ(1)))
#endif
#if GLOBAL_Q == 8
#define   _IQatan(A)  (_IQ8atan2((A),_IQ(1)))
#endif
#if GLOBAL_Q == 7
#define   _IQatan(A)  (_IQ7atan2((A),_IQ(1)))
#endif
#if GLOBAL_Q == 6
#define   _IQatan(A)  (_IQ6atan2((A),_IQ(1)))
#endif
#if GLOBAL_Q == 5
#define   _IQatan(A)  (_IQ5atan2((A),_IQ(1)))
#endif
#if GLOBAL_Q == 4
#define   _IQatan(A)  (_IQ4atan2((A),_IQ(1)))
#endif
#if GLOBAL_Q == 3
#define   _IQatan(A)  (_IQ3atan2((A),_IQ(1)))
#endif
#if GLOBAL_Q == 2
#define   _IQatan(A)  (_IQ2atan2((A),_IQ(1)))
#endif
#if GLOBAL_Q == 1
#define   _IQatan(A)  (_IQ1atan2((A),_IQ(1)))
#endif

//---------------------------------------------------------------------------
#ifdef _INLINE_IQMATH
static inline I32_IQ _IQNsqrt(I32_IQ x, U32_IQ qfmt);
#else
I32_IQ _IQNsqrt(I32_IQ x, U32_IQ qfmt);
#endif
#define _IQ30sqrt(A)    (_IQNsqrt((A),30))
#define _IQ29sqrt(A)    (_IQNsqrt((A),29))
#define _IQ28sqrt(A)    (_IQNsqrt((A),28))
#define _IQ27sqrt(A)    (_IQNsqrt((A),27))
#define _IQ26sqrt(A)    (_IQNsqrt((A),26))
#define _IQ25sqrt(A)    (_IQNsqrt((A),25))
#define _IQ24sqrt(A)    (_IQNsqrt((A),24))
#define _IQ23sqrt(A)    (_IQNsqrt((A),23))
#define _IQ22sqrt(A)    (_IQNsqrt((A),22))
#define _IQ21sqrt(A)    (_IQNsqrt((A),21))
#define _IQ20sqrt(A)    (_IQNsqrt((A),20))
#define _IQ19sqrt(A)    (_IQNsqrt((A),19))
#define _IQ18sqrt(A)    (_IQNsqrt((A),18))
#define _IQ17sqrt(A)    (_IQNsqrt((A),17))
#define _IQ16sqrt(A)    (_IQNsqrt((A),16))
#define _IQ15sqrt(A)    (_IQNsqrt((A),15))
#define _IQ14sqrt(A)    (_IQNsqrt((A),14))
#define _IQ13sqrt(A)    (_IQNsqrt((A),13))
#define _IQ12sqrt(A)    (_IQNsqrt((A),12))
#define _IQ11sqrt(A)    (_IQNsqrt((A),11))
#define _IQ10sqrt(A)    (_IQNsqrt((A),10))
#define _IQ9sqrt(A)     (_IQNsqrt((A),9 ))
#define _IQ8sqrt(A)     (_IQNsqrt((A),8 ))
#define _IQ7sqrt(A)     (_IQNsqrt((A),7 ))
#define _IQ6sqrt(A)     (_IQNsqrt((A),6 ))
#define _IQ5sqrt(A)     (_IQNsqrt((A),5 ))
#define _IQ4sqrt(A)     (_IQNsqrt((A),4 ))
#define _IQ3sqrt(A)     (_IQNsqrt((A),3 ))
#define _IQ2sqrt(A)     (_IQNsqrt((A),2 ))
#define _IQ1sqrt(A)     (_IQNsqrt((A),1 ))
#define _IQ0sqrt(A)     (_IQNsqrt((A),0 ))



#if GLOBAL_Q == 30
#define   _IQsqrt(A)  (_IQ30sqrt(A))
#endif
#if GLOBAL_Q == 29
#define   _IQsqrt(A)  (_IQ29sqrt(A))
#endif
#if GLOBAL_Q == 28
#define   _IQsqrt(A)  (_IQ28sqrt(A))
#endif
#if GLOBAL_Q == 27
#define   _IQsqrt(A)  (_IQ27sqrt(A))
#endif
#if GLOBAL_Q == 26
#define   _IQsqrt(A)  (_IQ26sqrt(A))
#endif
#if GLOBAL_Q == 25
#define   _IQsqrt(A)  (_IQ25sqrt(A))
#endif
#if GLOBAL_Q == 24
#define   _IQsqrt(A)  (_IQ24sqrt(A))
#endif
#if GLOBAL_Q == 23
#define   _IQsqrt(A)  (_IQ23sqrt(A))
#endif
#if GLOBAL_Q == 22
#define   _IQsqrt(A)  (_IQ22sqrt(A))
#endif
#if GLOBAL_Q == 21
#define   _IQsqrt(A)  (_IQ21sqrt(A))
#endif
#if GLOBAL_Q == 20
#define   _IQsqrt(A)  (_IQ20sqrt(A))
#endif
#if GLOBAL_Q == 19
#define   _IQsqrt(A)  (_IQ19sqrt(A))
#endif
#if GLOBAL_Q == 18
#define   _IQsqrt(A)  (_IQ18sqrt(A))
#endif
#if GLOBAL_Q == 17
#define   _IQsqrt(A)  (_IQ17sqrt(A))
#endif
#if GLOBAL_Q == 16
#define   _IQsqrt(A)  (_IQ16sqrt(A))
#endif
#if GLOBAL_Q == 15
#define   _IQsqrt(A)  (_IQ15sqrt(A))
#endif
#if GLOBAL_Q == 14
#define   _IQsqrt(A)  (_IQ14sqrt(A))
#endif
#if GLOBAL_Q == 13
#define   _IQsqrt(A)  (_IQ13sqrt(A))
#endif
#if GLOBAL_Q == 12
#define   _IQsqrt(A)  (_IQ12sqrt(A))
#endif
#if GLOBAL_Q == 11
#define   _IQsqrt(A)  (_IQ11sqrt(A))
#endif
#if GLOBAL_Q == 10
#define   _IQsqrt(A)  (_IQ10sqrt(A))
#endif
#if GLOBAL_Q == 9
#define   _IQsqrt(A)  (_IQ9sqrt(A))
#endif
#if GLOBAL_Q == 8
#define   _IQsqrt(A)  (_IQ8sqrt(A))
#endif
#if GLOBAL_Q == 7
#define   _IQsqrt(A)  (_IQ7sqrt(A))
#endif
#if GLOBAL_Q == 6
#define   _IQsqrt(A)  (_IQ6sqrt(A))
#endif
#if GLOBAL_Q == 5
#define   _IQsqrt(A)  (_IQ5sqrt(A))
#endif
#if GLOBAL_Q == 4
#define   _IQsqrt(A)  (_IQ4sqrt(A))
#endif
#if GLOBAL_Q == 3
#define   _IQsqrt(A)  (_IQ3sqrt(A))
#endif
#if GLOBAL_Q == 2
#define   _IQsqrt(A)  (_IQ2sqrt(A))
#endif
#if GLOBAL_Q == 1
#define   _IQsqrt(A)  (_IQ1sqrt(A))
#endif
#if GLOBAL_Q == 0
#define   _IQsqrt(A)  (_IQ0sqrt(A))
#endif
//---------------------------------------------------------------------------
#ifdef _INLINE_IQMATH
static inline I32_IQ _IQNisqrt(I32_IQ x, U32_IQ qfmt);
#else
I32_IQ _IQNisqrt(I32_IQ x, U32_IQ qfmt);
#endif
#define _IQ30isqrt(A)  ( _IQNisqrt((A),30))
#define _IQ29isqrt(A)  ( _IQNisqrt((A),29))
#define _IQ28isqrt(A)  ( _IQNisqrt((A),28))
#define _IQ27isqrt(A)  ( _IQNisqrt((A),27))
#define _IQ26isqrt(A)  ( _IQNisqrt((A),26))
#define _IQ25isqrt(A)  ( _IQNisqrt((A),25))
#define _IQ24isqrt(A)  ( _IQNisqrt((A),24))
#define _IQ23isqrt(A)  ( _IQNisqrt((A),23))
#define _IQ22isqrt(A)  ( _IQNisqrt((A),22))
#define _IQ21isqrt(A)  ( _IQNisqrt((A),21))
#define _IQ20isqrt(A)  ( _IQNisqrt((A),20))
#define _IQ19isqrt(A)  ( _IQNisqrt((A),19))
#define _IQ18isqrt(A)  ( _IQNisqrt((A),18))
#define _IQ17isqrt(A)  ( _IQNisqrt((A),17))
#define _IQ16isqrt(A)  ( _IQNisqrt((A),16))
#define _IQ15isqrt(A)  ( _IQNisqrt((A),15))
#define _IQ14isqrt(A)  ( _IQNisqrt((A),14))
#define _IQ13isqrt(A)  ( _IQNisqrt((A),13))
#define _IQ12isqrt(A)  ( _IQNisqrt((A),12))
#define _IQ11isqrt(A)  ( _IQNisqrt((A),11))
#define _IQ10isqrt(A)  ( _IQNisqrt((A),10))
#define _IQ9isqrt(A)       ( _IQNisqrt((A),9))
#define _IQ8isqrt(A)       ( _IQNisqrt((A),8))
#define _IQ7isqrt(A)       ( _IQNisqrt((A),7))
#define _IQ6isqrt(A)       ( _IQNisqrt((A),6))
#define _IQ5isqrt(A)       ( _IQNisqrt((A),5))
#define _IQ4isqrt(A)       ( _IQNisqrt((A),4))
#define _IQ3isqrt(A)       ( _IQNisqrt((A),3))
#define _IQ2isqrt(A)       ( _IQNisqrt((A),2))
#define _IQ1isqrt(A)       ( _IQNisqrt((A),1))
#define _IQ0isqrt(A)       ( _IQNisqrt((A),0))

#if GLOBAL_Q == 30
#define   _IQisqrt(A)  (_IQ30isqrt(A))
#endif
#if GLOBAL_Q == 29
#define   _IQisqrt(A)  (_IQ29isqrt(A))
#endif
#if GLOBAL_Q == 28
#define   _IQisqrt(A)  (_IQ28isqrt(A))
#endif
#if GLOBAL_Q == 27
#define   _IQisqrt(A)  (_IQ27isqrt(A))
#endif
#if GLOBAL_Q == 26
#define   _IQisqrt(A)  (_IQ26isqrt(A))
#endif
#if GLOBAL_Q == 25
#define   _IQisqrt(A)  (_IQ25isqrt(A))
#endif
#if GLOBAL_Q == 24
#define   _IQisqrt(A)  (_IQ24isqrt(A))
#endif
#if GLOBAL_Q == 23
#define   _IQisqrt(A)  (_IQ23isqrt(A))
#endif
#if GLOBAL_Q == 22
#define   _IQisqrt(A)  (_IQ22isqrt(A))
#endif
#if GLOBAL_Q == 21
#define   _IQisqrt(A)  (_IQ21isqrt(A))
#endif
#if GLOBAL_Q == 20
#define   _IQisqrt(A)  (_IQ20isqrt(A))
#endif
#if GLOBAL_Q == 19
#define   _IQisqrt(A)  (_IQ19isqrt(A))
#endif
#if GLOBAL_Q == 18
#define   _IQisqrt(A)  (_IQ18isqrt(A))
#endif
#if GLOBAL_Q == 17
#define   _IQisqrt(A)  (_IQ17isqrt(A))
#endif
#if GLOBAL_Q == 16
#define   _IQisqrt(A)  (_IQ16isqrt(A))
#endif
#if GLOBAL_Q == 15
#define   _IQisqrt(A)  (_IQ15isqrt(A))
#endif
#if GLOBAL_Q == 14
#define   _IQisqrt(A)  (_IQ14isqrt(A))
#endif
#if GLOBAL_Q == 13
#define   _IQisqrt(A)  (_IQ13isqrt(A))
#endif
#if GLOBAL_Q == 12
#define   _IQisqrt(A)  (_IQ12isqrt(A))
#endif
#if GLOBAL_Q == 11
#define   _IQisqrt(A)  (_IQ11isqrt(A))
#endif
#if GLOBAL_Q == 10
#define   _IQisqrt(A)  (_IQ10isqrt(A))
#endif
#if GLOBAL_Q == 9
#define   _IQisqrt(A)  (_IQ9isqrt(A))
#endif
#if GLOBAL_Q == 8
#define   _IQisqrt(A)  (_IQ8isqrt(A))
#endif
#if GLOBAL_Q == 7
#define   _IQisqrt(A)  (_IQ7isqrt(A))
#endif
#if GLOBAL_Q == 6
#define   _IQisqrt(A)  (_IQ6isqrt(A))
#endif
#if GLOBAL_Q == 5
#define   _IQisqrt(A)  (_IQ5isqrt(A))
#endif
#if GLOBAL_Q == 4
#define   _IQisqrt(A)  (_IQ4isqrt(A))
#endif
#if GLOBAL_Q == 3
#define   _IQisqrt(A)  (_IQ3isqrt(A))
#endif
#if GLOBAL_Q == 2
#define   _IQisqrt(A)  (_IQ2isqrt(A))
#endif
#if GLOBAL_Q == 1
#define   _IQisqrt(A)  (_IQ1isqrt(A))
#endif
#if GLOBAL_Q == 0
#define   _IQisqrt(A)  (_IQ0isqrt(A))
#endif

//---------------------------------------------------------------------------
#ifdef _INLINE_IQMATH
static inline I32_IQ _IQNint(I32_IQ input, U32_IQ qfmt);
#else
I32_IQ _IQNint(I32_IQ input, U32_IQ qfmt);
#endif
#define _IQ31int(A)     (_IQNint((A),31))
#define _IQ30int(A)     (_IQNint((A),30))
#define _IQ29int(A)     (_IQNint((A),29))
#define _IQ28int(A)     (_IQNint((A),28))
#define _IQ27int(A)     (_IQNint((A),27))
#define _IQ26int(A)     (_IQNint((A),26))
#define _IQ25int(A)     (_IQNint((A),25))
#define _IQ24int(A)     (_IQNint((A),24))
#define _IQ23int(A)     (_IQNint((A),23))
#define _IQ22int(A)     (_IQNint((A),22))
#define _IQ21int(A)     (_IQNint((A),21))
#define _IQ20int(A)     (_IQNint((A),20))
#define _IQ19int(A)     (_IQNint((A),19))
#define _IQ18int(A)     (_IQNint((A),18))
#define _IQ17int(A)     (_IQNint((A),17))
#define _IQ16int(A)     (_IQNint((A),16))
#define _IQ15int(A)     (_IQNint((A),15))
#define _IQ14int(A)     (_IQNint((A),14))
#define _IQ13int(A)     (_IQNint((A),13))
#define _IQ12int(A)     (_IQNint((A),12))
#define _IQ11int(A)     (_IQNint((A),11))
#define _IQ10int(A)     (_IQNint((A),10))
#define _IQ9int(A)      (_IQNint((A),9))
#define _IQ8int(A)      (_IQNint((A),8))
#define _IQ7int(A)      (_IQNint((A),7))
#define _IQ6int(A)      (_IQNint((A),6))
#define _IQ5int(A)      (_IQNint((A),5))
#define _IQ4int(A)      (_IQNint((A),4))
#define _IQ3int(A)      (_IQNint((A),3))
#define _IQ2int(A)      (_IQNint((A),2))
#define _IQ1int(A)      (_IQNint((A),1))
#define _IQ0int(A)      (_IQNint((A),0))


#if GLOBAL_Q == 31
#define   _IQint(A)  (_IQ31int(A))
#endif
#if GLOBAL_Q == 30
#define   _IQint(A)  (_IQ30int(A))
#endif
#if GLOBAL_Q == 29
#define   _IQint(A)  (_IQ29int(A))
#endif
#if GLOBAL_Q == 28
#define   _IQint(A)  (_IQ28int(A))
#endif
#if GLOBAL_Q == 27
#define   _IQint(A)  (_IQ27int(A))
#endif
#if GLOBAL_Q == 26
#define   _IQint(A)  (_IQ26int(A))
#endif
#if GLOBAL_Q == 25
#define   _IQint(A)  (_IQ25int(A))
#endif
#if GLOBAL_Q == 24
#define   _IQint(A)  (_IQ24int(A))
#endif
#if GLOBAL_Q == 23
#define   _IQint(A)  (_IQ23int(A))
#endif
#if GLOBAL_Q == 22
#define   _IQint(A)  (_IQ22int(A))
#endif
#if GLOBAL_Q == 21
#define   _IQint(A)  (_IQ21int(A))
#endif
#if GLOBAL_Q == 20
#define   _IQint(A)  (_IQ20int(A))
#endif
#if GLOBAL_Q == 19
#define   _IQint(A)  (_IQ19int(A))
#endif
#if GLOBAL_Q == 18
#define   _IQint(A)  (_IQ18int(A))
#endif
#if GLOBAL_Q == 17
#define   _IQint(A)  (_IQ17int(A))
#endif
#if GLOBAL_Q == 16
#define   _IQint(A)  (_IQ16int(A))
#endif
#if GLOBAL_Q == 15
#define   _IQint(A)  (_IQ15int(A))
#endif
#if GLOBAL_Q == 14
#define   _IQint(A)  (_IQ14int(A))
#endif
#if GLOBAL_Q == 13
#define   _IQint(A)  (_IQ13int(A))
#endif
#if GLOBAL_Q == 12
#define   _IQint(A)  (_IQ12int(A))
#endif
#if GLOBAL_Q == 11
#define   _IQint(A)  (_IQ11int(A))
#endif
#if GLOBAL_Q == 10
#define   _IQint(A)  (_IQ10int(A))
#endif
#if GLOBAL_Q == 9
#define   _IQint(A)  (_IQ9int(A))
#endif
#if GLOBAL_Q == 8
#define   _IQint(A)  (_IQ8int(A))
#endif
#if GLOBAL_Q == 7
#define   _IQint(A)  (_IQ7int(A))
#endif
#if GLOBAL_Q == 6
#define   _IQint(A)  (_IQ6int(A))
#endif
#if GLOBAL_Q == 5
#define   _IQint(A)  (_IQ5int(A))
#endif
#if GLOBAL_Q == 4
#define   _IQint(A)  (_IQ4int(A))
#endif
#if GLOBAL_Q == 3
#define   _IQint(A)  (_IQ3int(A))
#endif
#if GLOBAL_Q == 2
#define   _IQint(A)  (_IQ2int(A))
#endif
#if GLOBAL_Q == 1
#define   _IQint(A)  (_IQ1int(A))
#endif
#if GLOBAL_Q == 0
#define   _IQint(A)  (_IQ0int(A))
#endif
//---------------------------------------------------------------------------
#ifdef _INLINE_IQMATH
static inline I32_IQ _IQNfrac(I32_IQ f1, U32_IQ var2);
#else
I32_IQ _IQNfrac(I32_IQ f1, U32_IQ var2);
#endif
#define _IQ31frac(A)    (_IQNfrac((A),31))
#define _IQ30frac(A)    (_IQNfrac((A),30))
#define _IQ29frac(A)    (_IQNfrac((A),29))
#define _IQ28frac(A)    (_IQNfrac((A),28))
#define _IQ27frac(A)    (_IQNfrac((A),27))
#define _IQ26frac(A)    (_IQNfrac((A),26))
#define _IQ25frac(A)    (_IQNfrac((A),25))
#define _IQ24frac(A)    (_IQNfrac((A),24))
#define _IQ23frac(A)    (_IQNfrac((A),23))
#define _IQ22frac(A)    (_IQNfrac((A),22))
#define _IQ21frac(A)    (_IQNfrac((A),21))
#define _IQ20frac(A)    (_IQNfrac((A),20))
#define _IQ19frac(A)    (_IQNfrac((A),19))
#define _IQ18frac(A)    (_IQNfrac((A),18))
#define _IQ17frac(A)    (_IQNfrac((A),17))
#define _IQ16frac(A)    (_IQNfrac((A),16))
#define _IQ15frac(A)    (_IQNfrac((A),15))
#define _IQ14frac(A)    (_IQNfrac((A),14))
#define _IQ13frac(A)    (_IQNfrac((A),13))
#define _IQ12frac(A)    (_IQNfrac((A),12))
#define _IQ11frac(A)    (_IQNfrac((A),11))
#define _IQ10frac(A)    (_IQNfrac((A),10))
#define _IQ9frac(A)         (_IQNfrac((A),9))
#define _IQ8frac(A)         (_IQNfrac((A),8))
#define _IQ7frac(A)         (_IQNfrac((A),7))
#define _IQ6frac(A)         (_IQNfrac((A),6))
#define _IQ5frac(A)         (_IQNfrac((A),5))
#define _IQ4frac(A)         (_IQNfrac((A),4))
#define _IQ3frac(A)         (_IQNfrac((A),3))
#define _IQ2frac(A)         (_IQNfrac((A),2))
#define _IQ1frac(A)         (_IQNfrac((A),1))
#define _IQ0frac(A)         (_IQNfrac((A),0))

#if GLOBAL_Q == 31
#define   _IQfrac(A)  (_IQ31frac(A))
#endif
#if GLOBAL_Q == 30
#define   _IQfrac(A)  (_IQ30frac(A))
#endif
#if GLOBAL_Q == 29
#define   _IQfrac(A)  (_IQ29frac(A))
#endif
#if GLOBAL_Q == 28
#define   _IQfrac(A)  (_IQ28frac(A))
#endif
#if GLOBAL_Q == 27
#define   _IQfrac(A)  (_IQ27frac(A))
#endif
#if GLOBAL_Q == 26
#define   _IQfrac(A)  (_IQ26frac(A))
#endif
#if GLOBAL_Q == 25
#define   _IQfrac(A)  (_IQ25frac(A))
#endif
#if GLOBAL_Q == 24
#define   _IQfrac(A)  (_IQ24frac(A))
#endif
#if GLOBAL_Q == 23
#define   _IQfrac(A)  (_IQ23frac(A))
#endif
#if GLOBAL_Q == 22
#define   _IQfrac(A)  (_IQ22frac(A))
#endif
#if GLOBAL_Q == 21
#define   _IQfrac(A)  (_IQ21frac(A))
#endif
#if GLOBAL_Q == 20
#define   _IQfrac(A)  (_IQ20frac(A))
#endif
#if GLOBAL_Q == 19
#define   _IQfrac(A)  (_IQ19frac(A))
#endif
#if GLOBAL_Q == 18
#define   _IQfrac(A)  (_IQ18frac(A))
#endif
#if GLOBAL_Q == 17
#define   _IQfrac(A)  (_IQ17frac(A))
#endif
#if GLOBAL_Q == 16
#define   _IQfrac(A)  (_IQ16frac(A))
#endif
#if GLOBAL_Q == 15
#define   _IQfrac(A)  (_IQ15frac(A))
#endif
#if GLOBAL_Q == 14
#define   _IQfrac(A)  (_IQ14frac(A))
#endif
#if GLOBAL_Q == 13
#define   _IQfrac(A)  (_IQ13frac(A))
#endif
#if GLOBAL_Q == 12
#define   _IQfrac(A)  (_IQ12frac(A))
#endif
#if GLOBAL_Q == 11
#define   _IQfrac(A)  (_IQ11frac(A))
#endif
#if GLOBAL_Q == 10
#define   _IQfrac(A)  (_IQ10frac(A))
#endif
#if GLOBAL_Q == 9
#define   _IQfrac(A)  (_IQ9frac(A))
#endif
#if GLOBAL_Q == 8
#define   _IQfrac(A)  (_IQ8frac(A))
#endif
#if GLOBAL_Q == 7
#define   _IQfrac(A)  (_IQ7frac(A))
#endif
#if GLOBAL_Q == 6
#define   _IQfrac(A)  (_IQ6frac(A))
#endif
#if GLOBAL_Q == 5
#define   _IQfrac(A)  (_IQ5frac(A))
#endif
#if GLOBAL_Q == 4
#define   _IQfrac(A)  (_IQ4frac(A))
#endif
#if GLOBAL_Q == 3
#define   _IQfrac(A)  (_IQ3frac(A))
#endif
#if GLOBAL_Q == 2
#define   _IQfrac(A)  (_IQ2frac(A))
#endif
#if GLOBAL_Q == 1
#define   _IQfrac(A)  (_IQ1frac(A))
#endif
#if GLOBAL_Q == 0
#define   _IQfrac(A)  (_IQ0frac(A))
#endif

//---------------------------------------------------------------------------
#ifdef _INLINE_IQMATH
static inline I32_IQ  _IQNmpyIQx(I32_IQ in1, I32_IQ qfmt1,I32_IQ in2, I32_IQ qfmt2, U32_IQ qfmt);
#else
I32_IQ  _IQNmpyIQx(I32_IQ in1, I32_IQ qfmt1,I32_IQ in2, I32_IQ qfmt2, U32_IQ qfmt);
#endif

#define _IQmpyIQX(A, IQA, B, IQB)       (_IQNmpyIQx( (A),  (IQA),  (B),  (IQB),  (GLOBAL_Q)))
#define _IQ31mpyIQX(A, IQA, B, IQB)     (_IQNmpyIQx( (A),  (IQA),  (B),  (IQB),  31))
#define _IQ30mpyIQX(A, IQA, B, IQB)     (_IQNmpyIQx( (A),  (IQA),  (B),  (IQB),  30))
#define _IQ29mpyIQX(A, IQA, B, IQB)     (_IQNmpyIQx( (A),  (IQA),  (B),  (IQB),  29))
#define _IQ28mpyIQX(A, IQA, B, IQB)     (_IQNmpyIQx( (A),  (IQA),  (B),  (IQB),  28))
#define _IQ27mpyIQX(A, IQA, B, IQB)     (_IQNmpyIQx( (A),  (IQA),  (B),  (IQB),  27))
#define _IQ26mpyIQX(A, IQA, B, IQB)     (_IQNmpyIQx( (A),  (IQA),  (B),  (IQB),  26))
#define _IQ25mpyIQX(A, IQA, B, IQB)     (_IQNmpyIQx( (A),  (IQA),  (B),  (IQB),  25))
#define _IQ24mpyIQX(A, IQA, B, IQB)     (_IQNmpyIQx( (A),  (IQA),  (B),  (IQB),  24))
#define _IQ23mpyIQX(A, IQA, B, IQB)     (_IQNmpyIQx( (A),  (IQA),  (B),  (IQB),  23))
#define _IQ22mpyIQX(A, IQA, B, IQB)     (_IQNmpyIQx( (A),  (IQA),  (B),  (IQB),  22))
#define _IQ21mpyIQX(A, IQA, B, IQB)     (_IQNmpyIQx( (A),  (IQA),  (B),  (IQB),  21))
#define _IQ20mpyIQX(A, IQA, B, IQB)     (_IQNmpyIQx( (A),  (IQA),  (B),  (IQB),  20))
#define _IQ19mpyIQX(A, IQA, B, IQB)     (_IQNmpyIQx( (A),  (IQA),  (B),  (IQB),  19))
#define _IQ18mpyIQX(A, IQA, B, IQB)     (_IQNmpyIQx( (A),  (IQA),  (B),  (IQB),  18))
#define _IQ17mpyIQX(A, IQA, B, IQB)     (_IQNmpyIQx( (A),  (IQA),  (B),  (IQB),  17))
#define _IQ16mpyIQX(A, IQA, B, IQB)     (_IQNmpyIQx( (A),  (IQA),  (B),  (IQB),  16))
#define _IQ15mpyIQX(A, IQA, B, IQB)     (_IQNmpyIQx( (A),  (IQA),  (B),  (IQB),  15))
#define _IQ14mpyIQX(A, IQA, B, IQB)     (_IQNmpyIQx( (A),  (IQA),  (B),  (IQB),  14))
#define _IQ13mpyIQX(A, IQA, B, IQB)     (_IQNmpyIQx( (A),  (IQA),  (B),  (IQB),  13))
#define _IQ12mpyIQX(A, IQA, B, IQB)     (_IQNmpyIQx( (A),  (IQA),  (B),  (IQB),  12))
#define _IQ11mpyIQX(A, IQA, B, IQB)     (_IQNmpyIQx( (A),  (IQA),  (B),  (IQB),  11))
#define _IQ10mpyIQX(A, IQA, B, IQB)     (_IQNmpyIQx( (A),  (IQA),  (B),  (IQB),  10))
#define _IQ9mpyIQX(A, IQA, B, IQB)      (_IQNmpyIQx( (A),  (IQA),  (B),  (IQB),  9))
#define _IQ8mpyIQX(A, IQA, B, IQB)      (_IQNmpyIQx( (A),  (IQA),  (B),  (IQB),  8))
#define _IQ7mpyIQX(A, IQA, B, IQB)      (_IQNmpyIQx( (A),  (IQA),  (B),  (IQB),  7))
#define _IQ6mpyIQX(A, IQA, B, IQB)      (_IQNmpyIQx( (A),  (IQA),  (B),  (IQB),  6))
#define _IQ5mpyIQX(A, IQA, B, IQB)      (_IQNmpyIQx( (A),  (IQA),  (B),  (IQB),  5))
#define _IQ4mpyIQX(A, IQA, B, IQB)      (_IQNmpyIQx( (A),  (IQA),  (B),  (IQB),  4))
#define _IQ3mpyIQX(A, IQA, B, IQB)      (_IQNmpyIQx( (A),  (IQA),  (B),  (IQB),  3))
#define _IQ2mpyIQX(A, IQA, B, IQB)      (_IQNmpyIQx( (A),  (IQA),  (B),  (IQB),  2))
#define _IQ1mpyIQX(A, IQA, B, IQB)      (_IQNmpyIQx( (A),  (IQA),  (B),  (IQB),  1))
#define _IQ0mpyIQX(A, IQA, B, IQB)      (_IQNmpyIQx( (A),  (IQA),  (B),  (IQB),  0))

#if GLOBAL_Q == 0
#define   _IQmpyIQx( A,  IQA,  B,  IQB) (_IQ0mpyIQX((A), (IQA), (B), (IQB)))
#endif

#if GLOBAL_Q == 1
#define   _IQmpyIQx( A,  IQA,  B,  IQB) (_IQ1mpyIQX((A), (IQA), (B), (IQB)))
#endif

#if GLOBAL_Q == 2
#define   _IQmpyIQx( A,  IQA,  B,  IQB) (_IQ2mpyIQX((A), (IQA), (B), (IQB)))
#endif

#if GLOBAL_Q == 3
#define   _IQmpyIQx( A,  IQA,  B,  IQB) (_IQ3mpyIQX((A), (IQA), (B), (IQB)))
#endif

#if GLOBAL_Q == 4
#define   _IQmpyIQx( A,  IQA,  B,  IQB) (_IQ4mpyIQX((A), (IQA), (B), (IQB)))
#endif

#if GLOBAL_Q == 5
#define   _IQmpyIQx( A,  IQA,  B,  IQB) (_IQ5mpyIQX((A), (IQA), (B), (IQB)))
#endif

#if GLOBAL_Q == 6
#define   _IQmpyIQx( A,  IQA,  B,  IQB) (_IQ6mpyIQX((A), (IQA), (B), (IQB)))
#endif

#if GLOBAL_Q == 7
#define   _IQmpyIQx( A,  IQA,  B,  IQB) (_IQ7mpyIQX((A), (IQA), (B), (IQB)))
#endif

#if GLOBAL_Q == 8
#define   _IQmpyIQx( A,  IQA,  B,  IQB) (_IQ8mpyIQX((A), (IQA), (B), (IQB)))
#endif

#if GLOBAL_Q == 9
#define   _IQmpyIQx( A,  IQA,  B,  IQB) (_IQ9mpyIQX((A), (IQA), (B), (IQB)))
#endif

#if GLOBAL_Q == 10
#define   _IQmpyIQx( A,  IQA,  B,  IQB) (_IQ10mpyIQX((A), (IQA), (B), (IQB)))
#endif

#if GLOBAL_Q == 11
#define   _IQmpyIQx( A,  IQA,  B,  IQB) (_IQ11mpyIQX((A), (IQA), (B), (IQB)))
#endif

#if GLOBAL_Q == 12
#define   _IQmpyIQx( A,  IQA,  B,  IQB) (_IQ12mpyIQX((A), (IQA), (B), (IQB)))
#endif

#if GLOBAL_Q == 13
#define   _IQmpyIQx( A,  IQA,  B,  IQB) (_IQ13mpyIQX((A), (IQA), (B), (IQB)))
#endif

#if GLOBAL_Q == 14
#define   _IQmpyIQx( A,  IQA,  B,  IQB) (_IQ14mpyIQX((A), (IQA), (B), (IQB)))
#endif


#if GLOBAL_Q == 15
#define   _IQmpyIQx( A,  IQA,  B,  IQB) (_IQ15mpyIQX((A), (IQA), (B), (IQB)))
#endif

#if GLOBAL_Q == 16
#define   _IQmpyIQx( A,  IQA,  B,  IQB) (_IQ16mpyIQX((A), (IQA), (B), (IQB)))
#endif

#if GLOBAL_Q == 17
#define   _IQmpyIQx( A,  IQA,  B,  IQB) (_IQ17mpyIQX((A), (IQA), (B), (IQB)))
#endif

#if GLOBAL_Q == 18
#define   _IQmpyIQx( A,  IQA,  B,  IQB) (_IQ18mpyIQX((A), (IQA), (B), (IQB)))
#endif

#if GLOBAL_Q == 19
#define   _IQmpyIQx( A,  IQA,  B,  IQB) (_IQ19mpyIQX((A), (IQA), (B), (IQB)))
#endif

#if GLOBAL_Q == 20
#define   _IQmpyIQx( A,  IQA,  B,  IQB) (_IQ20mpyIQX((A), (IQA), (B), (IQB)))
#endif

#if GLOBAL_Q == 21
#define   _IQmpyIQx( A,  IQA,  B,  IQB) (_IQ21mpyIQX((A), (IQA), (B), (IQB)))
#endif

#if GLOBAL_Q == 22
#define   _IQmpyIQx( A,  IQA,  B,  IQB) (_IQ22mpyIQX((A), (IQA), (B), (IQB)))
#endif

#if GLOBAL_Q == 23
#define   _IQmpyIQx( A,  IQA,  B,  IQB) (_IQ23mpyIQX((A), (IQA), (B), (IQB)))
#endif

#if GLOBAL_Q == 24
#define   _IQmpyIQx( A,  IQA,  B,  IQB) (_IQ24mpyIQX((A), (IQA), (B), (IQB)))
#endif

#if GLOBAL_Q == 25
#define   _IQmpyIQx( A,  IQA,  B,  IQB) (_IQ25mpyIQX((A), (IQA), (B), (IQB)))
#endif

#if GLOBAL_Q == 26
#define   _IQmpyIQx( A,  IQA,  B,  IQB) (_IQ26mpyIQX((A), (IQA), (B), (IQB)))
#endif

#if GLOBAL_Q == 27
#define   _IQmpyIQx( A,  IQA,  B,  IQB) (_IQ27mpyIQX((A), (IQA), (B), (IQB)))
#endif

#if GLOBAL_Q == 28
#define   _IQmpyIQx( A,  IQA,  B,  IQB) (_IQ28mpyIQX((A), (IQA), (B), (IQB)))
#endif

#if GLOBAL_Q == 29
#define   _IQmpyIQx( A,  IQA,  B,  IQB) (_IQ29mpyIQX((A), (IQA), (B), (IQB)))
#endif

#if GLOBAL_Q == 30
#define   _IQmpyIQx( A,  IQA,  B,  IQB) (_IQ30mpyIQX((A), (IQA), (B), (IQB)))
#endif

#if GLOBAL_Q == 31
#define   _IQmpyIQx( A,  IQA,  B,  IQB) (_IQ31mpyIQX((A), (IQA), (B), (IQB)))
#endif



//---------------------------------------------------------------------------
#define   _IQmpyI32(A,B)    ((A)*(B))
#define   _IQ31mpyI32(A,B)  ((A)*(B))
#define   _IQ30mpyI32(A,B)  ((A)*(B))
#define   _IQ29mpyI32(A,B)  ((A)*(B))
#define   _IQ28mpyI32(A,B)  ((A)*(B))
#define   _IQ27mpyI32(A,B)  ((A)*(B))
#define   _IQ26mpyI32(A,B)  ((A)*(B))
#define   _IQ25mpyI32(A,B)  ((A)*(B))
#define   _IQ24mpyI32(A,B)  ((A)*(B))
#define   _IQ23mpyI32(A,B)  ((A)*(B))
#define   _IQ22mpyI32(A,B)  ((A)*(B))
#define   _IQ21mpyI32(A,B)  ((A)*(B))
#define   _IQ20mpyI32(A,B)  ((A)*(B))
#define   _IQ19mpyI32(A,B)  ((A)*(B))
#define   _IQ18mpyI32(A,B)  ((A)*(B))
#define   _IQ17mpyI32(A,B)  ((A)*(B))
#define   _IQ16mpyI32(A,B)  ((A)*(B))
#define   _IQ15mpyI32(A,B)  ((A)*(B))
#define   _IQ14mpyI32(A,B)  ((A)*(B))
#define   _IQ13mpyI32(A,B)  ((A)*(B))
#define   _IQ12mpyI32(A,B)  ((A)*(B))
#define   _IQ11mpyI32(A,B)  ((A)*(B))
#define   _IQ10mpyI32(A,B)  ((A)*(B))
#define   _IQ9mpyI32(A,B)   ((A)*(B))
#define   _IQ8mpyI32(A,B)   ((A)*(B))
#define   _IQ7mpyI32(A,B)   ((A)*(B))
#define   _IQ6mpyI32(A,B)   ((A)*(B))
#define   _IQ5mpyI32(A,B)   ((A)*(B))
#define   _IQ4mpyI32(A,B)   ((A)*(B))
#define   _IQ3mpyI32(A,B)   ((A)*(B))
#define   _IQ2mpyI32(A,B)   ((A)*(B))
#define   _IQ1mpyI32(A,B)   ((A)*(B))
#define   _IQ0mpyI32(A,B)   ((A)*(B))
//---------------------------------------------------------------------------
#ifdef _INLINE_IQMATH
static inline I32_IQ _IQNmpyI32int(I32_IQ x, I32_IQ y, U32_IQ qfmt);
#else
I32_IQ _IQNmpyI32int(I32_IQ x, I32_IQ y, U32_IQ qfmt);
#endif

#define _IQ31mpyI32int(A,B)     (_IQNmpyI32int((A),(B),31))
#define _IQ30mpyI32int(A,B)     (_IQNmpyI32int((A),(B),30))
#define _IQ29mpyI32int(A,B)     (_IQNmpyI32int((A),(B),29))
#define _IQ28mpyI32int(A,B)     (_IQNmpyI32int((A),(B),28))
#define _IQ27mpyI32int(A,B)     (_IQNmpyI32int((A),(B),27))
#define _IQ26mpyI32int(A,B)     (_IQNmpyI32int((A),(B),26))
#define _IQ25mpyI32int(A,B)     (_IQNmpyI32int((A),(B),25))
#define _IQ24mpyI32int(A,B)     (_IQNmpyI32int((A),(B),24))
#define _IQ23mpyI32int(A,B)     (_IQNmpyI32int((A),(B),23))
#define _IQ22mpyI32int(A,B)     (_IQNmpyI32int((A),(B),22))
#define _IQ21mpyI32int(A,B)     (_IQNmpyI32int((A),(B),21))
#define _IQ20mpyI32int(A,B)     (_IQNmpyI32int((A),(B),20))
#define _IQ19mpyI32int(A,B)     (_IQNmpyI32int((A),(B),19))
#define _IQ18mpyI32int(A,B)     (_IQNmpyI32int((A),(B),18))
#define _IQ17mpyI32int(A,B)     (_IQNmpyI32int((A),(B),17))
#define _IQ16mpyI32int(A,B)     (_IQNmpyI32int((A),(B),16))
#define _IQ15mpyI32int(A,B)     (_IQNmpyI32int((A),(B),15))
#define _IQ14mpyI32int(A,B)     (_IQNmpyI32int((A),(B),14))
#define _IQ13mpyI32int(A,B)     (_IQNmpyI32int((A),(B),13))
#define _IQ12mpyI32int(A,B)     (_IQNmpyI32int((A),(B),12))
#define _IQ11mpyI32int(A,B)     (_IQNmpyI32int((A),(B),11))
#define _IQ10mpyI32int(A,B)     (_IQNmpyI32int((A),(B),10))
#define _IQ9mpyI32int(A,B)  (_IQNmpyI32int((A),(B),9))
#define _IQ8mpyI32int(A,B)  (_IQNmpyI32int((A),(B),8))
#define _IQ7mpyI32int(A,B)  (_IQNmpyI32int((A),(B),7))
#define _IQ6mpyI32int(A,B)  (_IQNmpyI32int((A),(B),6))
#define _IQ5mpyI32int(A,B)  (_IQNmpyI32int((A),(B),5))
#define _IQ4mpyI32int(A,B)  (_IQNmpyI32int((A),(B),4))
#define _IQ3mpyI32int(A,B)  (_IQNmpyI32int((A),(B),3))
#define _IQ2mpyI32int(A,B)  (_IQNmpyI32int((A),(B),2))
#define _IQ1mpyI32int(A,B)  (_IQNmpyI32int((A),(B),1))
#define _IQ0mpyI32int(A,B)  (_IQNmpyI32int((A),(B),0))


#if GLOBAL_Q == 31
#define   _IQmpyI32int(A,B)  (_IQ31mpyI32int((A),(B)))
#endif
#if GLOBAL_Q == 30
#define   _IQmpyI32int(A,B)  (_IQ30mpyI32int((A),(B)))
#endif
#if GLOBAL_Q == 29
#define   _IQmpyI32int(A,B)  (_IQ29mpyI32int((A),(B)))
#endif
#if GLOBAL_Q == 28
#define   _IQmpyI32int(A,B)  (_IQ28mpyI32int((A),(B)))
#endif
#if GLOBAL_Q == 27
#define   _IQmpyI32int(A,B)  (_IQ27mpyI32int((A),(B)))
#endif
#if GLOBAL_Q == 26
#define   _IQmpyI32int(A,B)  (_IQ26mpyI32int((A),(B)))
#endif
#if GLOBAL_Q == 25
#define   _IQmpyI32int(A,B)  (_IQ25mpyI32int((A),(B)))
#endif
#if GLOBAL_Q == 24
#define   _IQmpyI32int(A,B)  (_IQ24mpyI32int((A),(B)))
#endif
#if GLOBAL_Q == 23
#define   _IQmpyI32int(A,B)  (_IQ23mpyI32int((A),(B)))
#endif
#if GLOBAL_Q == 22
#define   _IQmpyI32int(A,B)  (_IQ22mpyI32int((A),(B)))
#endif
#if GLOBAL_Q == 21
#define   _IQmpyI32int(A,B)  (_IQ21mpyI32int((A),(B)))
#endif
#if GLOBAL_Q == 20
#define   _IQmpyI32int(A,B)  (_IQ20mpyI32int((A),(B)))
#endif
#if GLOBAL_Q == 19
#define   _IQmpyI32int(A,B)  (_IQ19mpyI32int((A),(B)))
#endif
#if GLOBAL_Q == 18
#define   _IQmpyI32int(A,B)  (_IQ18mpyI32int((A),(B)))
#endif
#if GLOBAL_Q == 17
#define   _IQmpyI32int(A,B)  (_IQ17mpyI32int((A),(B)))
#endif
#if GLOBAL_Q == 16
#define   _IQmpyI32int(A,B)  (_IQ16mpyI32int((A),(B)))
#endif
#if GLOBAL_Q == 15
#define   _IQmpyI32int(A,B)  (_IQ15mpyI32int((A),(B)))
#endif
#if GLOBAL_Q == 14
#define   _IQmpyI32int(A,B)  (_IQ14mpyI32int((A),(B)))
#endif
#if GLOBAL_Q == 13
#define   _IQmpyI32int(A,B)  (_IQ13mpyI32int((A),(B)))
#endif
#if GLOBAL_Q == 12
#define   _IQmpyI32int(A,B)  (_IQ12mpyI32int((A),(B)))
#endif
#if GLOBAL_Q == 11
#define   _IQmpyI32int(A,B)  (_IQ11mpyI32int((A),(B)))
#endif
#if GLOBAL_Q == 10
#define   _IQmpyI32int(A,B)  (_IQ10mpyI32int((A),(B)))
#endif
#if GLOBAL_Q == 9
#define   _IQmpyI32int(A,B)  (_IQ9mpyI32int((A),(B)))
#endif
#if GLOBAL_Q == 8
#define   _IQmpyI32int(A,B)  (_IQ8mpyI32int((A),(B)))
#endif
#if GLOBAL_Q == 7
#define   _IQmpyI32int(A,B)  (_IQ7mpyI32int((A),(B)))
#endif
#if GLOBAL_Q == 6
#define   _IQmpyI32int(A,B)  (_IQ6mpyI32int((A),(B)))
#endif
#if GLOBAL_Q == 5
#define   _IQmpyI32int(A,B)  (_IQ5mpyI32int((A),(B)))
#endif
#if GLOBAL_Q == 4
#define   _IQmpyI32int(A,B)  (_IQ4mpyI32int((A),(B)))
#endif
#if GLOBAL_Q == 3
#define   _IQmpyI32int(A,B)  (_IQ3mpyI32int((A),(B)))
#endif
#if GLOBAL_Q == 2
#define   _IQmpyI32int(A,B)  (_IQ2mpyI32int((A),(B)))
#endif
#if GLOBAL_Q == 1
#define   _IQmpyI32int(A,B)  (_IQ1mpyI32int((A),(B)))
#endif
#if GLOBAL_Q == 0
#define   _IQmpyI32int(A,B)  (_IQ0mpyI32int((A),(B)))
#endif
//---------------------------------------------------------------------------
#ifdef _INLINE_IQMATH 
static inline I32_IQ _IQNmpyI32frac(I32_IQ x, I32_IQ y, U32_IQ qfmt);
#else
I32_IQ _IQNmpyI32frac(I32_IQ x, I32_IQ y, U32_IQ qfmt);
#endif

#define _IQ31mpyI32frac(A,B)    (_IQNmpyI32frac((A),(B),31))
#define _IQ30mpyI32frac(A,B)    (_IQNmpyI32frac((A),(B),30))
#define _IQ29mpyI32frac(A,B)    (_IQNmpyI32frac((A),(B),29))
#define _IQ28mpyI32frac(A,B)    (_IQNmpyI32frac((A),(B),28))
#define _IQ27mpyI32frac(A,B)    (_IQNmpyI32frac((A),(B),27))
#define _IQ26mpyI32frac(A,B)    (_IQNmpyI32frac((A),(B),26))
#define _IQ25mpyI32frac(A,B)    (_IQNmpyI32frac((A),(B),25))
#define _IQ24mpyI32frac(A,B)    (_IQNmpyI32frac((A),(B),24))
#define _IQ23mpyI32frac(A,B)    (_IQNmpyI32frac((A),(B),23))
#define _IQ22mpyI32frac(A,B)    (_IQNmpyI32frac((A),(B),22))
#define _IQ21mpyI32frac(A,B)    (_IQNmpyI32frac((A),(B),21))
#define _IQ20mpyI32frac(A,B)    (_IQNmpyI32frac((A),(B),20))
#define _IQ19mpyI32frac(A,B)    (_IQNmpyI32frac((A),(B),19))
#define _IQ18mpyI32frac(A,B)    (_IQNmpyI32frac((A),(B),18))
#define _IQ17mpyI32frac(A,B)    (_IQNmpyI32frac((A),(B),17))
#define _IQ16mpyI32frac(A,B)    (_IQNmpyI32frac((A),(B),16))
#define _IQ15mpyI32frac(A,B)    (_IQNmpyI32frac((A),(B),15))
#define _IQ14mpyI32frac(A,B)    (_IQNmpyI32frac((A),(B),14))
#define _IQ13mpyI32frac(A,B)    (_IQNmpyI32frac((A),(B),13))
#define _IQ12mpyI32frac(A,B)    (_IQNmpyI32frac((A),(B),12))
#define _IQ11mpyI32frac(A,B)    (_IQNmpyI32frac((A),(B),11))
#define _IQ10mpyI32frac(A,B)    (_IQNmpyI32frac((A),(B),10))
#define _IQ9mpyI32frac(A,B)     (_IQNmpyI32frac((A),(B),9))
#define _IQ8mpyI32frac(A,B)     (_IQNmpyI32frac((A),(B),8))
#define _IQ7mpyI32frac(A,B)     (_IQNmpyI32frac((A),(B),7))
#define _IQ6mpyI32frac(A,B)     (_IQNmpyI32frac((A),(B),6))
#define _IQ5mpyI32frac(A,B)     (_IQNmpyI32frac((A),(B),5))
#define _IQ4mpyI32frac(A,B)     (_IQNmpyI32frac((A),(B),4))
#define _IQ3mpyI32frac(A,B)     (_IQNmpyI32frac((A),(B),3))
#define _IQ2mpyI32frac(A,B)     (_IQNmpyI32frac((A),(B),2))
#define _IQ1mpyI32frac(A,B)     (_IQNmpyI32frac((A),(B),1))
#define _IQ0mpyI32frac(A,B)     (_IQNmpyI32frac((A),(B),0))

#if GLOBAL_Q == 31
#define   _IQmpyI32frac(A,B)  (_IQ31mpyI32frac((A),(B)))
#endif
#if GLOBAL_Q == 30
#define   _IQmpyI32frac(A,B)  (_IQ30mpyI32frac((A),(B)))
#endif
#if GLOBAL_Q == 29
#define   _IQmpyI32frac(A,B)  (_IQ29mpyI32frac((A),(B)))
#endif
#if GLOBAL_Q == 28
#define   _IQmpyI32frac(A,B)  (_IQ28mpyI32frac((A),(B)))
#endif
#if GLOBAL_Q == 27
#define   _IQmpyI32frac(A,B)  (_IQ27mpyI32frac((A),(B)))
#endif
#if GLOBAL_Q == 26
#define   _IQmpyI32frac(A,B)  (_IQ26mpyI32frac((A),(B)))
#endif
#if GLOBAL_Q == 25
#define   _IQmpyI32frac(A,B)  (_IQ25mpyI32frac((A),(B)))
#endif
#if GLOBAL_Q == 24
#define   _IQmpyI32frac(A,B)  (_IQ24mpyI32frac((A),(B)))
#endif
#if GLOBAL_Q == 23
#define   _IQmpyI32frac(A,B)  (_IQ23mpyI32frac((A),(B)))
#endif
#if GLOBAL_Q == 22
#define   _IQmpyI32frac(A,B)  (_IQ22mpyI32frac((A),(B)))
#endif
#if GLOBAL_Q == 21
#define   _IQmpyI32frac(A,B)  (_IQ21mpyI32frac((A),(B)))
#endif
#if GLOBAL_Q == 20
#define   _IQmpyI32frac(A,B)  (_IQ20mpyI32frac((A),(B)))
#endif
#if GLOBAL_Q == 19
#define   _IQmpyI32frac(A,B)  (_IQ19mpyI32frac((A),(B)))
#endif
#if GLOBAL_Q == 18
#define   _IQmpyI32frac(A,B)  (_IQ18mpyI32frac((A),(B)))
#endif
#if GLOBAL_Q == 17
#define   _IQmpyI32frac(A,B)  (_IQ17mpyI32frac((A),(B)))
#endif
#if GLOBAL_Q == 16
#define   _IQmpyI32frac(A,B)  (_IQ16mpyI32frac((A),(B)))
#endif
#if GLOBAL_Q == 15
#define   _IQmpyI32frac(A,B)  (_IQ15mpyI32frac((A),(B)))
#endif
#if GLOBAL_Q == 14
#define   _IQmpyI32frac(A,B)  (_IQ14mpyI32frac((A),(B)))
#endif
#if GLOBAL_Q == 13
#define   _IQmpyI32frac(A,B)  (_IQ13mpyI32frac((A),(B)))
#endif
#if GLOBAL_Q == 12
#define   _IQmpyI32frac(A,B)  (_IQ12mpyI32frac((A),(B)))
#endif
#if GLOBAL_Q == 11
#define   _IQmpyI32frac(A,B)  (_IQ11mpyI32frac((A),(B)))
#endif
#if GLOBAL_Q == 10
#define   _IQmpyI32frac(A,B)  (_IQ10mpyI32frac((A),(B)))
#endif
#if GLOBAL_Q == 9
#define   _IQmpyI32frac(A,B)  (_IQ9mpyI32frac((A),(B)))
#endif
#if GLOBAL_Q == 8
#define   _IQmpyI32frac(A,B)  (_IQ8mpyI32frac((A),(B)))
#endif
#if GLOBAL_Q == 7
#define   _IQmpyI32frac(A,B)  (_IQ7mpyI32frac((A),(B)))
#endif
#if GLOBAL_Q == 6
#define   _IQmpyI32frac(A,B)  (_IQ6mpyI32frac((A),(B)))
#endif
#if GLOBAL_Q == 5
#define   _IQmpyI32frac(A,B)  (_IQ5mpyI32frac((A),(B)))
#endif
#if GLOBAL_Q == 4
#define   _IQmpyI32frac(A,B)  (_IQ4mpyI32frac((A),(B)))
#endif
#if GLOBAL_Q == 3
#define   _IQmpyI32frac(A,B)  (_IQ3mpyI32frac((A),(B)))
#endif
#if GLOBAL_Q == 2
#define   _IQmpyI32frac(A,B)  (_IQ2mpyI32frac((A),(B)))
#endif
#if GLOBAL_Q == 1
#define   _IQmpyI32frac(A,B)  (_IQ1mpyI32frac((A),(B)))
#endif
#if GLOBAL_Q == 0
#define   _IQmpyI32frac(A,B)  (_IQ0mpyI32frac((A),(B)))
#endif

//---------------------------------------------------------------------------
#ifdef _INLINE_IQMATH
static inline I32_IQ _IQNmag(I32_IQ x, I32_IQ y, U32_IQ qfmt);
#else
I32_IQ _IQNmag(I32_IQ x, I32_IQ y, U32_IQ qfmt);
#endif

#define _IQ30mag(A,B)  ( _IQNmag((A),(B),30))
#define _IQ29mag(A,B)  ( _IQNmag((A),(B),29))
#define _IQ28mag(A,B)  ( _IQNmag((A),(B),28))
#define _IQ27mag(A,B)  ( _IQNmag((A),(B),27))
#define _IQ26mag(A,B)  ( _IQNmag((A),(B),26))
#define _IQ25mag(A,B)  ( _IQNmag((A),(B),25))
#define _IQ24mag(A,B)  ( _IQNmag((A),(B),24))
#define _IQ23mag(A,B)  ( _IQNmag((A),(B),23))
#define _IQ22mag(A,B)  ( _IQNmag((A),(B),22))
#define _IQ21mag(A,B)  ( _IQNmag((A),(B),21))
#define _IQ20mag(A,B)  ( _IQNmag((A),(B),20))
#define _IQ19mag(A,B)  ( _IQNmag((A),(B),19))
#define _IQ18mag(A,B)  ( _IQNmag((A),(B),18))
#define _IQ17mag(A,B)  ( _IQNmag((A),(B),17))
#define _IQ16mag(A,B)  ( _IQNmag((A),(B),16))
#define _IQ15mag(A,B)  ( _IQNmag((A),(B),15))
#define _IQ14mag(A,B)  ( _IQNmag((A),(B),14))
#define _IQ13mag(A,B)  ( _IQNmag((A),(B),13))
#define _IQ12mag(A,B)  ( _IQNmag((A),(B),12))
#define _IQ11mag(A,B)  ( _IQNmag((A),(B),11))
#define _IQ10mag(A,B)  ( _IQNmag((A),(B),10))
#define _IQ9mag(A,B)   ( _IQNmag((A),(B),9))
#define _IQ8mag(A,B)   ( _IQNmag((A),(B),8))
#define _IQ7mag(A,B)   ( _IQNmag((A),(B),7))
#define _IQ6mag(A,B)   ( _IQNmag((A),(B),6))
#define _IQ5mag(A,B)   ( _IQNmag((A),(B),5))
#define _IQ4mag(A,B)   ( _IQNmag((A),(B),4))
#define _IQ3mag(A,B)   ( _IQNmag((A),(B),3))
#define _IQ2mag(A,B)   ( _IQNmag((A),(B),2))
#define _IQ1mag(A,B)   ( _IQNmag((A),(B),1))
#define _IQ0mag(A,B)   ( _IQNmag((A),(B),0))

#if GLOBAL_Q == 30
#define   _IQmag(A,B)  (_IQ30mag((A),(B)))
#endif
#if GLOBAL_Q == 29
#define   _IQmag(A,B)  (_IQ29mag((A),(B)))
#endif
#if GLOBAL_Q == 28
#define   _IQmag(A,B)  (_IQ28mag((A),(B)))
#endif
#if GLOBAL_Q == 27
#define   _IQmag(A,B)  (_IQ27mag((A),(B)))
#endif
#if GLOBAL_Q == 26
#define   _IQmag(A,B)  (_IQ26mag((A),(B)))
#endif
#if GLOBAL_Q == 25
#define   _IQmag(A,B)  (_IQ25mag((A),(B)))
#endif
#if GLOBAL_Q == 24
#define   _IQmag(A,B)  (_IQ24mag((A),(B)))
#endif
#if GLOBAL_Q == 23
#define   _IQmag(A,B)  (_IQ23mag((A),(B)))
#endif
#if GLOBAL_Q == 22
#define   _IQmag(A,B)  (_IQ22mag((A),(B)))
#endif
#if GLOBAL_Q == 21
#define   _IQmag(A,B)  (_IQ21mag((A),(B)))
#endif
#if GLOBAL_Q == 20
#define   _IQmag(A,B)  (_IQ20mag((A),(B)))
#endif
#if GLOBAL_Q == 19
#define   _IQmag(A,B)  (_IQ19mag((A),(B)))
#endif
#if GLOBAL_Q == 18
#define   _IQmag(A,B)  (_IQ18mag((A),(B)))
#endif
#if GLOBAL_Q == 17
#define   _IQmag(A,B)  (_IQ17mag((A),(B)))
#endif
#if GLOBAL_Q == 16
#define   _IQmag(A,B)  (_IQ16mag((A),(B)))
#endif
#if GLOBAL_Q == 15
#define   _IQmag(A,B)  (_IQ15mag((A),(B)))
#endif
#if GLOBAL_Q == 14
#define   _IQmag(A,B)  (_IQ14mag((A),(B)))
#endif
#if GLOBAL_Q == 13
#define   _IQmag(A,B)  (_IQ13mag((A),(B)))
#endif
#if GLOBAL_Q == 12
#define   _IQmag(A,B)  (_IQ12mag((A),(B)))
#endif
#if GLOBAL_Q == 11
#define   _IQmag(A,B)  (_IQ11mag((A),(B)))
#endif
#if GLOBAL_Q == 10
#define   _IQmag(A,B)  (_IQ10mag((A),(B)))
#endif
#if GLOBAL_Q == 9
#define   _IQmag(A,B)  (_IQ9mag((A),(B)))
#endif
#if GLOBAL_Q == 8
#define   _IQmag(A,B)  (_IQ8mag((A),(B)))
#endif
#if GLOBAL_Q == 7
#define   _IQmag(A,B)  (_IQ7mag((A),(B)))
#endif
#if GLOBAL_Q == 6
#define   _IQmag(A,B)  (_IQ6mag((A),(B)))
#endif
#if GLOBAL_Q == 5
#define   _IQmag(A,B)  (_IQ5mag((A),(B)))
#endif
#if GLOBAL_Q == 4
#define   _IQmag(A,B)  (_IQ4mag((A),(B)))
#endif
#if GLOBAL_Q == 3
#define   _IQmag(A,B)  (_IQ3mag((A),(B)))
#endif
#if GLOBAL_Q == 2
#define   _IQmag(A,B)  (_IQ2mag((A),(B)))
#endif
#if GLOBAL_Q == 1
#define   _IQmag(A,B)  (_IQ1mag((A),(B)))
#endif
#if GLOBAL_Q == 0
#define   _IQmag(A,B)  (_IQ0mag((A),(B)))
#endif
//---------------------------------------------------------------------------

#ifdef _INLINE_IQMATH
static inline      I32_IQ _atoIQN(const char *A, Uword32 q_value);
#else
    I32_IQ _atoIQN(const char *A, Uword32 q_value);
#endif
#define   _atoIQ(A)    (_atoIQN((A), GLOBAL_Q))
#define   _atoIQ31(A)  (_atoIQN((A), 31))
#define   _atoIQ30(A)  (_atoIQN((A), 30))
#define   _atoIQ29(A)  (_atoIQN((A), 29))
#define   _atoIQ28(A)  (_atoIQN((A), 28))
#define   _atoIQ27(A)  (_atoIQN((A), 27))
#define   _atoIQ26(A)  (_atoIQN((A), 26))
#define   _atoIQ25(A)  (_atoIQN((A), 25))
#define   _atoIQ24(A)  (_atoIQN((A), 24))
#define   _atoIQ23(A)  (_atoIQN((A), 23))
#define   _atoIQ22(A)  (_atoIQN((A), 22))
#define   _atoIQ21(A)  (_atoIQN((A), 21))
#define   _atoIQ20(A)  (_atoIQN((A), 20))
#define   _atoIQ19(A)  (_atoIQN((A), 19))
#define   _atoIQ18(A)  (_atoIQN((A), 18))
#define   _atoIQ17(A)  (_atoIQN((A), 17))
#define   _atoIQ16(A)  (_atoIQN((A), 16))
#define   _atoIQ15(A)  (_atoIQN((A), 15))
#define   _atoIQ14(A)  (_atoIQN((A), 14))
#define   _atoIQ13(A)  (_atoIQN((A), 13))
#define   _atoIQ12(A)  (_atoIQN((A), 12))
#define   _atoIQ11(A)  (_atoIQN((A), 11))
#define   _atoIQ10(A)  (_atoIQN((A), 10))
#define   _atoIQ9(A)   (_atoIQN((A), 9))
#define   _atoIQ8(A)   (_atoIQN((A), 8))
#define   _atoIQ7(A)   (_atoIQN((A), 7))
#define   _atoIQ6(A)   (_atoIQN((A), 6))
#define   _atoIQ5(A)   (_atoIQN((A), 5))
#define   _atoIQ4(A)   (_atoIQN((A), 4))
#define   _atoIQ3(A)   (_atoIQN((A), 3))
#define   _atoIQ2(A)   (_atoIQN((A), 2))
#define   _atoIQ1(A)   (_atoIQN((A), 1))
#define   _atoIQ0(A)   (_atoIQN((A), 0))

//---------------------------------------------------------------------------
#ifdef _INLINE_IQMATH
static inline I32_IQ _IQNabs(I32_IQ input);
#else
I32_IQ _IQNabs(I32_IQ input);
#endif
#define   _IQabs(A)    (_IQNabs(A))
#define   _IQ31abs(A)  (_IQNabs(A))
#define   _IQ30abs(A)  (_IQNabs(A))
#define   _IQ29abs(A)  (_IQNabs(A))
#define   _IQ28abs(A)  (_IQNabs(A))
#define   _IQ27abs(A)  (_IQNabs(A))
#define   _IQ26abs(A)  (_IQNabs(A))
#define   _IQ25abs(A)  (_IQNabs(A))
#define   _IQ24abs(A)  (_IQNabs(A))
#define   _IQ23abs(A)  (_IQNabs(A))
#define   _IQ22abs(A)  (_IQNabs(A))
#define   _IQ21abs(A)  (_IQNabs(A))
#define   _IQ20abs(A)  (_IQNabs(A))
#define   _IQ19abs(A)  (_IQNabs(A))
#define   _IQ18abs(A)  (_IQNabs(A))
#define   _IQ17abs(A)  (_IQNabs(A))
#define   _IQ16abs(A)  (_IQNabs(A))
#define   _IQ15abs(A)  (_IQNabs(A))
#define   _IQ14abs(A)  (_IQNabs(A))
#define   _IQ13abs(A)  (_IQNabs(A))
#define   _IQ12abs(A)  (_IQNabs(A))
#define   _IQ11abs(A)  (_IQNabs(A))
#define   _IQ10abs(A)  (_IQNabs(A))
#define   _IQ9abs(A)   (_IQNabs(A))
#define   _IQ8abs(A)   (_IQNabs(A))
#define   _IQ7abs(A)   (_IQNabs(A))
#define   _IQ6abs(A)   (_IQNabs(A))
#define   _IQ5abs(A)   (_IQNabs(A))
#define   _IQ4abs(A)   (_IQNabs(A))
#define   _IQ3abs(A)   (_IQNabs(A))
#define   _IQ2abs(A)   (_IQNabs(A))
#define   _IQ1abs(A)   (_IQNabs(A))
#define   _IQ0abs(A)   (_IQNabs(A))


//###########################################################################
#else   // MATH_TYPE == FLOAT_MATH
//###########################################################################
// If FLOAT_MATH is used, the IQmath library function are replaced by
// equivalent floating point operations:
//===========================================================================
typedef   float   _iq;
typedef   float   _iq30;
typedef   float   _iq29;
typedef   float   _iq28;
typedef   float   _iq27;
typedef   float   _iq26;
typedef   float   _iq25;
typedef   float   _iq24;
typedef   float   _iq23;
typedef   float   _iq22;
typedef   float   _iq21;
typedef   float   _iq20;
typedef   float   _iq19;
typedef   float   _iq18;
typedef   float   _iq17;
typedef   float   _iq16;
typedef   float   _iq15;
typedef   float   _iq14;
typedef   float   _iq13;
typedef   float   _iq12;
typedef   float   _iq11;
typedef   float   _iq10;
typedef   float   _iq9;
typedef   float   _iq8;
typedef   float   _iq7;
typedef   float   _iq6;
typedef   float   _iq5;
typedef   float   _iq4;
typedef   float   _iq3;
typedef   float   _iq2;
typedef   float   _iq1;
//---------------------------------------------------------------------------
#define   _IQ(A)         (A)
#define   _IQ30(A)       (A)
#define   _IQ29(A)       (A)
#define   _IQ28(A)       (A)
#define   _IQ27(A)       (A)
#define   _IQ26(A)       (A)
#define   _IQ25(A)       (A)
#define   _IQ24(A)       (A)
#define   _IQ23(A)       (A)
#define   _IQ22(A)       (A)
#define   _IQ21(A)       (A)
#define   _IQ20(A)       (A)
#define   _IQ19(A)       (A)
#define   _IQ18(A)       (A)
#define   _IQ17(A)       (A)
#define   _IQ16(A)       (A)
#define   _IQ15(A)       (A)
#define   _IQ14(A)       (A)
#define   _IQ13(A)       (A)
#define   _IQ12(A)       (A)
#define   _IQ10(A)       (A)
#define   _IQ9(A)        (A)
#define   _IQ8(A)        (A)
#define   _IQ7(A)        (A)
#define   _IQ6(A)        (A)
#define   _IQ5(A)        (A)
#define   _IQ4(A)        (A)
#define   _IQ3(A)        (A)
#define   _IQ2(A)        (A)
#define   _IQ1(A)        (A)
//---------------------------------------------------------------------------
#define   _IQtoF(A)      (A)
#define   _IQ30toF(A)    (A)
#define   _IQ29toF(A)    (A)
#define   _IQ28toF(A)    (A)
#define   _IQ27toF(A)    (A)
#define   _IQ26toF(A)    (A)
#define   _IQ25toF(A)    (A)
#define   _IQ24toF(A)    (A)
#define   _IQ23toF(A)    (A)
#define   _IQ22toF(A)    (A)
#define   _IQ21toF(A)    (A)
#define   _IQ20toF(A)    (A)
#define   _IQ19toF(A)    (A)
#define   _IQ18toF(A)    (A)
#define   _IQ17toF(A)    (A)
#define   _IQ16toF(A)    (A)
#define   _IQ15toF(A)    (A)
#define   _IQ14toF(A)    (A)
#define   _IQ13toF(A)    (A)
#define   _IQ12toF(A)    (A)
#define   _IQ11toF(A)    (A)
#define   _IQ10toF(A)    (A)
#define   _IQ9toF(A)     (A)
#define   _IQ8toF(A)     (A)
#define   _IQ7toF(A)     (A)
#define   _IQ6toF(A)     (A)
#define   _IQ5toF(A)     (A)
#define   _IQ4toF(A)     (A)
#define   _IQ3toF(A)     (A)
#define   _IQ2toF(A)     (A)
#define   _IQ1toF(A)     (A)
//---------------------------------------------------------------------------
extern  float _satf(float A, float Pos, float Neg);
#define   _IQsat(A, Pos, Neg)    _satf(A, Pos, Neg)

//---------------------------------------------------------------------------
#define   _IQtoIQ30(A)   (A)
#define   _IQtoIQ29(A)   (A)
#define   _IQtoIQ28(A)   (A)
#define   _IQtoIQ27(A)   (A)
#define   _IQtoIQ26(A)   (A)
#define   _IQtoIQ25(A)   (A)
#define   _IQtoIQ24(A)   (A)
#define   _IQtoIQ23(A)   (A)
#define   _IQtoIQ22(A)   (A)
#define   _IQtoIQ21(A)   (A)
#define   _IQtoIQ20(A)   (A)
#define   _IQtoIQ19(A)   (A)
#define   _IQtoIQ18(A)   (A)
#define   _IQtoIQ17(A)   (A)
#define   _IQtoIQ16(A)   (A)
#define   _IQtoIQ15(A)   (A)
#define   _IQtoIQ14(A)   (A)
#define   _IQtoIQ13(A)   (A)
#define   _IQtoIQ12(A)   (A)
#define   _IQtoIQ11(A)   (A)
#define   _IQtoIQ10(A)   (A)
#define   _IQtoIQ9(A)    (A)
#define   _IQtoIQ8(A)    (A)
#define   _IQtoIQ7(A)    (A)
#define   _IQtoIQ6(A)    (A)
#define   _IQtoIQ5(A)    (A)
#define   _IQtoIQ4(A)    (A)
#define   _IQtoIQ3(A)    (A)
#define   _IQtoIQ2(A)    (A)
#define   _IQtoIQ1(A)    (A)
//---------------------------------------------------------------------------
#define   _IQ30toIQ(A)   (A)
#define   _IQ29toIQ(A)   (A)
#define   _IQ28toIQ(A)   (A)
#define   _IQ27toIQ(A)   (A)
#define   _IQ26toIQ(A)   (A)
#define   _IQ25toIQ(A)   (A)
#define   _IQ24toIQ(A)   (A)
#define   _IQ23toIQ(A)   (A)
#define   _IQ22toIQ(A)   (A)
#define   _IQ21toIQ(A)   (A)
#define   _IQ20toIQ(A)   (A)
#define   _IQ19toIQ(A)   (A)
#define   _IQ18toIQ(A)   (A)
#define   _IQ17toIQ(A)   (A)
#define   _IQ16toIQ(A)   (A)
#define   _IQ15toIQ(A)   (A)
#define   _IQ14toIQ(A)   (A)
#define   _IQ13toIQ(A)   (A)
#define   _IQ12toIQ(A)   (A)
#define   _IQ11toIQ(A)   (A)
#define   _IQ10toIQ(A)   (A)
#define   _IQ9toIQ(A)    (A)
#define   _IQ8toIQ(A)    (A)
#define   _IQ7toIQ(A)    (A)
#define   _IQ6toIQ(A)    (A)
#define   _IQ5toIQ(A)    (A)
#define   _IQ4toIQ(A)    (A)
#define   _IQ3toIQ(A)    (A)
#define   _IQ2toIQ(A)    (A)
#define   _IQ1toIQ(A)    (A)
//---------------------------------------------------------------------------
#define   _IQtoQ15(A)    (short) ((I32_IQ)((A) * 32768.0L))
#define   _IQtoQ14(A)    (short) ((I32_IQ)((A) * 16384.0L))
#define   _IQtoQ13(A)    (short) ((I32_IQ)((A) * 8192.0L))
#define   _IQtoQ12(A)    (short) ((I32_IQ)((A) * 4096.0L))
#define   _IQtoQ11(A)    (short) ((I32_IQ)((A) * 2048.0L))
#define   _IQtoQ10(A)    (short) ((I32_IQ)((A) * 1024.0L))
#define   _IQtoQ9(A)     (short) ((I32_IQ)((A) * 512.0L))
#define   _IQtoQ8(A)     (short) ((I32_IQ)((A) * 256.0L))
#define   _IQtoQ7(A)     (short) ((I32_IQ)((A) * 128.0L))
#define   _IQtoQ6(A)     (short) ((I32_IQ)((A) * 64.0L))
#define   _IQtoQ5(A)     (short) ((I32_IQ)((A) * 32.0L))
#define   _IQtoQ4(A)     (short) ((I32_IQ)((A) * 16.0L))
#define   _IQtoQ3(A)     (short) ((I32_IQ)((A) * 8.0L))
#define   _IQtoQ2(A)     (short) ((I32_IQ)((A) * 4.0L))
#define   _IQtoQ1(A)     (short) ((I32_IQ)((A) * 2.0L))

//---------------------------------------------------------------------------
#define   _Q15toIQ(A)    (((float) (A)) * 0.000030518)
#define   _Q14toIQ(A)    (((float) (A)) * 0.000061035)
#define   _Q13toIQ(A)    (((float) (A)) * 0.000122070)
#define   _Q12toIQ(A)    (((float) (A)) * 0.000244141)
#define   _Q11toIQ(A)    (((float) (A)) * 0.000488281)
#define   _Q10toIQ(A)    (((float) (A)) * 0.000976563)
#define   _Q9toIQ(A)     (((float) (A)) * 0.001953125)
#define   _Q8toIQ(A)     (((float) (A)) * 0.003906250)
#define   _Q7toIQ(A)     (((float) (A)) * 0.007812500)
#define   _Q6toIQ(A)     (((float) (A)) * 0.015625000)
#define   _Q5toIQ(A)     (((float) (A)) * 0.031250000)
#define   _Q4toIQ(A)     (((float) (A)) * 0.062500000)
#define   _Q3toIQ(A)     (((float) (A)) * 0.125000000)
#define   _Q2toIQ(A)     (((float) (A)) * 0.250000000)
#define   _Q1toIQ(A)     (((float) (A)) * 0.500000000)
//---------------------------------------------------------------------------
#define   _IQmpy(A,B)         ((A) * (B))
#define   _IQ30mpy(A,B)       ((A) * (B))
#define   _IQ29mpy(A,B)       ((A) * (B))
#define   _IQ28mpy(A,B)       ((A) * (B))
#define   _IQ27mpy(A,B)       ((A) * (B))
#define   _IQ26mpy(A,B)       ((A) * (B))
#define   _IQ25mpy(A,B)       ((A) * (B))
#define   _IQ24mpy(A,B)       ((A) * (B))
#define   _IQ23mpy(A,B)       ((A) * (B))
#define   _IQ22mpy(A,B)       ((A) * (B))
#define   _IQ21mpy(A,B)       ((A) * (B))
#define   _IQ20mpy(A,B)       ((A) * (B))
#define   _IQ19mpy(A,B)       ((A) * (B))
#define   _IQ18mpy(A,B)       ((A) * (B))
#define   _IQ17mpy(A,B)       ((A) * (B))
#define   _IQ16mpy(A,B)       ((A) * (B))
#define   _IQ15mpy(A,B)       ((A) * (B))
#define   _IQ14mpy(A,B)       ((A) * (B))
#define   _IQ13mpy(A,B)       ((A) * (B))
#define   _IQ12mpy(A,B)       ((A) * (B))
#define   _IQ11mpy(A,B)       ((A) * (B))
#define   _IQ10mpy(A,B)       ((A) * (B))
#define   _IQ9mpy(A,B)        ((A) * (B))
#define   _IQ8mpy(A,B)        ((A) * (B))
#define   _IQ7mpy(A,B)        ((A) * (B))
#define   _IQ6mpy(A,B)        ((A) * (B))
#define   _IQ5mpy(A,B)        ((A) * (B))
#define   _IQ4mpy(A,B)        ((A) * (B))
#define   _IQ3mpy(A,B)        ((A) * (B))
#define   _IQ2mpy(A,B)        ((A) * (B))
#define   _IQ1mpy(A,B)        ((A) * (B))
//---------------------------------------------------------------------------
#define   _IQrmpy(A,B)        ((A) * (B))
#define   _IQ31rmpy(A,B)      ((A) * (B))
#define   _IQ30rmpy(A,B)      ((A) * (B))
#define   _IQ29rmpy(A,B)      ((A) * (B))
#define   _IQ28rmpy(A,B)      ((A) * (B))
#define   _IQ27rmpy(A,B)      ((A) * (B))
#define   _IQ26rmpy(A,B)      ((A) * (B))
#define   _IQ25rmpy(A,B)      ((A) * (B))
#define   _IQ24rmpy(A,B)      ((A) * (B))
#define   _IQ23rmpy(A,B)      ((A) * (B))
#define   _IQ22rmpy(A,B)      ((A) * (B))
#define   _IQ21rmpy(A,B)      ((A) * (B))
#define   _IQ20rmpy(A,B)      ((A) * (B))
#define   _IQ19rmpy(A,B)      ((A) * (B))
#define   _IQ18rmpy(A,B)      ((A) * (B))
#define   _IQ17rmpy(A,B)      ((A) * (B))
#define   _IQ16rmpy(A,B)      ((A) * (B))
#define   _IQ15rmpy(A,B)      ((A) * (B))
#define   _IQ14rmpy(A,B)      ((A) * (B))
#define   _IQ13rmpy(A,B)      ((A) * (B))
#define   _IQ12rmpy(A,B)      ((A) * (B))
#define   _IQ11rmpy(A,B)      ((A) * (B))
#define   _IQ10rmpy(A,B)      ((A) * (B))
#define   _IQ9rmpy(A,B)       ((A) * (B))
#define   _IQ8rmpy(A,B)       ((A) * (B))
#define   _IQ7rmpy(A,B)       ((A) * (B))
#define   _IQ6rmpy(A,B)       ((A) * (B))
#define   _IQ5rmpy(A,B)       ((A) * (B))
#define   _IQ4rmpy(A,B)       ((A) * (B))
#define   _IQ3rmpy(A,B)       ((A) * (B))
#define   _IQ2rmpy(A,B)       ((A) * (B))
#define   _IQ1rmpy(A,B)       ((A) * (B))
#define   _IQ0rmpy(A,B)       ((A) * (B))

//---------------------------------------------------------------------------
#define   _IQrsmpy(A,B)       ((A) * (B))
#define   _IQ30rsmpy(A,B)     ((A) * (B))
#define   _IQ29rsmpy(A,B)     ((A) * (B))
#define   _IQ28rsmpy(A,B)     ((A) * (B))
#define   _IQ27rsmpy(A,B)     ((A) * (B))
#define   _IQ26rsmpy(A,B)     ((A) * (B))
#define   _IQ25rsmpy(A,B)     ((A) * (B))
#define   _IQ24rsmpy(A,B)     ((A) * (B))
#define   _IQ23rsmpy(A,B)     ((A) * (B))
#define   _IQ22rsmpy(A,B)     ((A) * (B))
#define   _IQ21rsmpy(A,B)     ((A) * (B))
#define   _IQ20rsmpy(A,B)     ((A) * (B))
#define   _IQ19rsmpy(A,B)     ((A) * (B))
#define   _IQ18rsmpy(A,B)     ((A) * (B))
#define   _IQ17rsmpy(A,B)     ((A) * (B))
#define   _IQ16rsmpy(A,B)     ((A) * (B))
#define   _IQ15rsmpy(A,B)     ((A) * (B))
#define   _IQ14rsmpy(A,B)     ((A) * (B))
#define   _IQ13rsmpy(A,B)     ((A) * (B))
#define   _IQ12rsmpy(A,B)     ((A) * (B))
#define   _IQ11rsmpy(A,B)     ((A) * (B))
#define   _IQ10rsmpy(A,B)     ((A) * (B))
#define   _IQ9rsmpy(A,B)      ((A) * (B))
#define   _IQ8rsmpy(A,B)      ((A) * (B))
#define   _IQ7rsmpy(A,B)      ((A) * (B))
#define   _IQ6rsmpy(A,B)      ((A) * (B))
#define   _IQ5rsmpy(A,B)      ((A) * (B))
#define   _IQ4rsmpy(A,B)      ((A) * (B))
#define   _IQ3rsmpy(A,B)      ((A) * (B))
#define   _IQ2rsmpy(A,B)      ((A) * (B))
#define   _IQ1rsmpy(A,B)      ((A) * (B))
//---------------------------------------------------------------------------
#define   _IQdiv(A,B)         ((A) / (B))
#define   _IQ30div(A,B)       ((A) / (B))
#define   _IQ29div(A,B)       ((A) / (B))
#define   _IQ28div(A,B)       ((A) / (B))
#define   _IQ27div(A,B)       ((A) / (B))
#define   _IQ26div(A,B)       ((A) / (B))
#define   _IQ25div(A,B)       ((A) / (B))
#define   _IQ24div(A,B)       ((A) / (B))
#define   _IQ23div(A,B)       ((A) / (B))
#define   _IQ22div(A,B)       ((A) / (B))
#define   _IQ21div(A,B)       ((A) / (B))
#define   _IQ20div(A,B)       ((A) / (B))
#define   _IQ19div(A,B)       ((A) / (B))
#define   _IQ18div(A,B)       ((A) / (B))
#define   _IQ17div(A,B)       ((A) / (B))
#define   _IQ16div(A,B)       ((A) / (B))
#define   _IQ15div(A,B)       ((A) / (B))
#define   _IQ14div(A,B)       ((A) / (B))
#define   _IQ13div(A,B)       ((A) / (B))
#define   _IQ12div(A,B)       ((A) / (B))
#define   _IQ11div(A,B)       ((A) / (B))
#define   _IQ10div(A,B)       ((A) / (B))
#define   _IQ9div(A,B)        ((A) / (B))
#define   _IQ8div(A,B)        ((A) / (B))
#define   _IQ7div(A,B)        ((A) / (B))
#define   _IQ6div(A,B)        ((A) / (B))
#define   _IQ5div(A,B)        ((A) / (B))
#define   _IQ4div(A,B)        ((A) / (B))
#define   _IQ3div(A,B)        ((A) / (B))
#define   _IQ2div(A,B)        ((A) / (B))
#define   _IQ1div(A,B)        ((A) / (B))
//---------------------------------------------------------------------------
#define   _IQsin(A)           sin(A)
#define   _IQ30sin(A)         sin(A)
#define   _IQ29sin(A)         sin(A)
#define   _IQ28sin(A)         sin(A)
#define   _IQ27sin(A)         sin(A)
#define   _IQ26sin(A)         sin(A)
#define   _IQ25sin(A)         sin(A)
#define   _IQ24sin(A)         sin(A)
#define   _IQ23sin(A)         sin(A)
#define   _IQ22sin(A)         sin(A)
#define   _IQ21sin(A)         sin(A)
#define   _IQ20sin(A)         sin(A)
#define   _IQ19sin(A)         sin(A)
#define   _IQ18sin(A)         sin(A)
#define   _IQ17sin(A)         sin(A)
#define   _IQ16sin(A)         sin(A)
#define   _IQ15sin(A)         sin(A)
#define   _IQ14sin(A)         sin(A)
#define   _IQ13sin(A)         sin(A)
#define   _IQ12sin(A)         sin(A)
#define   _IQ11sin(A)         sin(A)
#define   _IQ10sin(A)         sin(A)
#define   _IQ9sin(A)          sin(A)
#define   _IQ8sin(A)          sin(A)
#define   _IQ7sin(A)          sin(A)
#define   _IQ6sin(A)          sin(A)
#define   _IQ5sin(A)          sin(A)
#define   _IQ4sin(A)          sin(A)
#define   _IQ3sin(A)          sin(A)
#define   _IQ2sin(A)          sin(A)
#define   _IQ1sin(A)          sin(A)
//---------------------------------------------------------------------------
#define   _IQsinPU(A)         sin((A)*6.283185307)
#define   _IQ30sinPU(A)       sin((A)*6.283185307)
#define   _IQ29sinPU(A)       sin((A)*6.283185307)
#define   _IQ28sinPU(A)       sin((A)*6.283185307)
#define   _IQ27sinPU(A)       sin((A)*6.283185307)
#define   _IQ26sinPU(A)       sin((A)*6.283185307)
#define   _IQ25sinPU(A)       sin((A)*6.283185307)
#define   _IQ24sinPU(A)       sin((A)*6.283185307)
#define   _IQ23sinPU(A)       sin((A)*6.283185307)
#define   _IQ22sinPU(A)       sin((A)*6.283185307)
#define   _IQ21sinPU(A)       sin((A)*6.283185307)
#define   _IQ20sinPU(A)       sin((A)*6.283185307)
#define   _IQ19sinPU(A)       sin((A)*6.283185307)
#define   _IQ18sinPU(A)       sin((A)*6.283185307)
#define   _IQ17sinPU(A)       sin((A)*6.283185307)
#define   _IQ16sinPU(A)       sin((A)*6.283185307)
#define   _IQ15sinPU(A)       sin((A)*6.283185307)
#define   _IQ14sinPU(A)       sin((A)*6.283185307)
#define   _IQ13sinPU(A)       sin((A)*6.283185307)
#define   _IQ12sinPU(A)       sin((A)*6.283185307)
#define   _IQ11sinPU(A)       sin((A)*6.283185307)
#define   _IQ10sinPU(A)       sin((A)*6.283185307)
#define   _IQ9sinPU(A)        sin((A)*6.283185307)
#define   _IQ8sinPU(A)        sin((A)*6.283185307)
#define   _IQ7sinPU(A)        sin((A)*6.283185307)
#define   _IQ6sinPU(A)        sin((A)*6.283185307)
#define   _IQ5sinPU(A)        sin((A)*6.283185307)
#define   _IQ4sinPU(A)        sin((A)*6.283185307)
#define   _IQ3sinPU(A)        sin((A)*6.283185307)
#define   _IQ2sinPU(A)        sin((A)*6.283185307)
#define   _IQ1sinPU(A)        sin((A)*6.283185307)
//---------------------------------------------------------------------------
#define   _IQcos(A)           cos(A)
#define   _IQ30cos(A)         cos(A)
#define   _IQ29cos(A)         cos(A)
#define   _IQ28cos(A)         cos(A)
#define   _IQ27cos(A)         cos(A)
#define   _IQ26cos(A)         cos(A)
#define   _IQ25cos(A)         cos(A)
#define   _IQ24cos(A)         cos(A)
#define   _IQ23cos(A)         cos(A)
#define   _IQ22cos(A)         cos(A)
#define   _IQ21cos(A)         cos(A)
#define   _IQ20cos(A)         cos(A)
#define   _IQ19cos(A)         cos(A)
#define   _IQ18cos(A)         cos(A)
#define   _IQ17cos(A)         cos(A)
#define   _IQ16cos(A)         cos(A)
#define   _IQ15cos(A)         cos(A)
#define   _IQ14cos(A)         cos(A)
#define   _IQ13cos(A)         cos(A)
#define   _IQ12cos(A)         cos(A)
#define   _IQ11cos(A)         cos(A)
#define   _IQ10cos(A)         cos(A)
#define   _IQ9cos(A)          cos(A)
#define   _IQ8cos(A)          cos(A)
#define   _IQ7cos(A)          cos(A)
#define   _IQ6cos(A)          cos(A)
#define   _IQ5cos(A)          cos(A)
#define   _IQ4cos(A)          cos(A)
#define   _IQ3cos(A)          cos(A)
#define   _IQ2cos(A)          cos(A)
#define   _IQ1cos(A)          cos(A)
//---------------------------------------------------------------------------
#define   _IQcosPU(A)         cos((A)*6.283185307)
#define   _IQ30cosPU(A)       cos((A)*6.283185307)
#define   _IQ29cosPU(A)       cos((A)*6.283185307)
#define   _IQ28cosPU(A)       cos((A)*6.283185307)
#define   _IQ27cosPU(A)       cos((A)*6.283185307)
#define   _IQ26cosPU(A)       cos((A)*6.283185307)
#define   _IQ25cosPU(A)       cos((A)*6.283185307)
#define   _IQ24cosPU(A)       cos((A)*6.283185307)
#define   _IQ23cosPU(A)       cos((A)*6.283185307)
#define   _IQ22cosPU(A)       cos((A)*6.283185307)
#define   _IQ21cosPU(A)       cos((A)*6.283185307)
#define   _IQ20cosPU(A)       cos((A)*6.283185307)
#define   _IQ19cosPU(A)       cos((A)*6.283185307)
#define   _IQ18cosPU(A)       cos((A)*6.283185307)
#define   _IQ17cosPU(A)       cos((A)*6.283185307)
#define   _IQ16cosPU(A)       cos((A)*6.283185307)
#define   _IQ15cosPU(A)       cos((A)*6.283185307)
#define   _IQ14cosPU(A)       cos((A)*6.283185307)
#define   _IQ13cosPU(A)       cos((A)*6.283185307)
#define   _IQ12cosPU(A)       cos((A)*6.283185307)
#define   _IQ11cosPU(A)       cos((A)*6.283185307)
#define   _IQ10cosPU(A)       cos((A)*6.283185307)
#define   _IQ9cosPU(A)        cos((A)*6.283185307)
#define   _IQ8cosPU(A)        cos((A)*6.283185307)
#define   _IQ7cosPU(A)        cos((A)*6.283185307)
#define   _IQ6cosPU(A)        cos((A)*6.283185307)
#define   _IQ5cosPU(A)        cos((A)*6.283185307)
#define   _IQ4cosPU(A)        cos((A)*6.283185307)
#define   _IQ3cosPU(A)        cos((A)*6.283185307)
#define   _IQ2cosPU(A)        cos((A)*6.283185307)
#define   _IQ1cosPU(A)        cos((A)*6.283185307)
//---------------------------------------------------------------------------
#define   _IQatan(A)          atan(A)
#define   _IQ30atan(A)        atan(A)
#define   _IQ29atan(A)        atan(A)
#define   _IQ28atan(A)        atan(A)
#define   _IQ27atan(A)        atan(A)
#define   _IQ26atan(A)        atan(A)
#define   _IQ25atan(A)        atan(A)
#define   _IQ24atan(A)        atan(A)
#define   _IQ23atan(A)        atan(A)
#define   _IQ22atan(A)        atan(A)
#define   _IQ21atan(A)        atan(A)
#define   _IQ20atan(A)        atan(A)
#define   _IQ19atan(A)        atan(A)
#define   _IQ18atan(A)        atan(A)
#define   _IQ17atan(A)        atan(A)
#define   _IQ16atan(A)        atan(A)
#define   _IQ15atan(A)        atan(A)
#define   _IQ14atan(A)        atan(A)
#define   _IQ13atan(A)        atan(A)
#define   _IQ12atan(A)        atan(A)
#define   _IQ11atan(A)        atan(A)
#define   _IQ10atan(A)        atan(A)
#define   _IQ9atan(A)         atan(A)
#define   _IQ8atan(A)         atan(A)
#define   _IQ7atan(A)         atan(A)
#define   _IQ6atan(A)         atan(A)
#define   _IQ5atan(A)         atan(A)
#define   _IQ4atan(A)         atan(A)
#define   _IQ3atan(A)         atan(A)
#define   _IQ2atan(A)         atan(A)
#define   _IQ1atan(A)         atan(A)
//---------------------------------------------------------------------------
#define   _IQatan2(A,B)       atan2(A,B)
#define   _IQ30atan2(A,B)     atan2(A,B)
#define   _IQ29atan2(A,B)     atan2(A,B)
#define   _IQ28atan2(A,B)     atan2(A,B)
#define   _IQ27atan2(A,B)     atan2(A,B)
#define   _IQ26atan2(A,B)     atan2(A,B)
#define   _IQ25atan2(A,B)     atan2(A,B)
#define   _IQ24atan2(A,B)     atan2(A,B)
#define   _IQ23atan2(A,B)     atan2(A,B)
#define   _IQ22atan2(A,B)     atan2(A,B)
#define   _IQ21atan2(A,B)     atan2(A,B)
#define   _IQ20atan2(A,B)     atan2(A,B)
#define   _IQ19atan2(A,B)     atan2(A,B)
#define   _IQ18atan2(A,B)     atan2(A,B)
#define   _IQ17atan2(A,B)     atan2(A,B)
#define   _IQ16atan2(A,B)     atan2(A,B)
#define   _IQ15atan2(A,B)     atan2(A,B)
#define   _IQ14atan2(A,B)     atan2(A,B)
#define   _IQ13atan2(A,B)     atan2(A,B)
#define   _IQ12atan2(A,B)     atan2(A,B)
#define   _IQ11atan2(A,B)     atan2(A,B)
#define   _IQ10atan2(A,B)     atan2(A,B)
#define   _IQ9atan2(A,B)      atan2(A,B)
#define   _IQ8atan2(A,B)      atan2(A,B)
#define   _IQ7atan2(A,B)      atan2(A,B)
#define   _IQ6atan2(A,B)      atan2(A,B)
#define   _IQ5atan2(A,B)      atan2(A,B)
#define   _IQ4atan2(A,B)      atan2(A,B)
#define   _IQ3atan2(A,B)      atan2(A,B)
#define   _IQ2atan2(A,B)      atan2(A,B)
#define   _IQ1atan2(A,B)      atan2(A,B)
//---------------------------------------------------------------------------
#define   _IQatan2PU(A,B)     ((atan2(A,B)/6.283185307) >= 0.0 ? (atan2(A,B)/6.283185307):1.0 + (atan2(A,B)/6.283185307))
#define   _IQ30atan2PU(A,B)   ((atan2(A,B)/6.283185307) >= 0.0 ? (atan2(A,B)/6.283185307):1.0 + (atan2(A,B)/6.283185307))
#define   _IQ29atan2PU(A,B)   ((atan2(A,B)/6.283185307) >= 0.0 ? (atan2(A,B)/6.283185307):1.0 + (atan2(A,B)/6.283185307))
#define   _IQ28atan2PU(A,B)   ((atan2(A,B)/6.283185307) >= 0.0 ? (atan2(A,B)/6.283185307):1.0 + (atan2(A,B)/6.283185307))
#define   _IQ27atan2PU(A,B)   ((atan2(A,B)/6.283185307) >= 0.0 ? (atan2(A,B)/6.283185307):1.0 + (atan2(A,B)/6.283185307))
#define   _IQ26atan2PU(A,B)   ((atan2(A,B)/6.283185307) >= 0.0 ? (atan2(A,B)/6.283185307):1.0 + (atan2(A,B)/6.283185307))
#define   _IQ25atan2PU(A,B)   ((atan2(A,B)/6.283185307) >= 0.0 ? (atan2(A,B)/6.283185307):1.0 + (atan2(A,B)/6.283185307))
#define   _IQ24atan2PU(A,B)   ((atan2(A,B)/6.283185307) >= 0.0 ? (atan2(A,B)/6.283185307):1.0 + (atan2(A,B)/6.283185307))
#define   _IQ23atan2PU(A,B)   ((atan2(A,B)/6.283185307) >= 0.0 ? (atan2(A,B)/6.283185307):1.0 + (atan2(A,B)/6.283185307))
#define   _IQ22atan2PU(A,B)   ((atan2(A,B)/6.283185307) >= 0.0 ? (atan2(A,B)/6.283185307):1.0 + (atan2(A,B)/6.283185307))
#define   _IQ21atan2PU(A,B)   ((atan2(A,B)/6.283185307) >= 0.0 ? (atan2(A,B)/6.283185307):1.0 + (atan2(A,B)/6.283185307))
#define   _IQ20atan2PU(A,B)   ((atan2(A,B)/6.283185307) >= 0.0 ? (atan2(A,B)/6.283185307):1.0 + (atan2(A,B)/6.283185307))
#define   _IQ19atan2PU(A,B)   ((atan2(A,B)/6.283185307) >= 0.0 ? (atan2(A,B)/6.283185307):1.0 + (atan2(A,B)/6.283185307))
#define   _IQ18atan2PU(A,B)   ((atan2(A,B)/6.283185307) >= 0.0 ? (atan2(A,B)/6.283185307):1.0 + (atan2(A,B)/6.283185307))
#define   _IQ17atan2PU(A,B)   ((atan2(A,B)/6.283185307) >= 0.0 ? (atan2(A,B)/6.283185307):1.0 + (atan2(A,B)/6.283185307))
#define   _IQ16atan2PU(A,B)   ((atan2(A,B)/6.283185307) >= 0.0 ? (atan2(A,B)/6.283185307):1.0 + (atan2(A,B)/6.283185307))
#define   _IQ15atan2PU(A,B)   ((atan2(A,B)/6.283185307) >= 0.0 ? (atan2(A,B)/6.283185307):1.0 + (atan2(A,B)/6.283185307))
#define   _IQ14atan2PU(A,B)   ((atan2(A,B)/6.283185307) >= 0.0 ? (atan2(A,B)/6.283185307):1.0 + (atan2(A,B)/6.283185307))
#define   _IQ13atan2PU(A,B)   ((atan2(A,B)/6.283185307) >= 0.0 ? (atan2(A,B)/6.283185307):1.0 + (atan2(A,B)/6.283185307))
#define   _IQ12atan2PU(A,B)   ((atan2(A,B)/6.283185307) >= 0.0 ? (atan2(A,B)/6.283185307):1.0 + (atan2(A,B)/6.283185307))
#define   _IQ11atan2PU(A,B)   ((atan2(A,B)/6.283185307) >= 0.0 ? (atan2(A,B)/6.283185307):1.0 + (atan2(A,B)/6.283185307))
#define   _IQ10atan2PU(A,B)   ((atan2(A,B)/6.283185307) >= 0.0 ? (atan2(A,B)/6.283185307):1.0 + (atan2(A,B)/6.283185307))
#define   _IQ9atan2PU(A,B)    ((atan2(A,B)/6.283185307) >= 0.0 ? (atan2(A,B)/6.283185307):1.0 + (atan2(A,B)/6.283185307))
#define   _IQ8atan2PU(A,B)    ((atan2(A,B)/6.283185307) >= 0.0 ? (atan2(A,B)/6.283185307):1.0 + (atan2(A,B)/6.283185307))
#define   _IQ7atan2PU(A,B)    ((atan2(A,B)/6.283185307) >= 0.0 ? (atan2(A,B)/6.283185307):1.0 + (atan2(A,B)/6.283185307))
#define   _IQ6atan2PU(A,B)    ((atan2(A,B)/6.283185307) >= 0.0 ? (atan2(A,B)/6.283185307):1.0 + (atan2(A,B)/6.283185307))
#define   _IQ5atan2PU(A,B)    ((atan2(A,B)/6.283185307) >= 0.0 ? (atan2(A,B)/6.283185307):1.0 + (atan2(A,B)/6.283185307))
#define   _IQ4atan2PU(A,B)    ((atan2(A,B)/6.283185307) >= 0.0 ? (atan2(A,B)/6.283185307):1.0 + (atan2(A,B)/6.283185307))
#define   _IQ3atan2PU(A,B)    ((atan2(A,B)/6.283185307) >= 0.0 ? (atan2(A,B)/6.283185307):1.0 + (atan2(A,B)/6.283185307))
#define   _IQ2atan2PU(A,B)    ((atan2(A,B)/6.283185307) >= 0.0 ? (atan2(A,B)/6.283185307):1.0 + (atan2(A,B)/6.283185307))
#define   _IQ1atan2PU(A,B)    ((atan2(A,B)/6.283185307) >= 0.0 ? (atan2(A,B)/6.283185307):1.0 + (atan2(A,B)/6.283185307))
//---------------------------------------------------------------------------
#define   _IQsqrt(A)          sqrt(A)
#define   _IQ30sqrt(A)        sqrt(A)
#define   _IQ29sqrt(A)        sqrt(A)
#define   _IQ28sqrt(A)        sqrt(A)
#define   _IQ27sqrt(A)        sqrt(A)
#define   _IQ26sqrt(A)        sqrt(A)
#define   _IQ25sqrt(A)        sqrt(A)
#define   _IQ24sqrt(A)        sqrt(A)
#define   _IQ23sqrt(A)        sqrt(A)
#define   _IQ22sqrt(A)        sqrt(A)
#define   _IQ21sqrt(A)        sqrt(A)
#define   _IQ20sqrt(A)        sqrt(A)
#define   _IQ19sqrt(A)        sqrt(A)
#define   _IQ18sqrt(A)        sqrt(A)
#define   _IQ17sqrt(A)        sqrt(A)
#define   _IQ16sqrt(A)        sqrt(A)
#define   _IQ15sqrt(A)        sqrt(A)
#define   _IQ14sqrt(A)        sqrt(A)
#define   _IQ13sqrt(A)        sqrt(A)
#define   _IQ12sqrt(A)        sqrt(A)
#define   _IQ11sqrt(A)        sqrt(A)
#define   _IQ10sqrt(A)        sqrt(A)
#define   _IQ9sqrt(A)         sqrt(A)
#define   _IQ8sqrt(A)         sqrt(A)
#define   _IQ7sqrt(A)         sqrt(A)
#define   _IQ6sqrt(A)         sqrt(A)
#define   _IQ5sqrt(A)         sqrt(A)
#define   _IQ4sqrt(A)         sqrt(A)
#define   _IQ3sqrt(A)         sqrt(A)
#define   _IQ2sqrt(A)         sqrt(A)
#define   _IQ1sqrt(A)         sqrt(A)
//---------------------------------------------------------------------------
#define   _IQisqrt(A)         (1.0/sqrt(A))
#define   _IQ30isqrt(A)       (1.0/sqrt(A))
#define   _IQ29isqrt(A)       (1.0/sqrt(A))
#define   _IQ28isqrt(A)       (1.0/sqrt(A))
#define   _IQ27isqrt(A)       (1.0/sqrt(A))
#define   _IQ26isqrt(A)       (1.0/sqrt(A))
#define   _IQ25isqrt(A)       (1.0/sqrt(A))
#define   _IQ24isqrt(A)       (1.0/sqrt(A))
#define   _IQ23isqrt(A)       (1.0/sqrt(A))
#define   _IQ22isqrt(A)       (1.0/sqrt(A))
#define   _IQ21isqrt(A)       (1.0/sqrt(A))
#define   _IQ20isqrt(A)       (1.0/sqrt(A))
#define   _IQ19isqrt(A)       (1.0/sqrt(A))
#define   _IQ18isqrt(A)       (1.0/sqrt(A))
#define   _IQ17isqrt(A)       (1.0/sqrt(A))
#define   _IQ16isqrt(A)       (1.0/sqrt(A))
#define   _IQ15isqrt(A)       (1.0/sqrt(A))
#define   _IQ14isqrt(A)       (1.0/sqrt(A))
#define   _IQ13isqrt(A)       (1.0/sqrt(A))
#define   _IQ12isqrt(A)       (1.0/sqrt(A))
#define   _IQ11isqrt(A)       (1.0/sqrt(A))
#define   _IQ10isqrt(A)       (1.0/sqrt(A))
#define   _IQ9isqrt(A)        (1.0/sqrt(A))
#define   _IQ8isqrt(A)        (1.0/sqrt(A))
#define   _IQ7isqrt(A)        (1.0/sqrt(A))
#define   _IQ6isqrt(A)        (1.0/sqrt(A))
#define   _IQ5isqrt(A)        (1.0/sqrt(A))
#define   _IQ4isqrt(A)        (1.0/sqrt(A))
#define   _IQ3isqrt(A)        (1.0/sqrt(A))
#define   _IQ2isqrt(A)        (1.0/sqrt(A))
#define   _IQ1isqrt(A)        (1.0/sqrt(A))
//---------------------------------------------------------------------------
#define   _IQint(A)           ((I32_IQ) (A))
#define   _IQ30int(A)         ((I32_IQ) (A))
#define   _IQ29int(A)         ((I32_IQ) (A))
#define   _IQ28int(A)         ((I32_IQ) (A))
#define   _IQ27int(A)         ((I32_IQ) (A))
#define   _IQ26int(A)         ((I32_IQ) (A))
#define   _IQ25int(A)         ((I32_IQ) (A))
#define   _IQ24int(A)         ((I32_IQ) (A))
#define   _IQ23int(A)         ((I32_IQ) (A))
#define   _IQ22int(A)         ((I32_IQ) (A))
#define   _IQ21int(A)         ((I32_IQ) (A))
#define   _IQ20int(A)         ((I32_IQ) (A))
#define   _IQ19int(A)         ((I32_IQ) (A))
#define   _IQ18int(A)         ((I32_IQ) (A))
#define   _IQ17int(A)         ((I32_IQ) (A))
#define   _IQ16int(A)         ((I32_IQ) (A))
#define   _IQ15int(A)         ((I32_IQ) (A))
#define   _IQ14int(A)         ((I32_IQ) (A))
#define   _IQ13int(A)         ((I32_IQ) (A))
#define   _IQ12int(A)         ((I32_IQ) (A))
#define   _IQ11int(A)         ((I32_IQ) (A))
#define   _IQ10int(A)         ((I32_IQ) (A))
#define   _IQ9int(A)          ((I32_IQ) (A))
#define   _IQ8int(A)          ((I32_IQ) (A))
#define   _IQ7int(A)          ((I32_IQ) (A))
#define   _IQ6int(A)          ((I32_IQ) (A))
#define   _IQ5int(A)          ((I32_IQ) (A))
#define   _IQ4int(A)          ((I32_IQ) (A))
#define   _IQ3int(A)          ((I32_IQ) (A))
#define   _IQ2int(A)          ((I32_IQ) (A))
#define   _IQ1int(A)          ((I32_IQ) (A))
//---------------------------------------------------------------------------
#define   _IQfrac(A)          ((A) - (float)((I32_IQ) (A)))
#define   _IQ30frac(A)        ((A) - (float)((I32_IQ) (A)))
#define   _IQ29frac(A)        ((A) - (float)((I32_IQ) (A)))
#define   _IQ28frac(A)        ((A) - (float)((I32_IQ) (A)))
#define   _IQ27frac(A)        ((A) - (float)((I32_IQ) (A)))
#define   _IQ26frac(A)        ((A) - (float)((I32_IQ) (A)))
#define   _IQ25frac(A)        ((A) - (float)((I32_IQ) (A)))
#define   _IQ24frac(A)        ((A) - (float)((I32_IQ) (A)))
#define   _IQ23frac(A)        ((A) - (float)((I32_IQ) (A)))
#define   _IQ22frac(A)        ((A) - (float)((I32_IQ) (A)))
#define   _IQ21frac(A)        ((A) - (float)((I32_IQ) (A)))
#define   _IQ20frac(A)        ((A) - (float)((I32_IQ) (A)))
#define   _IQ19frac(A)        ((A) - (float)((I32_IQ) (A)))
#define   _IQ18frac(A)        ((A) - (float)((I32_IQ) (A)))
#define   _IQ17frac(A)        ((A) - (float)((I32_IQ) (A)))
#define   _IQ16frac(A)        ((A) - (float)((I32_IQ) (A)))
#define   _IQ15frac(A)        ((A) - (float)((I32_IQ) (A)))
#define   _IQ14frac(A)        ((A) - (float)((I32_IQ) (A)))
#define   _IQ13frac(A)        ((A) - (float)((I32_IQ) (A)))
#define   _IQ12frac(A)        ((A) - (float)((I32_IQ) (A)))
#define   _IQ11frac(A)        ((A) - (float)((I32_IQ) (A)))
#define   _IQ10frac(A)        ((A) - (float)((I32_IQ) (A)))
#define   _IQ9frac(A)         ((A) - (float)((I32_IQ) (A)))
#define   _IQ8frac(A)         ((A) - (float)((I32_IQ) (A)))
#define   _IQ7frac(A)         ((A) - (float)((I32_IQ) (A)))
#define   _IQ6frac(A)         ((A) - (float)((I32_IQ) (A)))
#define   _IQ5frac(A)         ((A) - (float)((I32_IQ) (A)))
#define   _IQ4frac(A)         ((A) - (float)((I32_IQ) (A)))
#define   _IQ3frac(A)         ((A) - (float)((I32_IQ) (A)))
#define   _IQ2frac(A)         ((A) - (float)((I32_IQ) (A)))
#define   _IQ1frac(A)         ((A) - (float)((I32_IQ) (A)))
//---------------------------------------------------------------------------
#define   _IQmpyIQX(A, IQA, B, IQB)    ((A)*(B))    
#define   _IQ30mpyIQX(A, IQA, B, IQB)  ((A)*(B))    
#define   _IQ29mpyIQX(A, IQA, B, IQB)  ((A)*(B))    
#define   _IQ28mpyIQX(A, IQA, B, IQB)  ((A)*(B))    
#define   _IQ27mpyIQX(A, IQA, B, IQB)  ((A)*(B))    
#define   _IQ26mpyIQX(A, IQA, B, IQB)  ((A)*(B))    
#define   _IQ25mpyIQX(A, IQA, B, IQB)  ((A)*(B))    
#define   _IQ24mpyIQX(A, IQA, B, IQB)  ((A)*(B))    
#define   _IQ23mpyIQX(A, IQA, B, IQB)  ((A)*(B))    
#define   _IQ22mpyIQX(A, IQA, B, IQB)  ((A)*(B))    
#define   _IQ21mpyIQX(A, IQA, B, IQB)  ((A)*(B))    
#define   _IQ20mpyIQX(A, IQA, B, IQB)  ((A)*(B))    
#define   _IQ19mpyIQX(A, IQA, B, IQB)  ((A)*(B))    
#define   _IQ18mpyIQX(A, IQA, B, IQB)  ((A)*(B))    
#define   _IQ17mpyIQX(A, IQA, B, IQB)  ((A)*(B))    
#define   _IQ16mpyIQX(A, IQA, B, IQB)  ((A)*(B))    
#define   _IQ15mpyIQX(A, IQA, B, IQB)  ((A)*(B))    
#define   _IQ14mpyIQX(A, IQA, B, IQB)  ((A)*(B))    
#define   _IQ13mpyIQX(A, IQA, B, IQB)  ((A)*(B))    
#define   _IQ12mpyIQX(A, IQA, B, IQB)  ((A)*(B))    
#define   _IQ11mpyIQX(A, IQA, B, IQB)  ((A)*(B))    
#define   _IQ10mpyIQX(A, IQA, B, IQB)  ((A)*(B))    
#define   _IQ9mpyIQX(A, IQA, B, IQB)   ((A)*(B))    
#define   _IQ8mpyIQX(A, IQA, B, IQB)   ((A)*(B))    
#define   _IQ7mpyIQX(A, IQA, B, IQB)   ((A)*(B))    
#define   _IQ6mpyIQX(A, IQA, B, IQB)   ((A)*(B))    
#define   _IQ5mpyIQX(A, IQA, B, IQB)   ((A)*(B))    
#define   _IQ4mpyIQX(A, IQA, B, IQB)   ((A)*(B))    
#define   _IQ3mpyIQX(A, IQA, B, IQB)   ((A)*(B))    
#define   _IQ2mpyIQX(A, IQA, B, IQB)   ((A)*(B))    
#define   _IQ1mpyIQX(A, IQA, B, IQB)   ((A)*(B))    
//---------------------------------------------------------------------------
#define   _IQmpyI32(A,B)      ((A) * (float) (B))    
#define   _IQ30mpyI32(A,B)    ((A) * (float) (B))
#define   _IQ29mpyI32(A,B)    ((A) * (float) (B))
#define   _IQ28mpyI32(A,B)    ((A) * (float) (B))
#define   _IQ27mpyI32(A,B)    ((A) * (float) (B))
#define   _IQ26mpyI32(A,B)    ((A) * (float) (B))
#define   _IQ25mpyI32(A,B)    ((A) * (float) (B))
#define   _IQ24mpyI32(A,B)    ((A) * (float) (B))
#define   _IQ23mpyI32(A,B)    ((A) * (float) (B))
#define   _IQ22mpyI32(A,B)    ((A) * (float) (B))
#define   _IQ21mpyI32(A,B)    ((A) * (float) (B))
#define   _IQ20mpyI32(A,B)    ((A) * (float) (B))
#define   _IQ19mpyI32(A,B)    ((A) * (float) (B))
#define   _IQ18mpyI32(A,B)    ((A) * (float) (B))
#define   _IQ17mpyI32(A,B)    ((A) * (float) (B))
#define   _IQ16mpyI32(A,B)    ((A) * (float) (B))
#define   _IQ15mpyI32(A,B)    ((A) * (float) (B))
#define   _IQ14mpyI32(A,B)    ((A) * (float) (B))
#define   _IQ13mpyI32(A,B)    ((A) * (float) (B))
#define   _IQ12mpyI32(A,B)    ((A) * (float) (B))
#define   _IQ11mpyI32(A,B)    ((A) * (float) (B))
#define   _IQ10mpyI32(A,B)    ((A) * (float) (B))
#define   _IQ9mpyI32(A,B)     ((A) * (float) (B))
#define   _IQ8mpyI32(A,B)     ((A) * (float) (B))
#define   _IQ7mpyI32(A,B)     ((A) * (float) (B))
#define   _IQ6mpyI32(A,B)     ((A) * (float) (B))
#define   _IQ5mpyI32(A,B)     ((A) * (float) (B))
#define   _IQ4mpyI32(A,B)     ((A) * (float) (B))
#define   _IQ3mpyI32(A,B)     ((A) * (float) (B))
#define   _IQ2mpyI32(A,B)     ((A) * (float) (B))
#define   _IQ1mpyI32(A,B)     ((A) * (float) (B))
//---------------------------------------------------------------------------
#define   _IQmpyI32int(A,B)   ((I32_IQ) ((A) * (float) (B)))
#define   _IQ30mpyI32int(A,B) ((I32_IQ) ((A) * (float) (B)))
#define   _IQ29mpyI32int(A,B) ((I32_IQ) ((A) * (float) (B)))
#define   _IQ28mpyI32int(A,B) ((I32_IQ) ((A) * (float) (B)))
#define   _IQ27mpyI32int(A,B) ((I32_IQ) ((A) * (float) (B)))
#define   _IQ26mpyI32int(A,B) ((I32_IQ) ((A) * (float) (B)))
#define   _IQ25mpyI32int(A,B) ((I32_IQ) ((A) * (float) (B)))
#define   _IQ24mpyI32int(A,B) ((I32_IQ) ((A) * (float) (B)))
#define   _IQ23mpyI32int(A,B) ((I32_IQ) ((A) * (float) (B)))
#define   _IQ22mpyI32int(A,B) ((I32_IQ) ((A) * (float) (B)))
#define   _IQ21mpyI32int(A,B) ((I32_IQ) ((A) * (float) (B)))
#define   _IQ20mpyI32int(A,B) ((I32_IQ) ((A) * (float) (B)))
#define   _IQ19mpyI32int(A,B) ((I32_IQ) ((A) * (float) (B)))
#define   _IQ18mpyI32int(A,B) ((I32_IQ) ((A) * (float) (B)))
#define   _IQ17mpyI32int(A,B) ((I32_IQ) ((A) * (float) (B)))
#define   _IQ16mpyI32int(A,B) ((I32_IQ) ((A) * (float) (B)))
#define   _IQ15mpyI32int(A,B) ((I32_IQ) ((A) * (float) (B)))
#define   _IQ14mpyI32int(A,B) ((I32_IQ) ((A) * (float) (B)))
#define   _IQ13mpyI32int(A,B) ((I32_IQ) ((A) * (float) (B)))
#define   _IQ12mpyI32int(A,B) ((I32_IQ) ((A) * (float) (B)))
#define   _IQ11mpyI32int(A,B) ((I32_IQ) ((A) * (float) (B)))
#define   _IQ10mpyI32int(A,B) ((I32_IQ) ((A) * (float) (B)))
#define   _IQ9mpyI32int(A,B)  ((I32_IQ) ((A) * (float) (B)))
#define   _IQ8mpyI32int(A,B)  ((I32_IQ) ((A) * (float) (B)))
#define   _IQ7mpyI32int(A,B)  ((I32_IQ) ((A) * (float) (B)))
#define   _IQ6mpyI32int(A,B)  ((I32_IQ) ((A) * (float) (B)))
#define   _IQ5mpyI32int(A,B)  ((I32_IQ) ((A) * (float) (B)))
#define   _IQ4mpyI32int(A,B)  ((I32_IQ) ((A) * (float) (B)))
#define   _IQ3mpyI32int(A,B)  ((I32_IQ) ((A) * (float) (B)))
#define   _IQ2mpyI32int(A,B)  ((I32_IQ) ((A) * (float) (B)))
#define   _IQ1mpyI32int(A,B)  ((I32_IQ) ((A) * (float) (B)))
//---------------------------------------------------------------------------
#define   _IQmpyI32frac(A,B)    ((A) - (float)((I32_IQ) ((A) * (float) (B))))
#define   _IQ30mpyI32frac(A,B)  ((A) - (float)((I32_IQ) ((A) * (float) (B))))
#define   _IQ29mpyI32frac(A,B)  ((A) - (float)((I32_IQ) ((A) * (float) (B))))
#define   _IQ28mpyI32frac(A,B)  ((A) - (float)((I32_IQ) ((A) * (float) (B))))
#define   _IQ27mpyI32frac(A,B)  ((A) - (float)((I32_IQ) ((A) * (float) (B))))
#define   _IQ26mpyI32frac(A,B)  ((A) - (float)((I32_IQ) ((A) * (float) (B))))
#define   _IQ25mpyI32frac(A,B)  ((A) - (float)((I32_IQ) ((A) * (float) (B))))
#define   _IQ24mpyI32frac(A,B)  ((A) - (float)((I32_IQ) ((A) * (float) (B))))
#define   _IQ23mpyI32frac(A,B)  ((A) - (float)((I32_IQ) ((A) * (float) (B))))
#define   _IQ22mpyI32frac(A,B)  ((A) - (float)((I32_IQ) ((A) * (float) (B))))
#define   _IQ21mpyI32frac(A,B)  ((A) - (float)((I32_IQ) ((A) * (float) (B))))
#define   _IQ20mpyI32frac(A,B)  ((A) - (float)((I32_IQ) ((A) * (float) (B))))
#define   _IQ19mpyI32frac(A,B)  ((A) - (float)((I32_IQ) ((A) * (float) (B))))
#define   _IQ18mpyI32frac(A,B)  ((A) - (float)((I32_IQ) ((A) * (float) (B))))
#define   _IQ17mpyI32frac(A,B)  ((A) - (float)((I32_IQ) ((A) * (float) (B))))
#define   _IQ16mpyI32frac(A,B)  ((A) - (float)((I32_IQ) ((A) * (float) (B))))
#define   _IQ15mpyI32frac(A,B)  ((A) - (float)((I32_IQ) ((A) * (float) (B))))
#define   _IQ14mpyI32frac(A,B)  ((A) - (float)((I32_IQ) ((A) * (float) (B))))
#define   _IQ13mpyI32frac(A,B)  ((A) - (float)((I32_IQ) ((A) * (float) (B))))
#define   _IQ12mpyI32frac(A,B)  ((A) - (float)((I32_IQ) ((A) * (float) (B))))
#define   _IQ11mpyI32frac(A,B)  ((A) - (float)((I32_IQ) ((A) * (float) (B))))
#define   _IQ10mpyI32frac(A,B)  ((A) - (float)((I32_IQ) ((A) * (float) (B))))
#define   _IQ9mpyI32frac(A,B)   ((A) - (float)((I32_IQ) ((A) * (float) (B))))
#define   _IQ8mpyI32frac(A,B)   ((A) - (float)((I32_IQ) ((A) * (float) (B))))
#define   _IQ7mpyI32frac(A,B)   ((A) - (float)((I32_IQ) ((A) * (float) (B))))
#define   _IQ6mpyI32frac(A,B)   ((A) - (float)((I32_IQ) ((A) * (float) (B))))
#define   _IQ5mpyI32frac(A,B)   ((A) - (float)((I32_IQ) ((A) * (float) (B))))
#define   _IQ4mpyI32frac(A,B)   ((A) - (float)((I32_IQ) ((A) * (float) (B))))
#define   _IQ3mpyI32frac(A,B)   ((A) - (float)((I32_IQ) ((A) * (float) (B))))
#define   _IQ2mpyI32frac(A,B)   ((A) - (float)((I32_IQ) ((A) * (float) (B))))
#define   _IQ1mpyI32frac(A,B)   ((A) - (float)((I32_IQ) ((A) * (float) (B))))
//---------------------------------------------------------------------------
#define   _IQmag(A,B)         sqrt((A)*(A) + (B)*(B))
#define   _IQ30mag(A,B)       sqrt((A)*(A) + (B)*(B))
#define   _IQ29mag(A,B)       sqrt((A)*(A) + (B)*(B))
#define   _IQ28mag(A,B)       sqrt((A)*(A) + (B)*(B))
#define   _IQ27mag(A,B)       sqrt((A)*(A) + (B)*(B))
#define   _IQ26mag(A,B)       sqrt((A)*(A) + (B)*(B))
#define   _IQ25mag(A,B)       sqrt((A)*(A) + (B)*(B))
#define   _IQ24mag(A,B)       sqrt((A)*(A) + (B)*(B))
#define   _IQ23mag(A,B)       sqrt((A)*(A) + (B)*(B))
#define   _IQ22mag(A,B)       sqrt((A)*(A) + (B)*(B))
#define   _IQ21mag(A,B)       sqrt((A)*(A) + (B)*(B))
#define   _IQ20mag(A,B)       sqrt((A)*(A) + (B)*(B))
#define   _IQ19mag(A,B)       sqrt((A)*(A) + (B)*(B))
#define   _IQ18mag(A,B)       sqrt((A)*(A) + (B)*(B))
#define   _IQ17mag(A,B)       sqrt((A)*(A) + (B)*(B))
#define   _IQ16mag(A,B)       sqrt((A)*(A) + (B)*(B))
#define   _IQ15mag(A,B)       sqrt((A)*(A) + (B)*(B))
#define   _IQ14mag(A,B)       sqrt((A)*(A) + (B)*(B))
#define   _IQ13mag(A,B)       sqrt((A)*(A) + (B)*(B))
#define   _IQ12mag(A,B)       sqrt((A)*(A) + (B)*(B))
#define   _IQ11mag(A,B)       sqrt((A)*(A) + (B)*(B))
#define   _IQ10mag(A,B)       sqrt((A)*(A) + (B)*(B))
#define   _IQ9mag(A,B)        sqrt((A)*(A) + (B)*(B))
#define   _IQ8mag(A,B)        sqrt((A)*(A) + (B)*(B))
#define   _IQ7mag(A,B)        sqrt((A)*(A) + (B)*(B))
#define   _IQ6mag(A,B)        sqrt((A)*(A) + (B)*(B))
#define   _IQ5mag(A,B)        sqrt((A)*(A) + (B)*(B))
#define   _IQ4mag(A,B)        sqrt((A)*(A) + (B)*(B))
#define   _IQ3mag(A,B)        sqrt((A)*(A) + (B)*(B))
#define   _IQ2mag(A,B)        sqrt((A)*(A) + (B)*(B))
#define   _IQ1mag(A,B)        sqrt((A)*(A) + (B)*(B))
//---------------------------------------------------------------------------
#define   _atoIQ(A)           atof(A)
#define   _atoIQ30(A)         atof(A)
#define   _atoIQ29(A)         atof(A)
#define   _atoIQ28(A)         atof(A)
#define   _atoIQ27(A)         atof(A)
#define   _atoIQ26(A)         atof(A)
#define   _atoIQ25(A)         atof(A)
#define   _atoIQ24(A)         atof(A)
#define   _atoIQ23(A)         atof(A)
#define   _atoIQ22(A)         atof(A)
#define   _atoIQ21(A)         atof(A)
#define   _atoIQ20(A)         atof(A)
#define   _atoIQ19(A)         atof(A)
#define   _atoIQ18(A)         atof(A)
#define   _atoIQ17(A)         atof(A)
#define   _atoIQ16(A)         atof(A)
#define   _atoIQ15(A)         atof(A)
#define   _atoIQ14(A)         atof(A)
#define   _atoIQ13(A)         atof(A)
#define   _atoIQ12(A)         atof(A)
#define   _atoIQ11(A)         atof(A)
#define   _atoIQ10(A)         atof(A)
#define   _atoIQ9(A)          atof(A)
#define   _atoIQ8(A)          atof(A)
#define   _atoIQ7(A)          atof(A)
#define   _atoIQ6(A)          atof(A)
#define   _atoIQ5(A)          atof(A)
#define   _atoIQ4(A)          atof(A)
#define   _atoIQ3(A)          atof(A)
#define   _atoIQ2(A)          atof(A)
#define   _atoIQ1(A)          atof(A)
//###########################################################################
#endif  // No more.
//###########################################################################

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __IQMATH_H_INCLUDED__ */
