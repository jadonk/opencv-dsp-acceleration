
#ifndef __TYPEDEF_H_INCLUDED__
#define __TYPEDEF_H_INCLUDED__


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define MAX_32 ((Word32)0x7fffffff)
#define MIN_32 ((Word32)0x80000000)

#define MAX_16 ((Word16)0x7fff)
#define MIN_16 ((Word16)0x8000)

typedef short               I16_IQ;
typedef unsigned short      U16_IQ;
typedef int                 I32_IQ;
typedef unsigned int        U32_IQ;

#ifdef _MSC_VER
typedef  __int64            I64_IQ;
typedef  unsigned __int64   U64_IQ;
typedef  signed __int64     Word64;
typedef  unsigned __int64   Uword64;
#else
typedef  long long          I64_IQ;
typedef  unsigned long long U64_IQ;
typedef  signed long long   Word64;
typedef  unsigned long long Uword64;
#endif /* _TMS320C6X */


typedef  short int          Word16;
typedef  unsigned short int Uword16;
typedef  int                Word32;
typedef  unsigned int       Uword32;
typedef  signed long        Word40;
typedef  unsigned long      Uword40;
typedef  unsigned char      Uchar8;
typedef  float              Fword32;

/* Some shift Macros for host. (Shift by 32 not supported) */

#ifdef _TMS320C6X
# define _SHR(a, b) ( (a) >> (b) )
# define _SHL(a, b) ( (a) << (b) )
#else
# define _SHR(a, b) ( ((b) == 32) ? 0 : ( (a) >> (b) ) )
# define _SHL(a, b) ( ((b) == 32) ? 0 : ( (a) << (b) ) )
#endif /* _TMS320C6X */

typedef  struct {
        I32_IQ acc;
        I32_IQ p;
    }word64;


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __TYPEDEF_H_INCLUDED__ */


