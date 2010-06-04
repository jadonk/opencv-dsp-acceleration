/*
 *  Copyright 2006 by Texas Instruments Incorporated.
 *
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *
 */

/*
 *  ======== firtest.c ========
 *  Test application for CVDFTTEST algorithm.
 */

#include <xdc/std.h>
#include <ti/sdo/ce/Engine.h>
#include <ti/sdo/ce/osal/Memory.h>
#include <ti/sdo/ce/trace/gt.h>

#include <ti/sdo/ce/universal/universal.h>
#include <gsoc/algos/cvDFT/icvDFT.h>
#include <gsoc/algos/cvDFT/cvDFT_gsoc.h>

/* Standard Linux headers */
#include <stdio.h>
#include <stdlib.h>
#include "cvDFTtest.h"

#ifndef BUFALIGN
#define BUFALIGN 128
#endif

static String progName     = "app";
static String cvDFTName  = "cvdft";

//static XDAS_Int16 coeff[] = {
    /*
     * The following filters have been designed with the compromise to let
     * through a reasonable amount of audio content when used with sampling
     * rates from 8 to 48 kHz.
     */

    /* Low-pass, 32 taps, passband 0 to 500 Hz for sampling rate of 8kHz */
   /* 0x08FC, 0xF6DE, 0xF92A, 0xFA50, 0xFB17, 0xFBF0, 0xFD2A, 0xFECF,
    0x00EC, 0x036C, 0x0623, 0x08E1, 0x0B6E, 0x0D91, 0x0F1A, 0x0FE9,
    0x0FE9, 0x0F1A, 0x0D91, 0x0B6E, 0x08E1, 0x0623, 0x036C, 0x00EC,
    0xFECF, 0xFD2A, 0xFBF0, 0xFB17, 0xFA50, 0xF92A, 0xF6DE, 0x08FC
};*/


/*
 * XDAS_Int16 input[] = {
 *     0x4000, 0x5000, 0x5000, 0x0000, 0x0000, 0x0000, 0x0000,
 *     0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
 * };
 */
#define TEST_SIZE   128
//#define FRAMELEN    (sizeof (input) / sizeof (XDAS_Int16))
//#define FRAMELENx2  2 * FRAMELEN                             //For real and imaginary
//#define FILTERLEN   (sizeof (coeff) / sizeof (XDAS_Int16))

static XDAS_Int16 FRAMELEN;
static XDAS_Int16 FRAMELENx2;
static XDAS_Int16 *inBuf0; // input buffer
static XDAS_Int16 *twiddleBuf0; // twiddle factor buffer
static XDAS_Int16 *outBuf;

void CVDFT_GetInput(XDAS_Int16 *);//need to work on CVDFT_GetInput

static void CVDFT_runTests();
static void CVDFTTEST_runTests(XDAS_Int16 *, XDAS_Int16 *, XDAS_Int16 *, CVDFT_Operation);


static Void display(XDAS_Int16 a[], XDAS_Int16 n);

/* Universal  Codec Handle */
static UNIVERSAL_Handle codec = NULL;

/* Global flag used to signal when it is time to exit test loop */
int CVDFTTEST_gfxExitThread = 0;

/* Global Quit Function to call */
static Fxn CVDFTTEST_getQuitFxn = NULL;

/*
 *  ======== CVDFT_runTests ========
 *
 *  Execute the test suite and print out benchmark numbers
 *
 */

Void CVDFT_GetInput(XDAS_Int16 *inputbuffer)
{
    XDAS_Int32 i;
    
    inputbuffer = (XDAS_Int16 *)malloc(2 * TEST_SIZE * sizeof(XDAS_Int16));
    twiddleBuf0 = (XDAS_Int16 *)malloc(2 * TEST_SIZE * sizeof(XDAS_Int16));
    outBuf = (XDAS_Int16 *)malloc(2 * TEST_SIZE * sizeof(XDAS_Int16));
    
    for (i=0;i<128;i++){
	*inputbuffer = (XDAS_Int16)i +1;
	inputbuffer++;
	*inputbuffer = 0;
	inputbuffer++;
	}
    
    FRAMELEN = TEST_SIZE;   
    FRAMELENx2 = 2 * FRAMELEN;     
       
}

