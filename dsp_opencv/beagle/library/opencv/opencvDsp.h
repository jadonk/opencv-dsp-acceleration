#ifndef _OPENCVDSP_H
#define _OPENCVDSP_H

#include <xdc/std.h>
#include "beagle_opencv_API.h"

#define CN_MAX     64
#define CN_SHIFT   3
#define DEPTH_MAX  (1 << CN_SHIFT)
#define MAT_CN_MASK          ((CN_MAX - 1) << CN_SHIFT)
#define MAT_CN(flags)        ((((flags) & MAT_CN_MASK) >> CN_SHIFT) + 1)
#define MAT_TYPE_MASK        (DEPTH_MAX*CN_MAX - 1)
#define MAT_TYPE(flags)      ((flags) & MAT_TYPE_MASK)

#ifdef __cplusplus
extern "C" {
#endif

/* Define Error - Code */
typedef enum ErrorCode {
        OPENCVDSP_OK,
 	OPENCVDSP_ERR
} ErrorCode;


/* Define DSP matrix type */
typedef struct DSP_Mat {
    // includes several bit-fields:
    //  * the magic signature
    //  * continuity flag
    //  * depth
    //  * number of channels
    //Note * Its use in not supported currenly.
    int flags; 
    // the number of rows and columns
    int rows, cols;
    // a distance between successive rows in bytes; includes the gap if any
    size_t step;
    // pointer to the data
    unsigned char * data;
    // helper fields used in locateROI and adjustROI
    unsigned char * datastart;
    unsigned char * dataend;
} DSP_Mat;

/* Define DSP flags for pre-frocessing datas */
typedef struct DSP_Flags {
    int flag1;
    int flag2;
    int flag3;
    int flag4;
    int flag5;
} DSP_Flags;


int DSP_cvStartDSPProcess();

int DSP_cvSobelProcess(DSP_Mat *, DSP_Mat *, DSP_Flags *);

int DSP_cvDFTProcess(void *, void *, DSP_Flags *);

int DSP_cvIntegralProcess(DSP_Mat *, DSP_Mat *, DSP_Mat *, DSP_Mat *);

int DSP_cvCvtColorProcess(DSP_Mat *, DSP_Mat *, DSP_Flags *);

int DSP_cvCvtColor_cvSobelProcess(DSP_Mat *, DSP_Mat *, DSP_Flags *, DSP_Mat *, DSP_Mat *, DSP_Flags *);

void DSP_cvSyncDSPProcess();

void DSP_cvEndDSPProcess();


#ifdef __cplusplus
}
#endif

#endif // _OPENCVDSP_H
