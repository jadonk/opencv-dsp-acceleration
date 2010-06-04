/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.30.00.01 10-24-05 (swat-j02)" */
/*
 *  ======== cvDFT_gsoc_vt.c ========
 *  Definition of CVDFT_GSOC_ICVDFT, the ICVDFT function structure the defines TI's
 *  implementation of the ICVDFT interface.  Since ICVDFT derives from IALG,
 *  we also define the symbol CVDFT_GSOC_IALG, TI's implementation of the IALG
 *  interface.
 *
 *  We place these tables in a separate file for two reasons:
 *     1. We want allow one to replace these tables with
 *    different definitions.  For example, one may
 *    want to build a system where the CVDFT is activated
 *    once and never deactivated, moved, or freed.
 *
 *     2. Eventually there will be a separate "system build"
 *    tool that builds these tables automatically 
 *    and if it determines that only one implementation
 *    of an API exists, "short circuits" the vtable by
 *    linking calls directly to the algorithm's functions.
 */

#include <xdc/std.h>
#include <ti/xdais/dm/iuniversal.h>
#include <ti/xdais/ialg.h>
#include "icvDFT.h"
#include "cvDFT_gsoc.h" 

extern Void CVDFT_GSOC_activate(IALG_Handle handle);
extern Int CVDFT_GSOC_alloc(const IALG_Params *, IALG_Fxns **, IALG_MemRec *);
extern Int CVDFT_GSOC_xdmControl(IUNIVERSAL_Handle, IUNIVERSAL_Cmd, IUNIVERSAL_DynamicParams *, IUNIVERSAL_Status *);
extern Int CVDFT_GSOC_control(IUNIVERSAL_Handle, IUNIVERSAL_Cmd, IUNIVERSAL_Status *);
extern Void CVDFT_GSOC_deactivate(IALG_Handle handle);
extern Int  CVDFT_GSOC_free(IALG_Handle, IALG_MemRec *);
extern Int CVDFT_GSOC_initObj(IALG_Handle, const IALG_MemRec *, IALG_Handle, const IALG_Params *);
extern Void CVDFT_GSOC_moved(IALG_Handle, const IALG_MemRec *, IALG_Handle, const IALG_Params *);
extern Int CVDFT_GSOC_process(IUNIVERSAL_Handle, XDM1_BufDesc *, XDM1_BufDesc *, XDM1_BufDesc, IUNIVERSAL_InArgs *, IUNIVERSAL_OutArgs *);
extern Int  CVDFT_GSOC_numAlloc(Void);

#define IALGFXNS                                                 \
    &CVDFT_GSOC_IALG,       /* module ID */                          \
    CVDFT_GSOC_activate,    /* activate */                           \
    CVDFT_GSOC_alloc,       /* algAlloc */                           \
    NULL,               /* control (NULL => no control ops) */   \
    CVDFT_GSOC_deactivate,  /* deactivate */                         \
    CVDFT_GSOC_free,        /* free */                               \
    CVDFT_GSOC_initObj,     /* init */                               \
    CVDFT_GSOC_moved,       /* moved (NULL => not suported) */       \
    CVDFT_GSOC_numAlloc     /* numAlloc (NULL => IALG_DEFMEMRECS) */

/*
 *  ======== CVDFT_GSOC_IFIR ========
 *  This structure defines TI's implementation of the IFIR interface
 *  for the CVDFT_GSOC module.
 */
/* Note that the typecasting of the process and control functions is needed as the IUNIVERSAL_Fxns struct expects IUNIVERSAL_process() to return XDAS_Int32 whereas our CVDFT_GSOC_process() call returns Int for legacy purposes */
IUNIVERSAL_Fxns CVDFT_GSOC_IFIR = { /* module_vendor_interface */
    {IALGFXNS},                 /* IALG functions */
    (XDAS_Int32(*)(IUNIVERSAL_Handle, XDM1_BufDesc *, XDM1_BufDesc *, XDM1_BufDesc *, IUNIVERSAL_InArgs *, IUNIVERSAL_OutArgs *))CVDFT_GSOC_process,             /* Process function */
    (XDAS_Int32(*)(IUNIVERSAL_Handle, IUNIVERSAL_Cmd, IUNIVERSAL_DynamicParams *, IUNIVERSAL_Status *))CVDFT_GSOC_xdmControl,             /* Control function */
};

/*
 *  ======== CVDFT_GSOC_IALG ========
 *  This structure defines TI's implementation of the IALG interface
 *  for the CVDFT_GSOC module.
 */
#ifdef __TI_COMPILER_VERSION__
asm("_CVDFT_GSOC_IALG .set _CVDFT_GSOC_IFIR");
#else
IALG_Fxns CVDFT_GSOC_IALG = { IALGFXNS };
#endif
/*
 *  We could have also duplicate the structure here to allow this code to be 
 *  compiled and run non-DSP platforms at the expense of unnecessary data space
 *  consumed by the definition below.
 *
 *  IALG_Fxns CVDFT_GSOC_IALG = {
 *      IALGFXNS
 *  };
 */


