/*
 *  Copyright 2006 by Texas Instruments Incorporated.
 *
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *
 */

#include <xdc/std.h>
#include <ti/sdo/ce/CERuntime.h>
#include <ti/sdo/ce/trace/gt.h>

extern Int smain(Int argc, String argv[]);

/* trace info: module name, mask */
GT_Mask curMask = {0,0};

/*
 *  ======== main ========
 */
Int main(Int argc, String argv[])
{
    CERuntime_init();

    GT_create(&curMask, "ZZ");

    /* Enable all trace for this "ZZ" module */
    GT_set("ZZ=01234567");

    GT_0trace(curMask, GT_2CLASS, "main> Welcome to app's main().\n");

    return (smain(argc, argv));
}

/*
 *  @(#) codec_engine_1_02 1,0,0,147 7-14-2006 ce-d14
*/

