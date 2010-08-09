/****************************************************************************/
/*  ieeef.h          v4.20                                                 */
/*  Copyright (c) 1997-2001  Texas Instruments Incorporated                 */
/****************************************************************************/
 
/* This source automatically generated on Wed Aug  2 15:54:34 CDT 2000 */
 
#ifndef __ieeef__
#define __ieeef__

#include <limits.h>
#include "access.h"

__EXTERN float _absf(float x);
__EXTERN float _negf(float x);

__EXTERN float _addf(float left, float right);
__EXTERN float _subf(float left, float right);
__EXTERN float _mpyf(float left, float right);
__EXTERN float _divf(float left, float right);

__EXTERN int _cmpf(float left,
                 float right); /* <0 if LT, 0 if EQ, >0 if GT     */
                               /* no correct result if NaN source */

__EXTERN int _eqlf(float left, float right);
__EXTERN int _neqf(float left, float right);
__EXTERN int _leqf(float left, float right);
__EXTERN int _geqf(float left, float right);
__EXTERN int _lssf(float left, float right);
__EXTERN int _gtrf(float left, float right);


/* char <-> float */
__EXTERN float         _fltcf(  signed char x);
__EXTERN float         _fltbf(unsigned char x);
__EXTERN   signed char _fixfc(float x);
__EXTERN unsigned char _fixfb(float x);

/* short <-> float */
__EXTERN float          _fltsif(         short x);
__EXTERN float          _fltusf(unsigned short x);
__EXTERN short          _fixfsi(float x);
__EXTERN unsigned short _fixfus(float x);

/* int <-> float */
__EXTERN float    _fltif(int      x);
__EXTERN float    _fltuf(unsigned x);
__EXTERN int      _fixfi(float x);
__EXTERN unsigned _fixfu(float x);

/* long <-> float */
__EXTERN float         _fltlif(         long x);
__EXTERN float         _fltulf(unsigned long x);
__EXTERN long          _fixfli(float x);
__EXTERN unsigned long _fixful(float x);

#include "unaccess.h"

#endif /* __ieeef__*/
