
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
    XDAS_Int8 *isNull = inBufs->descs[2].buf;
    XDAS_Int8 *twiddleBuf;
    XDAS_Int8 *inputBuf;
        
    if (!(isNull[0])) {
        inputBuf = (XDAS_Int8 *) inBufs->descs[0].buf;
    }
    if (!(isNull[1])) {
        twiddleBuf = (XDAS_Int8 *) inBufs->descs[1].buf;
    }

    switch ( inArgs->operation) {
	   case OPENCV_OPERATION_GEN_TWIDDLE :
		OPENCV_BEAGLE_generateTwiddle(handle, inArgs->dftSize, outBufs->descs[0].buf);
		break;
 
	   case OPENCV_OPERATION_DFT :
	   case OPENCV_OPERATION_IDFT :
	   case OPENCV_OPERATION_DFTROWS :
	   case OPENCV_OPERATION_IDFTROWS :
		OPENCV_BEAGLE_findDFT( handle, inArgs->operation, inputBuf, outBufs->descs[0].buf);
	        break; 

	   case OPENCV_OPERATION_SOBEL3x3 :
	   case OPENCV_OPERATION_SOBEL5x5 :
 	   case OPENCV_OPERATION_SOBEL7x7 :
		OPENCV_BEAGLE_sobel( handle, inArgs->operation, inputBuf, outBufs->descs[0].buf);
  		break;

	   case OPENCV_OPERATION_INTEGRAL :
		OPENCV_BEAGLE_integral( handle, inputBuf, outBufs->descs[0].buf);
  		break;
 
	   default:
		return (IUNIVERSAL_EFAIL);
    }


    /* report how we accessed the input buffer */
//    inBufs->descs[0].accessMask = 0;
    XDM_SETACCESSMODE_READ(inBufs->descs[0].accessMask);
    
//    inBufs->descs[1].accessMask = 0;
    XDM_SETACCESSMODE_READ(inBufs->descs[1].accessMask);

//    inBufs->descs[2].accessMask = 0;
    XDM_SETACCESSMODE_READ(inBufs->descs[2].accessMask);

    
    /* report how we accessed the output buffer */
//    outBufs->descs[0].accessMask = 0;
    XDM_SETACCESSMODE_WRITE(outBufs->descs[0].accessMask);

    return (IUNIVERSAL_EOK);
}



