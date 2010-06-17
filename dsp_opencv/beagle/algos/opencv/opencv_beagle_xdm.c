
/*
 *  ======== opencv_beagle_xdm.c ========
 *  XDM content needed to use OPENCV with CE via IUNIVERSAL
 */

#ifdef __TI_COMPILER_VERSION__
#pragma    CODE_SECTION(OPENCV_BEAGLE_process,        ".text:algProcess")
#endif

#include <xdc/std.h>
#include <ti/xdais/xdas.h>
#include <ti/xdais/dm/iuniversal.h>
#include    "iopencv.h"
#include    "opencv_beagle.h"



/*
 *  ======== OPENCV_BEAGLE_process ========
 *  TI's implementation of the filter operation.
 */
Int OPENCV_BEAGLE_process(IUNIVERSAL_Handle handle,
        XDM1_BufDesc *inBufs, XDM1_BufDesc *outBufs, XDM1_BufDesc *inOutBufs,
        IOPENCV_InArgs *inArgs, IUNIVERSAL_OutArgs *outArgs)
{
    XDAS_Int8 *isNull = inBufs->descs[3].buf;
    XDAS_Int8 *twiddleBuf;
    XDAS_Int8 *inputBuf;
    IOPENCV_Operation *operation = (IOPENCV_Operation *)(inBufs->descs[0].buf); 
    
    if (!(isNull[0])) {
        inputBuf = (XDAS_Int8 *) inBufs->descs[1].buf;
    }
    if (!(isNull[1])) {
        twiddleBuf = (XDAS_Int8 *) inBufs->descs[2].buf;
    }

    switch ( *(operation)) {
	   case OPENCV_OPERATION_GEN_TWIDDLE :
		OPENCV_BEAGLE_generateTwiddle(handle, inArgs->dftSize, outBufs->descs[0].buf);
		break;
 
	   case OPENCV_OPERATION_DFT :
		OPENCV_BEAGLE_findDFT( handle, inArgs->dftSize, inBufs->descs[1].buf, inBufs->descs[2].buf, outBufs->descs[0].buf);
	        break; 
	   case OPENCV_OPERATION_SOBEL3x3 :

	   case OPENCV_OPERATION_SOBEL5x5 :

 	   case OPENCV_OPERATION_SOBEL7x7 :
		OPENCV_BEAGLE_sobel( handle, *(operation), inBufs->descs[1].buf,outBufs->descs[0].buf);
  		break;
 
	   default:
		return (IUNIVERSAL_EFAIL);
    }


//    /* Filter the data using the xDAIS OPENCV_BEAGLE_filter function */
//    OPENCV_BEAGLE_filter(handle,(Short *) inBufs->descs[0].buf, (Short *) outBufs->descs[0].buf);

    /* report how we accessed the input buffer */
    inBufs->descs[0].accessMask = 0;
    XDM_SETACCESSMODE_READ(inBufs->descs[0].accessMask);

    /* report how we accessed the output buffer */
    inBufs->descs[1].accessMask = 0;
    XDM_SETACCESSMODE_READ(inBufs->descs[1].accessMask);

    inBufs->descs[2].accessMask = 0;
    XDM_SETACCESSMODE_READ(inBufs->descs[2].accessMask);

    inBufs->descs[3].accessMask = 0;
    XDM_SETACCESSMODE_READ(inBufs->descs[3].accessMask);

    outBufs->descs[0].accessMask = 0;
    XDM_SETACCESSMODE_WRITE(outBufs->descs[0].accessMask);

    return (IUNIVERSAL_EOK);
}



