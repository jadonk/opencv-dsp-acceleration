/*
 *  ======== opencvtest.c ========
 *  Test application for OPENCVTEST algorithm.
 */

#include <xdc/std.h>
#include <ti/sdo/ce/Engine.h>
#include <ti/sdo/ce/osal/Memory.h>
#include <ti/sdo/ce/trace/gt.h>
#include <string.h>

#include <ti/sdo/ce/CERuntime.h>
#include <ti/sdo/ce/trace/gt.h>

#include <ti/sdo/ce/universal/universal.h>
#include <beagle/algos/opencv/iopencv.h>
//#include <beagle/algos/opencv/opencv_beagle.h>

/* Standard Linux headers */
#include <stdio.h>
#include <stdlib.h>

//#include "beagle_opencv_API.h"
#include "opencvDsp.h"




#ifndef BUFALIGN
#define BUFALIGN 128
#endif

    

static OPENCV_Operation operation;
static XDAS_Int32 inBufLen;
static XDAS_Int32 outBufLen;
static IOPENCV_Params *opencvParams;

/* trace info: module name, mask */
GT_Mask curMask = {0,0};


static String progName     = "app";
static String opencvName  = "opencv";



static void opencvIuniversal(XDAS_Int8 *, XDAS_Int8 *, XDAS_Int8 *);


/* Universal  Codec Handle */
static UNIVERSAL_Handle codec = NULL;

static  Engine_Handle ce = NULL;



void opencvDspSetParams(int s_type, int s_step , char *s_data, int s_rows , int s_cols , int s_channels , int d_type , int d_step , char *d_data, int d_rows , int d_cols , int d_channels )   
{ 
    opencvParams = (IOPENCV_Params *)malloc(sizeof(IOPENCV_Params));

    /* Assign OpenCV parameters */
    opencvParams->size 		= sizeof (IOPENCV_Params);
    
    opencvParams->s_type	= s_type;
    opencvParams->s_step	= s_step;
    opencvParams->s_ptr		= (XDAS_Int8 *)s_data;
    opencvParams->s_rows	= s_rows;
    opencvParams->s_cols	= s_cols;
    opencvParams->s_channels 	= s_channels;

    opencvParams->d_type	= d_type;
    opencvParams->d_step	= d_step;
    opencvParams->d_ptr		= (XDAS_Int8 *)d_data;
    opencvParams->d_rows	= d_rows;
    opencvParams->d_cols	= d_cols;
    opencvParams->d_channels   	= d_channels;

    inBufLen			= (opencvParams->s_cols * opencvParams->s_rows * opencvParams->s_channels * ( (07 & opencvParams->s_type) < 2 ? sizeof(XDAS_Int8) : (07 & opencvParams->s_type) < 4 ? sizeof(XDAS_Int16) : sizeof(XDAS_Int32) ));
    outBufLen			= (opencvParams->d_cols * opencvParams->d_rows * opencvParams->d_channels * ( (07 & opencvParams->d_type) < 2 ? sizeof(XDAS_Int8) : (07 & opencvParams->d_type) < 4 ? sizeof(XDAS_Int16) : sizeof(XDAS_Int32) ) );

    
}

/*
 *  ======== opencvDspOperations ========
 *
 *  separate different operation
 *
 */


void opencvDspOperations()
{


    
    /*
     * Format for -> Void OPENCVTEST_runTests(XDAS_Int16 *inbuf_1, XDAS_Int16 *inbuf_2, XDAS_Int16 *outbuf, OPENCV_Operation opencvOperation);
     */
    switch ( operation ) {

	   case OPERATION_GEN_TWIDDLE :
    	   	opencvIuniversal(NULL, NULL, opencvParams->d_ptr);
		break;
           
           case OPERATION_DFT :
	   case OPERATION_IDFT :
	   case OPERATION_DFTROWS :
	   case OPERATION_IDFTROWS :
// 		opencvIuniversal(NULL, NULL, opencvParams->d_ptr);
//		printf("Twiddle-factors Generated....Starting DFT\n");
		opencvIuniversal(opencvParams->s_ptr, NULL, opencvParams->d_ptr); 
		break;

	   case OPERATION_SOBEL3x3 :
	   case OPERATION_SOBEL5x5 :
 	   case OPERATION_SOBEL7x7 :
		opencvIuniversal(opencvParams->s_ptr, NULL, opencvParams->d_ptr); 
		break;

	   case OPERATION_INTEGRAL :
		opencvIuniversal(opencvParams->s_ptr, NULL, opencvParams->d_ptr); 
		break;

	   default :
		return;
    }


}
     


/*
 *  ======== OPENCVTEST_runTests ========
 *
 *  call iuniversal API
 *
 */
