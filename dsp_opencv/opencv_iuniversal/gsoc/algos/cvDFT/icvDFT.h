/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.30.00.01 10-24-05 (swat-j02)" */
/*
 *  ======== icvDFT.h ========
 *  ICVDFT Interface Header
 */
#ifndef ICVDFT_
#define ICVDFT_

#include <ti/xdais/ialg.h>
#include <ti/xdais/xdas.h>

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/* Extend IUNIVERSAL_Params to include filterLen and frameLen */
typedef struct ICVDFT_Params {
    Int size;
    Int filterLen;
    Int frameLen;
} ICVDFT_Params;

/* ICVDFT_Status should not have a pointer since it will not be address translated - instead, IUNIVERSAL_Status.data is used (which will be address translated */
typedef struct ICVDFT_Status {
    Int size;
    Int *coeffPtr;
    Int padding;
} ICVDFT_Status;

/*
 *  ======== ICVDFT_Cmd ========
 *  The Cmd enumeration defines the control commands for the CVDFT
 *  control method.
 */
typedef enum ICVDFT_Cmd {
    ICVDFT_GETSTATUS,
    ICVDFT_SETSTATUS
} ICVDFT_Cmd;


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  /* ICVDFT_ */


