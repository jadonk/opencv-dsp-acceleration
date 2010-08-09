/*
 *  Copyright 2008 by Texas Instruments Incorporated.
 *
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *
 */

/*
 *  ======== std.h ========
 *
 */

#ifndef xdc_std__include
#define xdc_std__include

#include <stdarg.h>
#include <stddef.h>

/* macros to simplify "stringification" and computed includes */
#define xdc__stringify(a) #a
#define xdc__local_include(a) xdc__stringify(a.h)
#define xdc__system_include(m) <m.h>

/* TitleCase standard types */

#define xdc_Void        void

typedef char            xdc_Char;
typedef unsigned char   xdc_UChar;
typedef short           xdc_Short;
typedef unsigned short  xdc_UShort;
typedef int             xdc_Int;
typedef unsigned int    xdc_UInt;
typedef long            xdc_Long;
typedef unsigned long   xdc_ULong;
typedef float           xdc_Float;
typedef double          xdc_Double;
typedef long double     xdc_LDouble;
typedef size_t          xdc_SizeT;
typedef va_list         xdc_VaList;

/* Generic Extended Types */

typedef unsigned short  xdc_Bool;
typedef void            *xdc_Ptr;       /* data pointer */
typedef char            *xdc_String;    /* null terminated string */

/* we intentionally omit arguments from Fxn to indicate that it can have
 * any (or none).  Unfortunately this causes gcc to emit warnings when
 * -Wstrict-prototypes is used.  Newer gcc's support a pragma that
 * may work around this:
 *
 *    #pragma GCC diagnostic ignored "-Wstrict-prototype"
 */
typedef int             (*xdc_Fxn)();   /* function pointer */

/*
 * Import the target-specific std.h
 *
 * 'xdc_target__' is defined by build world to reference the
 * target-specific <std.h> file (i.e. <ti/targets/std.h>).
 */
#ifdef xdc_target_types__
#define xdc_target__ <xdc_target_types__>
#endif
#ifdef xdc_target__
#include xdc_target__
#endif

/* Each modules' internal header file defines 'module' as 
 * xdc__MODOBJADDR__(Module__state__V), where Module__state__V is the actual
 * object where the module state is kept. For most targets, the default macro
 * given here results in the construct '(&Module__state__V)->field', when the
 * expression 'module->field' is used. Compilers then generate the code that
 * doesn't dereference a pointer, but puts the address of the field in the
 * code.
 * The targets that need to do something different can define
 * xdc__MODOBJADDR__ in std.h for their target package.
 */
#ifndef xdc__MODOBJADDR__
#define xdc__MODOBJADDR__(symbol) &symbol
#endif

/* Long Long Types */

#ifdef xdc__LONGLONG__
typedef long long               xdc_LLong;
typedef unsigned long long      xdc_ULLong;

#else

#ifndef xdc__INT64__
/* If the target doesn't support "long long" or a 64-bit integral type, we
 * simply use "long".  This is done to ensure that the type LLong always
 * exists, it's at least as long as a "long", and it's 64-bits wide whenever
 * possible.
 */
typedef long                    xdc_LLong;
typedef unsigned long           xdc_ULLong;
#endif

#endif

/* Arg to Ptr and Fxn conversion operators
 *
 * Individual targets may override these definitions in the event
 * that compilers issue warnings about shortening of an Arg to a pointer,
 * for example.
 */
#ifndef xdc__ARGTOPTR
static inline xdc_Ptr xdc_iargToPtr(xdc_IArg a) { return ((xdc_Ptr)a); }
static inline xdc_Ptr xdc_uargToPtr(xdc_UArg a) { return ((xdc_Ptr)a); }
#endif

#ifndef xdc__ARGTOFXN
static inline xdc_Fxn xdc_iargToFxn(xdc_IArg a) { return ((xdc_Fxn)a); }
static inline xdc_Fxn xdc_uargToFxn(xdc_UArg a) { return ((xdc_Fxn)a); }
#endif