static void CVDFT_runTests()
{
    CVDFT_GetInput(inBuf0);
    /*
     * Format for -> Void CVDFTTEST_runTests(XDAS_Int16 *inbuf_1, XDAS_Int16 *inbuf_2, XDAS_Int16 *outbuf, CVDFT_Operation cvdftOperation);
     */
    CVDFTTEST_runTests(inBuf0, NULL, twiddleBuf0, CVDFT_OPERATION_GEN_TWIDDLE);

    printf("Twiddle-factors Generated....Starting DFT");

    CVDFTTEST_runTests(inBuf0, twiddleBuf0, outBuf, CVDFT_OPERATION_DFT); 

    printf("CVDFT Input: ");
    display(inBuf0, FRAMELENx2 * sizeof(XDAS_Int16));

    

    printf("CVDFT Output: ");
    display(outBuf, FRAMELENx2 * sizeof(XDAS_Int16)); // need to check, I think it should be x2 for real and img
}
     


/*
 *  ======== CVDFTTEST_runTests ========
 *
 *  Execute the test suite and print out benchmark numbers
 *
 */
static void CVDFTTEST_runTests(XDAS_Int16 *inbuf_1, XDAS_Int16 *inbuf_2, XDAS_Int16 *outbuf, CVDFT_Operation cvdftOperation)
{
    int	status;
    
    CVDFT_Operation *inputBuf0;
    XDAS_Int16 *inputBuf1, *inputBuf2;
    XDAS_Int16 *outputBuf0;
    XDAS_Int8 *isNull;

    UNIVERSAL_InArgs universalInArgs;
    ICVDFT_InArgs inArgs;
    inArgs.universalInArgs = universalInArgs;
    inArgs.frameLen = FRAMELENx2;
    inArgs.dftSize = FRAMELEN; 
    inArgs.universalInArgs.size = sizeof(ICVDFT_InArgs); 

    UNIVERSAL_OutArgs outArgs;
    outArgs.size = sizeof(outArgs);

    XDM1_BufDesc universalInBufDesc;
    XDM1_BufDesc universalOutBufDesc;

    inputBuf0 = Memory_contigAlloc(sizeof(CVDFT_Operation), BUFALIGN);
    inputBuf1 = Memory_contigAlloc(FRAMELENx2 * sizeof(XDAS_Int16), BUFALIGN);
    inputBuf2 = Memory_contigAlloc(FRAMELENx2 * sizeof(XDAS_Int16), BUFALIGN);
    outputBuf0 = Memory_contigAlloc(FRAMELENx2 * sizeof(XDAS_Int16), BUFALIGN);
    if ((inputBuf0 == NULL) || (inputBuf1 == NULL) || (inputBuf2 == NULL) || (outputBuf0 == NULL)) {
        printf("App-> ERROR: Memory_contigAlloc for buffers failed");
    }

    isNull = Memory_contigAlloc(sizeof(XDAS_Int8) * 2, BUFALIGN); // currently support only 2 operation. That is why it is * 2.
    isNull[0] = 1;
    isNull[1] = 1;

    memcpy( inputBuf0, &cvdftOperation, sizeof(XDAS_Int16));

    if (inbuf_1) {
       memcpy( inputBuf1, inbuf_1, FRAMELENx2 * sizeof(XDAS_Int16));
       isNull[0] = 0;
    }
    if (inbuf_2) {
       memcpy( inputBuf2, inbuf_2, FRAMELENx2 * sizeof(XDAS_Int16));
       isNull[0] = 0;
    }

    
    universalInBufDesc.numBufs = 4; 
    universalOutBufDesc.numBufs = 1;
    
    universalInBufDesc.descs[0].bufSize = sizeof(CVDFT_Operation); 
    universalInBufDesc.descs[0].buf = (XDAS_Int8 *)inputBuf0;
 
    universalInBufDesc.descs[1].bufSize = FRAMELENx2 * sizeof(XDAS_Int16); 
    universalInBufDesc.descs[1].buf = (XDAS_Int8 *)inputBuf1;

    universalInBufDesc.descs[2].bufSize = FRAMELENx2 * sizeof(XDAS_Int16); 
    universalInBufDesc.descs[2].buf = (XDAS_Int8 *)inputBuf2;

    universalInBufDesc.descs[3].bufSize = (sizeof(XDAS_Int8) * 2); 
    universalInBufDesc.descs[3].buf = isNull;

    universalOutBufDesc.descs[0].bufSize = FRAMELENx2*sizeof(XDAS_Int16);// need to look at this, this may be changes to hold real and img data
    universalOutBufDesc.descs[0].buf = (XDAS_Int8 *)outputBuf0;


/*
 * We dont need control operation.

    UNIVERSAL_DynamicParams universalDynParams;// need to look more into dynamic parameter
    universalDynParams.size = sizeof(universalDynParams);
    UNIVERSAL_Status universalStatus;// need to look more into UNIVERSAL Status
    universalStatus.size = sizeof(universalStatus);
    universalStatus.data.numBufs = 1;
//    universalStatus.data.numBufs = 0;// made it '0' since we dont need instance status parameter
    universalStatus.data.descs[0].bufSize = FRAMELEN*sizeof(XDAS_Int16);
//    universalStatus.data.descs[0].bufSize = 0;// made it '0 since we dont need instance status parameter
    universalStatus.data.descs[0].buf = (XDAS_Int8 *)twiddleBuf0;
//    universalStatus.data.descs[0].buf = NULL;// made it null since we dont have any instance parameter
    
    status = UNIVERSAL_control(codec, XDM_SETPARAMS, &universalDynParams, &universalStatus);// it is not necessary to call control, i need to check this
    if (status==UNIVERSAL_EOK) {
        printf("App-> Control completed.\n");
    }
  
 */

    status = UNIVERSAL_process(codec, &universalInBufDesc, &universalOutBufDesc, NULL, (IUNIVERSAL_InArgs *) &inArgs, &outArgs);

    if (status != UNIVERSAL_EOK) {
        printf("Error calling CVDFT module:%x\n",status);
        goto exit;
    }

    memcpy( outbuf, universalOutBufDesc.descs[0].buf, FRAMELENx2 * sizeof(XDAS_Int16));


    exit:
    
    Memory_contigFree(inputBuf0, sizeof(CVDFT_Operation));
    Memory_contigFree(inputBuf1, sizeof(XDAS_Int16) * FRAMELENx2);
    Memory_contigFree(inputBuf2, sizeof(XDAS_Int16) * FRAMELENx2);
    Memory_contigFree(outputBuf0, sizeof(XDAS_Int16) * FRAMELENx2);
    Memory_contigFree(isNull, sizeof(XDAS_Int8) * 2);
}


