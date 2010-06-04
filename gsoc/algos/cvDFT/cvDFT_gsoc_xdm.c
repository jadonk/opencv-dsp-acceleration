
/*
 *  ======== cvDFT_gsoc_xdm.c ========
 *  XDM content needed to use CVDFT with CE via IUNIVERSAL
 */

#ifdef __TI_COMPILER_VERSION__
#pragma    CODE_SECTION(CVDFT_GSOC_process,        ".text:algProcess")
#pragma    CODE_SECTION(CVDFT_GSOC_xdmControl,        ".text:algControl")
#endif

#include <xdc/std.h>
#include <ti/xdais/xdas.h>
#include <ti/xdais/dm/iuniversal.h>
#include    "icvDFT.h"
#include    "cvDFT_gsoc.h"
#include    "cvDFT_gsoc.h"


/*
 *  ======== CVDFT_GSOC_process ========
 *  TI's implementation of the filter operation.
 */
Int CVDFT_GSOC_process(IUNIVERSAL_Handle handle,
        XDM1_BufDesc *inBufs, XDM1_BufDesc *outBufs, XDM1_BufDesc *inOutBufs,
        IUNIVERSAL_InArgs *inArgs, IUNIVERSAL_OutArgs *outArgs)
{
    /* Filter the data using the xDAIS CVDFT_GSOC_filter function */
    CVDFT_GSOC_filter(handle,(Short *) inBufs->descs[0].buf, (Short *) outBufs->descs[0].buf);

   /* report how we accessed the input buffer */
   inBufs->descs[0].accessMask = 0;
   XDM_SETACCESSMODE_READ(inBufs->descs[0].accessMask);

   /* report how we accessed the output buffer */
   outBufs->descs[0].accessMask = 0;
   XDM_SETACCESSMODE_WRITE(outBufs->descs[0].accessMask);

   return (IUNIVERSAL_EOK);
}


/*
 *  ======== CVDFT_GSOC_xdmControl ========
 */
Int CVDFT_GSOC_xdmControl(IUNIVERSAL_Handle handle, IUNIVERSAL_Cmd id, IUNIVERSAL_DynamicParams *dynParams, IUNIVERSAL_Status *status)
{
    Int retVal;
    Int cvDFTRetVal;

    /* initialize for the general case where we don't access the data buffer */
    XDM_CLEARACCESSMODE_READ(status->data.descs[0].accessMask);
    XDM_CLEARACCESSMODE_WRITE(status->data.descs[0].accessMask);
    
    switch (id) {
        case XDM_SETPARAMS:
            cvDFTRetVal = CVDFT_GSOC_control(handle, ICVDFT_SETSTATUS, status);
            XDM_SETACCESSMODE_READ(status->data.descs[0].accessMask);
            if (cvDFTRetVal == IALG_EOK) retVal = IUNIVERSAL_EOK;
            else retVal = IUNIVERSAL_EFAIL;
        break;
    default:
        retVal = IUNIVERSAL_EFAIL;
    break;
    }

    return (retVal);
}