/*
 * functions to convert a single precision float to an arg
 * Here assumption is that sizeof(Float) <= sizeof(IArg);
 */
typedef union xdc_FloatData {
    xdc_Float f;
    xdc_IArg  a;
} xdc_FloatData;

static inline xdc_IArg xdc_floatToArg(xdc_Float f)
{
     xdc_FloatData u;
     u.f = f;

     return (u.a);
}

static inline xdc_Float xdc_argToFloat(xdc_IArg a)
{
     xdc_FloatData u;
     u.a = a;

     return (u.f);
}

/* restrict keyword */
#ifndef xdc__RESTRICT__
#define restrict
#endif

/* Unprefixed Aliases */

#ifndef xdc__nolocalnames

#define iargToPtr(a) xdc_iargToPtr(a)
#define uargToPtr(a) xdc_uargToPtr(a)
#define iargToFxn(a) xdc_iargToFxn(a)
#define uargToFxn(a) xdc_uargToFxn(a)
#define floatToArg(a) xdc_floatToArg(a)
#define argToFloat(a) xdc_argToFloat(a)

#define Void xdc_Void

typedef xdc_Char        Char;
typedef xdc_UChar       UChar;
typedef xdc_Short       Short;
typedef xdc_UShort      UShort;
typedef xdc_Int         Int;
typedef xdc_UInt        UInt;
typedef xdc_Long        Long;
typedef xdc_ULong       ULong;
typedef xdc_LLong       LLong;
typedef xdc_ULLong      ULLong;
typedef xdc_Float       Float;
typedef xdc_Double      Double;
typedef xdc_LDouble     LDouble;
typedef xdc_SizeT       SizeT;
typedef xdc_VaList      VaList;

typedef xdc_IArg        IArg;
typedef xdc_UArg        UArg;
typedef xdc_Bool        Bool;
typedef xdc_Int8        Int8;
typedef xdc_Int16       Int16;
typedef xdc_Int32       Int32;
typedef xdc_Fxn         Fxn;
typedef xdc_Ptr         Ptr;
typedef xdc_String      String;

typedef xdc_UInt8       UInt8;
typedef xdc_UInt16      UInt16;
typedef xdc_UInt32      UInt32;

/* DEPRECATED Aliases */
#ifndef xdc__strict
#define _TI_STD_TYPES

/* xdc_Arg is defined only in ti/targets/std.h; use IArg and UArg instead */
#ifdef xdc__ARG__
typedef xdc_Arg         Arg;
#endif

typedef xdc_UInt8       Uint8;
typedef xdc_UInt16      Uint16;
typedef xdc_UInt32      Uint32;
typedef xdc_UInt        Uns;
#endif

/*
 *  ======== optional types ========
 *  The following types are not always defined for all targets
 */
#ifdef xdc__INT64__
typedef xdc_Int64       Int64;
typedef xdc_UInt64      UInt64;
#endif

/* The following exact size types are not required by C99 and may not be
 * supported by some compiler/processor environments.  For greater
 * portability, use the IntN or UIntN types above.
 */
#ifdef xdc__BITS8__
typedef xdc_Bits8       Bits8;
#endif

#ifdef xdc__BITS16__
typedef xdc_Bits16      Bits16;
#endif

#ifdef xdc__BITS32__
typedef xdc_Bits32      Bits32;
#endif

#ifdef xdc__BITS64__
typedef xdc_Bits64      Bits64;
#endif

#endif /* xdc__nolocalnames */

/* Standard Constants */

#undef NULL
#define NULL 0

#undef FALSE
#define FALSE 0

#undef TRUE
#define TRUE 1

/* Declaration Qualifiers */

#ifndef __FAR__
#define __FAR__
#endif

/* Code-Section Directive */

#ifndef xdc__CODESECT
#define xdc__CODESECT(fn, sn)
#endif

#endif /* xdc_std__include */
/*
 *  @(#) xdc; 1, 1, 1, 0,143; 11-1-2008 22:31:41; /db/ztree/library/trees/xdc-s46x/src/packages/
 */

