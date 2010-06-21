/*
 *  ======== opencv_beagle_sobel.c ========
 *  Implementation of the SOBEL operation. 
 */
#ifdef __TI_COMPILER_VERSION__
#pragma     CODE_SECTION(OPENCV_BEAGLE_sobel,         ".text:sobel")
#endif 

#include <string.h>
#include <xdc/std.h>
#include <ti/xdais/xdas.h>
#include <ti/xdais/dm/iuniversal.h>
#include <include/IMG_sobel_3x3_8.h>
#include <include/IMG_sobel_5x5_16s.h>
#include <include/IMG_sobel_7x7_16s.h>

#include    "iopencv.h"
#include    "opencv_beagle.h"
#include    "opencv_beagle_priv.h"


/*
 *======== OPENCV_BEAGLE_sobel ========
 *
 * in = input array of image data
 * out = output array of processed image
 * row = No of rows
 * cols = No of columns
 *
 * Performs sobel filter for image data stored in vector in. The 
 *  The output result is stored in vector out.
 * 
 *
 */

Void OPENCV_BEAGLE_sobel(IUNIVERSAL_Handle handle, IOPENCV_Operation operation, XDAS_Int8 *in, XDAS_Int8 *out)
{
     OPENCV_BEAGLE_Obj *sobel = (Void *)handle;
     XDAS_Int32 cols = sobel->s_cols;
     XDAS_Int32 rows = sobel->s_rows;
     XDAS_Int16 *input = (XDAS_Int16 *)in;
     XDAS_Int16 *output = (XDAS_Int16 *)out;
     switch (operation) {
	    case OPENCV_OPERATION_SOBEL3x3 :
		 IMG_sobel_3x3_8(in, out, cols, rows);
		 break;

	    case OPENCV_OPERATION_SOBEL5x5 :
		 IMG_sobel_5x5_16s (input, output, cols, rows);
		 break;

	    case OPENCV_OPERATION_SOBEL7x7 :
		 IMG_sobel_7x7_16s (input, output, cols, rows);
		 break;

	    default :
		return;
     }
   
}
