/*
 *  ======== iopencv.h ========
 *  IOPENCV Interface Header
 */
#ifndef IOPENCV_
#define IOPENCV_

#include <ti/xdais/ialg.h>
#include <ti/xdais/xdas.h>
#include <ti/xdais/dm/iuniversal.h>

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

typedef XDAS_Int32 IOPENCV_Operation;

typedef enum IOPENCV_OperationTypes {
        OPENCV_OPERATION_GEN_TWIDDLE = (1 << 0),
        OPENCV_OPERATION_DFT = (1 << 1),
	OPENCV_OPERATION_SOBEL3x3 = (1 << 2),
	OPENCV_OPERATION_SOBEL5x5 = (1 << 3),
	OPENCV_OPERATION_SOBEL7x7 = (1 << 4)
} IOPENCV_OperationTypes;


/* Extend IUNIVERSAL_Params to include filterLen and frameLen */
typedef struct IOPENCV_Params {
    XDAS_Int32 size;
    XDAS_Int32 dftSize;
    XDAS_Int32 frameLen;
    XDAS_Int32 type;
    XDAS_Int32 nchannel;
    XDAS_Int32 depth;
    XDAS_Int32 step;
    XDAS_Int16 width;    /* width of image */
    XDAS_Int16 height;   /* height of image */
    XDAS_Int16 *data;
    XDAS_Int16 *output;
    
} IOPENCV_Params;


typedef struct IOPENCV_InArgs {
    IUNIVERSAL_InArgs universalInArgs;
    XDAS_UInt32 frameLen;
    XDAS_UInt32 dftSize;
} IOPENCV_InArgs;



/* IOPENCV_Status should not have a pointer since it will not be address translated - instead, IUNIVERSAL_Status.data is used (which will be address translated */
/*
typedef struct IOPENCV_Status {
    Int size;
    Int *coeffPtr;
    Int padding;
} IOPENCV_Status;
*/

/*
 *  ======== IOPENCV_Cmd ========
 *  The Cmd enumeration defines the control commands for the OPENCV
 *  control method.
 */
/*
typedef enum IOPENCV_Cmd {
    IOPENCV_GETSTATUS,
    IOPENCV_SETSTATUS
} IOPENCV_Cmd;
*/

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  /* IOPENCV_ */


