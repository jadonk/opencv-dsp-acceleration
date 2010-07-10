/*
 *  ======== iopencv.h ========
 *  IOPENCV Interface Header
 */
#ifndef IOPENCV_
#define IOPENCV_

//#include <ti/xdais/ialg.h>
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
	OPENCV_OPERATION_SOBEL7x7 = (1 << 4),
	OPENCV_OPERATION_INTEGRAL = (1 << 5),
        OPENCV_OPERATION_IDFT = (1 << 6),
	OPENCV_OPERATION_DFTROWS = (1 << 7),
	OPENCV_OPERATION_IDFTROWS = (1 << 8),
	OPENCV_OPERATION_SCALE = (1 << 9)
} IOPENCV_OperationTypes;


/* Extend IUNIVERSAL_Params to include filterLen and frameLen */
typedef struct IOPENCV_Params {
    XDAS_Int32 size;

    XDAS_Int32 s_type;
    XDAS_Int32 s_step;
    XDAS_Int8 *s_ptr;
    XDAS_Int32 s_rows;
    XDAS_Int32 s_cols;
    XDAS_Int32 s_channels;
	
    XDAS_Int32 d_type;
    XDAS_Int32 d_step;
    XDAS_Int8 *d_ptr;
    XDAS_Int32 d_rows;
    XDAS_Int32 d_cols;
    XDAS_Int32 d_channels;
            
} IOPENCV_Params;


typedef struct IOPENCV_InArgs {
    IUNIVERSAL_InArgs universalInArgs;
    IOPENCV_OperationTypes operation;
    XDAS_UInt32 dftSize;
} IOPENCV_InArgs;




#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  /* IOPENCV_ */


