#ifndef _IOPENCVDSP_H
#define _IOPENCVDSP_H

#include <opencv/cv.h>
#include "opencvDsp.h"

#define FLAGS1_OpenCV2DSP(a) flags_OpenCV2DSP(a,-1,-1,-1,-1)
#define FLAGS2_OpenCV2DSP(a,b) flags_OpenCV2DSP(a,b,-1,-1,-1)
#define FLAGS3_OpenCV2DSP(a,b,c) flags_OpenCV2DSP(a,b,c,-1,-1)
#define FLAGS4_OpenCV2DSP(a,b,c,d) flags_OpenCV2DSP(a,b,c,d,-1)
#define FLAGS5_OpenCV2DSP(a,b,c,d,e) flags_OpenCV2DSP(a,b,c,d,e)

#ifdef __cplusplus
extern "C" {
#endif

DSP_Mat mat_OpenCV2DSP(cv::Mat src);


DSP_Flags flags_OpenCV2DSP(int a, int b, int c, int d, int e);

int dftOptimumSize_OpenCVDSP( int size);

#ifdef __cplusplus
}
#endif

#endif // _IOPENCVDSP_H
