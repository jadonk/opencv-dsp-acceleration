#ifndef _OPENCVDSP_H
#define _OPENCVDSP_H

#include <xdc/std.h>
#include "beagle_opencv_API.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void opencvDspSetParams(int , int , char *, int , int , int , int , int , char *, int , int , int );
extern int cvDspInit();
extern int cvCreateEngine(OPENCV_Operation);

#ifdef __cplusplus
}
#endif

#endif // _OPENCVDSP_H
