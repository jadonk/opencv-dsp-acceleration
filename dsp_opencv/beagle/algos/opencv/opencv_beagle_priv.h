
/*
 *  ======== opencv_beagle_priv.h ========
 *  Interface header for OPENCV
 *  algorithm. Only the implementation source files include
 *  this header; this header is not shipped as part of the
 *  algorithm.
 *
 *  This header contains declarations that are specific to
 *  this implementation and which do not need to be exposed
 *  in order for an application to use the OPENCV algorithm.
 */

#ifndef OPENCV_BEAGLE_PRIV_
#define OPENCV_BEAGLE_PRIV_
 
#include <xdc/std.h>

#include    "iopencv.h"
#include    "opencv_beagle.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== OPENCV_BEAGLE_Obj ========
 */
typedef struct OPENCV_BEAGLE_Obj {
    IALG_Obj           alg;            /* MUST be first field of all OPENCV objs */
    XDAS_Int16        *workBuf;       /* on-chip scratch history */
//    Short             *history;       /* off chip presistant history */
    XDAS_Int32         dftSize;
    XDAS_Int32         frameLen;
    XDAS_Int32         type;
    XDAS_Int32         nchannel;
    XDAS_Int32         depth;
    XDAS_Int32         step; 
    XDAS_Int16         width;    /* width of image */
    XDAS_Int16         height;   /* height of image */

} OPENCV_BEAGLE_Obj;

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  /* OPENCV_BEAGLE_PRIV_ */


