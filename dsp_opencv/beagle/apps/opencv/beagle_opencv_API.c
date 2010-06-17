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
#include <beagle/algos/opencv/opencv_beagle.h>

/* Standard Linux headers */
#include <stdio.h>
#include <stdlib.h>

#include "beagle_opencv_API.h"

#ifndef BUFALIGN
#define BUFALIGN 128
#endif

static XDAS_Int32 FRAMELEN;    
static IOPENCV_Params *opencvParams;


/* trace info: module name, mask */
GT_Mask curMask = {0,0};


static String progName     = "app";
static String opencvName  = "opencv";


static void OPENCV_runTests( OPENCV_Operation );
static void OPENCVTEST_runTests(XDAS_Int16 *, XDAS_Int16 *, XDAS_Int16 *, OPENCV_Operation);


/* Universal  Codec Handle */
static UNIVERSAL_Handle codec = NULL;

/* Global flag used to signal when it is time to exit test loop */
int OPENCVTEST_gfxExitThread = 0;

/* Global Quit Function to call */
static Fxn OPENCVTEST_getQuitFxn = NULL;


static int myGetQuitFunction(void)
{
	
	return (0);
}


/*
 *  ======== OPENCV_runTests ========
 *
 *  Execute the test suite and print out benchmark numbers
 *
 */

static void OPENCV_runTests( OPENCV_Operation operation )
{
    
    /*
     * Format for -> Void OPENCVTEST_runTests(XDAS_Int16 *inbuf_1, XDAS_Int16 *inbuf_2, XDAS_Int16 *outbuf, OPENCV_Operation opencvOperation);
     */
    switch ( operation ) {

	   case OPENCV_OPERATION_GEN_TWIDDLE :
    	   	OPENCVTEST_runTests(NULL, NULL, opencvParams->output, OPENCV_OPERATION_GEN_TWIDDLE);
		break;
           
           case OPENCV_OPERATION_DFT :
 		OPENCVTEST_runTests(NULL, NULL, opencvParams->output, OPENCV_OPERATION_GEN_TWIDDLE);
		printf("Twiddle-factors Generated....Starting DFT");
		OPENCVTEST_runTests(opencvParams->data, opencvParams->output, opencvParams->output, OPENCV_OPERATION_DFT); 
		break;

	   case OPENCV_OPERATION_SOBEL3x3 :
	   case OPENCV_OPERATION_SOBEL5x5 :
 	   case OPENCV_OPERATION_SOBEL7x7 :
		OPENCVTEST_runTests(opencvParams->data, NULL, opencvParams->output, operation); 
		break;
	   default :
		return;
    }


}
     


/*
 *  ======== OPENCVTEST_runTests ========
 *
 *  Execute the test suite and print out benchmark numbers
 *
 */
