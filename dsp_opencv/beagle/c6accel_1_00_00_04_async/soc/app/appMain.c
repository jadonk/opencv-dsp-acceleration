/*==================================================================*/
/*  Copyright (c) 2010, Texas Instruments Incorporated              */
/*  All rights reserved.                                            */
/*                                                                  */
/*                                                                  */
/* ======== appMain.c ========                                      */
/*This is main app file that sets the testing mechnaism and calls   */
/*  functions to test kernels in C6Accel in C6ACCEL                 */
/*                                                                  */
/*                                                                  */
/*  Version: 0.0.1                                                  */
/*==================================================================*/

/* Standard includes */
#include <stdio.h>
#include <math.h>

/* Codec Engine and xdc includes */ 
#include <xdc/std.h>
#include <ti/sdo/ce/Engine.h>
#include <ti/sdo/ce/CERuntime.h>
#include <ti/sdo/ce/osal/Memory.h>

/* system time include*/
#include <sys/time.h>

/* Test application include */
#include "appMain.h"

/* Include C6ACCEL headers */
#include "../c6accelw/c6accelw.h"
#include "c6accel_testfxns.h"

/* Define Engine ,algorithm and application names */

/* Unit server for OMAP3530 device */

/* Pick engine name based on platform */
#if (PLATFORM == 3530)
 #define ENGINENAME "omap3530"
 #warning ENGINENAME "omap3530"
#elif (PLATFORM == 138)
 #define ENGINENAME "omapl138" 
 #warning ENGINENAME "omapl138"
 #define DEVICE_FLOAT  
#endif
#define ALGNAME "c6accel"
#define APPNAME "app"


/* Align on cache-line boundary since encoder uses DMA */
#define BUFALIGN 128

/* Align buffers to this cache line size (in bytes)*/
#define BUFSIZEALIGN            128

/* Number of points for FFT */
#define N       65536

/* Parameter for dot prod */
#define Nx      2400
#define Nr      32

//Parameters for MAT MUL
#define ROW1    (8)
#define COL1    (8)
#define COL2    (8)
#define SHIFT   (0)     //shift after matrix multiplication

/* Maximum elements in test vectors defined for MATH functions*/
#define NMAX     255

/*Parameters for fir gen
 Force Nout to be a multiple of 4
*/
#define NCOEFF   8
#define NXIN     120
#define NOUT     ((NXIN-NCOEFF+1) & 0xFC)

/*Maximum width and height for IMGLIB kernels*/
#define WIDTH   640
#define HEIGHT  480

/* define largest image sizes in use (ship.yuv)*/
#define YUV_WIDTH 800
#define YUV_HEIGHT 600

/* Size buffers for max */
#define MAX_HEIGHT YUV_HEIGHT
#define MAX_WIDTH  YUV_WIDTH

/* Q format for IQMath test functions */
#define GLOBAL_Q 16
#define Q1 12

/* Defines for Flaoting point RTS functions*/
#define BUFSIZE  128

/* Defines for floating point Autocorrelation and convolution kernel*/
#define NumX 256
#define NumR 64
#define NumH 8

/*Defines for the floating point matrix multiplication functions */
#define R1  64
#define C1  64
#define C2  64

/*Define to test floating point dotproduct function */
#define Nelements 1024
#define Mfactor 0.3333333
#define NPAD 4

/* Defines for floating point fft kerenls */
#define Npt 8192
#define rad 2

/* Memory buffers used for test */
Int8 *pSrcBuf_16bpp=NULL;
Int8 *pRefBuf_16bpp=NULL;
Int8 *pOutBuf_16bpp=NULL;
Int8 *pWorkingBuf_16bpp=NULL;
Int8 *pWorkingBuf2_16bpp=NULL;
#ifdef DEVICE_FLOAT
Int8 *pWorkingBuf3_16bpp=NULL;
#endif

/* Following vars/code are required by MACROs defined in benchmark.h*/
FILE* benchmarkFd;

/* Codec Engine engine and codec labels, defined in cfg file: */
static String algName      = ALGNAME;
static String engineName   = ENGINENAME;
static String progName     = APPNAME;

/* This object is used in MACRO in benchmark.h */
#include "timeObj.h"
Time_Object sTime;
UInt32      time;

