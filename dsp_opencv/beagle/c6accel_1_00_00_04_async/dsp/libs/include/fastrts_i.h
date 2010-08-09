
/* Contains the include files of the inline C functions */

#ifndef _FASTRTS_I_H_
#define _FASTRTS_I_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef _DEBUG
	#define CHECK_OVERFLOW 1
#endif
#include <limits.h>
#include <errno.h>
#include "c60_32.h"
#include "formi32.h"
#include "syntfi.h"
#include "ieeef.h"

#include "common_fastrts.h"
#include "addsp_i.h"		/* Single precision Inline Addition  */
#include "subsp_i.h"		/* Single precision Inline Subtracton */
#include "mpysp_i.h"		/* Single precision Inline Multiplication */
#include "divsp_i.h"		/* Single precision Inline Division */
#include "recipsp_i.h"	/* Single precsion Inline Reciprocal */
#include "intsp_i.h"		/* Single precision Signed Int to Float conversion */
#include "uintsp_i.h"	  /* Single precsion Unsigned Int to Float conversion */
#include "spint_i.h"		/* Single precision Float to Signed Int conversion */
#include "spuint_i.h"	  /* Single precision Float to Unsigned Int conversion */
#include "sqrtsp_i.h"   /*Single precision Inline Square root */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _FASTRTS_I_H_ */