static void OPENCVTEST_runTests(XDAS_Int16 *inbuf_1, XDAS_Int16 *inbuf_2, XDAS_Int16 *outbuf, OPENCV_Operation opencvOperation)
{
    int	status;
    
    OPENCV_Operation *inputBuf0;
    XDAS_Int16 *inputBuf1, *inputBuf2;
    XDAS_Int16 *outputBuf0;
    XDAS_Int8 *isNull;

    UNIVERSAL_InArgs universalInArgs;
    IOPENCV_InArgs inArgs;
    inArgs.universalInArgs = universalInArgs;
    inArgs.frameLen = FRAMELEN;
    inArgs.dftSize = FRAMELEN/4; // dftsize is the N size not the size of buffer
    inArgs.universalInArgs.size = sizeof(IOPENCV_InArgs); 

    UNIVERSAL_OutArgs outArgs;
    outArgs.size = sizeof(outArgs);

    XDM1_BufDesc universalInBufDesc;
    XDM1_BufDesc universalOutBufDesc;




    inputBuf0 = Memory_contigAlloc(sizeof(OPENCV_Operation), BUFALIGN);
    inputBuf1 = Memory_contigAlloc(FRAMELEN , BUFALIGN);
    inputBuf2 = Memory_contigAlloc(FRAMELEN , BUFALIGN);
    outputBuf0 = Memory_contigAlloc(FRAMELEN , BUFALIGN);
    if ((inputBuf0 == NULL) || (inputBuf1 == NULL) || (inputBuf2 == NULL) || (outputBuf0 == NULL)) {
        printf("App-> ERROR: Memory_contigAlloc for buffers failed");
    }

    isNull = Memory_contigAlloc(sizeof(XDAS_Int8) * 2, BUFALIGN); // currently support only 2 buffer. That is why it is * 2.
    isNull[0] = 1;
    isNull[1] = 1;

    memcpy( inputBuf0, &opencvOperation, sizeof(OPENCV_Operation));

    if (inbuf_1) {

       memcpy( inputBuf1, inbuf_1, FRAMELEN );
       isNull[0] = 0;
    }
    if (inbuf_2) {

       memcpy( inputBuf2, inbuf_2, FRAMELEN );
       isNull[0] = 0;
    }

    /* Passing buffer size and buffer pointer which will be address translated */
    universalInBufDesc.numBufs = 4; 
    universalOutBufDesc.numBufs = 1;
    
    universalInBufDesc.descs[0].bufSize = sizeof(OPENCV_Operation); 
    universalInBufDesc.descs[0].buf = (XDAS_Int8 *)inputBuf0;
 

    universalInBufDesc.descs[1].bufSize = FRAMELEN; 
    universalInBufDesc.descs[1].buf = (XDAS_Int8 *)inputBuf1;


    universalInBufDesc.descs[2].bufSize = FRAMELEN; 
    universalInBufDesc.descs[2].buf = (XDAS_Int8 *)inputBuf2;

    universalInBufDesc.descs[3].bufSize = (sizeof(XDAS_Int8) * 2); 
    universalInBufDesc.descs[3].buf = isNull;


    universalOutBufDesc.descs[0].bufSize = FRAMELEN;
    universalOutBufDesc.descs[0].buf = (XDAS_Int8 *)outputBuf0;

   
/* Need to work on this as it avoids unnecessary buffer creation by CMEM. Did not have much time to debug on this.

    universalInBufDesc.numBufs = 4; 
    universalOutBufDesc.numBufs = 1;

    
    isNull = Memory_contigAlloc(sizeof(XDAS_Int8) * 2, BUFALIGN); // currently support only 2 buffer. That is why it is * 2.
    isNull[0] = 1;
    isNull[1] = 1;

    inputBuf0 = Memory_contigAlloc(sizeof(OPENCV_Operation), BUFALIGN);
    memcpy( inputBuf0, &opencvOperation, sizeof(OPENCV_Operation));

    universalInBufDesc.descs[0].bufSize = sizeof(OPENCV_Operation); 
    universalInBufDesc.descs[0].buf = (XDAS_Int8 *)inputBuf0;

    // Copy input data from non-continious memory to continious memory 
    if (inbuf_1) {
       inputBuf1 = Memory_contigAlloc(FRAMELEN , BUFALIGN);
       memcpy( inputBuf1, inbuf_1, FRAMELEN );
       isNull[0] = 0;

       universalInBufDesc.descs[1].bufSize = FRAMELEN; 
       universalInBufDesc.descs[1].buf = (XDAS_Int8 *)inputBuf1;
    } 
    else {
         universalInBufDesc.descs[1].bufSize = 0; 
         universalInBufDesc.descs[1].buf = NULL;
    }

    if (inbuf_2) {
       inputBuf2 = Memory_contigAlloc(FRAMELEN , BUFALIGN);
       memcpy( inputBuf2, inbuf_2, FRAMELEN );
       isNull[0] = 0;

       universalInBufDesc.descs[2].bufSize = FRAMELEN; 
       universalInBufDesc.descs[2].buf = (XDAS_Int8 *)inputBuf2;
    }
    else {
         universalInBufDesc.descs[2].bufSize = 0; 
         universalInBufDesc.descs[2].buf = NULL; 
    }
    
    
    universalInBufDesc.descs[3].bufSize = (sizeof(XDAS_Int8) * 2); // This is for 2 input buffers
    universalInBufDesc.descs[3].buf = isNull;

    outputBuf0 = Memory_contigAlloc(FRAMELEN , BUFALIGN);

    universalOutBufDesc.descs[0].bufSize = FRAMELEN;
    universalOutBufDesc.descs[0].buf = (XDAS_Int8 *)outputBuf0;

    if ((inputBuf0 == NULL) || (inputBuf1 == NULL) || (inputBuf2 == NULL) || (outputBuf0 == NULL)) {
        printf("App-> ERROR: Memory_contigAlloc for buffers failed");
    }

*/

    status = UNIVERSAL_process(codec, &universalInBufDesc, &universalOutBufDesc, NULL, (IUNIVERSAL_InArgs *) &inArgs, &outArgs);

    if (status != UNIVERSAL_EOK) {
        printf("Error calling OPENCV module:%x\n",status);
        goto exit;
    }

    memcpy( opencvParams->output, outputBuf0, FRAMELEN);


    exit:
    
    Memory_contigFree(inputBuf0, sizeof(OPENCV_Operation));
    Memory_contigFree(inputBuf1,  FRAMELEN);
    Memory_contigFree(inputBuf2,  FRAMELEN);
    Memory_contigFree(outputBuf0, FRAMELEN);
    Memory_contigFree(isNull, sizeof(XDAS_Int8) * 2);
}


