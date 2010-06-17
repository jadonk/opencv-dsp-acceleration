
/*
 *  ======== OPENCV_BEAGLE_ialg.c ========
 *  Implementation of the OPENCV_BEAGLE.h interface; Implementation 
 *  of the IOPENCV interface. 
 */
#ifdef __TI_COMPILER_VERSION__
/* xDAIS Rule 13 - this #pragma should only apply to TI codegen */
#pragma     CODE_SECTION(OPENCV_BEAGLE_activate,       ".text:algActivate")
#pragma     CODE_SECTION(OPENCV_BEAGLE_alloc,          ".text:algAlloc")  
#pragma     CODE_SECTION(OPENCV_BEAGLE_deactivate,     ".text:algDeactivate")
#pragma     CODE_SECTION(OPENCV_BEAGLE_free,           ".text:algFree")
#pragma     CODE_SECTION(OPENCV_BEAGLE_initObj,        ".text:algInit")
#pragma     CODE_SECTION(OPENCV_BEAGLE_moved,          ".text:algMoved")
#pragma     CODE_SECTION(OPENCV_BEAGLE_numAlloc,       ".text:algNumAlloc")
#endif
 
#include    <xdc/std.h>
#include    <string.h>
#include    <ti/xdais/dm/iuniversal.h>
#include    "iopencv.h"
#include    "opencv_beagle.h"
#include    "opencv_beagle_priv.h"


#define     OBJECT      0
#define     WORKBUF     1
#define     NUMBUFS     2

/* Added default PARAMS within the codec */
static IOPENCV_Params IOPENCV_PARAMS = {
    sizeof(IOPENCV_Params), /* size */
    512,                 /* dftSize */
    512,                 /* frameLen */
    0,		         /* CV_<bit_depth>(S|U|F)C<number_of_channels> */// This is just a bogus. Need to define it dspserver as I cannot use types_c.h
    1,			 /* nchannel */
    0,                   /* image pixel type */  // This is also a bogus. Need to define only for dspserver as I cannot use types_c.h
    512,		 /* step-width from row to row */
    512,		 /* image width */
    512,		 /* image height */
    NULL,                /* Data Pointer */
    NULL,                /* Output Pointer */
};

/*
 *  ======== OPENCV_BEAGLE_activate ========
 *  Activate our object; e.g., initialize any scratch memory required
 *  by the OPENCV_BEAGLE processing methods.
 */
Void OPENCV_BEAGLE_activate(IALG_Handle handle)
{
    OPENCV_BEAGLE_Obj *opencv = (Void *)handle;
    
    
}

/*
 *  ======== OPENCV_BEAGLE_alloc ========
 *  Return a table of memory descriptors that describe the memory needed 
 *  to construct a OPENCV_BEAGLE_Obj structure.
 */
Int OPENCV_BEAGLE_alloc(const IALG_Params *opencvParams, IALG_Fxns **fxns, IALG_MemRec memTab[])
{
    const IOPENCV_Params *params = (Void *)opencvParams;
    
    if (params == NULL) 
    {
        params = &IOPENCV_PARAMS;  /* set default parameters */
    }

    /* Request memory for OPENCV object */
    memTab[OBJECT].size         = sizeof(OPENCV_BEAGLE_Obj);
    memTab[OBJECT].alignment    = 4;
    memTab[OBJECT].space        = IALG_EXTERNAL;
    memTab[OBJECT].attrs        = IALG_PERSIST;
    

    /* Request memory for shared working buffer */

    memTab[WORKBUF].size        = params->frameLen;
    memTab[WORKBUF].alignment   = 8;
    memTab[WORKBUF].space       = IALG_DARAM0;
    memTab[WORKBUF].attrs       = IALG_SCRATCH;
    
    return (NUMBUFS);
}



/*
 *  ======== OPENCV_BEAGLE_deactivate ========
 *  Deactivate our object; e.g., save any scratch memory required
 *  by the OPENCV_BEAGLE processing methods to persistent memory.
 */
Void OPENCV_BEAGLE_deactivate(IALG_Handle handle)
{

}

/*
 *  ======== OPENCV_BEAGLE_free ========
 *  Return a table of memory pointers that should be freed.  Note
 *  that this should include *all* memory requested in the 
 *  OPENCV_BEAGLE_alloc operation above.
 */
Int OPENCV_BEAGLE_free(IALG_Handle handle, IALG_MemRec memTab[])
{
    OPENCV_BEAGLE_Obj *opencv = (Void *)handle;
    
    OPENCV_BEAGLE_alloc(NULL, NULL, memTab);
      
    memTab[OBJECT].base         = opencv;
    
    memTab[WORKBUF].size        = opencv->frameLen;
    memTab[WORKBUF].base        = opencv->workBuf;
    
    return (NUMBUFS);
}

/*  ======== OPENCV_BEAGLE_initObj ========
 *  Initialize the memory allocated for our instance.
 */
Int OPENCV_BEAGLE_initObj(IALG_Handle handle, const IALG_MemRec memTab[], IALG_Handle p, const IALG_Params *opencvParams)
{
    OPENCV_BEAGLE_Obj *opencv = (Void *)handle;
    const IOPENCV_Params *params = (Void *)opencvParams;

    if (params == NULL) 
    {
        params = &IOPENCV_PARAMS;  /* set default parameters */
    }

    opencv->workBuf = memTab[WORKBUF].base;
    opencv->dftSize = params->dftSize;
    opencv->frameLen = params->frameLen;
    opencv->type = params->type;
    opencv->nchannel = params->nchannel;
    opencv->depth = params->depth;
    opencv->step = params->step;
    opencv->width = params->width;
    opencv->height = params->height;
/* Note: As no address translation takes place for the params, we don't need to assign data and output pointer */


    return (IALG_EOK);
}

/*  ======== OPENCV_BEAGLE_moved ========
 *  Fix up any pointers to data that have been moved by the client.
 */
Void OPENCV_BEAGLE_moved(IALG_Handle handle, const IALG_MemRec memTab[], IALG_Handle p, const IALG_Params *fopencvParams)
{
    OPENCV_BEAGLE_Obj *opencv = (Void *)handle;



    opencv->workBuf = memTab[WORKBUF].base;

}

/*  ======== OPENCV_BEAGLE_numAlloc ========
 *  Returns the number of records needed for the MEM tab by the client.
 */
Int OPENCV_BEAGLE_numAlloc()
{
    return(NUMBUFS);
}


