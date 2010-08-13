/*
 *  ======== opencvtest.c ========
 *  Test application for OPENCVTEST algorithm.
 */

#include <xdc/std.h>
#include <ti/sdo/ce/Engine.h>
#include <ti/sdo/ce/osal/Memory.h>
#include <ti/sdo/ce/CERuntime.h>
#include "../c6accelw/c6accelw.h"

#include "beagle_opencv_API.h"
#include "opencvDsp.h"

#include <string.h>

#ifndef SUPPORT_ASYNC
#define SUPPORT_ASYNC
#endif
    
#define ENGINENAME "omap3530"
#define ALGNAME "c6accel"
#define APPNAME "app"

#define BUFALIGN 128

#define MAX_IMAGE_WIDTH 640
#define MAX_IMAGE_HEIGHT 480
#define DFT_SIZE_IN_BYTES   2
#define DFT_EXPECTED_CHANNELS 2

#ifdef __cplusplus
extern "C" {
#endif

static C6accel_Handle hC6accel = NULL;

/* Memory buffers used */
/*
Int8 *pSrcBuf=NULL;
Int8 *pRefBuf=NULL;
*/
static Int8 *pWorkingBuf = NULL;
static Int8 *pWorkingBuf2 = NULL;
static Int8 *twiddleBuf = NULL;
static int pWorkingBuf_Size;
static int pWorkingBuf2_Size;
static int twiddleBuf_Size;

/* Codec Engine engine and codec labels, defined in cfg file: */

static String algName      = (String)ALGNAME;
static String engineName   = (String)ENGINENAME;
//static String progName     = APPNAME;

static Memory_AllocParams MemParams = {
    Memory_CONTIGHEAP,
    Memory_CACHED,
    8,
    0
};

/* initialize DSP */
int DSP_cvStartDSPProcess()
{
    CERuntime_init();
    hC6accel = C6accel_create(engineName, NULL,algName, NULL);
    if (hC6accel == NULL) {
        printf("Error: DSP Engine create failed. \n");
	return OPENCVDSP_ERR;
    }
    return OPENCVDSP_OK;
}

/* Process Sobel algorithm on the DSP */
int DSP_cvSobelProcess(DSP_Mat * src, DSP_Mat * dst, DSP_Flags *flags)
{
    int status;  
    status = C6accel_IMG_sobel_3x3_8_16(hC6accel,src->data,(unsigned short *)dst->data, src->cols, src->rows,ASYNC);
    if(status != 0)
        return OPENCVDSP_ERR;
    return OPENCVDSP_OK;
}

/* Process DFT algorithm on the DSP */
int DSP_cvDFTProcess(DSP_Mat * src, DSP_Mat * dst, DSP_Flags * flags, int optimumCols, int optimumRows)
{
    int status;
    int dxtType = flags->flag1;
    int nonZeroRows = flags->flag2;
    int srcMatFlag = src->flags;
    int dstMatFlag = dst->flags;
    int dftRows = optimumRows; // Get Optimum size of DSP. It should be power of 2.
    int dftCols = optimumCols;
    int highestSize = (dftRows > dftCols)?dftRows:dftCols;
    if((dftRows == 0) || (dftCols == 0)) {
	printf("Error: Invalid DFT size.(1024 >= size >= 16)\n");
	return OPENCVDSP_ERR;
    } 
    /* Calculate size of working buffer needed. DSP expects interleaved real and imaginary data. That is why EXPECTED_CHANNEL=2. We are using*/
    /* DSP_fft_16x16 kernel of DSPLib which expects input data(real and imaginary) to be 16 bit(2 bytes)                                     */
    int tempBufSize = dftRows * dftCols * DFT_SIZE_IN_BYTES * DFT_EXPECTED_CHANNELS;
    
    /* Allocate workBuf to be passed to DSP since C6accel doesnot provide any working Buffer. We need 2 buffer to take care of float to Q15 conversion */
    pWorkingBuf = (Int8 *)Memory_alloc(tempBufSize, &MemParams);
    pWorkingBuf_Size = tempBufSize;
    pWorkingBuf2 = (Int8 *)Memory_alloc(tempBufSize, &MemParams);
    pWorkingBuf2_Size = tempBufSize;
    twiddleBuf = (Int8 *)Memory_alloc(highestSize*DFT_SIZE_IN_BYTES * DFT_EXPECTED_CHANNELS, &MemParams);
    twiddleBuf_Size = highestSize*DFT_SIZE_IN_BYTES * DFT_EXPECTED_CHANNELS;

    status = C6accel_DSP_dft_f(hC6accel,src->data, dst->data, src->cols, src->rows, dftCols, dftRows, srcMatFlag, dstMatFlag, dxtType, nonZeroRows,
					(unsigned char *)twiddleBuf, (unsigned char *)pWorkingBuf, (unsigned char *)pWorkingBuf2, ASYNC);
    #ifdef SUPPORT_ASYNC
    if(status != 0)
	return OPENCVDSP_ERR;
    #else
    if(pWorkingBuf)
	Memory_free(pWorkingBuf, pWorkingBuf_Size, &MemParams);
    if(pWorkingBuf2)
	Memory_free(pWorkingBuf2, pWorkingBuf2_Size, &MemParams);
    if(twiddleBuf)
	Memory_free(twiddleBuf, twiddleBuf_Size, &MemParams);
    if(status != 0)
	return OPENCVDSP_ERR;
    #endif
    return OPENCVDSP_OK;
    

}

/* Process Integral algorithm on the DSP */
int DSP_cvIntegralProcess( DSP_Mat * src, DSP_Mat * sum, DSP_Mat * sqsum, DSP_Mat * tiltedsum)
{
    int status;
    pWorkingBuf = (Int8 *)Memory_alloc(src->cols * sizeof(int), &MemParams);
    pWorkingBuf_Size = (src->cols * sizeof(int));

    memset(pWorkingBuf,0,pWorkingBuf_Size);

    status = C6accel_VLIB_integralImage8(hC6accel, src->data, (unsigned short)src->cols, (unsigned short)src->rows, (unsigned int *)pWorkingBuf , 															(unsigned int *)sum->data, ASYNC);
    #ifdef SUPPORT_ASYNC
    if(status != 0)
	return OPENCVDSP_ERR;
    #else
    if(pWorkingBuf)
	Memory_free(pWorkingBuf, (size_t)pWorkingBuf_Size, &MemParams);
    if(status != 0)
	return OPENCVDSP_ERR;
    #endif
    return OPENCVDSP_OK;

}

/* Process color space conversion on DSP */
int DSP_cvCvtColorProcess(DSP_Mat * src, DSP_Mat * dst, DSP_Flags * flags)
{   
    int status;
    unsigned int count = src->rows * src->cols;
    status = C6accel_RGB_To_Y(hC6accel, src->data, dst->data, count, ASYNC);
    if(status != 0)
	return OPENCVDSP_ERR;

    return OPENCVDSP_OK;
}
/* Process RGB_To_Y and sobel filtering by chaining the DSP side functions */
/*            	This saves codec-engine overhead                           */
int DSP_cvCvtColor_cvSobelProcess(DSP_Mat *src, DSP_Mat *dst, DSP_Flags *flags, DSP_Mat *src_s, DSP_Mat *dst_s, DSP_Flags *flags_s)
{
    int status;
    unsigned int count = src->rows * src->cols;
    
    status = C6accel_RGB_To_Y_IMG_sobel_3x3_8_16(hC6accel, src->data, dst->data, count, src_s->data,(unsigned short *)dst_s->data, 
												    src_s->cols, src_s->rows,ASYNC);
    if(status != 0)
	return OPENCVDSP_ERR;

    return OPENCVDSP_OK;
}

/* Synchronize DSP with ARM */
void DSP_cvSyncDSPProcess()
{
    C6accel_waitAsyncCall(hC6accel);
#ifdef SUPPORT_ASYNC
    if(pWorkingBuf) {
	Memory_free(pWorkingBuf, (size_t)pWorkingBuf_Size, &MemParams);
	pWorkingBuf = NULL;
	pWorkingBuf_Size = 0;
    }
    if(pWorkingBuf2) {
	Memory_free(pWorkingBuf2, (size_t)pWorkingBuf2_Size, &MemParams);
	pWorkingBuf2 = NULL;
	pWorkingBuf2_Size = 0;
    }
    if(twiddleBuf) {
	Memory_free(twiddleBuf, (size_t)twiddleBuf_Size, &MemParams);
	twiddleBuf = NULL;
	twiddleBuf_Size = 0;
    }
#endif
}

void DSP_cvEndDSPProcess()
{
    if (hC6accel)
        C6accel_delete(hC6accel);
}

#ifdef __cplusplus
}
#endif
