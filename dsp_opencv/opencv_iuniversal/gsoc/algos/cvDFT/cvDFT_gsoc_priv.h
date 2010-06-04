/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.30.00.01 10-24-05 (swat-j02)" */
/*
 *  ======== cvDFT_gsoc_priv.h ========
 *  Internal vendor specific (TI) interface header for CVDFT
 *  algorithm. Only the implementation source files include
 *  this header; this header is not shipped as part of the
 *  algorithm.
 *
 *  This header contains declarations that are specific to
 *  this implementation and which do not need to be exposed
 *  in order for an application to use the CVDFT algorithm.
 */

#ifndef CVDFT_GSOC_PRIV_
#define CVDFT_GSOC_PRIV_
 
#include <xdc/std.h>

#include    "icvDFT.h"
#include    "cvDFT_gsoc.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== CVDFT_GSOC_Obj ========
 */
typedef struct CVDFT_GSOC_Obj {
    IALG_Obj    alg;            /* MUST be first field of all CVDFT objs */
    Short       *workBuf;       /* on-chip scratch history */
    Short       *history;       /* off chip presistant history */
    Short       *coeff;         /* on-chip persistant coeff */
    Int         filterLenM1;    /* length of coefficient array - 1 */
    Int         frameLen;       /* length of input (output) buffer */
} CVDFT_GSOC_Obj;

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  /* CVDFT_GSOC_PRIV_ */


