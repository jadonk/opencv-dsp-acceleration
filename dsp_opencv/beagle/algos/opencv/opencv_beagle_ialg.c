
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
//#define     WORKBUF2  	2
//#define     NUMBUFS     3
#define     NUMBUFS     2

/* Added default PARAMS within the codec */
static IOPENCV_Params IOPENCV_PARAMS = {
    sizeof(IOPENCV_Params), /* size */
    1,                   /* s_type */
    640,                 /* s_step */
    NULL,	         /* s_ptr */
    640,		 /* s_cols */
    480,		 /* s_rows */
    1,                   /* s_channels */

    1,                   /* d_type */
    640,                 /* d_step */
    NULL,	         /* d_ptr */
    640,		 /* d_cols */
    480,		 /* d_rows */
    1,                   /* d_channels */
};

/*
 *  ======== OPENCV_BEAGLE_activate ========
 *  Activate our object; e.g., initialize any scratch memory required
 *  by the OPENCV_BEAGLE processing methods.
 */
Void OPENCV_BEAGLE_activate(IALG_Handle handle)
{
//    OPENCV_BEAGLE_Obj *opencv = (Void *)handle;
    
    
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

    memTab[WORKBUF].size        = ( (params->s_cols * params->s_rows) > (params->d_cols * params->d_rows) ) ? (4 * params->s_cols * params->s_rows 					* params->s_channels) : ( 4 * params->d_cols * params->d_rows * params->d_channels) ;
    memTab[WORKBUF].alignment   = 8;
    memTab[WORKBUF].space       = IALG_DARAM0;
    memTab[WORKBUF].attrs       = IALG_SCRATCH;

//    memTab[WORKBUF2].size        = ( (params->s_cols * params->s_rows) > (params->d_cols * params->d_rows) ) ? (4 * params->s_cols * params->s_rows 					* params->s_channels) : ( 4 * params->d_cols * params->d_rows * params->d_channels) ;
//    memTab[WORKBUF2].alignment   = 8;
//    memTab[WORKBUF2].space       = IALG_DARAM0;
//    memTab[WORKBUF2].attrs       = IALG_SCRATCH;
    
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
    
    memTab[WORKBUF].size        = ( (opencv->s_cols * opencv->s_rows) > (opencv->d_cols * opencv->d_rows) ) ? ( 4 * opencv->s_cols * opencv->s_rows * 					opencv->s_channels) : ( 4 * opencv->d_cols * opencv->d_rows * opencv->d_channels) ;
    memTab[WORKBUF].base        = opencv->workBuf;

//    memTab[WORKBUF2].size        = ( (opencv->s_cols * opencv->s_rows) > (opencv->d_cols * opencv->d_rows) ) ? ( 4 * opencv->s_cols * opencv->s_rows * 					opencv->s_channels) : ( 4 * opencv->d_cols * opencv->d_rows * opencv->d_channels) ;
//    memTab[WORKBUF2].base        = opencv->workBuf2;
    
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
//    opencv->workBuf2 = memTab[WORKBUF2].base;
    opencv->workBuf2	= opencv->workBuf + opencv->s_cols * opencv->s_rows * opencv->s_channels;
    opencv->s_type	= params->s_type;
    opencv->s_step 	= params->s_step;
    opencv->s_rows    	= params->s_rows;
    opencv->s_cols   	= params->s_cols;
    opencv->s_channels	= params->s_channels;

    opencv->d_type	= params->d_type;
    opencv->d_step 	= params->d_step;
    opencv->d_rows    	= params->d_rows;
    opencv->d_cols   	= params->d_cols;
    opencv->d_channels	= params->d_channels;

/* Note: As no address translation takes place for the params, we don't need to assign source and destination data pointer */


    return (IALG_EOK);
}

/*  ======== OPENCV_BEAGLE_moved ========
 *  Fix up any pointers to data that have been moved by the client.
 */
Void OPENCV_BEAGLE_moved(IALG_Handle handle, const IALG_MemRec memTab[], IALG_Handle p, const IALG_Params *fopencvParams)
{
    OPENCV_BEAGLE_Obj *opencv = (Void *)handle;

    opencv->workBuf = memTab[WORKBUF].base;
//    opencv->workBuf2 = memTab[WORKBUF2].base;
    opencv->workBuf2 = opencv->workBuf + opencv->s_cols * opencv->s_rows * opencv->s_channels;
}

/*  ======== OPENCV_BEAGLE_numAlloc ========
 *  Returns the number of records needed for the MEM tab by the client.
 */
Int OPENCV_BEAGLE_numAlloc()
{
    return(NUMBUFS);
}


