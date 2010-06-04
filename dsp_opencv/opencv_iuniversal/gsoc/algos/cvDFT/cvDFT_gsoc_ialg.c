/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.30.00.01 10-24-05 (swat-j02)" */
/*
 *  ======== CVDFT_GSOC_ialg.c ========
 *  Implementation of the CVDFT_GSOC.h interface; TI's implementation 
 *  of the ICVDFT interface. 
 */
#ifdef __TI_COMPILER_VERSION__
/* xDAIS Rule 13 - this #pragma should only apply to TI codegen */
#pragma     CODE_SECTION(CVDFT_GSOC_activate,       ".text:algActivate")
#pragma     CODE_SECTION(CVDFT_GSOC_alloc,          ".text:algAlloc")  
#pragma     CODE_SECTION(CVDFT_GSOC_control,        ".text:algOrigControl") 
#pragma     CODE_SECTION(CVDFT_GSOC_deactivate,     ".text:algDeactivate")
#pragma     CODE_SECTION(CVDFT_GSOC_free,           ".text:algFree")
#pragma     CODE_SECTION(CVDFT_GSOC_initObj,        ".text:algInit")
#pragma     CODE_SECTION(CVDFT_GSOC_moved,          ".text:algMoved")
#pragma     CODE_SECTION(CVDFT_GSOC_numAlloc,       ".text:algNumAlloc")
#endif
 
#include <xdc/std.h>
#include    <string.h>
#include <ti/xdais/dm/iuniversal.h>
#include    "icvDFT.h"
#include    "cvDFT_gsoc.h"
#include    "cvDFT_gsoc_priv.h"

#define     OBJECT      0
#define     HISTORY     1
#define     WORKBUF     2
#define     NUMBUFS     3

/* Added default PARAMS within the codec */
static ICVDFT_Params ICVDFT_PARAMS = {
    sizeof(ICVDFT_Params), /* size */
    50,                  /* filterLen */
    50,                  /* frameLen */
};

static Short defaultCoeff[] = { 
/*    0x08FC, 0xF6DE, 0xF92A, 0xFA50, 0xFB17, 0xFBF0, 0xFD2A, 0xFECF,
    0x00EC, 0x036C, 0x0623, 0x08E1, 0x0B6E, 0x0D91, 0x0F1A, 0x0FE9,
    0x0FE9, 0x0F1A, 0x0D91, 0x0B6E, 0x08E1, 0x0623, 0x036C, 0x00EC,
    0xFECF, 0xFD2A, 0xFBF0, 0xFB17, 0xFA50, 0xF92A, 0xF6DE, 0x08FC
*/ 0
 };
/*
 *  ======== CVDFT_GSOC_activate ========
 *  Activate our object; e.g., initialize any scratch memory required
 *  by the CVDFT_GSOC processing methods.
 */
Void CVDFT_GSOC_activate(IALG_Handle handle)
{
    CVDFT_GSOC_Obj *cvDFT = (Void *)handle;
    
    /* copy saved history to working buffer */
    memcpy((Void *)cvDFT->workBuf, (Void *)cvDFT->history, cvDFT->filterLenM1 * sizeof(Short));
}

/*
 *  ======== CVDFT_GSOC_alloc ========
 *  Return a table of memory descriptors that describe the memory needed 
 *  to construct a CVDFT_GSOC_Obj structure.
 */
Int CVDFT_GSOC_alloc(const IALG_Params *cvDFTParams, IALG_Fxns **fxns, IALG_MemRec memTab[])
{
    const ICVDFT_Params *params = (Void *)cvDFTParams;
    
    if (params == NULL) 
    {
        params = &ICVDFT_PARAMS;  /* set default parameters */
    }

    /* Request memory for CVDFT object */
    memTab[OBJECT].size         = sizeof(CVDFT_GSOC_Obj);
    memTab[OBJECT].alignment    = 4;
    memTab[OBJECT].space        = IALG_EXTERNAL;
    memTab[OBJECT].attrs        = IALG_PERSIST;
    
    /*
     *  Request memory filter's "inter-frame" state (i.e., the
     *  delay history)
     *
     *  Note we could have simply added the delay buffer size to the
     *  end of the CVDFT object by combining this request with the one
     *  above, thereby saving some code.  We separate it here for
     *  clarity.
     */
    memTab[HISTORY].size        = (params->filterLen - 1) * sizeof(Short);
    memTab[HISTORY].alignment   = 2;
    memTab[HISTORY].space       = IALG_EXTERNAL;
    memTab[HISTORY].attrs       = IALG_PERSIST;

    /* Request memory for shared working buffer */
    memTab[WORKBUF].size        =  (params->filterLen - 1 + params->frameLen) * sizeof(Short);
    memTab[WORKBUF].alignment   = 2;
    memTab[WORKBUF].space       = IALG_DARAM0;
    memTab[WORKBUF].attrs       = IALG_SCRATCH;
    
    return (NUMBUFS);
}

