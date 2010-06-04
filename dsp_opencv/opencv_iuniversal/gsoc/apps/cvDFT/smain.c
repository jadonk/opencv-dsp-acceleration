/*
 *  Copyright 2006 by Texas Instruments Incorporated.
 *
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *
 */

/**
 *  @file       main.c
 *
 *  @brief      This is a BIOS main() routine
 */
#include <xdc/std.h>
#include <gsoc/apps/cvDFT/cvDFTtest.h>

static int myGetQuitFunction(void)
{
	// Never Quit - change this later...
	return (0);
}


/**
 *  @brief      The BIOS main() entry point.
 *
 *  @remark     The purpose of this function is to create a BIOS worker task
 *              to house our example.
 *
 *  @remark     This is called during BIOS_init, but before the scheduler
 *              has begun running.
 */
Int smain(Int argc, String argv[])
{
	CVDFTTEST_main(myGetQuitFunction, FALSE, "cvDFTEngine");
	
    return (0);
}

/*
 *  @(#) codec_engine_1_02 1,0,0,147 7-14-2006 ce-d14
*/

