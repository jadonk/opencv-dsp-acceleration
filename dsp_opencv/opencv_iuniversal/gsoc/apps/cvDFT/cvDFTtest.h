/*
 *  Copyright 2006 by Texas Instruments Incorporated.
 *
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *
 */


/*
 *  ======== cvDFTtest.h ========
 *  
 */


#ifndef _CVDFTTEST_H
#define _CVDFTTEST_H


#include <gsoc/algos/cvDFT/icvDFT.h>


typedef ICVDFT_Operation  	  CVDFT_Operation;


typedef ICVDFT_OperationTypes    CVDFT_OperationTypes;
/*
 *  ======== CVDFTTEST_main ========
 *  
 *  Test Routines Entry Point
 */
Int CVDFTTEST_main(Fxn quitFxn, Bool askTestNumber, char *engineName);


#endif // _CVDFTTEST_H