/*
 *  ======== CVDFT_GSOC_control ========
 */
/* It's no longer necessary to use CVDFT specific types (ICVDFT_Cmd, etc) since we can use IUNIVERSAL instead */
//Int CVDFT_GSOC_control(ICVDFT_Handle handle, ICVDFT_Cmd cmd, ICVDFT_Status *status)
Int CVDFT_GSOC_control(IUNIVERSAL_Handle handle, IUNIVERSAL_Cmd cmd, IUNIVERSAL_Status *status)
{
    CVDFT_GSOC_Obj *cvDFT = (CVDFT_GSOC_Obj *)handle;

    if (cmd == ICVDFT_SETSTATUS)
    {
    //    cvDFT->coeff = status->coeffPtr;
    /* Instead of saving a pointer in status (not address translated), use IUNIVERSAL's status->data (which will be address translated) */
        cvDFT->coeff = (Short *)status->data.descs[0].buf;
        cvDFT->filterLenM1 = (status->data.descs[0].bufSize-sizeof(Short))/sizeof(Short);
    }
    /*else if (cmd == ICVDFT_GETSTATUS)
    {
        status->coeffPtr =  cvDFT->coeff;
    }*/ /* status->data is a read only field */
    else {
        return (IALG_EFAIL);
    }

    return (IALG_EOK);
}

/*
 *  ======== CVDFT_GSOC_deactivate ========
 *  Deactivate our object; e.g., save any scratch memory requred
 *  by the CVDFT_GSOC processing methods to persistent memory.
 */
Void CVDFT_GSOC_deactivate(IALG_Handle handle)
{
    CVDFT_GSOC_Obj *cvDFT = (Void *)handle;
    
    /* copy history to external history buffer */
    memcpy((Void *)cvDFT->history, (Void *)cvDFT->workBuf, cvDFT->filterLenM1 * sizeof(Short));

}

/*
 *  ======== CVDFT_GSOC_free ========
 *  Return a table of memory pointers that should be freed.  Note
 *  that this should include *all* memory requested in the 
 *  CVDFT_GSOC_alloc operation above.
 */
Int CVDFT_GSOC_free(IALG_Handle handle, IALG_MemRec memTab[])
{
    CVDFT_GSOC_Obj *cvDFT = (Void *)handle;
    
    CVDFT_GSOC_alloc(NULL, NULL, memTab);
      
    memTab[OBJECT].base         = cvDFT;
    
    memTab[HISTORY].size        = cvDFT->filterLenM1 * sizeof(Short);
    memTab[HISTORY].base        = cvDFT->history;

    memTab[WORKBUF].size        = (cvDFT->filterLenM1 + cvDFT->frameLen) * sizeof(Short);
    memTab[WORKBUF].base        = cvDFT->workBuf;
    
    return (NUMBUFS);
}

/*  ======== CVDFT_GSOC_initObj ========
 *  Initialize the memory allocated for our instance.
 */
Int CVDFT_GSOC_initObj(IALG_Handle handle, const IALG_MemRec memTab[], IALG_Handle p, const IALG_Params *cvDFTParams)
{
    CVDFT_GSOC_Obj *cvDFT = (Void *)handle;
    const ICVDFT_Params *params = (Void *)cvDFTParams;

    if (params == NULL) 
    {
        params = &ICVDFT_PARAMS;  /* set default parameters */
    }

    cvDFT->workBuf        = memTab[WORKBUF].base;
    cvDFT->history        = memTab[HISTORY].base;
    //cvDFT->coeff = params->coeffPtr; //removed coeffPtr from params (coeff set via control call using Status.data buffer
    cvDFT->filterLenM1    = params->filterLen - 1;
    cvDFT->frameLen       = params->frameLen;

    // Currently the app sets the coeff via UNIVERSAL_control(), but if control() is not called prior to process(), there may not be any coeff set and the alg will fail
    if (cvDFT->coeff == NULL) {
        cvDFT->coeff = (Void *)defaultCoeff;
        cvDFT->filterLenM1 = (sizeof(defaultCoeff) - sizeof(Short))/sizeof(Short);
    }
    
    return (IALG_EOK);
}

/*  ======== CVDFT_GSOC_moved ========
 *  Fix up any pointers to data that have been moved by the client.
 */
Void CVDFT_GSOC_moved(IALG_Handle handle, const IALG_MemRec memTab[], IALG_Handle p, const IALG_Params *fcvDFTParams)
{
    CVDFT_GSOC_Obj *cvDFT = (Void *)handle;

    cvDFT->workBuf = memTab[WORKBUF].base;
    cvDFT->history = memTab[HISTORY].base;
}

/*  ======== CVDFT_GSOC_numAlloc ========
 *  Returns the number of records needed for the MEM tab by the client.
 */
Int CVDFT_GSOC_numAlloc()
{
    return(NUMBUFS);
}