/*
 *  ======== OPENCVTEST_main ========
 */
void OPENCVTEST_main( IOPENCV_Params *opencv_Params, OPENCV_Operation operation )
{
    char engine_Name[] = "opencvEngine";
    char *engineName = engine_Name;

    opencvParams = opencv_Params;

    if (opencvParams->width != 0 && opencvParams->height != 0)
       FRAMELEN = opencvParams->width * opencvParams->height * sizeof (XDAS_Int8);
    else if (opencvParams->width != 0)
       FRAMELEN = opencvParams->width * sizeof (XDAS_Int8);
    else if (opencvParams->height !=0)
       FRAMELEN = opencvParams->height * sizeof (XDAS_Int8);
    else
       printf("Wrong Data Size \n");
    

    /* For real and imaginary. (DFT type is XDAS_Int16 where as image is XDAS_Int8) */
    if(operation == OPENCV_OPERATION_DFT) FRAMELEN = (FRAMELEN / sizeof (XDAS_Int8)) * 2 * sizeof (XDAS_Int16);   
    
    /* Inatilaize codec-engine */    

    CERuntime_init();

    GT_create(&curMask, "ZZ");

    /* Enable all trace for this "ZZ" module */
    GT_set("ZZ=01234567");

    GT_0trace(curMask, GT_2CLASS, "main> Welcome to app's main().\n");
   

    Engine_Handle ce = NULL;

    printf("App-> Application started.\n");

    /* Init the global quit function */
    OPENCVTEST_getQuitFxn = myGetQuitFunction;
    
    /* reset, load, and start DSP Engine */
    if ((ce = Engine_open(engineName, NULL, NULL)) == NULL) {
        printf("%s: error: can't open engine %s\n", progName, engineName);
        goto endFailOpenEngine;
    }

    /* allocate and initialize opencv algo on the engine */
    codec = UNIVERSAL_create(ce, opencvName, (IUNIVERSAL_Params *)opencvParams);
    if (codec == NULL) {
        printf( "App-> ERROR: can't open codec %s\n", opencvName);
        goto endFailOpenCodec;
    }
    printf("App-> Codec created.\n");

    /* Enter test app */
    OPENCV_runTests( operation ); 

    printf( "App-> Completed Successfully\n");

    /* teardown the codec */
    if (codec) {
        UNIVERSAL_delete(codec);
    }

endFailOpenCodec:
    /* close the engine */
    if (ce) {
        Engine_close(ce);
    }

endFailOpenEngine:
    printf("app ended.\n");
    
}

