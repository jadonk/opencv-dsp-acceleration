/*
 *  ======== main.c ========
 */

#include <xdc/std.h>

#include <ti/sdo/ce/CERuntime.h>
#include <ti/sdo/ce/trace/gt.h>

/* trace info: module name, mask */
static GT_Mask gtMask = {0,0};
Int SDRAM = 0;

//volatile int CE_exitSpinLock = 0;

//void CE_spinLock(void)
//{
// while (CE_exitSpinLock == 0){}
//}

/*
 *  ======== main ========
 */
Void main(Int argc, Char *argv[])
{
    /* init Codec Engine */
    CERuntime_init();

    /* init trace */
    GT_init();

    /* create a mask to allow a trace-print welcome message below */
    GT_create(&gtMask, "codec_unitserver");

    /* ...and initialize all masks in this module to "on" */
    GT_set("codec_unitserver=01234567");

    GT_0trace(gtMask, GT_4CLASS, "main> Welcome to DSP server's main().\n");

}
/*
 *  @(#) ti.sdo.ce.wizards.genserver; 1, 0, 0,40; 11-18-2009 14:14:16; /db/atree/library/trees/ce/ce-o05x/src/
 */

