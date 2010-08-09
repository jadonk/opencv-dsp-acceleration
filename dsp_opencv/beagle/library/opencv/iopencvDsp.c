#include <opencv/cv.h>
#include "opencvDsp.h"

#ifdef __cplusplus
extern "C" {
#endif
DSP_Mat mat_OpenCV2DSP(cv::Mat src)
{
    DSP_Mat matStruct;

    matStruct.flags = src.flags;
    matStruct.rows = src.rows;
    matStruct.cols = src.cols;
    matStruct.step = src.step;
    matStruct.data = src.data;
    matStruct.datastart = src.datastart;
    matStruct.dataend = src.dataend;

    return matStruct;
}

DSP_Flags flags_OpenCV2DSP(int a, int b, int c, int d, int e)
{
    DSP_Flags flags;
    flags.flag1 = a;
    flags.flag2 = b;
    flags.flag3 = c;
    flags.flag4 = d;
    flags.flag5 = e;

    return flags;
}

int dftOptimumSize_OpenCVDSP( int size)
{
    int optimumSize;
    optimumSize = (size == 1)?1:
			(size >1 && size < 16)?0:
 			(size == 16)?16:
			(size < 32)?32:
			(size < 64)?64:
			(size < 128)?128:
			(size < 256)?256:
			(size < 512)?512:
			(size < 1024)?1024:0;
    return(optimumSize);
}

#ifdef __cplusplus
}
#endif