void Time_reset(Time_Object *sTime)
{
 struct timeval tv;

    if (gettimeofday(&tv, NULL) == -1) {
        return;
    }

    sTime->original = tv;
    sTime->previous = tv;

    return;
}

void Time_delta(Time_Object* sTime, UInt32 *deltaPtr)
{
    struct timeval tv;
    time_t         s;
    suseconds_t    us;

    if (gettimeofday(&tv, NULL) == -1) {
        return;
    }

    s = tv.tv_sec - sTime->previous.tv_sec;
    us = tv.tv_usec - sTime->previous.tv_usec;

    *deltaPtr = s * 1000000l + us;

    sTime->previous = tv;

    return;
}

/* Now include benchmark.h after it's supporting functions/vars are declared*/
#include "benchmark.h"

extern void CERuntime_init(void);

/*Define a C6Accel Handle to call the abstracted wrapper APIs*/
C6accel_Handle hC6 = NULL;

/******************************************************************************
 * appMain
 ******************************************************************************/
Int main(Int argc, Char *argv[])
{
    UInt32 framesize;
    Memory_AllocParams memParams = Memory_DEFAULTPARAMS;
    
    printf("******************************************************************************\n");
    printf("Sample application for testing kernels in C6Accel started.\n");
    printf("******************************************************************************\n");
    /* This call must be made before the Memory_xxx() functions as it is required for the tracing functions
     in all the codec engine APIs that are used*/
    CERuntime_init();

    /* Reset timeObj used for benchmarking*/
    Time_reset(&sTime);

    /* Create call generates a C6ACCEL handle */
    hC6 = C6accel_create(engineName, NULL,algName, NULL);

    /*Check for failure*/
    if ( hC6 == NULL)
       {printf("%s: C6accel_create() failed \n",progName);
        goto end;
    }

    /* Create buffers for use by algorithms */

    /* Want to use cached & contiguous memory to get best performance from cortex when it also uses the buffers.*/
    memParams.flags = Memory_CACHED;
    memParams.type = Memory_CONTIGHEAP;

    /* Size all buffers for 6 bytes, to cope with worst case 16 bit 422Planar*/
    framesize = (MAX_WIDTH * MAX_HEIGHT * sizeof(Int32)*3/2);

    /* Create 16bit buffers for use by algorithms*/
    pSrcBuf_16bpp = Memory_alloc(framesize, &memParams);
    if (pSrcBuf_16bpp == NULL) {
        goto end;
    }
    else {
       Memory_cacheWbInv(pSrcBuf_16bpp, framesize);
    }
    
      pOutBuf_16bpp = Memory_alloc(framesize, &memParams);
    if (pOutBuf_16bpp == NULL) {
        goto end;
    }
    else {
       Memory_cacheWbInv(pOutBuf_16bpp, framesize);
    }
    
    pRefBuf_16bpp = Memory_alloc(framesize, &memParams);
    if (pRefBuf_16bpp == NULL) {
        goto end;
    }
    else {
       Memory_cacheWbInv(pRefBuf_16bpp, framesize);
    }

      pWorkingBuf_16bpp = Memory_alloc(framesize, &memParams);
    if (pWorkingBuf_16bpp == NULL) {
        goto end;
    }
    else {
       Memory_cacheWbInv(pWorkingBuf_16bpp, framesize);
    }
    
    pWorkingBuf2_16bpp = Memory_alloc(framesize, &memParams);
    if (pWorkingBuf2_16bpp == NULL) {
        goto end;
    }
    else {
       Memory_cacheWbInv(pWorkingBuf2_16bpp, framesize);
    }
  
   #ifdef DEVICE_FLOAT
   pWorkingBuf3_16bpp = Memory_alloc(framesize, &memParams);
    if (pWorkingBuf3_16bpp == NULL) {
        goto end;
    }
    else {
       Memory_cacheWbInv(pWorkingBuf3_16bpp, framesize);
    }
    #endif
    
    /* open file for csv output*/
    OPEN_LOG_FILE("benchmarking.txt");
   
    /* Call test functions for kernels in C6accel*/
    LOG_STRING("IMGLib Functions\n");
    LOG_STRING("640x480 8bit/pixel b/w Test Image \n");
    
    printf("-----------------------------------------------------------------------------\n");
    printf("Test for Image processing functions in C6Accel: \n");
    printf("-----------------------------------------------------------------------------\n");

    c6accel_test_IMG_histogram(hC6,WIDTH,HEIGHT);
    c6accel_test_IMG_median(hC6,WIDTH,HEIGHT);
    c6accel_test_IMG_conv(hC6,WIDTH,HEIGHT);
    c6accel_test_IMG_corr(hC6,WIDTH,HEIGHT);
    c6accel_test_IMG_sobel(hC6,WIDTH,HEIGHT);
    c6accel_test_IMG_muls(hC6,WIDTH,HEIGHT);
    c6accel_test_IMG_adds(hC6,WIDTH,HEIGHT);
    c6accel_test_IMG_subs(hC6,WIDTH,HEIGHT);
    LOG_STRING("800x600 YUYV Test Image \n");
    c6accel_test_IMG_YC_demux(hC6,YUV_WIDTH,YUV_HEIGHT);
    c6accel_test_IMG_YUV422PLtoYUV422SP(hC6,2,16,16,16, 8);
    c6accel_test_IMG_YUV422SPtoYUV422ILE( hC6,2,16,16,32);
    c6accel_test_IMG_YUV422SPtoYUV420PL(hC6,2,16,16,16, 8);
    LOG_STRING("DSPLib Functions\n");
    LOG_STRING("64k sample FFT \n");
    
    printf("-----------------------------------------------------------------------------\n");
    printf("Test for Fixed point Signal processing functions in C6Accel \n");
    printf("-----------------------------------------------------------------------------\n");
    c6accel_test_DSP_FFT(hC6,N);
    c6accel_test_DSP_IFFT(hC6,N);
    c6accel_test_DSP_AUTOCOR(hC6,Nx,Nr);
    c6accel_test_DSP_DOTPROD(hC6,Nr);

    /* Implementation of this function limits
    the rows and columns of matrices to be multiples of 4 and r1 >8 */
    c6accel_test_DSP_MATMUL(hC6,ROW1,COL1,COL2,SHIFT);
    c6accel_test_DSP_FIR(hC6,NOUT,NCOEFF);
    c6accel_test_DSP_IIR(hC6,NXIN,NCOEFF);

  // No need to use these on floating point devices
  #ifndef DEVICE_FLOAT
    LOG_STRING_P1("MATH kernels tested with size of data block %d \n", NMAX+1);
    printf("-----------------------------------------------------------------------------\n");
    printf("Test for Fixed point Math functions in C6Accel\n");
    printf("-----------------------------------------------------------------------------\n");
    c6accel_test_MATH_RTSARITH(hC6,NMAX);
    c6accel_test_MATH_RTSCONV(hC6,NMAX);
    c6accel_test_MATH_IQCONV(hC6,NMAX,GLOBAL_Q, Q1);
    c6accel_test_MATH_IQMATH(hC6,NMAX,GLOBAL_Q);
    c6accel_test_MATH_IQARITH(hC6,NMAX,GLOBAL_Q);
    c6accel_test_MATH_IQTRIG(hC6,NMAX,GLOBAL_Q);
  #endif
  
  #ifdef DEVICE_FLOAT
  /*Test function calls for floating point kernels in C6accel*/ 
    printf("-----------------------------------------------------------------------------\n");
    printf("Test for Floating point Math Functions in C6Accel \n");
    printf("-----------------------------------------------------------------------------\n");
    c6accel_test_MATH_RTSARITH(hC6,NMAX);
    c6accel_test_MATH_RTSCONV(hC6,NMAX); 
    c6accel_test_MATH_RTSFLT(hC6,BUFSIZE) ;
    c6accel_test_MATH_RTSFLTDP(hC6,BUFSIZE) ;
    
    printf("-----------------------------------------------------------------------------\n");
    printf("Test for Floating point Signal processing Functions in C6accel\n");
    printf("-----------------------------------------------------------------------------\n");
    c6accel_test_DSPF_sp_fftSPxSP(hC6, Npt, rad, 0, Npt);
    c6accel_test_DSPF_VECMUL(hC6,  BUFSIZE );
    c6accel_test_DSPF_VECRECIP(hC6,  NumX );
    c6accel_test_DSPF_VECSUM_SQ(hC6,  Nelements );
    c6accel_test_DSPF_W_VEC(hC6, Mfactor,   BUFSIZE );
    c6accel_test_DSPF_DOTPRODFXNS(hC6,  Nelements);
    c6accel_test_DSPF_MATFXNS(hC6,  16,  16,  16 );
    c6accel_test_DSPF_MAT_MUL_CPLX(hC6,  4,  8,  8 ); 
    c6accel_test_DSPF_MAT_TRANS(hC6,  NumR,  NumR );
    c6accel_test_DSPF_AUTOCOR(hC6,NumX,NumR);
    c6accel_test_DSPF_CONVOL(hC6,NumH,NumR); 
    //c6accel_test_DSPF_IIR(hC6,  NumX);
    c6accel_test_DSPF_FIR(hC6, 128,  4);
    c6accel_test_DSPF_sp_ifftSPxSP(hC6, Npt, rad, 0, Npt);
    c6accel_test_DSPF_BIQUAD(hC6,  BUFSIZE);
  #endif   

    CLOSE_LOG_FILE();

end:
     // Tear down C6ACCEL
    if (hC6)
       C6accel_delete(hC6);

    if(pSrcBuf_16bpp)
        Memory_free(pSrcBuf_16bpp, framesize, &memParams);

    if(pOutBuf_16bpp)
        Memory_free(pOutBuf_16bpp, framesize, &memParams);

    if(pRefBuf_16bpp)
        Memory_free(pRefBuf_16bpp, framesize, &memParams);

    if(pWorkingBuf_16bpp)
        Memory_free(pWorkingBuf_16bpp, framesize, &memParams);

    if(pWorkingBuf2_16bpp)
        Memory_free(pWorkingBuf2_16bpp, framesize, &memParams);

    #ifdef DEVICE_FLOAT
    if(pWorkingBuf3_16bpp)
        Memory_free(pWorkingBuf3_16bpp, framesize, &memParams);
    #endif

    printf("******************************************************************************\n");
    printf("All tests done.\n");
    printf("******************************************************************************\n");
    printf("\n");

   return (0);
}



