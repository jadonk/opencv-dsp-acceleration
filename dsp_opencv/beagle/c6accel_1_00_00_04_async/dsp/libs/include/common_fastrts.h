
/* Contains the macros used in the inline C functions                       */


#define MINUS (((biguns)1)<<(BPbigint-1))
#define REALasINT(I, R) (I)=(biguns)_ftoi(R)
#define REALasUNS(U, R) (U)=_ftoi(R)
#define INTasREAL(R, I) (R)=_itof((biguns)(I))
#define UNSasREAL(R, U) (R)=_itof(U)

#define return_REALasINT(R) return (biguns)_ftoi(R)
#define return_REALasUNS(R) return _ftoi(R)
#define return_INTasREAL(I) return _itof((biguns)(I))
#define return_UNSasREAL(U) return _itof(U)
#define UNORDERED (((int)1)<<(BPint-1)) /* return this for NAN input to CMPF */
#define MASK(bb) ((((bigint)1)<<(bb))-1) /*clear bits above the 2**(bb-1) bit*/

#define REAL_EXP_INFNAN ((((bigint)1)<<REAL_EXP_BITS)-1)
#define REAL_EXP_MASK   ((((bigint)1)<<REAL_EXP_BITS)-1)

#define REAL_FRC_MSBs (REAL_FRC_BITS % BPbigint) /*frc bits MS part*/

#define REAL_FRC_MASK  ((((bigint)1)<< REAL_FRC_MSBs)-1)
#define REAL_HIDDEN_BIT (((bigint)1)<< REAL_FRC_MSBs)
#define REAL_INFNAN (((bigint)REAL_EXP_INFNAN) << REAL_FRC_MSBs)
#define REAL_NSNAN_BIT (((bigint)1)<<(REAL_FRC_MSBs-1)) /* MSB of fraction */
#define REAL_NAN    (REAL_INFNAN+1) /* signalling NAN */
#define REAL_NSNAN  (REAL_INFNAN+REAL_NSNAN_BIT) /*non-signl NAN*/

#define REAL_MOSTPOS (((REAL_EXP_INFNAN-1)<<REAL_FRC_MSBs)+REAL_FRC_MASK)
#define REAL_MOSTNEG (((REAL_EXP_INFNAN-1)<<REAL_FRC_MSBs)+REAL_FRC_MASK+MINUS)


#define RENORM2F(ff,ee) do {int tmp=\
                                COUNT_LZERO(ff)-BPbigint+\
                                (REAL_FRC_BITS%BPbigint)+2;\
                               ff <<= tmp; ee -= tmp;} while (0)

#define CPYF(dst,right) dst=(right)
#define CNST(x) (x##f)/*  constants within statements */
#define CMPF3(dst,left,right) dst=CMPF(left,right)


#define EQLF(left,right) (_eqlf(left,right))
#define NEQF(left,right) (_neqf(left,right))
#define LEQF(left,right) (_leqf(left,right))
#define GEQF(left,right) (_geqf(left,right))
#define LSSF(left,right) (_lssf(left,right))
#define GTRF(left,right) (_gtrf(left,right))

#define EQZF(left) EQLF(left,ZERO)
#define NEZF(left) NEQF(left,ZERO)
#define LEZF(left) LEQF(left,ZERO)
#define GTZF(left) GTRF(left,ZERO)

#define GEZF(left) GEQF(left,ZERO)
#define LSZF(left) LSSF(left,ZERO)
