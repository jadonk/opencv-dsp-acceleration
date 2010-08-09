/* ======================================================================== */
/*  TEXAS INSTRUMENTS, INC.                                                 */
/*                                                                          */
/*  IMGLIB-2  DSP Image/Video Processing Library                            */
/*                                                                          */
/*      Release:        Revision 2.0.1                                      */
/*      Snapshot date:  6-May-2008                                          */
/*                                                                          */
/*  This library contains proprietary intellectual property of Texas        */
/*  Instruments, Inc.  The library and its source code are protected by     */
/*  various copyrights, and portions may also be protected by patents or    */
/*  other legal protections.                                                */
/*                                                                          */
/*  This software is licensed for use with Texas Instruments TMS320         */
/*  family DSPs.  This license was provided to you prior to installing      */
/*  the software.  You may review this license by consulting the file       */
/*  TI_license.PDF which accompanies the files in this library.             */
/* ------------------------------------------------------------------------ */
/*          Copyright (C) 2008 Texas Instruments, Incorporated.             */
/*                          All Rights Reserved.                            */
/* ======================================================================== */

#ifdef _HOST_BUILD

/* To build img_lib on pc with VC++, masking out restrict. */
#define restrict
#define _nassert(a) assert(a)

#define abs(X) ( (X) > 0 ? (X) : -(X))

#define MIN_INT40_HI      ((int8)(0xFFFFFF80))
#define MIN_INT40_LO    ((uint32)(0x00000000))

typedef char               int8;
typedef int               int32;
typedef unsigned char     uint8;
typedef unsigned int     uint32;
typedef __int64           int40;
typedef __int64          uint40;
typedef __int64           int64;

typedef struct _INT40X2
{
  int32 lo;
  int8  hi;
} int40x2;

typedef struct _INT40X2U
{
  uint32 lo;
  uint8  hi;
} int40x2u;


union reg40
{
  int40    x1;
  int40x2  x2;

  uint40   x1u;
  int40x2u x2u;
};

uint32 _lnorm(int40 a);

#else /* HOST_BUILD */

typedef long long         int64;

#endif /* _HOST_BUILD */