/*
 *  ======== CVDFTTEST_main ========
 */
Int CVDFTTEST_main(Fxn getQuitFxn, Bool askTestNumber, char *engineName)
{
/*

    ICVDFT_Params cvDFTParams;
//  cvDFTParams.filterLen = FILTERLEN;
    cvDFTParams.frameLen = FRAMELEN;
    cvDFTParams.size = sizeof(cvDFTParams);
 */

    CVDFT_GetInput(inBuf0);    

    ICVDFT_Params cvDFTParams;
    cvDFTParams.frameLen = FRAMELEN;
    cvDFTParams.dftSize = FRAMELENx2;
    cvDFTParams.size = sizeof(cvDFTParams);
    

    Engine_Handle ce = NULL;

    printf("App-> Application started.\n");

    /* Init the global quit function */
    CVDFTTEST_getQuitFxn = getQuitFxn;
    
    /* reset, load, and start DSP Engine */
    if ((ce = Engine_open(engineName, NULL, NULL)) == NULL) {
        printf("%s: error: can't open engine %s\n", progName, engineName);
        goto endFailOpenEngine;
    }

    /* allocate and initialize fir algo on the engine */
    codec = UNIVERSAL_create(ce, cvDFTName, (IUNIVERSAL_Params *)&cvDFTParams);
    if (codec == NULL) {
        printf( "App-> ERROR: can't open codec %s\n", cvDFTName);
        goto endFailOpenCodec;
    }
    printf("App-> Codec created.\n");

    /* Enter test app */
    CVDFT_runTests(); // changed this as per bitblit example

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
    return (0);
}

static Void display(XDAS_Int16 a[], XDAS_Int16 n)
{
    XDAS_Int16 i;
    for (i=0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
}