static void opencvIuniversal(XDAS_Int8 *inbuf_1, XDAS_Int8 *inbuf_2, XDAS_Int8 *outbuf )
{
    int	status;
    
    
    XDAS_Int8 *inputBuf1, *inputBuf2;
    XDAS_Int8 *outputBuf;
    XDAS_Int8 *isNull;

    UNIVERSAL_InArgs universalInArgs;
    IOPENCV_InArgs inArgs;
    inArgs.universalInArgs = universalInArgs;
    inArgs.operation = operation;	
    inArgs.dftSize = opencvParams->s_cols; 
    inArgs.universalInArgs.size = sizeof(IOPENCV_InArgs); 

    UNIVERSAL_OutArgs outArgs;
    outArgs.size = sizeof(outArgs);

    XDM1_BufDesc universalInBufDesc;
    XDM1_BufDesc universalOutBufDesc;


    inputBuf1 = Memory_contigAlloc(inBufLen, BUFALIGN);
    inputBuf2 = Memory_contigAlloc(inBufLen, BUFALIGN);
    outputBuf = Memory_contigAlloc(outBufLen, BUFALIGN);
    if ((inputBuf1 == NULL) || (inputBuf2 == NULL) || (outputBuf == NULL)) {
        printf("App-> ERROR: Memory_contigAlloc for buffers failed");
    }

    isNull = Memory_contigAlloc(sizeof(XDAS_Int8) * 2, BUFALIGN); // currently support only 2 buffer. That is why it is * 2.
    isNull[0] = 1;
    isNull[1] = 1;

    if (inbuf_1) {

       memcpy( inputBuf1, inbuf_1, inBufLen);
       isNull[0] = 0;
    }
    if (inbuf_2) {

       memcpy( inputBuf2, inbuf_2, inBufLen);
       isNull[0] = 0;
    }

    /* Passing buffer size and buffer pointer which will be address translated */
    universalInBufDesc.numBufs = 3; 
    universalOutBufDesc.numBufs = 1;
    
    universalInBufDesc.descs[0].bufSize = inBufLen; 
    universalInBufDesc.descs[0].buf = (XDAS_Int8 *)inputBuf1;


    universalInBufDesc.descs[1].bufSize = inBufLen; 
    universalInBufDesc.descs[1].buf = (XDAS_Int8 *)inputBuf2;

    universalInBufDesc.descs[2].bufSize = (sizeof(XDAS_Int8) * 2); 
    universalInBufDesc.descs[2].buf = isNull;


    universalOutBufDesc.descs[0].bufSize = outBufLen;
    universalOutBufDesc.descs[0].buf = (XDAS_Int8 *)outputBuf;

   

    
    status = UNIVERSAL_process(codec, &universalInBufDesc, &universalOutBufDesc, NULL, (IUNIVERSAL_InArgs *) &inArgs, &outArgs);
    
    if (status != UNIVERSAL_EOK) {
        printf("Error calling OPENCV module:%x\n",status);
        goto exit;
    }
    
    memcpy( opencvParams->d_ptr, outputBuf, outBufLen);
    

    exit:
    
    Memory_contigFree(inputBuf1,  inBufLen);
    Memory_contigFree(inputBuf2,  inBufLen);
    Memory_contigFree(outputBuf, outBufLen );
    Memory_contigFree(isNull, sizeof(XDAS_Int8) * 2);
    
}


/*
 *  ======== OPENCVTEST_main ========
 */
int cvDspInit( )
{
    char engine_Name[] = "opencvEngine";
    char *engineName = engine_Name;


    /* Inatilaize codec-engine */    

    CERuntime_init();

//    printf("App-> Application started.\n");

        
    /* reset, load, and start DSP Engine */
    if ((ce = Engine_open(engineName, NULL, NULL)) == NULL) {
        printf("%s: error: can't open engine %s\n", progName, engineName);
        return (0);
    }

    return -1;
}


int cvCreateEngine(OPENCV_Operation operationFxn)
{

    operation = operationFxn;
    /* allocate and initialize opencv algo on the engine */
    codec = UNIVERSAL_create(ce, opencvName, (IUNIVERSAL_Params *)opencvParams);
    if (codec == NULL) {
        printf( "App-> ERROR: can't open codec %s\n", opencvName);
        return 0;
    }

    /* Enter app */
    opencvDspOperations( ); 

    /* teardown the codec */
    if (codec) {
        UNIVERSAL_delete(codec);
    }
    return -1;
}


void cvEndDSP()
{
    /* close the engine */
    if (ce) {
        Engine_close(ce);
    }

//    printf("app ended.\n");
    
}