/*Functions to compare arrays for the test, used across test files*/
unsigned int compare8bitArrays(unsigned char *pSrc1, unsigned char *pSrc2,unsigned int length)
{    unsigned int                i;

     for(i=0;i< length;i++)
         {
           if (pSrc1[i] != pSrc2[i])
            {  printf("Pixel mismatch at offset %i, C6Accel = %x Native = %x\n",
                      i,pSrc1[i],pSrc2[i]);
               return(0);
               }
          }
     return (1);

}


unsigned int compare16bitArrays(unsigned short *pSrc1, unsigned short *pSrc2,unsigned int length)
{    unsigned int                i;

     for(i=0;i< length;i++)
         {     if (pSrc1[i] != pSrc2[i])
            {  printf("Pixel mismatch at offset %i, C6Accel = %x Native = %x\n",
                      i,pSrc1[i],pSrc2[i]);
               return(0);
               }
          }
     return (1);
}



unsigned int compare32bitArrays(unsigned int *pSrc1, unsigned int *pSrc2,unsigned int length)
{    unsigned int                i;

     for(i=0;i< length;i++)
         {
           if (pSrc1[i] != pSrc2[i])
            {  printf("Pixel mismatch at offset %i, C6Accel = %x Native = %x\n",
                      i,pSrc1[i],pSrc2[i]);
               return(0);
               }
          }
     return (1);
}


unsigned int comparefloatArrays(float *pSrc1, float *pSrc2,unsigned int length)
{    unsigned int                i;

     for(i=0;i< length;i++)
         {
           // For float do comparison by being closer than a small error  
           if (abs(pSrc1[i]-pSrc2[i])> 0.00001)
            {  printf("Pixel mismatch at offset %i, C6Accel = %f Native = %f\n",
                      i,pSrc1[i],pSrc2[i]);
               return(0);
               }
          }
     return (1);
}

unsigned int comparedoubleArrays(double *pSrc1, double *pSrc2,unsigned int length)
{    unsigned int                i;

     for(i=0;i< length;i++)
         {
           // For float do comparison by being closer than a small error  
           if (abs(pSrc1[i]-pSrc2[i])> 0.00001)
            {  printf("Pixel mismatch at offset %i, C6Accel = %f Native = %f\n",
                      i,pSrc1[i],pSrc2[i]);
               return(0);
               }
          }
     return (1);
}

/*=============================================================================*/
/*                  End of File appMain.c                                      */
/*                              Version : 0.0.1                                */
/*=============================================================================*/

