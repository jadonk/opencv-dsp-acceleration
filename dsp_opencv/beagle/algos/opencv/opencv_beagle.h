/*
 *  ======== opencv_beagle.h ========
 *  Interface for the OPENCV_BEAGLE module; implementation 
 *  of the IOPENCV interface
 */
#ifndef OPENCV_BEAGLE_
#define OPENCV_BEAGLE_

#include <ti/xdais/ialg.h>

#include "iopencv.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== OPENCV_BEAGLE_IALG ========
 *  Implementation of the IALG interface for OPENCV
 */
extern IALG_Fxns OPENCV_BEAGLE_IALG;

/*
 *  ======== OPENCV_BEAGLE_IFIR ========
 *  Implementation of the IOPENCV interface
 */
extern IUNIVERSAL_Fxns OPENCV_BEAGLE_IOPENCV;

/*
 *  ======== OPENCV_BEAGLE_exit ========
 *  Required module finalization function
 */
extern Void OPENCV_BEAGLE_exit(Void);

/*
 *  ======== OPENCV_BEAGLE_init ========
 *  Required module initialization function
 */
extern Void OPENCV_BEAGLE_init(Void);

extern Void OPENCV_BEAGLE_findDFT(IUNIVERSAL_Handle, XDAS_Int32, XDAS_Int8 *, XDAS_Int8 *, XDAS_Int8 *);
extern Void OPENCV_BEAGLE_generateTwiddle(IUNIVERSAL_Handle, XDAS_Int32, XDAS_Int8 *);
extern Void OPENCV_BEAGLE_sobel(IUNIVERSAL_Handle, IOPENCV_Operation, XDAS_Int8 *, XDAS_Int8 *);

//extern Int OPENCV_BEAGLE_control(IUNIVERSAL_Handle, IUNIVERSAL_Cmd, IUNIVERSAL_Status *);

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  /* OPENCV_BEAGLE_ */


