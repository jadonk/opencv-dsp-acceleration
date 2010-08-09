/*---------------------------------------------------------------------------------**
**                                                                                 **
**   Copyright (c) 2010, Texas Instruments Incorporated                            **
**   All rights reserved.                                                          **
**                                                                                 **
**                                                                                 **
**  *  ======== iC6ACCEL_ti_priv.h ========                                         **
**  *  Internal vendor specific (TI) interface header for C6Accel                  **
**  *  algorithm. Only the implementation source files include                     **
**  *  this header;                                                                **
**  *  This header contains declarations that are specific to                      **
**  *  this implementation and which do not need to be exposed                     **
**  *  in order for an application to use the C6Accel algorithm.                   **
**---------------------------------------------------------------------------------*/
                                                             
#include <ti/xdais/dm/iuniversal.h>
#ifndef ti_sdo_codecs_C6Accel_C6ACCEL_TI_PRIV_
#define ti_sdo_codecs_C6Accel_C6ACCEL_TI_PRIV_

#include <ti/xdais/dm/iuniversal.h>
#include <ti/xdais/idma3.h>
#include <ti/sdo/fc/acpy3/acpy3.h>

/* C6Accel Object definition */
typedef struct C6ACCEL_TI_Obj {
    IALG_Obj    alg;            /* MUST be first field of all XDAS algs */
    /* Alg stuff */
    IDMA3_Handle dmaHandle1D1D8B;  /* DMA logical channel for 1D to 1D xfers */

    void* pIntBuffer1;
    void* pIntBuffer2;

} C6ACCEL_TI_Obj;

/* C6Accel memory allocation function */
extern Int C6ACCEL_TI_alloc(const IALG_Params *algParams,
        IALG_Fxns **pf, IALG_MemRec memTab[]);

/*C6Accel memory free function */
extern Int C6ACCEL_TI_free(IALG_Handle handle, IALG_MemRec memTab[]);

/* C6Accel Object initialization function */
extern Int C6ACCEL_TI_initObj(IALG_Handle handle,
        const IALG_MemRec memTab[], IALG_Handle parent,
        const IALG_Params *algParams);

/* C6Accel process function */
extern XDAS_Int32 C6ACCEL_TI_process(IUNIVERSAL_Handle h,
        XDM1_BufDesc *inBufs, XDM1_BufDesc *outBufs, XDM1_BufDesc *inOutBufs,
        IUNIVERSAL_InArgs *inArgs, IUNIVERSAL_OutArgs *outArgs);

/*C6Accel control function */ 
extern XDAS_Int32 C6ACCEL_TI_control(IUNIVERSAL_Handle handle,
        IUNIVERSAL_Cmd id, IUNIVERSAL_DynamicParams *params,
        IUNIVERSAL_Status *status);

#endif
/*
 *  @(#) dsp.alg.C6Accel;
 */

