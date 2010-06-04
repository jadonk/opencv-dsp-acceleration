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

static XDAS_Int16 coeff[] = {
    /*
     * The following filters have been designed with the compromise to let
     * through a reasonable amount of audio content when used with sampling
     * rates from 8 to 48 kHz.
     */

    /* Low-pass, 32 taps, passband 0 to 500 Hz for sampling rate of 8kHz */
    0x08FC, 0xF6DE, 0xF92A, 0xFA50, 0xFB17, 0xFBF0, 0xFD2A, 0xFECF,
    0x00EC, 0x036C, 0x0623, 0x08E1, 0x0B6E, 0x0D91, 0x0F1A, 0x0FE9,
    0x0FE9, 0x0F1A, 0x0D91, 0x0B6E, 0x08E1, 0x0623, 0x036C, 0x00EC,
    0xFECF, 0xFD2A, 0xFBF0, 0xFB17, 0xFA50, 0xF92A, 0xF6DE, 0x08FC
};

XDAS_Int16 input[] = {
    0x4000, 0x5000, 0x5000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};

#define FRAMELEN    (sizeof (input) / sizeof (XDAS_Int16))
#define FILTERLEN   (sizeof (coeff) / sizeof (XDAS_Int16))

static XDAS_Int16 *inBuf0; // input buffer
static XDAS_Int16 *dataBuf0; // coeff buffer
static XDAS_Int16 *outBuf;

static Void display(XDAS_Int16 a[], XDAS_Int16 n);

/* Universal  Codec Handle */
static UNIVERSAL_Handle codec = NULL;

/* Global flag used to signal when it is time to exit test loop */
int CVDFTTEST_gfxExitThread = 0;

/* Global Quit Function to call */
static Fxn CVDFTTEST_getQuitFxn = NULL;

/*
 *  ======== CVDFTTEST_runTests ========
 *
 *  Execute the test suite and print out benchmark numbers
 *
 */
static void CVDFTTEST_runTests(void)
{
    int	status;

    UNIVERSAL_InArgs inArgs;
    inArgs.size = sizeof(inArgs);
    
    UNIVERSAL_OutArgs outArgs;
    outArgs.size = sizeof(outArgs);

    XDM1_BufDesc universalInBufDesc;
    XDM1_BufDesc universalOutBufDesc;

    memcpy( inBuf0, (Void *)input, FRAMELEN*sizeof(XDAS_Int16) );
    memcpy( dataBuf0, (Void *)coeff, FILTERLEN*sizeof(XDAS_Int16) );

    universalInBufDesc.numBufs = 1; 
    universalOutBufDesc.numBufs = 1;
    universalInBufDesc.descs[0].bufSize = FRAMELEN*sizeof(XDAS_Int16); 
    universalInBufDesc.descs[0].buf = (XDAS_Int8 *)inBuf0;
    universalOutBufDesc.descs[0].bufSize = (FRAMELEN+FILTERLEN)*sizeof(XDAS_Int16);
    universalOutBufDesc.descs[0].buf = (XDAS_Int8 *)outBuf;

    UNIVERSAL_DynamicParams universalDynParams;
    universalDynParams.size = sizeof(universalDynParams);
    UNIVERSAL_Status universalStatus;
    universalStatus.size = sizeof(universalStatus);
    universalStatus.data.numBufs = 1;
    universalStatus.data.descs[0].bufSize = FILTERLEN*sizeof(XDAS_Int16);
    universalStatus.data.descs[0].buf = (XDAS_Int8 *)dataBuf0;
    
    status = UNIVERSAL_control(codec, XDM_SETPARAMS, &universalDynParams, &universalStatus);
    if (status==UNIVERSAL_EOK) {
        printf("App-> Control completed.\n");
    }

    printf("CVDFT Input: ");
    display((XDAS_Int16 *)universalInBufDesc.descs[0].buf, FRAMELEN);

    status = UNIVERSAL_process(codec, &universalInBufDesc, &universalOutBufDesc, NULL, &inArgs, &outArgs);

    if (status != UNIVERSAL_EOK) {
        printf("Error calling CVDFT module:%x\n",status);
    }

    printf("CVDFT Output: ");
    display((XDAS_Int16 *)universalOutBufDesc.descs[0].buf, FRAMELEN);
}


/*
 *  ======== CVDFTTEST_main ========
 */
Int CVDFTTEST_main(Fxn getQuitFxn, Bool askTestNumber, char *engineName)
{
    inBuf0 = (XDAS_Int16 *)Memory_contigAlloc(FRAMELEN, BUFALIGN);
    dataBuf0 = (XDAS_Int16 *)Memory_contigAlloc(FILTERLEN, BUFALIGN);
    outBuf = (XDAS_Int16 *)Memory_contigAlloc(FRAMELEN+FILTERLEN, BUFALIGN);
    if ((inBuf0 == NULL) || (dataBuf0 == NULL) || (outBuf == NULL)) {
        printf("App-> ERROR: Memory_contigAlloc for buffers failed");
    }

    ICVDFT_Params cvDFTParams;
    cvDFTParams.filterLen = FILTERLEN;
    cvDFTParams.frameLen = FRAMELEN;
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
    CVDFTTEST_runTests();

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
