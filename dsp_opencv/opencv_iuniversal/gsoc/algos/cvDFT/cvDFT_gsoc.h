/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.30.00.01 10-24-05 (swat-j02)" */
/*
 *  ======== fir_ti.h ========
 *  Interface for the CVDFT_GSOC module; TI's implementation 
 *  of the ICVDFT interface
 */
#ifndef CVDFT_GSOC_
#define CVDFT_GSOC_

#include <ti/xdais/ialg.h>

#include "icvDFT.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== CVDFT_GSOC_IALG ========
 *  TI's implementation of the IALG interface for CVDFT
 */
extern IALG_Fxns CVDFT_GSOC_IALG;

/*
 *  ======== CVDFT_GSOC_IFIR ========
 *  TI's implementation of the ICVDFT interface
 */
extern IUNIVERSAL_Fxns CVDFT_GSOC_ICVDFT;

/*
 *  ======== CVDFT_GSOC_exit ========
 *  Required module finalization function
 */
extern Void CVDFT_GSOC_exit(Void);

/*
 *  ======== CVDFT_GSOC_init ========
 *  Required module initialization function
 */
extern Void CVDFT_GSOC_init(Void);

extern Void CVDFT_GSOC_filter(IUNIVERSAL_Handle, Short *, Short *);
extern Int CVDFT_GSOC_control(IUNIVERSAL_Handle, IUNIVERSAL_Cmd, IUNIVERSAL_Status *);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  /* CVDFT_GSOC_ */


