/*==================================================================*/
/*  Copyright (c) 2010, Texas Instruments Incorporated              */
/*  All rights reserved.                                            */
/*                                                                  */
/*                                                                  */
/*  Name: c6accelw.c                                                */
/*                                                                  */
/*  Description:                                                    */
/*  This function implements the ARM wrapper functions for C6ACCEL  */
/*                                                                  */
/*  Version: 0.0.1                                                  */
/*==================================================================*/

/* Codec Engine include files: */
#include <xdc/std.h>
#include <ti/sdo/ce/Engine.h>
#include <ti/sdo/ce/osal/Memory.h>
#include <ti/sdo/ce/universal/universal.h>

/* Run Time lib include files: */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "c6accelw.h"

#ifdef SUPPORT_ASYNC


static T_ASYNC_CALL_PARAMS c6accelAsyncParams;

#endif


/* These will be static and internal in full implementation*/
static UNIVERSAL_Params uniParams;

/* Create default heap memory configuration for wrapper
 Use non-cached as temp workaround for stability issue with Memory_alloc()*/
static Memory_AllocParams wrapperMemParams = {
    Memory_CONTIGHEAP,
    Memory_NONCACHED,
    Memory_DEFAULTALIGNMENT,
    0
};

/* Wrapper API includes */
 #include "c6accelw_i.h"
 #include "c6accelw.h"

/* Include the header for C6ACCEL codec support*/
#include "ti/c6accel/iC6Accel_ti.h"

/*Parameter definition */
#define BUFALIGN 128

/*C6accel object definition to pass the engine and universal handle*/
typedef struct C6accel_Object{
    Engine_Handle hEngine;
    UNIVERSAL_Handle hUni;
    //fields related to this instance...
}C6accel_Object;

/* Function to open and initialise codec engine */
C6accel_Handle C6accel_create(String engName, Engine_Handle hEngine,String algName, UNIVERSAL_Handle hUni)
{
  C6accel_Handle hC6accel = (C6accel_Handle)calloc(1, sizeof(C6accel_Object));

  if(hC6accel == NULL){
   return NULL;
  }

  if (hEngine == NULL){
    /* reset, load, and start DSP Engine */
    if (engName==NULL){
       return NULL;
     }
    else{
     /* reset, load, and start DSP Engine */
       if ((hC6accel->hEngine = Engine_open(engName, NULL, NULL)) == NULL) {
        return NULL;
       }
     }
  }
   else{
        hC6accel->hEngine = hEngine;
    }
     /* Set the dummy Params structure */
    uniParams.size = sizeof(UNIVERSAL_Params);

     /* Instantiate the encoder: */
    if (hUni == NULL) {
      if (algName == NULL){
       return NULL;
      }
      else{
        hC6accel->hUni = UNIVERSAL_create(hC6accel->hEngine, algName, &uniParams);
        if(hC6accel->hUni == NULL ){
            // Close Engine here
            Engine_close(hC6accel->hEngine);
            return NULL;
        }
      }
    }
    else{
         hC6accel->hUni= hUni;
    }


#ifdef SUPPORT_ASYNC
    //Initialise count
    c6accelAsyncParams.asyncCallCount=0;
#endif

    return hC6accel;
}



/* Function to delete algorithm and codec engine */
int C6accel_delete(C6accel_Handle hC6accel)
{
    /* teardown the codecs */
    if (hC6accel->hUni) {
        UNIVERSAL_delete(hC6accel->hUni);
    }

    /* close the engine */
    if (hC6accel->hEngine) {
        Engine_close(hC6accel->hEngine);
    }

    return (0);
}

/*
void C6Accel_DSP_fft16x16(C6accel_Handle hC6accel,short * restrict w, int nx, short * restrict x, short * restrict y)
Arguments
* hC6accel C6accel Handle
* w[2*nx] Pointer to complex Q.15 FFT coefficients.
* nx Length of FFT in complex samples. Must be a power of 2 or 4, and 16 = nx = 65536.
* x[2*nx] Pointer to complex 16-bit data input.
* y[2*nx] Pointer to complex 16-bit data output.

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description This routine computes a complex forward mixed radix FFT with rounding and digit reversal.
Input data x[ ], output data y[ ], and coefficients w[ ] are 16-bit.The output is returned in the separate array
y[ ] in normal order. Each complex value is stored with interleaved real and imaginary parts. The code uses a
special ordering of FFT coefficients (also called twiddle factors) and memory accesses to improve performance in the presence of cache.

Special Requirements

In-place computation is not allowed.
The size of the FFT, nx, must be a power of 2 or 4, and 16 = nx = 65536.
The arrays for the complex input data x[ ], complex output data y[ ], and twiddle factors w[ ] must be double-word aligned.
The input and output data are complex, with the real/imaginary components stored in adjacent locations in the array.
*/

int C6accel_DSP_fft16x16 ( C6accel_Handle hC6accel,
                            const short * restrict ptr_w,       /* Input twiddle factors */
                            int           npoints,              /* number of points      */
                            short       * restrict ptr_x,       /* transformed data reversed */
                            short       * restrict ptr_y        /* linear transformed data */
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int                         status;

    /* Define pointer to function parameter structure */
    DSP_fft16x16_Params         *fp0;
    XDAS_Int8                   *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(DSP_fft16x16_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size    = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 2;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_w,0,2*npoints*sizeof(short));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,1,2*npoints*sizeof(short));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_y,0,2*npoints*sizeof(short));


    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID     = DSP_FFT16x16_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

     /* Initialize pointers to function parameters */
    fp0 = (DSP_fft16x16_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->w_InArrID1  = INBUF0;
    fp0->nx          = npoints;
    fp0->x_InArrID2  = INBUF1;
    fp0->y_OutArrID1 = OUTBUF0;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
void C6Accel_DSP_ifft16x16(C6accel_Handle hC6accel,short * restrict w, int nx, short * restrict x, short * restrict y)
Arguments
* hC6accel C6accel Handle
* w[2*nx] Pointer to complex Q.15 FFT coefficients.
* nx Length of FFT in complex samples. Must be power of 2 or 4, and 16 = nx = 65536.
* x[2*nx] Pointer to complex 16-bit data input.
* y[2*nx] Pointer to complex 16-bit data output.

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description This routine computes a complex inverse mixed radix IFFT with rounding and digit reversal.
Input data x[ ], output data y[ ], and coefficients w[ ] are 16-bit.The output is returned in the separate
array y[ ] in normal order. Each complex value is stored with interleaved real and imaginary parts. The code
uses a special ordering of IFFT coefficients (also called twiddle factors) and memory accesses to improve
performance in the presence of cache. The fft16x16 can be used to perform IFFT, by first conjugating the input,
performing the FFT, and conjugating again. This allows fft16x16 to perform the IFFT as well. However,
if the double conjugation needs to be avoided, then this routine uses the same twiddle factors as the FFT and
performs an IFFT. The change in the sign of the twiddle factors is adjusted for in the routine. Hence,this routine.
uses the same twiddle factors as the fft16x16 routine.

*/

int C6accel_DSP_ifft16x16 (C6accel_Handle hC6accel,
                            const short * restrict ptr_w,       /* Input twiddle factors */
                            int           npoints,              /* number of points      */
                            short       * restrict ptr_x,        /* transformed data reversed */
                            short       * restrict ptr_y        /* linear transformed data */
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int                         status;
    /* Define pointer to function parameter structure */
    DSP_ifft16x16_Params        *fp0;
    XDAS_Int8                   *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(DSP_ifft16x16_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size    = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 2;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_w,0,2*npoints*sizeof(short));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,1,2*npoints*sizeof(short));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_y,0,2*npoints*sizeof(short));


    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns = 1;
    CInArgs->size = InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= DSP_IFFT16x16_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (DSP_ifft16x16_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->w_InArrID1 =  INBUF0;
    fp0->nx         = npoints;
    fp0->x_InArrID2 = INBUF1;
    fp0->y_OutArrID1= OUTBUF0;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
void C6ACCEL_DSP_fft32x32(C6accel_Handle hC6accel,short *w, int nx, short *x, short *y)
Arguments
* hC6accel C6accel Handle
* w[2*nx] Pointer to complex 32-bit FFT coefficients.
* nx Length of FFT in complex samples. Must be power of 2 or 4,and 16 = nx = 65536.
* x[2*nx] Pointer to complex 32-bit data input.
* y[2*nx] Pointer to complex 32-bit data output.

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description This routine computes an extended precision complex forward mixed radix FFT with rounding and
digit reversal. Input data x[ ], output data y[ ], and coefficients w[ ] are 32-bit. The output is returned
in the separate array y[ ] in normal order. Each complex value is stored with interleaved real and
imaginary parts. The code uses a special ordering of FFT coefficients (also called twiddle factors) and
memory accesses to improve performance in the presence of cache.

*/
int C6accel_DSP_fft32x32 (C6accel_Handle hC6accel,
                           const int * restrict ptr_w,             /* Input twiddle factors */
                                 int   npoints,                    /* number of points      */
                                 int * restrict ptr_x,             /* transformed data reversed */
                                 int * restrict ptr_y              /* linear transformed data */
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int status;
    /* Define pointer to function parameter structure */
    DSP_fft32x32_Params         *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(DSP_fft32x32_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 2;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_w,0,2*npoints*sizeof(int));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,1,2*npoints*sizeof(int));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_y,0,2*npoints*sizeof(int));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID     = DSP_FFT32x32_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (DSP_fft32x32_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->w_InArrID1 = INBUF0;
    fp0->nx         = npoints;
    fp0->x_InArrID2 = INBUF1;
    fp0->y_OutArrID1= OUTBUF0;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
void C6ACCEL_DSP_ifft32x32(C6accel_Handle hC6accel,short *w, int nx, short *x, short *y)
Arguments
* hC6accel C6accel Handle
* w[2*nx] Pointer to complex 32-bit FFT coefficients.
* nx Length of FFT in complex samples. Must be power of 2 or 4, and 16 = nx = 65536.
* x[2*nx] Pointer to complex 32-bit data input.
* y[2*nx] Pointer to complex 32-bit data output.

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description This routine computes an extended precision complex inverse mixed radix FFT with rounding and
digit reversal. Input data x[ ], output data y[ ], and coefficients w[ ] are 32-bit. The output is returned
in the separate array y[ ] in normal order. Each complex value is stored with interleaved real and imaginary
parts. The code uses a special ordering of FFT coefficients (also called twiddle factors) and memory accesses
to improve performance in the presence of cache. fft32x32 can be reused to perform IFFT, by first conjugating
the input, performing the FFT, and conjugating again. This allows fft32x32 to perform the IFFT as well.
However, if the double conjugation needs to be avoided, then this routine uses the same twiddle factors as the
FFT and performs an IFFT. The change in the sign of the twiddle factors is adjusted for in the routine. Hence,
this routine uses the same twiddle factors as the fft32x32 routine.

*/
int C6accel_DSP_ifft32x32 ( C6accel_Handle hC6accel,
                             const int * restrict ptr_w,       /* Input twiddle factors */
                                   int            npoints,     /* number of points      */
                                   int * restrict ptr_x,       /* transformed data reversed */
                                   int * restrict ptr_y        /* linear transformed data */
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int                         status;
    /* Define pointer to function parameter structure */
    DSP_ifft32x32_Params         *fp0;
    XDAS_Int8                    *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(DSP_ifft32x32_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size    = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 2;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_w,0,2*npoints*sizeof(int));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,1,2*npoints*sizeof(int));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_y,0,2*npoints*sizeof(int));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID     = DSP_IFFT32x32_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

     /* Initialize pointers to function parameters */
    fp0 = (DSP_ifft32x32_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->w_InArrID1 = INBUF0;
    fp0->nx         = npoints;
    fp0->x_InArrID2 = INBUF1;
    fp0->y_OutArrID1= OUTBUF0;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;
}


/*
int C6ACCEL_DSP_autocor(C6accel_Handle hC6accel,short *r,short *x, int nx, int nr)
Arguments
* hC6accel C6accel Handle
* r[nr] Output array
* x[nx+nr] Input array. Must be double-word aligned.
* nx Length of autocorrelation. Must be a multiple of 8.
* nr Number of lags. Must be a multiple of 4.

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description This routine accepts an input array of length nx + nr and performs nr autocorrelations each of length nx producing nr output results. This is typically used in VSELP code.

Special Requirements

* nx must be a multiple of 8.
* nr must be a multiple of 4.
* x[ ] must be double-word aligned

*/
int C6accel_DSP_autocor(C6accel_Handle hC6accel,
                         short * restrict ptr_r,  /* Output array */
                         short * restrict ptr_x,  /*Input array. Must be double-word aligned */
                         int     nx,              /*Length of autocorrelation.*/
                         int     nr )             /* Number of lags */

{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int                         status;
    /* Define pointer to function parameter structure */
    DSP_autocor_Params         *fp0;
    XDAS_Int8                  *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(DSP_autocor_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size    = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,(nx+nr)*sizeof(short));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,nr*sizeof(short));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID     = DSP_AUTOCOR_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (DSP_autocor_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->x_InArrID1 = INBUF0;
    fp0->nx         = nx;
    fp0->nr         = nr;
    fp0->r_OutArrID1= OUTBUF0;



    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

   /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int C6ACCEL_DSP_dotprod(C6accel_Handle hC6accel,short *x, short *y,int r, int nx)

Arguments
* hC6accel C6accel Handle
* x[nx] First vector array. Must be double-word aligned.
* y[nx] Second vector array. Must be double word-aligned.
* nx Number of elements of vector. Must be multiple of 4.
* r return int Dot product of x and y.

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description This routine takes two vectors and calculates their dot product.
The inputs are 16-bit short data and the output is a 32-bit number.

*/
int C6accel_DSP_dotprod(C6accel_Handle hC6accel,
                         short * restrict ptr_x,  /* Output array */
                         short * restrict ptr_y,  /*Input array. Must be double-word aligned */
                         int   * ptr_r,
                         int     nx)              /* Number of elements */

{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int                         status;
    /* Define pointer to function parameter structure */
    DSP_dotprod_Params          *fp0;
    //int                         outRet[1];
    XDAS_Int8                   *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(DSP_dotprod_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size    = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 2;
    outBufDesc.numBufs = 1;


    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,(nx)*sizeof(short));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_y,1,(nx)*sizeof(short));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,sizeof(int));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Initialize the extended InArgs structure */
    CInArgs->fxn[0].FxnID     = DSP_DOTPROD_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Set function Id and parameter pointers for first function call */
    fp0 = (DSP_dotprod_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);
    /* Fill in the fields in the parameter structure */
    fp0->x_InArrID1    = INBUF0;
    fp0->y_InArrID2    = INBUF1;
    fp0->nx            = nx;
    fp0->Ret_outArrID1 = OUTBUF0;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        //Free the InArgs structure
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

    //return(outRet[0]);

}

/*
int C6ACCEL_DSP_dotp_sqr(C6accel_Handle hC6accel,int G, short *x, short *y, int *r,int G_out, int nx)

Arguments:
* hC6accel C6accel Handle
* G Calculated value of G (used in the VSELP coder).
* x[nx] First vector array
* y[nx] Second vector array
* r Result of vector dot product of x and y.
* nx Number of elements. Must be multiple of 4, and =12.
* G_out: New value of G_out

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description This routine performs an nx element dot product of x[ ] and y[ ] and stores it in r.
It also squares each element of y[ ] and accumulates it in G. G is passed back to the calling function
in register A4. This computation of G is used in the VSELP coder.

*/
int C6accel_DSP_dotp_sqr(C6accel_Handle hC6accel,
                         int     Gin,
                         short * restrict ptr_x,  /* Output array */
                         short * restrict ptr_y,  /*Input array. Must be double-word aligned */
                         int   * restrict ptr_r,
                         int   * restrict ptr_Gout,
                         int     nx)                 /* Number of elements */
{

    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int                         status;
    /* Define pointer to function parameter structure */
    DSP_dotp_sqr_Params         *fp0;
   // int                        G[1];
    XDAS_Int8                   *pAlloc;

    ACQUIRE_CODEC_ENGINE;

      /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size    = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 2;
    outBufDesc.numBufs = 2;


    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,(nx)*sizeof(short));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_y,1,(nx)*sizeof(short));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,sizeof(int));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_Gout,1,sizeof(int));

      /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size =  sizeof(Fxn_struct)+
                      sizeof(DSP_dotp_sqr_Params)+
                      sizeof(CInArgs->size)+
                      sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Set up buffers to pass buffers in and out to alg  */
    CInArgs->fxn[0].FxnID     = DSP_DOTP_SQR_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

     /* Initialize pointers to function parameters */
    fp0 = (DSP_dotp_sqr_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->G = Gin;
    fp0->x_InArrID1 = INBUF0;
    fp0->y_InArrID2 = INBUF1;
    fp0->r_OutArrID1= OUTBUF0;
    fp0->nx = nx;
    fp0->Ret_outArrID2=OUTBUF1;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

    //return(G[0]);

}

/*
int C6ACCEL_DSP_mat_mul(C6accel_Handle hC6accel,short * restrict x, int r1, int c1, short * restrict y, int c2, short* restrict r, int qs)

Arguments
* hC6accel C6accel Handle
* x [r1*c1] Pointer to input matrix of size r1*c1.
* r1 Number of rows in matrix x.
* c1 Number of columns in matrix x. Also number of rows in y.
* y [c1*c2] Pointer to input matrix of size c1*c2.
* c2 Number of columns in matrix y.
* r [r1*c2] Pointer to output matrix of size r1*c2.
* qs Final right-shift to apply to the result.

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description This function computes the expression �r = x * y� for the matrices x and y.
The columnar dimension of x must match the row dimension of y. The resulting matrix has
the same number of rows as x and the same number of columns as y. The values stored in
the matrices are assumed to be fixed-point or integer values. All intermediate sums are
retained to 32-bit precision, and no overflow checking is performed. The results are
right-shifted by a user-specified amount, and then truncated to 16 bits.

*/
int C6accel_DSP_mat_mul( C6accel_Handle hC6accel,
                          short *restrict ptr_x,  /* Input matrix 1 */
                          int     r1,              /* Number of rows in matrix 1 */
                          int     c1,              /* Number of cols in matrix 1 */
                          short *restrict ptr_y,  /* Input array. Must be double-word aligned */
                          int     c2,              /* Number of cols for matrix 2 */
                          short *restrict ptr_r,  /* Output matrix */
                          int     qs)              /* Number of elements */
{

    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int status;
    /* Define pointer to function parameter structure */
    DSP_mat_mul_Params          *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 2;
    outBufDesc.numBufs = 1;


    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,r1*c1*sizeof(short));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_y,1,c1*c2*sizeof(short));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,r1*c2*sizeof(short));


    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size =  sizeof(Fxn_struct)+
                      sizeof(DSP_mat_mul_Params)+
                      sizeof(CInArgs->size)+
                      sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID     = DSP_MAT_MUL_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

     /* Initialize pointers to function parameters */
    fp0 = (DSP_mat_mul_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->x_InArrID1  = INBUF0;
    fp0->y_InArrID2  = INBUF1;
    fp0->r_OutArrID1 = OUTBUF0;
    fp0->r1          = r1;
    fp0->c1          = c1;
    fp0->c2          = c2;
    fp0->qs          = qs;

    /* Call the actual algorithm */
    status  = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int C6ACCEL_DSP_fir_gen(C6accel_Handle hC6accel,short *x, short *h, short *r, int nh, int nr)

Arguments
* hC6accel C6accel Handle
* x[nr+nh-1] Pointer to input array of size nr + nh - 1.
* h[nh] Pointer to coefficient array of size nh (coefficients must be in reverse order).
* r[nr] Pointer to output array of size nr. Must be word aligned.
* nh Number of coefficients. Must be =5.
* nr Number of samples to calculate. Must be a multiple of 4.

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description Computes a real FIR filter (direct-form) using coefficients stored in vector h[ ].The real data input is stored in vector x[ ]. The filter output result is stored in vector r[ ]. It operates on 16-bit data with a 32-bit accumulate. The filter calculates nr output samples using nh coefficients.


*/
int C6accel_DSP_fir_gen( C6accel_Handle hC6accel,
                          short * restrict ptr_x,
                          short * restrict ptr_h,
                          short * restrict ptr_r,
                          int     nh,
                          int     nr
                         )
{

    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int                         status;
    /* Define pointer to function parameter structure */
    DSP_fir_gen_Params          *fp0;
    XDAS_Int8                   *pAlloc;

    ACQUIRE_CODEC_ENGINE;
    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 2;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,(nr+nh+1)*sizeof(short));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_h,1,(nh)*sizeof(short));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,(nr)*sizeof(short));

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(DSP_fir_gen_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;


    /* Initialize .size fields for dummy input and output arguments */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID     = DSP_FIR_GEN_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

     /* Initialize pointers to function parameters */
    fp0 = (DSP_fir_gen_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->x_InArrID1  = INBUF0;
    fp0->h_InArrID2  = INBUF1;
    fp0->r_OutArrID1 = OUTBUF0;
    fp0->nh          = nh;
    fp0->nr          = nr;

    /* Call the actual algorithm */
    status  = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;
}

/*
int C6ACCEL_DSP_fir_r4(C6accel_Handle hC6accel,short *x, short *h, short *r, int nh, int nr)

Arguments x[nr+nh-1] Pointer to input array of size nr + nh - 1.
* hC6accel C6accel Handle
h[nh] Pointer to coefficient array of size nh (coefficients must be in reverse order).
r[nr] Pointer to output array of size nr.
nh Number of coefficients. Must be multiple of 4 and =8.
nr Number of samples to calculate. Must be multiple of 4.

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description Computes a real FIR filter (direct-form) using coefficients stored in vector h[ ].
The real data input is stored in vector x[ ]. The filter output result is stored in vector r[ ].
This FIR operates on 16-bit data with a 32-bit accumulate. The filter calculates nr output samples
using nh coefficients.


*/
int C6accel_DSP_fir_r4(  C6accel_Handle hC6accel,
                          short * restrict ptr_x,
                          short * restrict ptr_h,
                          short * restrict ptr_r,
                          int              nh,
                          int              nr
                         )
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs            *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int                         status;
    /* Define pointer to function parameter structure */
    DSP_fir_r4_Params          *fp0;
    XDAS_Int8                  *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size    = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 2;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,(nr+nh-1)*sizeof(short));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_h,1,(nh)*sizeof(short));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,(nr)*sizeof(short));

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(DSP_fir_r4_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;


    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= DSP_FIR_R4_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (DSP_fir_r4_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);
    fp0->x_InArrID1 = INBUF0;
    fp0->h_InArrID2 = INBUF1;
    fp0->r_OutArrID1= OUTBUF0;
    fp0->nh = nh;
    fp0->nr = nr;

    /* Call the actual algorithm */
    status  = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int C6ACCEL_DSP_fir_r8(C6accel_Handle hC6accel,short *x, short *h, short *r, int nh, int nr)

Arguments
* hC6accel C6accel Handle
x[nr+nh-1] Pointer to input array of size nr + nh-1.
h[nh] Pointer to coefficient array of size nh (coefficients must be in reverse order).
r[nr] Pointer to output array of size nr. Must be word aligned.
nh Number of coefficients. Must be multiple of 8, = 8.
nr Number of samples to calculate. Must be multiple of 4.

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description Computes a real FIR filter (direct-form) using coefficients stored in vector h[ ].
The real data input is stored in vector x[ ]. The filter output result is stored in vector r[ ].
This FIR operates on 16-bit data with a 32-bit accumulate. The filter calculates nr output samples
using nh coefficients.

*/
int C6accel_DSP_fir_r8(  C6accel_Handle hC6accel,
                          short * restrict ptr_x,
                          short * restrict ptr_h,
                          short * restrict ptr_r,
                          int     nh,
                          int     nr
                         )
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int status;
    /* Define pointer to function parameter structure */
    DSP_fir_r8_Params          *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 2;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,(nr+nh-1)*sizeof(short));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_h,1,(nh)*sizeof(short));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,(nr)*sizeof(short));

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(DSP_fir_r8_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= DSP_FIR_R8_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (DSP_fir_r8_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->x_InArrID1 = INBUF0;
    fp0->h_InArrID2 = INBUF1;
    fp0->r_OutArrID1= OUTBUF0;
    fp0->nh = nh;
    fp0->nr = nr;

    /* Call the actual algorithm */
    status  = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int C6ACCEL_DSP_iir(C6accel_Handle hC6accel,short Input, short *Coefs, int nCoefs, short State)

Arguments
* hC6accel C6accel Handle
* x Input value (16-bit).
* h[nh] Coefficient input vector.
* nh Number of coefficients.
* b[nh] State vector.

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description This function implements an IIR filter, with a number of biquad stages given by nh / 4.
It accepts a single sample of input and returns a single sample of output. Coefficients are expected
to be in the range [-2.0, 2.0) with Q14 precision.

*/
int C6accel_DSP_iir(     C6accel_Handle hC6accel,
                          short * restrict ptr_x,
                          short * restrict ptr_h,
                          int              nh,
                          short * restrict ptr_b,
                          short * restrict ptr_r,
                          int              npoints
                         )
{

    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int                         status;
    /* Define pointer to function parameter structure */
    DSP_iir_Params              *fp0;
    XDAS_Int8                   *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 3;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,(npoints)*sizeof(short));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_h,1,(nh)*sizeof(short));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_b,2,(nh)*sizeof(short));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,(npoints)*sizeof(short));

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(DSP_iir_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;
   /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= DSP_IIR_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (DSP_iir_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->x_InArrID1   = INBUF0;
    fp0->h_InArrID2   = INBUF1;
    fp0->b_InArrID3   = INBUF2;
    fp0->Ret_OutArrID1= OUTBUF0;
    fp0->nh           = nh;
    fp0->n            = npoints;

    /* Call the actual algorithm */
    status  = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}


/*
int C6ACCEL_IMG_histogram_8(C6accel_Handle hC6accel,const unsigned char * restrict in_data, int n, short accumulate,unsigned short * restrict t_hist, unsigned short * restrict hist)

Arguments
* hC6accel C6accel Handle
* in_data[n] Input image. Must be word aligned.
* n Number of pixels in input image. Must be multiple of 8.
* accumulate
  1: Add to existing histogram in hist[ ]
 -1: Subtract from existing histogram in hist[ ]
* t_hist[1024] Array of temporary histogram bins. Must be initialized to zero.
* hist[256] Array of updated histogram bins.

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description This routine computes the histogram of the array in_data[ ] which contains n 8-bit elements.
It returns a histogram in the array hist[ ] with 256 bins at 16-bit precision. It can either add or subtract
to an existing histogram, using the accumulate control. It requires temporary storage for four temporary
histograms, t_hist[ ], which are later summed together.

*/
int C6accel_IMG_histogram_8
(   C6accel_Handle hC6accel,
    const unsigned char *restrict in,   /* Input image data   */
    int n,                              /* Number of pixels in image */
    short accumulate,                   /* accumulate +1, or subtract -1 from hist[] */
    unsigned short      *restrict t_hist,
    unsigned short      *restrict hist  /* Output histogram data  */
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int status;
    /* Define pointer to function parameter structure */
    IMG_histogram_8_Params      *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IMG_histogram_8_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 1;
    outBufDesc.numBufs = 2;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(in,0,(n)*sizeof(char));
    // For special case when we actually need outBuf t_hist to be an inOutBuf we need to write back here as well
    //Memory_cacheWbInv((Ptr)hist, 256*sizeof(short));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(t_hist,0,1024*sizeof(short));

    // For special case when we actually need outBuf to be an inOutBuf we need to write back here as well
    //Memory_cacheWbInv((Ptr)hist, 256*sizeof(short));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(hist,1,256*sizeof(short));

     /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= IMG_HISTOGRAM_8_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IMG_histogram_8_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->indata_InArrID1=  INBUF0;
    fp0->n= n;
    fp0->accumulate = accumulate;
    fp0->t_hist_OutArrID1= OUTBUF0;
    fp0->hist_OutArrID2 = OUTBUF1;

   /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc,NULL, (UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;


}

/*
int C6ACCEL_IMG_histogram_16(C6accel_Handle hC6accel, unsigned short *restrict in, short *restrict hist, short *restrict t_hist, int n, int accumulate, int img_bits)

Arguments :
* hC6accel C6accel Handle
*in Input image of size n
*hist Array of updated histogram bins
*t_hist Array of temporary histogram bins
*n Nunber of pixels in input image
*accumulate
  1: add to existing histogram in hist[ ]
 -1: subtract from existing histogram in hist[ ]
*img_bits Number of valid data bits in a pixel

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description: This code takes a histogram of an array (of type short) with n number of pixels, with img_bits
being the number of valid data bits in a pixel. It returns the histogram of corresponding number of bins at
img_bits bits precision. It can either add or subtract to an existing histogram, using the accumulate control.
It requires some temporary storage for four temporary histograms, which are later summed together. The length
of the hist and the t_hist arrays depends on the value of img_bits. The length of the hist array is 2(img_bits)
and that of t_hist is 4 * 2(img_bits) as there are no pixel values greater than or equal to 2(img_bits) in the given image.

*/
int C6accel_IMG_histogram_16
(    C6accel_Handle hC6accel,
     unsigned short *restrict       in,  /* input image data                */
                int                  n,  /* number of input image pixels    */
                int         accumulate,  /* define add/sum for histogtram   */
              short *restrict   t_hist,  /* temporary histogram bins        */
              short *restrict     hist,  /* running histogram bins          */
                int           img_bits   /* number of data bits in a pixel  */
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int status;
     /* Define pointer to function parameter structure */
    IMG_histogram_16_Params      *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IMG_histogram_16_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 1;
    outBufDesc.numBufs = 2;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(in,0,(n)*sizeof(short));
    // For special case when we actually need outBuf to be an inOutBuf we need to write back here as well
    Memory_cacheWbInv((Ptr)t_hist, 4*(1<<img_bits)*sizeof(short));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(t_hist,0,4*(1<<img_bits)*sizeof(short));
    // For special case when we actually need outBuf to be an inOutBuf we need to write back here as well
    Memory_cacheWbInv((Ptr)hist, (1<<img_bits)*sizeof(short));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(hist,1,(1<<img_bits)*sizeof(short));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= IMG_HISTOGRAM_16_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IMG_histogram_16_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->in_InArrID1=  INBUF0;
    fp0->n= n;
    fp0->accumulate = accumulate;
    fp0->t_hist_OutArrID1= OUTBUF0;
    fp0->hist_OutArrID2 =  OUTBUF1;
    fp0->img_bits = img_bits;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc,&outBufDesc, NULL, (UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;


}

/*
int C6ACCEL_IMG_median_3x3_8(C6accel_Handle hC6accel, unsigned char *in_data, int cols, unsigned char *out_data)

Arguments
* hC6accel C6accel Handle
*in_data Pointer to input image data. No alignment is required.
*cols Number of columns in input (or output). Must be multiple of 4.
*out_data Pointer to output image data. No alignment is required.

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description This routine performs a 3�3 median filtering algorithm. The gray level at each pixel is replaced
by the median of the nine neighborhood values. The function processes three lines of input data pointed to
by in_data, where each line is cols� pixels wide, and writes one line of output data to out_data. For the
first output pixel, two columns of input data outside the input image are assumed to be all 127. The median
of a set of nine numbers is the middle element, so that half of the elements in the list are larger and half
are smaller. A median filter removes the effect of extreme values from data. It is a commonly used operation
for reducing impulsive noise in images.

*/
int C6accel_IMG_median_3x3_8
(   C6accel_Handle hC6accel,
    const unsigned char *restrict in,   /* Input image data, pointer to a line of image   */
    int cols,                           /* Number of pixels in image */
    unsigned char       *restrict out   /* Output image line  */
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int status;
    /* Define pointer to function parameter structure */
    IMG_median_3x3_8_Params     *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IMG_median_3x3_8_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(in,0,(3*cols)*sizeof(char));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(out,0,(cols)*sizeof(char));

     /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    /* Based on number of functions (here 1) we overwrite the generic fxn[1] structure with params for fxn[0] */
    CInArgs->fxn[0].FxnID= IMG_MEDIAN_3X3_8_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=(sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct));

    /* Initialize pointers to function parameters */
    fp0 = (IMG_median_3x3_8_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->InArrID1=  INBUF0;
    fp0->Col= cols;
    fp0->OutArrID1= OUTBUF0;


    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int C6ACCEL_IMG_median_3x3_16(C6accel_Handle hC6accel, const short *restrict i_data, int n, short *restrict o_data) ===

Arguments
* hC6accel C6accel Handle
*i_data Pointer to input array of size 3 x n
*n Width of the input image
*o_data Pointer to output array of size 1 x n

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description This function performs a 3x3 median filter operation on 16-bit signed values. The median filter
comes under the class of non-linear signal processing algorithms. The grey level at each pixel is replaced
by the median of the nine neighboring values. The median of a set of nine numbers is the middle element so
that half of the elements in the list are larger and half are smaller. The i_data points to an array which
consists of three rows of pixel values. The median value is calculated corresponding to the middle row of
i_data, and written into memory location pointed by o_data. The first two values in the output array will
not be containing any meaningful data. The 3rd value in the output array will be the median of 2nd value
in the middle row of input array and so on. The nth value in the output array will be the median of the
(n-1)th value in the mid row of input array. Hence, the output array will not contain the median values
corresponding to first and last elements in the middle row of input image. .

*/
int C6accel_IMG_median_3x3_16
(   C6accel_Handle hC6accel,
    const short *restrict in,   /* Input image data, pointer to a line of image   */
    int cols,                   /* Number of pixels in image */
    short       *restrict out   /* Output image line  */
)

{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int status;
    /* Define pointer to function parameter structure */
    IMG_median_3x3_16_Params     *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IMG_median_3x3_16_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(in,0,(3*cols)*sizeof(short));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(out,0,(cols)*sizeof(short));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= IMG_MEDIAN_3X3_16_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IMG_median_3x3_16_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->idata_InArrID1=  INBUF0;
    fp0->nwidth= cols;
    fp0->odata_OutArrID1= OUTBUF0;

   /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;


}

/*
int C6ACCEL_IMG_median_3x3_16s(C6accel_Handle hC6accel, const short *restrict i_data, int n, short *restrict o_data)
Arguments
* hC6accel C6accel Handle
* i_data Pointer to input array of size 3 x n
* n Width of the input image
* o_data Pointer to output array of size 1 x n

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description This function performs a 3x3 median filter operation on 16-bit signed values.
The median filter comes under the class of non-linear signal processing algorithms. The grey level
at each pixel is replaced by the median of the nine neighboring values. The median of a set of nine
numbers is the middle element so that half of the elements in the list are larger and half are smaller.
The i_data points to an array which consists of three rows of pixel values. The median value is calculated
corresponding to the middle row of i_data, and written into memory location pointed by o_data. The first
two values in the output array will not be containing any meaningful data. The 3rd value in the output
array will be the median of 2nd value in the middle row of input array and so on. The nth value in the
output array will be the median of the (n-1)th value in the mid row of input array. Hence, the output array
will not contain the median values corresponding to first and last elements in the middle row of input image.
*/
int C6accel_IMG_median_3x3_16s
(   C6accel_Handle hC6accel,
    const short *restrict in,   /* Input image data, pointer to a line of image   */
    int cols,                           /* Number of pixels in image */
    short       *restrict out   /* Output image line  */
)

{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int status;
    /* Define pointer to function parameter structure */
    IMG_median_3x3_16s_Params     *fp0;
    XDAS_Int8 *pAlloc;


    ACQUIRE_CODEC_ENGINE;


    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IMG_median_3x3_16s_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);


    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(in,0,(3*cols)*sizeof(short));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(out,0,(cols)*sizeof(short));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= IMG_MEDIAN_3X3_16S_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IMG_median_3x3_16s_Params *)((XDAS_Int8*)CInArgs+CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the parameters in the parameter structure */
    fp0->idata_InArrID1=  INBUF0;
    fp0->nwidth= cols;
    fp0->odata_OutArrID1= OUTBUF0;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;


}

/*
Variation of the function C6accel_IMG_median_3x3_8 where all rows and cols of the images are passes and
processed in sections of 3 rows at a time
*/
int C6accel_IMG_median_3x3_8_Frame
(   C6accel_Handle hC6accel,
    const unsigned char *restrict in,   /* Input image data, pointer to a line of image   */
    int rows,                           /* Number of rows in image */
    int cols,                           /* Number of cols in image */
    unsigned char       *restrict out   /* Output image line  */
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int status;
    /* Define pointer to function parameter structure */
    IMG_median_3x3_8_Frame_Params     *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IMG_median_3x3_8_Frame_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(in,0,(rows*cols)*sizeof(char));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(out,0,(rows*cols)*sizeof(char));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= IMG_MEDIAN_3X3_8_FRAME_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IMG_median_3x3_8_Frame_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->InArrID1=  INBUF0;
    fp0->Col= cols;
    fp0->Row= rows;
    fp0->OutArrID1= OUTBUF0;


    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
Variation of the function C6accel_IMG_median_3x3_16 where all rows and cols of the images are passes and
processed in sections of 3 rows at a time
*/
int C6accel_IMG_median_3x3_16_Frame
(   C6accel_Handle hC6accel,
    const short *restrict in,   /* Input image data, pointer to a line of image   */
    int rows,                   /* Number of rows in image */
    int cols,                   /* Number of cols in image */
    short       *restrict out   /* Output image line  */
)

{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int status;
    /* Define pointer to function parameter structure */
    IMG_median_3x3_16_Frame_Params     *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IMG_median_3x3_16_Frame_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(in,0,(rows*cols)*sizeof(short));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(out,0,(rows*cols)*sizeof(short));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= IMG_MEDIAN_3X3_16_FRAME_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IMG_median_3x3_16_Frame_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->idata_InArrID1=  INBUF0;
    fp0->Row= rows;
    fp0->nwidth= cols;
    fp0->odata_OutArrID1= OUTBUF0;


    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;


}

/*
Variation of the function C6accel_IMG_median_3x3_16s where all rows and cols of the images are passes and
processed in sections of 3 rows at a time
*/
int C6accel_IMG_median_3x3_16s_Frame
(   C6accel_Handle hC6accel,
    const short *restrict in,   /* Input image data, pointer to a line of image   */
    int rows,                   /* Number of rows in image */
    int cols,                   /* Number of cols in image */
    short       *restrict out   /* Output image line  */
)

{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int status;
    /* Define pointer to function parameter structure */
    IMG_median_3x3_16s_Frame_Params     *fp0;
    XDAS_Int8 *pAlloc;


    ACQUIRE_CODEC_ENGINE;


    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IMG_median_3x3_16s_Frame_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);


    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(in,0,(rows*cols)*sizeof(short));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(out,0,(rows*cols)*sizeof(short));


    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= IMG_MEDIAN_3X3_16S_FRAME_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IMG_median_3x3_16s_Frame_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the parameters in the parameter structure */
    fp0->idata_InArrID1=  INBUF0;
    fp0->Row= rows;
    fp0->nwidth= cols;
    fp0->odata_OutArrID1= OUTBUF0;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;


}

/*
int C6ACCEL_IMG_conv_3x3_i8_c8s(C6accel_Handle hC6accel, unsigned char *in_data, unsigned char *out_data, int cols, char *mask, int shift)
Arguments
* hC6accel C6accel Handle
in_data[ ] Input image
out_data[ ] Output image
cols Number of columns in the input image. Must be multiple of 8
mask[3][3] 3x3 mask shift Shift value

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description The convolution kernel accepts three rows of cols input pixels and produces one output row of cols
pixels using the input mask of 3 by 3. The user-defined shift value is used to shift the convolution value
down to the byte range. The convolution sum is also range limited to 0.255. The shift amount is non-zero for
low pass filters, and zero for high pass and sharpening filters.

*/
int C6accel_IMG_conv_3x3_i8_c8s (C6accel_Handle hC6accel,
                                  const unsigned char *restrict inptr,
                                  unsigned char *restrict outptr,
                                  int            x_dim,
                                  const          char *restrict mask,
                                  int            shift)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int status;
    /* Define pointer to function parameter structure */
    IMG_conv_3x3_i8_c8s_Params  *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IMG_conv_3x3_i8_c8s_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 2;
    outBufDesc.numBufs = 1;

     /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(inptr,0,(3*x_dim)*sizeof(char));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(mask,1,(3*3)*sizeof(char));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(outptr,0,(x_dim)*sizeof(char));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= IMG_CONV_3X3_I8_C8S_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IMG_conv_3x3_i8_c8s_Params *)((XDAS_Int8*)CInArgs+CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->in_data_InArrID1=  INBUF0;
    fp0->mask_InArrID2   =  INBUF1;
    fp0->cols= x_dim;
    fp0->out_data_OutArrID1= OUTBUF0;
    fp0->shift= shift;

   /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int C6ACCEL_IMG_conv_3x3_i16s_c16s(C6accel_Handle hC6accel, const short *restrict imgin_ptr, short *restrict imgout_ptr, short width, short pitch,const short *restrict mask_ptr, short shift)
Arguments

* imgin_ptr Pointer to input image 16-bit signed
* imgout_ptr Pointer to output image 16-bit signed
* width Number of outputs to be calculated
* pitch Number of columns in the input image
* mask_ptr Pointer to 3x3 mask used-16 bit signed
* shift User specified shift value

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description
The convolution kernel accepts three rows of'pitch input pixels and produces one row of width
output pixels using the input mask of 3 by 3. This convolution performs a point by point multiplication of 3 by
3 masks with the input image. The result of 9 multiplications are then summed to produce a 32-bit convolution
intermediate sum. Overflow while accumulation is not handled. However assumptions are made on filter gain to
avoid overflow. The user-defined shift value is used to shift this convolution sum down to the short range and
store in an output array. The result being stored is also saturated to the -32768 to 32767 inclusive.
The mask is moved one column at a time, advancing the mask over the entire image until the entire width is
covered. The input, output image pixels and the masks are provided as 16-bit signed values.


*/
//Note that width is a short so too small to contain all pixels in an image
int C6accel_IMG_conv_3x3_i16s_c16s(C6accel_Handle hC6accel,
                                    const short     *restrict    imgin_ptr,
                                          short     *restrict    imgout_ptr,
                                          short                  width,
                                          short                  pitch,
                                    const short     *restrict    mask_ptr,
                                          short                  shift)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int status;
    /* Define pointer to function parameter structure */
    IMG_conv_3x3_i16s_c16s_Params  *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IMG_conv_3x3_i16s_c16s_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 2;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(imgin_ptr,0,(3*width)*sizeof(short));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(mask_ptr,1,(3*3)*sizeof(short));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(imgout_ptr,0,(width)*sizeof(short));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= IMG_CONV_3X3_I16S_C16S_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IMG_conv_3x3_i16s_c16s_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->in_data_InArrID1=  INBUF0;
    fp0->mask_InArrID2   =  INBUF1;
    fp0->width= width;
    fp0->pitch= pitch;
    fp0->out_data_OutArrID1= OUTBUF0;
    fp0->shift= shift;


   /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;


}

/*
Variation of the function C6accel_IMG_conv_3x3_i8_c8s where all rows and cols of the images are passed and
processed in sections.
*/
int C6accel_IMG_conv_3x3_i8_c8s_Frame (C6accel_Handle hC6accel,
                                  const unsigned char *restrict inptr,
                                  unsigned char *restrict outptr,
                                  int            rows,
                                  int            x_dim,
                                  const          char *restrict mask,
                                  int            shift)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int status;
    /* Define pointer to function parameter structure */
    IMG_conv_3x3_i8_c8s_Frame_Params  *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IMG_conv_3x3_i8_c8s_Frame_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 2;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(inptr,0,(x_dim*rows)*sizeof(char));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(mask,1,(3*3)*sizeof(char));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(outptr,0,(x_dim*rows)*sizeof(char));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= IMG_CONV_3X3_I8_C8S_FRAME_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IMG_conv_3x3_i8_c8s_Frame_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->in_data_InArrID1=  INBUF0;
    fp0->mask_InArrID2   =  INBUF1;
    fp0->rows = rows;
    fp0->cols= x_dim;
    fp0->out_data_OutArrID1= OUTBUF0;
    fp0->shift= shift;

  /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
Variation of the function C6accel_IMG_conv_3x3_i8_c8s where all rows and cols of the images are passed and
processed in sections.
*/
//Note that width is a short so too small to contain all pixels in an image
int C6accel_IMG_conv_3x3_i16s_c16s_Frame(C6accel_Handle hC6accel,
                                          const short     *restrict    imgin_ptr,
                                          short     *restrict    imgout_ptr,
                                          short                  rows,
                                          short                  width,
                                          short                  pitch,
                                    const short     *restrict    mask_ptr,
                                          short                  shift)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int status;
    /* Define pointer to function parameter structure */
    IMG_conv_3x3_i16s_c16s_Frame_Params  *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IMG_conv_3x3_i16s_c16s_Frame_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 2;
    outBufDesc.numBufs = 1;


    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(imgin_ptr,0,(width*rows)*sizeof(short));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(mask_ptr,1,(3*3)*sizeof(short));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(imgout_ptr,0,(width*rows)*sizeof(short));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= IMG_CONV_3X3_I16S_C16S_FRAME_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IMG_conv_3x3_i16s_c16s_Frame_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->in_data_InArrID1=  INBUF0;
    fp0->mask_InArrID2   =  INBUF1;
    fp0->rows = rows;
    fp0->width= width;
    fp0->pitch= pitch;
    fp0->out_data_OutArrID1= OUTBUF0;
    fp0->shift= shift;

   /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;


}


/*
int C6ACCEL_IMG_corr_3x3_i8_c8(C6accel_Handle hC6accel,const unsigned char *restrict inptr, unsigned char *restrict outptr, int x_dim, const unsigned char *restrict mask_ptr, short shift, short round)

Arguments
* hC6accel C6accel Handle
* inptr Pointer to input image (8-bit signed)
* outptr Pointer to output image (32-bit signed)
* n_out Number of outputs to be calculated
* x_dim Number of columns in the input image
* mask Pointer to 3x3 mask used 16-bit signed
* shift User-specified shift amount
* round User-specified round value

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description The correlation performs a point by point multiplication of the 3 by 3 mask with the input image.
The result of the nine multiplications are then summed up together to produce a convolution sum. A rounding
constant is added to the sum and shifted by user specified amount.

*/
int C6accel_IMG_corr_3x3_i8_c8 ( C6accel_Handle hC6accel,
                                  const unsigned char *restrict inptr,
                                  int                *restrict outptr,
                                  int                 n_out,
                                  int                 x_dim,
                                  const unsigned char         *restrict mask,
                                  short               shift,
                                  int                 round
                                  )
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int status;
     /* Define pointer to function parameter structure */
    IMG_corr_3x3_i8_c8_Params  *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IMG_corr_3x3_i8_c8_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 2;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(inptr,0,(x_dim*3)*sizeof(char));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(mask,1,(3*3)*sizeof(char));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(outptr,0,(x_dim)*sizeof(int));

     /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= IMG_CORR_3X3_I8_C8_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IMG_corr_3x3_i8_c8_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->in_InArrID1=  INBUF0;
    fp0->mask_InArrID2   =  INBUF1;
    fp0->n_out = n_out;
    fp0->x_dim= x_dim;
    fp0->out_OutArrID1= OUTBUF0;
    fp0->shift= shift;
    fp0->round= round;


   /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure  */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int C6ACCEL_IMG_corr_3x3_i8_c16s(C6accel_Handle hC6accel, const unsigned char *restrict imgin_ptr, int *restrict imgout_ptr, short width, short pitch, const short *restrict mask_ptr)
Arguments
* hC6accel C6accel Handle
* imgin_ptr Pointer to input image (8-bit signed)
* imgout_ptr Pointer to output image (32-bit signed)
* width Number of outputs to be calculated
* pitch Number of columns in the input image
* mask_ptr Pointer to 3x3 mask used (16-bit signed)

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description: The correlation kernel accepts three rows of pitch input pixels and produces one row of width output
pixels using the input mask of 3x3. This correlation performs a point-by-point multiplication of 3x3 masks with
the input image. The result of the nine multiplications are then summed to produce a 32-bit sum and then
stored in an output array. The mask is moved one column at a time, advancing the mask over the entire image
until the entire width is covered. The masks are provided as 16-bit signed values, the input image pixels
are provided as 8-bit unsigned values, and the output pixels will be 32-bit signed. The image mask to be
correlated is typically part of the input image or another image.

*/
int C6accel_IMG_corr_3x3_i16s_c16s(  C6accel_Handle hC6accel,
                                      const short   *restrict    imgin_ptr,
                                      int           *restrict    imgout_ptr,
                                      short                      width,
                                      short                      pitch,
                                      const short   *restrict    mask_ptr,
                                      short                      shift,
                                      int                        round)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int status;
    /* Define pointer to function parameter structure */
    IMG_corr_3x3_i16s_c16s_Params  *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IMG_corr_3x3_i16s_c16s_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 2;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(imgin_ptr,0,(width*3)*sizeof(short));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(mask_ptr,1,(3*3)*sizeof(short));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(imgout_ptr,0,(width)*sizeof(int));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= IMG_CORR_3X3_I16S_C16S_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IMG_corr_3x3_i16s_c16s_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->imgin_InArrID1=  INBUF0;
    fp0->mask_InArrID2   =  INBUF1;
    fp0->width= width;
    fp0->pitch= pitch;
    fp0->imgout_OutArrID1= OUTBUF0;
    fp0->shift= shift;
    fp0->round= round;

   /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
Variation of the function C6accel_IMG_corr_3x3_i8_c8 where all rows and cols of the images are passed and
processed in sections
*/
int C6accel_IMG_corr_3x3_i8_c8_Frame ( C6accel_Handle hC6accel,
                                  const unsigned char *restrict inptr,
                                  int                *restrict outptr,
                                  int                 rows,
                                  int                 n_out,
                                  int                 x_dim,
                                  const unsigned char         *restrict mask,
                                  short               shift,
                                  int                 round
                                  )
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int status;
     /* Define pointer to function parameter structure */
    IMG_corr_3x3_i8_c8_Frame_Params  *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IMG_corr_3x3_i8_c8_Frame_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 2;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(inptr,0,(x_dim*rows)*sizeof(char));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(mask,1,(3*3)*sizeof(char));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(outptr,0,(x_dim*rows)*sizeof(int));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= IMG_CORR_3X3_I8_C8_FRAME_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IMG_corr_3x3_i8_c8_Frame_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->in_InArrID1=  INBUF0;
    fp0->mask_InArrID2   =  INBUF1;
    fp0->rows = rows;
    fp0->n_out = n_out;
    fp0->x_dim= x_dim;
    fp0->out_OutArrID1= OUTBUF0;
    fp0->shift= shift;
    fp0->round= round;

   /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure  */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
Variation of the function C6accel_IMG_corr_3x3_i16s_c16s where all rows and cols of the images are passed and
processed in sections
*/
int C6accel_IMG_corr_3x3_i16s_c16s_Frame(  C6accel_Handle hC6accel,
                                      const short   *restrict    imgin_ptr,
                                      int           *restrict    imgout_ptr,
                                      short                      rows,
                                      short                      width,
                                      short                      pitch,
                                      const short   *restrict    mask_ptr,
                                      short                      shift,
                                      int                        round)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int status;
    /* Define pointer to function parameter structure */
    IMG_corr_3x3_i16s_c16s_Frame_Params  *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IMG_corr_3x3_i16s_c16s_Frame_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 2;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(imgin_ptr,0,(width*rows)*sizeof(short));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(mask_ptr,1,(3*3)*sizeof(short));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(imgout_ptr,0,(width*rows)*sizeof(int));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= IMG_CORR_3X3_I16S_C16S_FRAME_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IMG_corr_3x3_i16s_c16s_Frame_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->imgin_InArrID1=  INBUF0;
    fp0->mask_InArrID2   =  INBUF1;
    fp0->rows = rows;
    fp0->width= width;
    fp0->pitch= pitch;
    fp0->imgout_OutArrID1= OUTBUF0;
    fp0->shift= shift;
    fp0->round= round;

   /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}


/*
int C6ACCEL_IMG_sobel_3x3_8(C6accel_Handle hC6accel, const unsigned char *in_data, unsigned char *out_data, short cols, short rows)
Arguments
* hC6accel C6accel Handle
* in_data[ ] Input image of size cols * rows.
* out_data[ ] Output image of size cols * (rows-2).
* cols Number of columns in the input image. Must be multiple of 2.
* rows Number of rows in the input image. cols * (rows-2) must be multiple of 8.

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description This routine applies horizontal and vertical Sobel edge detection masks to the input image and
produces an output image which is two rows shorter than the input image.Within each row of the output,
the first and the last pixel will not contain meaningful results.

*/
int C6accel_IMG_sobel_3x3_8
(   C6accel_Handle hC6accel,
    const unsigned char *restrict in,   /* Input image data   */
    unsigned char       *restrict out,  /* Output image data  */
    short cols, short rows              /* Image dimensions   */
#ifdef SUPPORT_ASYNC
    ,E_CALL_TYPE callType
#endif
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int status;
    /* Define pointer to function parameter structure */
    IMG_sobel_3x3_8_Params      *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IMG_sobel_3x3_8_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(in,0,cols * rows*sizeof(char));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(out,0,cols * rows*sizeof(char));

     /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= IMG_SOBEL_3X3_8_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IMG_sobel_3x3_8_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->indata_InArrID1= INBUF0;
    fp0->outdata_OutArrID1= OUTBUF0;
    fp0->Col= cols;
    fp0->Row= rows;

#ifdef SUPPORT_ASYNC
    /* Call the actual algorithm */
    if (callType == ASYNC)
      {

       //Update async structure
       if (c6accelAsyncParams.asyncCallCount!=0)
          { status = UNIVERSAL_EFAIL;
            printf("Async call failed as %d are still pending\n");
          }
       else
          {c6accelAsyncParams.asyncCallCount++;
           memcpy(&(c6accelAsyncParams.inBufs),&inBufDesc, sizeof (XDM1_BufDesc));
 //           memcpy(&(c6accelAsyncParams.inOutBufs),&inOutBufs, sizeof (XDM1_BufDesc));
           memcpy(&(c6accelAsyncParams.outBufs), &outBufDesc,sizeof(XDM1_BufDesc));
           memcpy(&(c6accelAsyncParams.inArgs), CInArgs,sizeof(UNIVERSAL_InArgs));
           memcpy(&(c6accelAsyncParams.outArgs),&uniOutArgs,sizeof(UNIVERSAL_OutArgs));
           c6accelAsyncParams.pBuf = pAlloc;
           c6accelAsyncParams.pBufSize = InArg_Buf_size;

           status = UNIVERSAL_processAsync(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
           }
      }
     else
       {
#endif
         // Sync call
        status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        /* Free the InArgs structure */
        Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);
#ifdef SUPPORT_ASYNC
        }
#endif 

    RELEASE_CODEC_ENGINE;

    return status;

}

/* This function defination is added by Pramod */
/*
int C6ACCEL_IMG_sobel_3x3_8_16(C6accel_Handle hC6accel, const unsigned char *in_data, unsigned short *out_data, short cols, short rows)
Arguments
* hC6accel C6accel Handle
* in_data[ ] Input image of size cols * rows.
* out_data[ ] Output image of size cols * (rows-2).
* cols Number of columns in the input image. Must be multiple of 2.
* rows Number of rows in the input image. cols * (rows-2) must be multiple of 8.

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description This routine applies horizontal and vertical Sobel edge detection masks to the input image and
produces an output image which is two rows shorter than the input image.Within each row of the output,
the first and the last pixel will not contain meaningful results.

*/
int C6accel_IMG_sobel_3x3_8_16
(   C6accel_Handle hC6accel,
    const unsigned char *restrict in,   /* Input image data   */
    unsigned short       *restrict out,  /* Output image data  */
    short cols, short rows              /* Image dimensions   */
#ifdef SUPPORT_ASYNC
    ,E_CALL_TYPE callType
#endif
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int status;
    /* Define pointer to function parameter structure */
    IMG_sobel_3x3_8_16_Params      *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IMG_sobel_3x3_8_16_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(in,0,cols * rows*sizeof(char));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(out,0,cols * rows*sizeof(short));

     /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= IMG_SOBEL_3X3_8_16_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IMG_sobel_3x3_8_16_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->indata_InArrID1= INBUF0;
    fp0->outdata_OutArrID1= OUTBUF0;
    fp0->Col= cols;
    fp0->Row= rows;

#ifdef SUPPORT_ASYNC
    /* Call the actual algorithm */
    if (callType == ASYNC)
      {

       //Update async structure
       if (c6accelAsyncParams.asyncCallCount!=0)
          { status = UNIVERSAL_EFAIL;
            printf("Async call failed as %d are still pending\n",c6accelAsyncParams.asyncCallCount);
          }
       else
          {c6accelAsyncParams.asyncCallCount++;
           memcpy(&(c6accelAsyncParams.inBufs),&inBufDesc, sizeof (XDM1_BufDesc));
 //           memcpy(&(c6accelAsyncParams.inOutBufs),&inOutBufs, sizeof (XDM1_BufDesc));
           memcpy(&(c6accelAsyncParams.outBufs), &outBufDesc,sizeof(XDM1_BufDesc));
           memcpy(&(c6accelAsyncParams.inArgs), CInArgs,sizeof(UNIVERSAL_InArgs));
           memcpy(&(c6accelAsyncParams.outArgs),&uniOutArgs,sizeof(UNIVERSAL_OutArgs));
           c6accelAsyncParams.pBuf = pAlloc;
           c6accelAsyncParams.pBufSize = InArg_Buf_size;

           status = UNIVERSAL_processAsync(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
           }
      }
     else
       {
#endif
         // Sync call
        status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        /* Free the InArgs structure */
        Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);
#ifdef SUPPORT_ASYNC
        }
#endif 

    RELEASE_CODEC_ENGINE;

    return status;

}


/*
int C6ACCEL_IMG_sobel_3x3_16(C6accel_Handle hC6accel,constant unsigned short *restrict in, unsigned short *restrict out, short cols, short rows)

Arguments
* hC6accel C6accel Handle
* in[ ] Image input of size rows x cols
* out[ ] Image output of size (rows - 2) x cols
* cols Number of columns in the input image
* rows Number of rows in the input image

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description The IMG_sobel filter is applied to the input image. The input image dimensions are given by the
arguments 'cols' and 'rows'. The output image is 'cols' pixels wide and 'rows - 2' pixels tall.

*/
int C6accel_IMG_sobel_3x3_16
(   C6accel_Handle hC6accel,
    const unsigned short *restrict in,   /* Input image data   */
    unsigned short       *restrict out,  /* Output image data  */
    short cols, short rows              /* Image dimensions   */
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int status;
    /* Define pointer to function parameter structure */
    IMG_sobel_3x3_16_Params      *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IMG_sobel_3x3_16_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(in,0,cols * rows*sizeof(short));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(out,0,cols * rows*sizeof(short));

    /*Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= IMG_SOBEL_3X3_16_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IMG_sobel_3x3_16_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->indata_InArrID1= INBUF0;
    fp0->outdata_OutArrID1= OUTBUF0;
    fp0->Col= cols;
    fp0->Row= rows;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;


}

/*
int C6ACCEL_IMG_mulS_8(C6accel_Handle hC6accel, short * restrict imgR,int * restrict imgW,short constData,int count )
Arguments:
* hC6accel C6accel Handle
* imgR: Read pointer for the input image
* restrict imgW: Write pointer for the output image
* constData: Constant data to be multiplied (16 bit value)
* count: Number of samples in the image
Description: This function performs multiplication of each pixel in a image with a constant value. The image consist of 8 bits per pixel. The constant is 8 bits in size

*/
int C6accel_IMG_mulS_8
(   C6accel_Handle hC6accel,
    const unsigned char *restrict imgR,    /* Input image data 8 bits/pixel  */
    short               *restrict imgW,    /* Output image data 16 bits/pixel */
    char                 constData,        /* 8 bit constant to multiply by */
    int                  count             /* Number of pixels in image */
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int status;
    /* Define pointer to function parameter structure */
    IMG_mulS_8_Params     *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IMG_mulS_8_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(imgR,0,count * sizeof(char));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(imgW,0,count * sizeof(int));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= IMG_MULS_8_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IMG_mulS_8_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->imgR_InArrID1=  INBUF0;
    fp0->imgW_OutArrID1= OUTBUF0;
    fp0->constData= constData;
    fp0->count= count;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int C6ACCEL_IMG_mulS_16s(C6accel_Handle hC6accel, short * restrict imgR,int * restrict imgW, short constData, int count )
Arguments:
* hC6accel C6accel Handle
* imgR: Read pointer for the input image
* restrict imgW: Write pointer for the output image
* constData: Constant data to be multiplied (16 bit value)
* count: Number of samples in the image

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description This function performs multiplication of each pixel in a image with a constant value.
The image consist of 16bits per pixel. The constant is 16bits in size

*/
int C6accel_IMG_mulS_16s
(   C6accel_Handle hC6accel,
    const short *restrict imgR,   /* Input image data 16 bits/pixel  */
    int         *restrict imgW,    /* Output image data 32 bits/pixel */
    short       constData,        /* 16 bit constant to multiply by */
    int         count             /* Number of pixels in image */
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int status;
    /* Define pointer to function parameter structure */
    IMG_mulS_16s_Params     *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IMG_mulS_16s_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(imgR,0,count * sizeof(short));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(imgW,0,count * sizeof(int));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= IMG_MULS_16S_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IMG_mulS_16s_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->imgR_InArrID1=  INBUF0;
    fp0->imgW_OutArrID1= OUTBUF0;
    fp0->constData= constData;
    fp0->count= count;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int C6ACCEL_IMG_addS_8(C6accel_Handle hC6accel, char * restrict imgR, char * restrict imgW, char constData,int count )
Arguments:
* hC6accel C6accel Handle
* imgR: Read pointer for the input image
* restrict imgW: Write pointer for the output image
* constData: Constant data to be multiplied (16 bit value)
* count: Number of samples in the image

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description This function performs addition of each pixel in a image with a constant value. The image consist
of 8 bits per pixel. The constant is 8 bits in size.

*/
int C6accel_IMG_addS_8
(   C6accel_Handle hC6accel,
    const char *restrict imgR,            /* Input image data 8 bits/pixel  */
    char       *restrict imgW,            /* Output image data 8 bits/pixel */
    char                constData,        /* 8 bit constant to add */
    int                 count             /* Number of pixels in image */
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int status;
    /* Define pointer to function parameter structure */
    IMG_addS_8_Params     *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IMG_addS_8_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(imgR,0,count * sizeof(char));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(imgW,0,count * sizeof(char));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= IMG_ADDS_8_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IMG_addS_8_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->imgR_InArrID1=  INBUF0;
    fp0->imgW_OutArrID1= OUTBUF0;
    fp0->constData= constData;
    fp0->count= count;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int C6ACCEL_IMG_addS_16s(C6accel_Handle hC6accel, short * restrict imgR,short * restrict imgW, short constData, int count)
Arguments:
* hC6accel C6accel Handle
* imgR: Read pointer for the input image
* restrict imgW: Write pointer for the output image
* constData: Constant data to be multiplied (16 bit value)
* count: Number of samples in the image

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description: This function performs addition of each pixel in a image with a constant value. The image
consist of 16bits per pixel. The constant is 16bits in size
*/
int C6accel_IMG_addS_16s
(   C6accel_Handle hC6accel,
    const short *restrict imgR,   /* Input image data 16 bits/pixel   */
    short       *restrict imgW,    /* Output image data 16 bits/pixel */
    short        constData,        /* 16 bit constant to add           */
    int          count             /* Number of pixels in image */
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int status;
    /* Define pointer to function parameter structure */
    IMG_addS_16s_Params     *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IMG_addS_16s_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(imgR,0,count * sizeof(short));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(imgW,0,count * sizeof(short));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= IMG_ADDS_16S_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IMG_addS_16s_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->imgR_InArrID1=  INBUF0;
    fp0->imgW_OutArrID1= OUTBUF0;
    fp0->constData= constData;
    fp0->count= count;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int C6ACCEL_IMG_subS_8(C6accel_Handle hC6accel, const char * restrict imgR, char * restrict imgW, char constData,int count)
Arguments:
* hC6accel C6accel Handle
* imgR: Read pointer for the input image
* restrict imgW: Write pointer for the output image
* constData: Constant data to be multiplied
* count: Number of samples in the image

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description This function performs subtraction of each pixel in a image with a constant value. The image
consist of 16bits per pixel. The constant is 16bits in size

*/
int C6accel_IMG_subS_8
(   C6accel_Handle hC6accel,
    const char *restrict imgR,            /* Input image data 8 bits/pixel  */
    char       *restrict imgW,            /* Output image data 8 bits/pixel */
    char                constData,        /* 8 bit constant to sub */
    int                 count             /* Number of pixels in image */
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int status;
    /* Define pointer to function parameter structure */
    IMG_subS_8_Params     *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IMG_subS_8_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(imgR,0,count * sizeof(char));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(imgW,0,count * sizeof(char));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= IMG_SUBS_8_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IMG_subS_8_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->imgR_InArrID1=  INBUF0;
    fp0->imgW_OutArrID1= OUTBUF0;
    fp0->constData= constData;
    fp0->count= count;


    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;


}

/*
int C6ACCEL_IMG_subS_16s(C6accel_Handle hC6accel, short * restrict imgR, short * restrict imgW, short constData,int count)

Arguments:
* hC6accel C6accel Handle
* imgR: Read pointer for the input image
* restrict imgW: Write pointer for the output image
* constData: Constant data to be multiplied (16 bit value)
* count: Number of samples in the image

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description This function performs subtraction of each pixel in a image with a constant value. The image
consist of 16bits per pixel. The constant is 16bits in size

*/
int C6accel_IMG_subS_16s
(   C6accel_Handle hC6accel,
    const short *restrict imgR,   /* Input image data 16 bits/pixel  */
    short       *restrict imgW,    /* Output image data 16 bits/pixel*/
    short       constData,        /* 16 bit constant to sub          */
    int         count             /* Number of pixels in image       */
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int status;
    /* Define pointer to function parameter structure */
    IMG_subS_16s_Params     *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IMG_subS_16s_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(imgR,0,count * sizeof(short));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(imgW,0,count * sizeof(short));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= IMG_SUBS_16S_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IMG_subS_16s_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->imgR_InArrID1=  INBUF0;
    fp0->imgW_OutArrID1= OUTBUF0;
    fp0->constData= constData;
    fp0->count= count;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;


}

/*
int C6ACCEL_IMG_yc_demux_8(C6accel_Handle hC6accel, int n, const unsigned char *restrict yc,unsigned char *restrict y, unsigned char *restrict cr, unsigned char *restrict cb)
Arguments
* hC6accel C6accel Handle
* n :Number of luma pixels
* yc: Interleaved luma/chroma
* y : Luma plane (8-bit)
* cr: Cr chroma plane (8-bit)
* cb: Cb chroma plane (8-bit)

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description:

This function reads the interleaved byte-oriented pixel data and then writes it to the appropriate result array.

*/
int C6accel_IMG_yc_demux_le8_8
(   C6accel_Handle hC6accel,
    int               n,              /* Number of luma points           */
    unsigned char     *restrict yc,   /* Packed YUYV image               */
    unsigned char     *restrict y,    /* Unpacked luma frame             */
    unsigned char     *restrict cr,   /* Unpacked red chroma frame       */
    unsigned char     *restrict cb    /* Unpacked green chroma frame     */
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int status;
    /* Define pointer to function parameter structure */
    IMG_yc_demux_le8_c_Params   *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IMG_yc_demux_le8_c_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 1;
    outBufDesc.numBufs = 3;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(yc,0,2*n*sizeof(char));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(y,0,n*sizeof(char));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(cr,1,n/2*sizeof(char));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(cb,2,n/2*sizeof(char));


    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= IMG_YC_DEMUX_LE8_C_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IMG_yc_demux_le8_c_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->n = n;
    fp0->yc_InArrID1=  INBUF0;
    fp0->y_OutArrID1=  OUTBUF0;
    fp0->cr_OutArrID2= OUTBUF1;
    fp0->cb_OutArrID3= OUTBUF2;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int C6ACCEL_IMG_yc_demux_16(C6accel_Handle hC6accel, int n, const unsigned short * yc, short *restrict y, short *restrict cr, short *restrict cb)
Arguments
* hC6accel C6accel Handle
* n :Number of luma pixels
* yc: Interleaved luma/chroma
* y : Luma plane (16-bit)
* cr: Cr chroma plane (16-bit)
* cb: Cb chroma plane (16-bit)

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description:

This function reads the byte-oriented pixel data, zero-extends it, and then writes it to the appropriate result
array. Both the luma and chroma values are expected to be unsigned

*/
int C6accel_IMG_yc_demux_le16_16
(   C6accel_Handle hC6accel,
    int               n,              /* Number of luma points           */
    unsigned short    *restrict yc,   /* Packed YUYV image               */
    short             *restrict y,    /* Unpacked luma frame             */
    short             *restrict cr,   /* Unpacked red chroma frame       */
    short             *restrict cb    /* Unpacked green chroma frame     */
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int status;
    /* Define pointer to function parameter structure */
    IMG_yc_demux_le16_16_Params   *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IMG_yc_demux_le16_16_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 1;
    outBufDesc.numBufs = 3;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(yc,0,2*n*sizeof(short));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(y,0,n*sizeof(short));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(cr,1,n/2*sizeof(short));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(cb,2,n/2*sizeof(short));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= IMG_YC_DEMUX_LE16_16_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IMG_yc_demux_le16_16_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->n = n;
    fp0->yc_InArrID1=  INBUF0;
    fp0->y_OutArrID1=  OUTBUF0;
    fp0->cr_OutArrID2= OUTBUF1;
    fp0->cb_OutArrID3= OUTBUF2;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;
}

/*
int C6ACCEL_IMG_yuv420pl_to_rgb565(C6accel_Handle hC6accel,const short *coeff,int Height, Int Width, unsigned char *pbuf_y, unsigned char *pbuf_cb, unsigned char *pbuf_cr,unsigned short *pbuf_rgb)

Arguments
* hC6accel C6accel Handle
* coeff[5] Matrix coefficients
* y_data Luminence data (Y�). Must be double-word aligned.
* cb_data Blue color-diff (B�-Y�). Must be word aligned.
* cr_data Red color-diff (R�-Y�). Must be word aligned.
* rgb_data RGB 5:6:5 packed pixel out. Must be double-word aligned.
* num_pixels Number of luma pixels to process. Must be multiple of 8.

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description This kernel performs Y�CbCr to RGB conversion.

*/
int C6accel_IMG_yuv420pl_to_rgb565
(   C6accel_Handle hC6accel,
    const short       *coeff,         /* YUV to RGB transformation matrix*/
    short             height,
    short             width,
    unsigned char     *restrict y,    /* Unpacked luma frame             */
    unsigned char     *restrict cb,   /* Unpacked blue chroma frame      */
    unsigned char     *restrict cr,   /* Unpacked red chroma frame       */
    unsigned short    *restrict rgb   /* output RGB565 frame             */
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int status;
    /* Define pointer to function parameter structure */
    IMG_yuv420pl_to_rgb565_Params   *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IMG_yuv420pl_to_rgb565_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 4;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(coeff,0,5*sizeof(short));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(y,1,height*width*sizeof(char));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(cb,2,(height*width/4)*sizeof(char));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(cr,3,(height*width/4)*sizeof(char));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(rgb,0,height*width*sizeof(short));

     /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= IMG_YUV420PL_RGB565_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IMG_yuv420pl_to_rgb565_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->coeff_InArrID1    = INBUF0;
    fp0->Height            = height;
    fp0->Width             = width;
    fp0->y_data_InArrID2   =  INBUF1;
    fp0->cb_data_InArrID3  = INBUF2;
    fp0->cr_data_InArrID4  = INBUF3;
    fp0->rgb_data_OutArrID1= OUTBUF0;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int C6ACCEL_IMG_ycbcr422pl_to_rgb565(C6accel_Handle hC6accel,const short * restrict coeff, const unsigned char * restrict y_data, const unsigned char * restrict cb_data, const unsigned char * restrict cr_data, unsigned short * restrict rgb_data, unsigned num_pixels)

Arguments
* hC6accel C6accel Handle
* coeff[5] Matrix coefficients
* y_data Luminence data (Y�). Must be double-word aligned.
* cb_data Blue color-diff (B�-Y�). Must be word aligned.
* cr_data Red color-diff (R�-Y�). Must be word aligned.
* rgb_data RGB 5:6:5 packed pixel out. Must be double-word aligned.
* num_pixels Number of luma pixels to process. Must be multiple of 8.

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description: This kernel performs Y�CbCr to RGB conversion.

*/
int C6accel_IMG_ycbcr422pl_to_rgb565
(   C6accel_Handle hC6accel,
    const short       *coeff,         /* YUV to RGB transformation matrix*/
    unsigned char     *restrict y,    /* Unpacked luma frame             */
    unsigned char     *restrict cb,   /* Unpacked blue chroma frame      */
    unsigned char     *restrict cr,   /* Unpacked red chroma frame       */
    unsigned short    *restrict rgb,  /* output RGB565 frame             */
    unsigned int      num_pixels
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int status;
    /* Define pointer to function parameter structure */
    IMG_ycbcr422pl_to_rgb565_Params   *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IMG_ycbcr422pl_to_rgb565_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 4;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(coeff,0,5*sizeof(short));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(y,1,num_pixels*sizeof(char));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(cb,2,(num_pixels/2)*sizeof(char));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(cr,3,(num_pixels/2)*sizeof(char));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(rgb,0,num_pixels*sizeof(short));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= IMG_YCBCR422P_RGB565_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IMG_ycbcr422pl_to_rgb565_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->coeff_InArrID1    = INBUF0;
    fp0->y_data_InArrID2   =  INBUF1;
    fp0->cb_data_InArrID3  = INBUF2;
    fp0->cr_data_InArrID4  = INBUF3;
    fp0->rgb_data_OutArrID1= OUTBUF0;
    fp0->num_pixels        =num_pixels;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int C6ACCEL_IMG_yuv420pl16_to_rgb565(C6accel_Handle hC6accel, const short *coeff,int Height, Int Width, short *pbuf_y, short *pbuf_cb, short *pbuf_cr,unsigned short *pbuf_rgb)
Arguments
* hC6accel C6accel Handle
* coeff[5] Matrix coefficients
* y_data Luminence data (Y�). Must be double-word aligned.
* cb_data Blue color-diff (B�-Y�). Must be word aligned.
* cr_data Red color-diff (R�-Y�). Must be word aligned.
* rgb_data RGB 5:6:5 packed pixel out. Must be double-word aligned.
* num_pixels Number of luma pixels to process. Must be multiple of 8.

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description
This kernel performs Y'CbCr to RGB conversion and is a 16 bit implementation of the C6ACCEL_IMG_YUV420P_to_RGB565

*/
int C6accel_IMG_ycbcr422pl16_to_rgb565
(   C6accel_Handle hC6accel,
    const short       *coeff,         /* YUV to RGB transformation matrix*/
    short             *restrict y,    /* Unpacked luma frame             */
    short             *restrict cb,   /* Unpacked blue chroma frame      */
    short             *restrict cr,   /* Unpacked red chroma frame       */
    unsigned short    *restrict rgb,  /* output RGB565 frame             */
    unsigned int      num_pixels
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int status;
    /* Define pointer to function parameter structure */
    IMG_ycbcr422pl16_to_rgb565_Params   *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IMG_ycbcr422pl16_to_rgb565_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 4;
    outBufDesc.numBufs = 1;


    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(coeff,0,5*sizeof(short));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(y,1,num_pixels*sizeof(short));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(cb,2,(num_pixels/2)*sizeof(short));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(cr,3,(num_pixels/2)*sizeof(short));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(rgb,0,num_pixels*sizeof(short));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= IMG_YCBCR422PL16_RGB565_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IMG_ycbcr422pl16_to_rgb565_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->coeff_InArrID1    = INBUF0;
    fp0->y_data_InArrID2   =  INBUF1;
    fp0->cb_data_InArrID3  = INBUF2;
    fp0->cr_data_InArrID4  = INBUF3;
    fp0->rgb_data_OutArrID1= OUTBUF0;
    fp0->num_pixels        = num_pixels;

     /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
C6ACCEL_IMG_ycbcr_422pl16_to_rgb565(C6accel_Handle hC6accel,const short * restrict coeff, const unsigned char * restrict y_data, const unsigned char * restrict cb_data, const unsigned char * restrict cr_data, unsigned short * restrict rgb_data, unsigned num_pixels)
Arguments
* hC6accel C6accel Handle
* coeff[5] Matrix coefficients
* y_data Luminence data (Y�). Must be double-word aligned.
* cb_data Blue color-diff (B�-Y�). Must be word aligned.
* cr_data Red color-diff (R�-Y�). Must be word aligned.
* rgb_data RGB 5:6:5 packed pixel out. Must be double-word aligned.
* num_pixels Number of luma pixels to process. Must be multiple of 8.

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description
This kernel performs Y'CbCr to RGB conversion and is a 16 bit implementation of the C6ACCEL_IMG_YUV422P_to_RGB565

*/
int C6accel_IMG_yuv420pl16_to_rgb565
(   C6accel_Handle hC6accel,
    const short       *coeff,         /* YUV to RGB transformation matrix*/
    short             height,
    short             width,
    short            *restrict y,     /* Unpacked luma frame             */
    short            *restrict cb,    /* Unpacked blue chroma frame      */
    short            *restrict cr,    /* Unpacked red chroma frame       */
    unsigned short   *restrict rgb   /* output RGB565 frame             */
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int status;
    /* Define pointer to function parameter structure */
    IMG_yuv420pl16_to_rgb565_Params   *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IMG_yuv420pl16_to_rgb565_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 4;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(coeff,0,5*sizeof(short));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(y,1,height*width*sizeof(short));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(cb,2,(height*width/4)*sizeof(short));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(cr,3,(height*width/4)*sizeof(short));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(rgb,0,height*width*sizeof(short));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= IMG_YUV420PL16_RGB565_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IMG_yuv420pl16_to_rgb565_Params *)((XDAS_Int8*)CInArgs+CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->coeff_InArrID1    = INBUF0;
    fp0->Height            = height;
    fp0->Width             = width;
    fp0->y_data_InArrID2   = INBUF1;
    fp0->cb_data_InArrID3  = INBUF2;
    fp0->cr_data_InArrID4  = INBUF3;
    fp0->rgb_data_OutArrID1= OUTBUF0;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int C6accel_MATH_addsp_i( C6accel_Handle hC6accel,  float *ptr_x, float *ptr_y, float *ptr_r,int    npoints)
* hC6accel C6accel Handle
* x,y :Input vectors
* r: output vector
* npoints : Number of points in input vector

Description: Single precision addition
*/
int C6accel_MATH_addsp_i( C6accel_Handle hC6accel,
                           float *ptr_x,
                           float *ptr_y,
                           float *ptr_r,
                           int    npoints
)
{
    XDM1_BufDesc               inBufDesc;
    XDM1_BufDesc               outBufDesc;
    XDAS_Int32                 InArg_Buf_size;
    IC6Accel_InArgs            *CInArgs;
    UNIVERSAL_OutArgs          uniOutArgs;
    Int                        status;
    /* Define pointer to function parameter structure */
    addsp_i_Params             *fp0;
    XDAS_Int8                  *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(addsp_i_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);


    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 2;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,npoints*sizeof(float));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_y,1,npoints*sizeof(float));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(float));

    /*  Initialize the extended InArgs structure */
    CInArgs->Num_fxns = 1;
    CInArgs->size = InArg_Buf_size;

    /*Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID     = MATH_ADDSP_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (addsp_i_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->InArrID1 =  INBUF0;
    fp0->InArrID2 = INBUF1;
    fp0->OutArrID1= OUTBUF0;
    fp0->n        = npoints;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int  C6accel_MATH_subsp_i( C6accel_Handle hC6accel,  float *ptr_x, float *ptr_y, float *ptr_r,int    npoints)
* hC6accel C6accel Handle
* x,y :Input vectors
* r: output vector
* npoints : Number of points in input vector

Description: Single precision substraction
*/
int C6accel_MATH_subsp_i(C6accel_Handle hC6accel,
                          float *ptr_x,
                          float *ptr_y,
                          float *ptr_r,
                          int npoints
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int status;
    /* Define pointer to function parameter structure */
    subsp_i_Params      *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(subsp_i_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 2;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,npoints*sizeof(float));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_y,1,npoints*sizeof(float));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(float));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= MATH_SUBSP_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /*Initialize pointers to function parameters */
    fp0 = (subsp_i_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->InArrID1 = INBUF0;
    fp0->InArrID2 = INBUF1;
    fp0->OutArrID1= OUTBUF0;
    fp0->n        = npoints;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}


/*
int C6accel_MATH_mpysp_i( C6accel_Handle hC6accel,  float *ptr_x, float *ptr_y, float *ptr_r,int    npoints)
* hC6accel C6accel Handle
* x,y :Input vectors
* r: output vector
* npoints : Number of points in input vector

Description: Single precision multiplication
*/
int C6accel_MATH_mpysp_i( C6accel_Handle hC6accel,
                           float *ptr_x,
                           float *ptr_y,
                           float *ptr_r,
                           int    npoints
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs            *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int status;
     /* Define pointer to function parameter structure */
    mpysp_i_Params             *fp0;
    XDAS_Int8                  *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(mpysp_i_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 2;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,npoints*sizeof(float));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_y,1,npoints*sizeof(float));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(float));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID = MATH_MPYSP_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (mpysp_i_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->InArrID1  =  INBUF0;
    fp0->InArrID2  = INBUF1;
    fp0->OutArrID1 = OUTBUF0;
    fp0->n         = npoints;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int C6accel_MATH_divsp_i( C6accel_Handle hC6accel,  float *ptr_x, float *ptr_y, float *ptr_r,int    npoints)
* hC6accel C6accel Handle
* x,y :Input vectors
* r: output vector
* npoints : Number of points in input vector

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description: Single precision division
*/
int C6accel_MATH_divsp_i( C6accel_Handle hC6accel,
                           float *ptr_x,
                           float *ptr_y,
                           float *ptr_r,
                           int    npoints
)
{
    XDM1_BufDesc         inBufDesc;
    XDM1_BufDesc         outBufDesc;
    XDAS_Int32           InArg_Buf_size;
    IC6Accel_InArgs     *CInArgs;
    UNIVERSAL_OutArgs    uniOutArgs;
    Int                  status;
    /* Define pointer to function parameter structure */
    divsp_i_Params      *fp0;
    XDAS_Int8           *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(divsp_i_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 2;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,npoints*sizeof(float));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_y,1,npoints*sizeof(float));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(float));

     /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID    = MATH_DIVSP_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset= sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (divsp_i_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->InArrID1 =  INBUF0;
    fp0->InArrID2 = INBUF1;
    fp0->OutArrID1= OUTBUF0;
    fp0->n        = npoints;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int C6accel_MATH_recipsp_i( C6accel_Handle hC6accel,  float *ptr_x, float *ptr_r,int    npoints)
* hC6accel C6accel Handle
* x :Input vectors
* r: output vector
* npoints : Number of points in input vector

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description: Single precision reciprocal
*/
int C6accel_MATH_recipsp_i( C6accel_Handle hC6accel,
                           float *ptr_x,
                           float *ptr_r,
                           int npoints
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int status;
    /* Define pointer to function parameter structure */
    recipsp_i_Params      *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(recipsp_i_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,npoints*sizeof(float));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(float));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= MATH_RECIPSP_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (recipsp_i_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->x_InArrID1 = INBUF0;
    fp0->OutArrID1  = OUTBUF0;
    fp0->n          = npoints;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int C6accel_MATH_sqrtsp_i( C6accel_Handle hC6accel,  float *ptr_x, float *ptr_r,int    npoints)
* hC6accel C6accel Handle
* x :Input vectors
* r: output vector
* npoints : Number of points in input vector

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description: Single precision squareroot
*/
int C6accel_MATH_sqrtsp_i( C6accel_Handle hC6accel,
                            float *ptr_x,
                            float *ptr_r,
                            int    npoints
)
{
    XDM1_BufDesc          inBufDesc;
    XDM1_BufDesc          outBufDesc;
    XDAS_Int32            InArg_Buf_size;
    IC6Accel_InArgs      *CInArgs;
    UNIVERSAL_OutArgs     uniOutArgs;
    Int                   status;
    /* Define pointer to function parameter structure */
    sqrtsp_i_Params      *fp0;
    XDAS_Int8            *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(sqrtsp_i_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,npoints*sizeof(float));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(float));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call*/
    CInArgs->fxn[0].FxnID    = MATH_SQRTSP_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset= sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (sqrtsp_i_Params *)((XDAS_Int8*)CInArgs+CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->x_InArrID1 = INBUF0;
    fp0->OutArrID1  = OUTBUF0;
    fp0->n          = npoints;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int C6accel_MATH_intsp_i( C6accel_Handle hC6accel,  float *ptr_x, float *ptr_r,int    npoints)
* hC6accel C6accel Handle
* x :Input vectors
* r: output vector
* npoints : Number of points in input vector

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description: int to single precision float values conversion
*/
int C6accel_MATH_intsp_i( C6accel_Handle hC6accel,
                           int   *ptr_x,
                           float *ptr_r,
                           int    npoints
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs            *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int                         status;
    /* Define pointer to function parameter structure */
    intsp_i_Params             *fp0;
    XDAS_Int8                  *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(intsp_i_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);


    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,npoints*sizeof(float));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(float));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= MATH_INTSP_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (intsp_i_Params *)((XDAS_Int8*)CInArgs+CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->x_InArrID1 =  INBUF0;
    fp0->OutArrID1  = OUTBUF0;
    fp0->n          = npoints;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int C6accel_MATH_uintsp_i( C6accel_Handle hC6accel,  float *ptr_x, float *ptr_r,int npoints)
* hC6accel C6accel Handle
* x :Input vectors
* r: output vector
* npoints : Number of points in input vector

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description: unsigned int to single precision float values conversion
*/
int C6accel_MATH_uintsp_i( C6accel_Handle hC6accel,
                            unsigned int *ptr_x,
                            float        *ptr_r,
                            int           npoints
)
{
    XDM1_BufDesc          inBufDesc;
    XDM1_BufDesc          outBufDesc;
    XDAS_Int32            InArg_Buf_size;
    IC6Accel_InArgs      *CInArgs;
    UNIVERSAL_OutArgs     uniOutArgs;
    Int                   status;
    /* Define pointer to function parameter structure */
    uintsp_i_Params      *fp0;
    XDAS_Int8            *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(uintsp_i_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,npoints*sizeof(float));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(float));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= MATH_UINTSP_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (uintsp_i_Params *)((XDAS_Int8*)CInArgs+CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->x_InArrID1 =  INBUF0;
    fp0->OutArrID1  = OUTBUF0;
    fp0->n          = npoints;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int C6accel_MATH_spint_i( C6accel_Handle hC6accel,  float *ptr_x, float *ptr_r,int npoints)
* hC6accel C6accel Handle
* x :Input vectors
* r: output vector
* npoints : Number of points in input vector

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description: single precision float value to integer conversion
*/
int C6accel_MATH_spint_i( C6accel_Handle hC6accel,
                           float *ptr_x,
                           int   *ptr_r,
                           int    npoints
)
{
    XDM1_BufDesc         inBufDesc;
    XDM1_BufDesc         outBufDesc;
    XDAS_Int32           InArg_Buf_size;
    IC6Accel_InArgs     *CInArgs;
    UNIVERSAL_OutArgs    uniOutArgs;
    Int                  status;
    /* Define pointer to function parameter structure */
    spint_i_Params      *fp0;
    XDAS_Int8           *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(spint_i_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,npoints*sizeof(float));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(int));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= MATH_SPINT_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (spint_i_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->x_InArrID1 = INBUF0;
    fp0->OutArrID1  = OUTBUF0;
    fp0->n          = npoints;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}


/*
int C6accel_MATH_spuint_i( C6accel_Handle hC6accel,  float *ptr_x, float *ptr_r,int npoints)
* hC6accel C6accel Handle
* x :Input vectors
* r: output vector
* npoints : Number of points in input vector

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description: single precision float value to unsigned integer conversion
*/
int C6accel_MATH_spuint_i(C6accel_Handle hC6accel,
                           float          *ptr_x,
                           unsigned int   *ptr_r,
                           int             npoints
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int                         status;
    /* Define pointer to function parameter structure */
    spuint_i_Params            *fp0;
    XDAS_Int8                  *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(spuint_i_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,npoints*sizeof(float));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(unsigned int));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    //Set function Id and parameter pointers for first function call
    CInArgs->fxn[0].FxnID     = MATH_SPUINT_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (spuint_i_Params *)((XDAS_Int8*)CInArgs+CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->x_InArrID1 = INBUF0;
    fp0->OutArrID1  = OUTBUF0;
    fp0->n          = npoints;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
void C6Accel_FtoIQN(C6accel_Handle hC6accel,float *x, unsigned qfmt,int *r, int n)
Arguments
* hC6accel C6accel Handle
* x : float vector
* qfmt : Q value for IQ format of the output
* n : number of inputs
* r : Fixed point equivalent of the inputs in float format

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description This function converts a floating-point constant or variable to the equivalent IQ value.
*/
int  C6accel_MATH_FTOIQN(C6accel_Handle hC6accel,
                         float       *ptr_x,
                         unsigned int  qfmt,
                         int       *ptr_r,
                         int           npoints
)
{
    XDM1_BufDesc        inBufDesc;
    XDM1_BufDesc        outBufDesc;
    XDAS_Int32          InArg_Buf_size;
    IC6Accel_InArgs    *CInArgs;
    UNIVERSAL_OutArgs   uniOutArgs;
    Int                 status;
    /* Define pointer to function parameter structure */
    FtoIQN_Params      *fp0;
    XDAS_Int8          *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(FtoIQN_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,npoints*sizeof(float));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(int));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID     = MATH_FTOIQN_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (FtoIQN_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->InArrID1  = INBUF0;
    fp0->OutArrID1 = OUTBUF0;
    fp0->qfmt      = qfmt;
    fp0->n         = npoints;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int C6Accel_IQN( C6accel_Handle hC6accel,int *x,int *r, unsigned int qfmt, int n)
Arguments
* hC6accel C6accel Handle
* x : Signed integer value to be converted
* qfmt : Q value for IQ format of the output
* n : numberof inputs
* r : Fixed point equivalent of the inputs in IQ format

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description This kernel converts an integer (short, char) to an equivalent IQ value


*/
int  C6accel_MATH_IQN(  C6accel_Handle hC6accel,
                         int          *ptr_x,
                         unsigned int  qfmt,
                         int       *ptr_r,
                         int           npoints
)
{

    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int                         status;
    /* Define pointer to function parameter structure */
    IQN_Params                 *fp0;
    XDAS_Int8                  *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IQN_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);


    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,npoints*sizeof(int));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(int));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= MATH_IQN_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IQN_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->InArrID1  = INBUF0;
    fp0->OutArrID1 = OUTBUF0;
    fp0->qfmt      = qfmt;
    fp0->n         = npoints;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}


/*
int C6Accel_IQXtoIQY(C6accel_Handle hC6accel, int *x,unsigned int qfmt1,unsigned int qfmt2,int *r, int n)
Arguments
* hC6accel C6accel Handle
* x  : Value in input IQ format to be converted
* qfmt1  : intput IQ format
* qfmt2  : output Q format
* n  : number of inputs
* r  : output in IQ format

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description This function converts Iq number in X format to IQ number in Y format.

*/
int  C6accel_MATH_IQXtoIQY(C6accel_Handle hC6accel,
                            int       *ptr_x,
                            unsigned int  qfmt1,
                            unsigned int  qfmt2,
                            int       *ptr_r,
                            int           npoints
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int                          status;
    /* Define pointer to function parameter structure */
    IQXtoIQY_Params             *fp0;
    XDAS_Int8                   *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size =  sizeof(Fxn_struct)+
                      sizeof(IQXtoIQY_Params)+
                      sizeof(CInArgs->size)+
                      sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,npoints*sizeof(int));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(int));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID    = MATH_IQXTOIQY_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset= sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IQXtoIQY_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->InArrID1  =  INBUF0;
    fp0->OutArrID1 = OUTBUF0;
    fp0->x         = qfmt1;
    fp0->y         = qfmt2;
    fp0->n         = npoints;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}


/*
void C6Accel_IQNtoF( C6accel_Handle hC6accel,int *x,unsigned int qfmt,float *r, int n)
Arguments
* hC6accel C6accel Handle
* x  : intput vector IQ format
* qfmt  : Q value for IQ format of the output
* r  : floating point equivalent of the inputs in IQ format
* n  : number of inputs.

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description This function converts a IQ number to equivalent floating-point value in IEEE 754 format

*/
int  C6accel_MATH_IQNtoF(C6accel_Handle hC6accel,
                          int       *ptr_x,
                          unsigned int  qfmt,
                          float        *ptr_r,
                          int           npoints
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int                         status;
    /* Define pointer to function parameter structure */
    IQNtoF_Params               *fp0;
    XDAS_Int8                   *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IQNtoF_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,npoints*sizeof(int));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(int));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= MATH_IQNTOF_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IQNtoF_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->InArrID1  =  INBUF0;
    fp0->OutArrID1 = OUTBUF0;
    fp0->qfmt      = qfmt;
    fp0->n         = npoints;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int C6Accel_IQNint( C6accel_Handle hC6accel,int *x,unsigned int qfmt,int *r, int n)
Arguments
* hC6accel C6accel Handle
* x  : intput vector in IQ format
* qfmt  : Q value for IQ format of the output
* n  : numberof inputs
* r  : integer part of the inputs in IQ format.

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description This This function returns the integer portion of IQ number

*/
int  C6accel_MATH_IQNint(C6accel_Handle hC6accel,
                          int       *ptr_x,
                          unsigned int  qfmt,
                          int          *ptr_r,
                          int           npoints
)
{
    XDM1_BufDesc        inBufDesc;
    XDM1_BufDesc        outBufDesc;
    XDAS_Int32          InArg_Buf_size;
    IC6Accel_InArgs    *CInArgs;
    UNIVERSAL_OutArgs   uniOutArgs;
    int                 status;
    /* Define pointer to function parameter structure */
    IQNint_Params      *fp0;
    XDAS_Int8          *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size =  sizeof(Fxn_struct)+
                      sizeof(IQNint_Params)+
                      sizeof(CInArgs->size)+
                      sizeof(CInArgs->Num_fxns);


    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,npoints*sizeof(int));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(int));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= MATH_IQNINT_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IQNint_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);
    /* Fill the fields of the parameter strructure */
    fp0->InArrID1  =  INBUF0;
    fp0->OutArrID1 = OUTBUF0;
    fp0->qfmt      = qfmt;
    fp0->n         = npoints;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}


/* int C6Accel_IQNfrac(C6accel_Handle hC6accel, int *x,unsigned int qfmt, int *r, int n)
Arguments
* hC6accel C6accel Handle
* x  : intput vector in IQ format
* qfmt  : Q value for IQ format of the output
* r  : fractional part of the inputs in IQ format.
* n  : numberof inputs

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description This function returns the integer portion of IQ number
*/
int  C6accel_MATH_IQNfrac(C6accel_Handle hC6accel,
                           int       *ptr_x,
                           unsigned int  qfmt,
                           int       *ptr_r,
                           int           npoints
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs            *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int                         status;
    /* Define pointer to function parameter structure */
    IQNfrac_Params             *fp0;
    XDAS_Int8                  *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size =  sizeof(Fxn_struct)+
                      sizeof(IQNfrac_Params)+
                      sizeof(CInArgs->size)+
                      sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,npoints*sizeof(int));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(int));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= MATH_IQNFRAC_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IQNfrac_Params *)((XDAS_Int8*)CInArgs+CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->InArrID1  =  INBUF0;
    fp0->OutArrID1 = OUTBUF0;
    fp0->qfmt      = qfmt;
    fp0->n         = npoints;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int  C6accel_MATH_IQNabs( C6accel_Handle hC6accel,int *ptr_x, int *ptr_r, unsigned int  qfmt,int npoints)
* hC6accel C6accel Handle
* x  : intput vector in IQ format
* qfmt  : Q value for IQ format of the output
* r  : integer part of the inputs in IQ format.
* n  : numberof inputs

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description This function returns the absolute value of IQ number

*/
int  C6accel_MATH_IQNabs( C6accel_Handle hC6accel,
                            int       *ptr_x,
                            int       *ptr_r,
                            unsigned int  qfmt,
                            int           npoints
)
{
    XDM1_BufDesc        inBufDesc;
    XDM1_BufDesc        outBufDesc;
    XDAS_Int32          InArg_Buf_size;
    IC6Accel_InArgs    *CInArgs;
    UNIVERSAL_OutArgs   uniOutArgs;
    Int                 status;
    /* Define pointer to function parameter structure */
    IQNabs_Params      *fp0;
    XDAS_Int8          *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IQNabs_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);


    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,npoints*sizeof(int));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(int));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size=InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= MATH_IQNABS_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IQNabs_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->InArrID1  =  INBUF0;
    fp0->OutArrID1 = OUTBUF0;
    fp0->qfmt      = qfmt;
    fp0->n         = npoints;


    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int  C6accel_MATH_IQNlog( C6accel_Handle hC6accel,int *ptr_x, int *ptr_r, unsigned int  qfmt,int npoints)
* hC6accel C6accel Handle
* x  : intput vector in IQ format
* qfmt  : Q value for IQ format of the output
* r  : integer part of the inputs in IQ format.
* n  : numberof inputs

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description This function returns the log value of IQ number

*/
int  C6accel_MATH_IQNlog(C6accel_Handle hC6accel,
                          int       *ptr_x,
                          int       *ptr_r,
                          unsigned int  qfmt,
                          int           npoints
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs            *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int                         status;
    /* Define pointer to function parameter structure */
    IQNlog_Params              *fp0;
    XDAS_Int8                  *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size =  sizeof(Fxn_struct)+
                      sizeof(IQNlog_Params)+
                      sizeof(CInArgs->size)+
                      sizeof(CInArgs->Num_fxns);


    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size  = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,npoints*sizeof(int));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(int));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call*/
    CInArgs->fxn[0].FxnID= MATH_IQNLOG_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IQNlog_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    fp0->InArrID1  =  INBUF0;
    fp0->OutArrID1 = OUTBUF0;
    fp0->qfmt      = qfmt;
    fp0->n         = npoints;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        /*Free the InArgs structure*/
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int  C6accel_MATH_IQNsqrt( C6accel_Handle hC6accel,int *ptr_x, int *ptr_r, unsigned int  qfmt,int npoints)
* hC6accel C6accel Handle
* x  : intput vector in IQ format
* qfmt  : Q value for IQ format of the output
* r  : integer part of the inputs in IQ format.
* n  : numberof inputs

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description This function returns the squareeoot of IQ number

*/
int  C6accel_MATH_IQNsqrt(C6accel_Handle hC6accel,
                           int      *ptr_x,
                           int      *ptr_r,
                           unsigned int qfmt,
                           int          npoints
)
{

    XDM1_BufDesc         inBufDesc;
    XDM1_BufDesc         outBufDesc;
    XDAS_Int32           InArg_Buf_size;
    IC6Accel_InArgs     *CInArgs;
    UNIVERSAL_OutArgs    uniOutArgs;
    Int                  status;
    /* Define pointer to function parameter structure */
    IQNsqrt_Params      *fp0;
    XDAS_Int8           *pAlloc;


    ACQUIRE_CODEC_ENGINE;


    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IQNsqrt_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,npoints*sizeof(int));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(int));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= MATH_IQNSQRT_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IQNsqrt_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->InArrID1  =  INBUF0;
    fp0->OutArrID1 = OUTBUF0;
    fp0->qfmt      = qfmt;
    fp0->n         = npoints;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int  C6accel_MATH_IQNisqrt( C6accel_Handle hC6accel,int *ptr_x, int *ptr_r, unsigned int  qfmt,int npoints)
* hC6accel C6accel Handle
* x  : intput vector in IQ format
* qfmt  : Q value for IQ format of the output
* r  : integer part of the inputs in IQ format.
* n  : numberof inputs

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description This function returns the inverse squareroot value of IQ number

*/
int  C6accel_MATH_IQNisqrt(C6accel_Handle hC6accel,
                            int       *ptr_x,
                            int       *ptr_r,
                            unsigned int  qfmt,
                            int           npoints
)
{
    XDM1_BufDesc          inBufDesc;
    XDM1_BufDesc          outBufDesc;
    XDAS_Int32            InArg_Buf_size;
    IC6Accel_InArgs      *CInArgs;
    UNIVERSAL_OutArgs     uniOutArgs;
    Int                   status;
    /* Define pointer to function parameter structure */
    IQNisqrt_Params      *fp0;
    XDAS_Int8            *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IQNisqrt_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);


    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size    = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,npoints*sizeof(int));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(int));

    //Initialize the extended InArgs structure //
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    //Set function Id and parameter pointers for first function call
    CInArgs->fxn[0].FxnID     = MATH_IQNISQRT_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IQNisqrt_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->InArrID1  =  INBUF0;
    fp0->OutArrID1 = OUTBUF0;
    fp0->qfmt      = qfmt;
    fp0->n         = npoints;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int  C6accel_MATH_IQNexp( C6accel_Handle hC6accel,int *ptr_x, int *ptr_r, unsigned int  qfmt,int npoints)
* hC6accel C6accel Handle
* x  : intput vector in IQ format
* qfmt  : Q value for IQ format of the output
* r  : integer part of the inputs in IQ format.
* n  : numberof inputs

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description This function returns the exponential value of IQ number

*/
int  C6accel_MATH_IQNexp(C6accel_Handle hC6accel,
                          int *ptr_x,
                          int *ptr_r,
                          unsigned int  qfmt,
                          int npoints
)
{
    XDM1_BufDesc        inBufDesc;
    XDM1_BufDesc        outBufDesc;
    XDAS_Int32          InArg_Buf_size;
    IC6Accel_InArgs    *CInArgs;
    UNIVERSAL_OutArgs   uniOutArgs;
    Int                 status;
    /* Define pointer to function parameter structure */
    IQNexp_Params      *fp0;
    XDAS_Int8          *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size =  sizeof(Fxn_struct)+
                      sizeof(IQNexp_Params)+
                      sizeof(CInArgs->size)+
                      sizeof(CInArgs->Num_fxns);


    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,npoints*sizeof(int));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(int));

    //Initialize the extended InArgs structure //
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    //Set function Id and parameter pointers for first function call
    CInArgs->fxn[0].FxnID    = MATH_IQNEXP_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset= sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    //Initialize pointers to function parameters
    fp0 = (IQNexp_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    fp0->InArrID1  =  INBUF0;
    fp0->OutArrID1 = OUTBUF0;
    fp0->qfmt      = qfmt;
    fp0->n         = npoints;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        //Free the InArgs structure
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int  C6accel_MATH_IQNisqrt( C6accel_Handle hC6accel,int *ptr_x, int *ptr_r, unsigned int  qfmt,int npoints)
* hC6accel C6accel Handle
* x  : intput vector in IQ format
* qfmt  : Q value for IQ format of the output
* r  : integer part of the inputs in IQ format.
* n  : numberof inputs

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description This function returns the nth power of an IQ number

*/
int  C6accel_MATH_IQNpow(C6accel_Handle hC6accel,
                          int       *ptr_x,
                          int       *ptr_y,
                          int       *ptr_r,
                          unsigned int  qfmt,
                          int           npoints
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs            *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int                         status;
    /* Define pointer to function parameter structure */
    IQNpow_Params              *fp0;
    XDAS_Int8                  *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IQNpow_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);


    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 2;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,npoints*sizeof(int));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_y,1,npoints*sizeof(int));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(int));

    //Initialize the extended InArgs structure //
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    //Set function Id and parameter pointers for first function call
    CInArgs->fxn[0].FxnID     = MATH_IQNPOW_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    //Initialize pointers to function parameters
    fp0 = (IQNpow_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->A_InArrID1 =  INBUF0;
    fp0->B_InArrID2 =  INBUF1;
    fp0->OutArrID1  = OUTBUF0;
    fp0->qfmt       = qfmt;
    fp0->n          = npoints;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int  C6accel_MATH_IQNmag( C6accel_Handle hC6accel,int *ptr_x,int *ptr_y, int *ptr_r, unsigned int  qfmt,int npoints)
* hC6accel C6accel Handle
* x  : intput real vector in IQ format
* y : input imaginary vector in IQ format
* qfmt  : Q value for IQ format of the output
* r  : integer part of the inputs in IQ format.
* n  : numberof inputs

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description This function returns magnitude of an complex number IQ number

*/
int  C6accel_MATH_IQNmag(C6accel_Handle hC6accel,
                          int       *ptr_x,
                          int       *ptr_y,
                          int       *ptr_r,
                          unsigned int  qfmt,
                          int           npoints
)
{

    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int                         status;
    /* Define pointer to function parameter structure */
    IQNmag_Params               *fp0;
    XDAS_Int8                   *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size =  sizeof(Fxn_struct)+
                      sizeof(IQNmag_Params)+
                      sizeof(CInArgs->size)+
                      sizeof(CInArgs->Num_fxns);


    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 2;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,npoints*sizeof(int));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_y,1,npoints*sizeof(int));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(int));

    //Initialize the extended InArgs structure //
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    //Set function Id and parameter pointers for first function call
    CInArgs->fxn[0].FxnID     = MATH_IQNMAG_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    //Initialize pointers to function parameters
    fp0 = (IQNmag_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->InArrID1  =  INBUF0;
    fp0->InArrID2  =  INBUF1;
    fp0->OutArrID1 = OUTBUF0;
    fp0->qfmt      = qfmt;
    fp0->n         = npoints;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        //Free the InArgs structure
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int  C6accel_MATH_IQNmpy( C6accel_Handle hC6accel,int *ptr_x,int *ptr_y, int *ptr_r, unsigned int  qfmt,int npoints)
* hC6accel C6accel Handle
* x,y   : intput vectors in IQ format
* qfmt  : Q value for IQ format of the output
* r     : output vector in IQ format.
* n     : numberof inputs

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description This function multiplies two IQ number and returns in the IQ format

*/
int  C6accel_MATH_IQNmpy(C6accel_Handle hC6accel,
                          int       *ptr_x,
                          int       *ptr_y,
                          int       *ptr_r,
                          unsigned int  qfmt,
                          int           npoints
)
{

    XDM1_BufDesc       inBufDesc;
    XDM1_BufDesc       outBufDesc;
    XDAS_Int32         InArg_Buf_size;
    IC6Accel_InArgs    *CInArgs;
    UNIVERSAL_OutArgs   uniOutArgs;
    Int                 status;
    /* Define pointer to function parameter structure */
    IQNmpy_Params      *fp0;
    XDAS_Int8          *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IQNmpy_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);


    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);

    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 2;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,npoints*sizeof(int));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_y,1,npoints*sizeof(int));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(int));

    //Initialize the extended InArgs structure //
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    //Set function Id and parameter pointers for first function call
    CInArgs->fxn[0].FxnID = MATH_IQNMPY_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    //Initialize pointers to function parameters
    fp0 = (IQNmpy_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->InArrID1 = INBUF0;
    fp0->InArrID2 = INBUF1;
    fp0->OutArrID1= OUTBUF0;
    fp0->qfmt     = qfmt;
    fp0->n        = npoints;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int  C6accel_MATH_IQNmpyI32int( C6accel_Handle hC6accel,int *ptr_x,int *ptr_y, int *ptr_r, unsigned int  qfmt,int npoints)
* hC6accel C6accel Handle
* x,   : intput vectors in IQ format
* y    : input vector of 32 bit integer
* qfmt  : Q value for IQ format of the output
* r     : output vector in IQ format.
* n     : numberof inputs

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description This function multiplies a IQ number with 32 bit integer and return integer part of output IQ format

*/
int  C6accel_MATH_IQNmpyI32int(C6accel_Handle hC6accel,
                                int       *ptr_x,
                                int          *ptr_y,
                                int          *ptr_r,
                                unsigned int  qfmt,
                                int           npoints
)
{

    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int                         status;
    /* Define pointer to function parameter structure */
    IQNmpyI32int_Params         *fp0;
    XDAS_Int8                   *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size =  sizeof(Fxn_struct)+
                      sizeof(IQNmpyI32int_Params)+
                      sizeof(CInArgs->size)+
                      sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 2;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,npoints*sizeof(int));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_y,1,npoints*sizeof(int));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(int));

    //Initialize the extended InArgs structure //
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    //Set function Id and parameter pointers for first function call
    CInArgs->fxn[0].FxnID    = MATH_IQNMPYI32INT_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset= sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    //Initialize pointers to function parameters
    fp0 = (IQNmpyI32int_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->InArrID1  =  INBUF0;
    fp0->InArrID2  =  INBUF1;
    fp0->OutArrID1 = OUTBUF0;
    fp0->qfmt      = qfmt;
    fp0->n         = npoints;

   /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int  C6accel_MATH_IQNmpyI32int( C6accel_Handle hC6accel,int *ptr_x,int *ptr_y, int *ptr_r, unsigned int  qfmt,int npoints)
* hC6accel C6accel Handle
* x,   : intput vectors in IQ format
* y    : input vector of 32 bit integer
* qfmt  : Q value for IQ format of the output
* r     : output vector contains fractional part of multiplication IQ format.
* n     : numberof inputs

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description This function multiplies a IQ number with 32 bit integer and return fractional part of output IQ format

*/
int  C6accel_MATH_IQNmpyI32frac(C6accel_Handle hC6accel,
                                 int       *ptr_x,
                                 int          *ptr_y,
                                 int       *ptr_r,
                                 unsigned int  qfmt,
                                 int           npoints
)
{
    XDM1_BufDesc               inBufDesc;
    XDM1_BufDesc               outBufDesc;
    XDAS_Int32                 InArg_Buf_size;
    IC6Accel_InArgs           *CInArgs;
    UNIVERSAL_OutArgs          uniOutArgs;
    Int                        status;
    /* Define pointer to function parameter structure */
    IQNmpyI32frac_Params      *fp0;
    XDAS_Int8                 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size =  sizeof(Fxn_struct)+
                      sizeof(IQNmpyI32frac_Params)+
                      sizeof(CInArgs->size)+
                      sizeof(CInArgs->Num_fxns);


    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 2;
    outBufDesc.numBufs = 1;

     /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,npoints*sizeof(int));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_y,1,npoints*sizeof(int));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(int));

    //Initialize the extended InArgs structure //
    CInArgs->Num_fxns= 1;
    CInArgs->size    = InArg_Buf_size;

    //Set function Id and parameter pointers for first function call
    CInArgs->fxn[0].FxnID     = MATH_IQNMPYI32FRAC_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    //Initialize pointers to function parameters
    fp0 = (IQNmpyI32frac_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->InArrID1  =  INBUF0;
    fp0->InArrID2  =  INBUF1;
    fp0->OutArrID1 = OUTBUF0;
    fp0->qfmt      = qfmt;
    fp0->n         = npoints;

   /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int  C6accel_MATH_IQNmpyIQx( C6accel_Handle hC6accel,int *ptr_x,int *ptr_y, int *ptr_r, unsigned int  qfmt,int npoints)
* hC6accel C6accel Handle
* x,   : intput vectors in IQ format
* y    : intput vectors in a different IQ format
* qfmt  : Q value for IQ format of the output
* r     : output vector in IQ format.
* n     : numberof inputs

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description This function multiplies a IQ number with another of different format and return the output in IQ format

*/
int  C6accel_MATH_IQNmpyIQx(    C6accel_Handle hC6accel,
                                 int       *ptr_x,
                                 unsigned int  qfmt1,
                                 int       *ptr_y,
                                 unsigned int  qfmt2,
                                 int       *ptr_r,
                                 unsigned int  qfmt,
                                 int           npoints
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs            *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int                         status;
    /* Define pointer to function parameter structure */
    IQNmpyIQX_Params           *fp0;
    XDAS_Int8                  *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IQNmpyIQX_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 2;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,npoints*sizeof(int));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_y,1,npoints*sizeof(int));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(int));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns= 1;
    CInArgs->size    = InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID     = MATH_IQNMPYIQX_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IQNmpyIQX_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->InArrID1  =  INBUF0;
    fp0->qfmt1     = qfmt1;
    fp0->InArrID2  =  INBUF1;
    fp0->qfmt2     = qfmt2;
    fp0->OutArrID1 = OUTBUF0;
    fp0->qfmt      = qfmt;
    fp0->n         = npoints;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int  C6accel_MATH_IQNrmpy( C6accel_Handle hC6accel,int *ptr_x,int *ptr_y, int *ptr_r, unsigned int  qfmt,int npoints)
* hC6accel C6accel Handle
* x,   : intput vectors in IQ format
* y    : intput vectors in IQ format
* qfmt  : Q value for IQ format of the output
* r     : output vector in IQ format.
* n     : numberof inputs

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description : Multiplication of two IQ numbers with rounding

*/
int  C6accel_MATH_IQNrmpy(C6accel_Handle hC6accel,
                           int      *ptr_x,
                           int      *ptr_y,
                           int      *ptr_r,
                           unsigned int qfmt,
                           int          npoints
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs            *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int                         status;
    /* Define pointer to function parameter structure */
    IQNrmpy_Params             *fp0;
    XDAS_Int8                  *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IQNrmpy_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 2;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,npoints*sizeof(int));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_y,1,npoints*sizeof(int));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(int));

     /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID     = MATH_IQNRMPY_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IQNrmpy_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->InArrID1  =  INBUF0;
    fp0->InArrID2  =  INBUF1;
    fp0->OutArrID1 = OUTBUF0;
    fp0->qfmt      = qfmt;
    fp0->n         = npoints;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int  C6accel_MATH_IQNrsmpy( C6accel_Handle hC6accel,int *ptr_x,int *ptr_y, int *ptr_r, unsigned int  qfmt,int npoints)
* hC6accel C6accel Handle
* x,   : intput vectors in IQ format
* y    : intput vectors in IQ format
* qfmt  : Q value for IQ format of the output
* r     : output vector in IQ format.
* n     : numberof inputs

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description : Multiplication of two IQ numbers with rounding and saturation

*/
int  C6accel_MATH_IQNrsmpy(C6accel_Handle hC6accel,
                            int      *ptr_x,
                            int      *ptr_y,
                            int      *ptr_r,
                            unsigned int qfmt,
                            int          npoints
)
{
    XDM1_BufDesc          inBufDesc;
    XDM1_BufDesc          outBufDesc;
    XDAS_Int32            InArg_Buf_size;
    IC6Accel_InArgs      *CInArgs;
    UNIVERSAL_OutArgs     uniOutArgs;
    int status;
    /* Define pointer to function parameter structure */
    IQNrsmpy_Params      *fp0;
    XDAS_Int8            *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size =  sizeof(Fxn_struct)+
                      sizeof(IQNrsmpy_Params)+
                      sizeof(CInArgs->size)+
                      sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 2;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,npoints*sizeof(int));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_y,1,npoints*sizeof(int));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(int));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID     = MATH_IQNRSMPY_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IQNrsmpy_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->InArrID1  =  INBUF0;
    fp0->InArrID2  =  INBUF1;
    fp0->OutArrID1 = OUTBUF0;
    fp0->qfmt      = qfmt;
    fp0->n         = npoints;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int C6accel_MATH_IQNdiv( C6accel_Handle hC6accel,int *ptr_x,int *ptr_y, int *ptr_r, unsigned int  qfmt,int npoints)
* hC6accel C6accel Handle
* x,y   : intput vectors in IQ format
* qfmt  : Q value for IQ format of the output
* r     : output vector in IQ format.
* n     : numberof inputs

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description : Division of two IQ numbers

*/
int  C6accel_MATH_IQNdiv(C6accel_Handle hC6accel,
                          int       *ptr_x,
                          int       *ptr_y,
                          int       *ptr_r,
                          unsigned int  qfmt,
                          int           npoints
)
{
    XDM1_BufDesc        inBufDesc;
    XDM1_BufDesc        outBufDesc;
    XDAS_Int32          InArg_Buf_size;
    IC6Accel_InArgs    *CInArgs;
    UNIVERSAL_OutArgs   uniOutArgs;
    Int                 status;
    /* Define pointer to function parameter structure */
    IQNdiv_Params      *fp0;
    XDAS_Int8          *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size =  sizeof(Fxn_struct)+
                      sizeof(IQNdiv_Params)+
                      sizeof(CInArgs->size)+
                      sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 2;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,npoints*sizeof(int));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_y,1,npoints*sizeof(int));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(int));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID     = MATH_IQNDIV_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IQNdiv_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->num_InArrID1 = INBUF0;
    fp0->den_InArrID2 = INBUF1;
    fp0->OutArrID1    = OUTBUF0;
    fp0->qfmt         = qfmt;
    fp0->n            = npoints;


    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int C6accel_MATH_IQNsin( C6accel_Handle hC6accel,int *ptr_x, int *ptr_r, unsigned int  qfmt,int npoints)
* hC6accel C6accel Handle
* x   : intput vectors in IQ format
* qfmt  : Q value for IQ format of the output
* r     : output vector in IQ format.
* n     : numberof inputs

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description : Sine of angles in IQ formats

*/
int  C6accel_MATH_IQNsin(  C6accel_Handle hC6accel,
                            int       *ptr_x,
                            int       *ptr_r,
                            unsigned int  qfmt,
                            int           npoints
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int status;
    /* Define pointer to function parameter structure */
    IQNsin_Params      *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IQNsin_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,npoints*sizeof(int));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(int));

    /* Fill in input/output buffer descriptor parameters */
    inBufDesc.descs[0].buf     = (XDAS_Int8 *) ptr_x;
    inBufDesc.descs[0].bufSize = npoints*sizeof(int);
    outBufDesc.descs[0].buf    = (XDAS_Int8 *) ptr_r;
    outBufDesc.descs[0].bufSize= npoints*sizeof(int);


    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID     = MATH_IQNSIN_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IQNsin_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->InArrID1  =  INBUF0;
    fp0->OutArrID1 = OUTBUF0;
    fp0->qfmt      = qfmt;
    fp0->n         = npoints;


    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int C6accel_MATH_IQNasin( C6accel_Handle hC6accel,int *ptr_x,int *ptr_r, unsigned int  qfmt,int npoints)
* hC6accel C6accel Handle
* x,   : intput vectors in IQ format
* qfmt  : Q value for IQ format of the output
* r     : output vector in IQ format.
* n     : numberof inputs

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description : Arc Sine of input in IQ formats

*/
int  C6accel_MATH_IQNasin( C6accel_Handle hC6accel,
                            int       *ptr_x,
                            int       *ptr_r,
                            unsigned int  qfmt,
                            int           npoints
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int                         status;
    /* Define pointer to function parameter structure */
    IQNasin_Params              *fp0;
    XDAS_Int8                   *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size =  sizeof(Fxn_struct)+
                      sizeof(IQNasin_Params)+
                      sizeof(CInArgs->size)+
                      sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,npoints*sizeof(int));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(int));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= MATH_IQNASIN_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IQNasin_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->InArrID1  =  INBUF0;
    fp0->OutArrID1 = OUTBUF0;
    fp0->qfmt      = qfmt;
    fp0->n         = npoints;

   /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int C6accel_MATH_IQNcos( C6accel_Handle hC6accel,int *ptr_x, int *ptr_r, unsigned int  qfmt,int npoints)
* hC6accel C6accel Handle
* x   : intput vectors in IQ format
* qfmt  : Q value for IQ format of the output
* r     : output vector in IQ format.
* n     : numberof inputs

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description : Cosine of angles in radians( IQ formats)

*/
int  C6accel_MATH_IQNcos(  C6accel_Handle hC6accel,
                            int *ptr_x,
                            int *ptr_r,
                            unsigned int  qfmt,
                            int npoints
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int status;
    /* Define pointer to function parameter structure */
    IQNcos_Params      *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IQNcos_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,npoints*sizeof(int));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(int));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= MATH_IQNCOS_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /*Initialize pointers to function parameters */
    fp0 = (IQNcos_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->InArrID1  = INBUF0;
    fp0->OutArrID1 = OUTBUF0;
    fp0->qfmt      = qfmt;
    fp0->n         = npoints;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int C6accel_MATH_IQNacos( C6accel_Handle hC6accel,int *ptr_x,int *ptr_r, unsigned int  qfmt,int npoints)
* hC6accel C6accel Handle
* x,   : intput vectors in IQ format
* qfmt  : Q value for IQ format of the output
* r     : output vector in IQ format.
* n     : numberof inputs

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description : Arc cosine of input in IQ formats

*/
int  C6accel_MATH_IQNacos(C6accel_Handle hC6accel,
                           int       *ptr_x,
                           int       *ptr_r,
                           unsigned int  qfmt,
                           int           npoints
)
{

    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int                         status;
    /* Define pointer to function parameter structure */
    IQNacos_Params             *fp0;
    XDAS_Int8                  *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size =  sizeof(Fxn_struct)+
                      sizeof(IQNacos_Params)+
                      sizeof(CInArgs->size)+
                      sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,npoints*sizeof(int));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(int));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID     = MATH_IQNACOS_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IQNacos_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->InArrID1  =  INBUF0;
    fp0->OutArrID1 = OUTBUF0;
    fp0->qfmt      = qfmt;
    fp0->n         = npoints;

   /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int C6accel_MATH_IQNcosPU( C6accel_Handle hC6accel,int *ptr_x, int *ptr_r, unsigned int  qfmt,int npoints)
* hC6accel C6accel Handle
* x   : intput vectors in IQ format
* qfmt  : Q value for IQ format of the output
* r     : output vector in IQ format.
* n     : numberof inputs

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description : Cosine of angles in per unit radians IQ formats

*/
int  C6accel_MATH_IQNcosPU(C6accel_Handle hC6accel,
                            int       *ptr_x,
                            int       *ptr_r,
                            unsigned int  qfmt,
                            int           npoints
)
{

    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int                         status;
    /* Define pointer to function parameter structure */
    IQNcosPU_Params             *fp0;
    XDAS_Int8                   *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IQNcosPU_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);


    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,npoints*sizeof(int));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(int));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID     = MATH_IQNCOSPU_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IQNcosPU_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->InArrID1  = INBUF0;
    fp0->OutArrID1 = OUTBUF0;
    fp0->qfmt      = qfmt;
    fp0->n         = npoints;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        //Free the InArgs structure
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int C6accel_MATH_IQNsinPU( C6accel_Handle hC6accel,int *ptr_x, int *ptr_r, unsigned int  qfmt,int npoints)
* hC6accel C6accel Handle
* x   : intput vectors in IQ format
* qfmt  : Q value for IQ format of the output
* r     : output vector in IQ format.
* n     : numberof inputs

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description : Sine of angles in per unit radians IQ formats

*/
int  C6accel_MATH_IQNsinPU(C6accel_Handle hC6accel,
                            int       *ptr_x,
                            int       *ptr_r,
                            unsigned int  qfmt,
                            int           npoints
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int                         status;
    /* Define pointer to function parameter structure */
    IQNsinPU_Params             *fp0;
    XDAS_Int8                   *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size =  sizeof(Fxn_struct)+
                      sizeof(IQNsinPU_Params)+
                      sizeof(CInArgs->size)+
                      sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,npoints*sizeof(int));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(int));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID     = MATH_IQNSINPU_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IQNsinPU_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->InArrID1  =  INBUF0;
    fp0->OutArrID1 = OUTBUF0;
    fp0->qfmt      = qfmt;
    fp0->n         = npoints;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int C6accel_MATH_IQNatan2( C6accel_Handle hC6accel,int *ptr_x,int *ptr_y, int *ptr_r, unsigned int  qfmt,int npoints)
* hC6accel C6accel Handle
* x,y   : intput vectors in IQ format
* qfmt  : Q value for IQ format of the output
* r     : output vector in IQ format.
* n     : numberof inputs

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description : arc tan of (y/x) in IQ formats

*/
int  C6accel_MATH_IQNatan2(C6accel_Handle hC6accel,
                            int       *ptr_y,
                            int       *ptr_x,
                            int       *ptr_r,
                            unsigned int  qfmt,
                            int           npoints
)
{

    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int status;
    /* Define pointer to function parameter structure */
    IQNatan2_Params            *fp0;
    XDAS_Int8                  *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IQNatan2_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);


    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 2;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_y,0,npoints*sizeof(int));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,1,npoints*sizeof(int));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(int));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID     = MATH_IQNATAN2_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IQNatan2_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->y_InArrID1 =  INBUF0;
    fp0->x_InArrID2 =  INBUF1;
    fp0->OutArrID1  = OUTBUF0;
    fp0->qfmt       = qfmt;
    fp0->n          = npoints;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
int C6accel_MATH_IQNatan2PU( C6accel_Handle hC6accel,int *ptr_x,int *ptr_y, int *ptr_r, unsigned int  qfmt,int npoints)
* hC6accel C6accel Handle
* x,y   : intput vectors in IQ format
* qfmt  : Q value for IQ format of the output
* r     : output vector in IQ format.
* n     : numberof inputs

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description : arc tan of (y/x) in per unit radians

*/
int  C6accel_MATH_IQNatan2PU(         C6accel_Handle hC6accel,
                                       int *ptr_y,
                                       int *ptr_x,
                                       int *ptr_r,
                               unsigned int  qfmt,
                                        int npoints
)
{

    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int                         status;

    /* Define pointer to function parameter structure */
    IQNatan2PU_Params          *fp0;
    XDAS_Int8                  *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IQNatan2PU_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 2;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_y,0,npoints*sizeof(int));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,1,npoints*sizeof(int));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(int));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID     = MATH_IQNATAN2PU_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IQNatan2PU_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->y_InArrID1 =  INBUF0;
    fp0->x_InArrID2 =  INBUF1;
    fp0->OutArrID1  = OUTBUF0;
    fp0->qfmt       = qfmt;
    fp0->n          = npoints;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/***********************************************
* Untested Wrapper APIs
************************************************/

int C6accel_MATH_sqrtsp( C6accel_Handle hC6accel,
                         float *ptr_z,
                         float *ptr_r,
                         int    npoints
)
{
    XDM1_BufDesc          inBufDesc;
    XDM1_BufDesc          outBufDesc;
    XDAS_Int32            InArg_Buf_size;
    IC6Accel_InArgs      *CInArgs;
    UNIVERSAL_OutArgs     uniOutArgs;
    Int                   status;
    // Define pointer to function parameter structure
    sqrtsp_Params        *fp0;
    XDAS_Int8            *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    // Allocate the InArgs structure as it varies in size
    // (Needs to be changed everytime we make a API call)
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(sqrtsp_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    // Request contiguous heap memory allocation for the extended input structure
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    // Initialize .size fields for dummy input and output arguments
    uniOutArgs.size = sizeof(uniOutArgs);

    // Set up buffers to pass buffers in and out to alg
    inBufDesc.numBufs  = 1;
    outBufDesc.numBufs = 1;

    // Fill in input/output buffer descriptor parameters and manage ARM cache
    // See wrapper_c6accel_i.h for more details of operation
      CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_z,0,npoints*sizeof(float));
      CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(float));

    // Initialize the extended InArgs structure
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    // Set function Id and parameter pointers for first function call
    CInArgs->fxn[0].FxnID    = MATH_SQRT_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset= sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    // Initialize pointers to function parameters
    fp0 = (sqrtsp_Params *)((XDAS_Int8*)CInArgs+CInArgs->fxn[0].Param_ptr_offset);

    // Fill in the fields in the parameter structure
    fp0->z_InArrID1 = INBUF0;
    fp0->OutArrID1  = OUTBUF0;
    fp0->n          = npoints;

    // Call the actual algorithm
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        // Free the InArgs structure
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

int C6accel_MATH_sqrtdp( C6accel_Handle hC6accel,
                         double *ptr_z,
                         double *ptr_r,
                         int    npoints
)
{
    XDM1_BufDesc          inBufDesc;
    XDM1_BufDesc          outBufDesc;
    XDAS_Int32            InArg_Buf_size;
    IC6Accel_InArgs      *CInArgs;
    UNIVERSAL_OutArgs     uniOutArgs;
    Int                   status;
    // Define pointer to function parameter structure
    sqrtdp_Params        *fp0;
    XDAS_Int8            *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    // Allocate the InArgs structure as it varies in size
    // (Needs to be changed everytime we make a API call)
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(sqrtdp_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    // Request contiguous heap memory allocation for the extended input structure
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    // Initialize .size fields for dummy input and output arguments
    uniOutArgs.size = sizeof(uniOutArgs);

    // Set up buffers to pass buffers in and out to alg
    inBufDesc.numBufs  = 1;
    outBufDesc.numBufs = 1;

    // Fill in input/output buffer descriptor parameters and manage ARM cache
    // See wrapper_c6accel_i.h for more details of operation
      CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_z,0,npoints*sizeof(double));
      CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(double));

    // Initialize the extended InArgs structure
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    // Set function Id and parameter pointers for first function call
    CInArgs->fxn[0].FxnID    = MATH_SQRTDP_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset= sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    // Initialize pointers to function parameters
    fp0 = (sqrtdp_Params *)((XDAS_Int8*)CInArgs+CInArgs->fxn[0].Param_ptr_offset);

    // Fill in the fields in the parameter structure
    fp0->z_InArrID1 = INBUF0;
    fp0->OutArrID1  = OUTBUF0;
    fp0->n          = npoints;

    // Call the actual algorithm
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        // Free the InArgs structure
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

int C6accel_MATH_rsqrtsp( C6accel_Handle hC6accel,
                         float *ptr_z,
                         float *ptr_r,
                         int    npoints
)
{
    XDM1_BufDesc          inBufDesc;
    XDM1_BufDesc          outBufDesc;
    XDAS_Int32            InArg_Buf_size;
    IC6Accel_InArgs      *CInArgs;
    UNIVERSAL_OutArgs     uniOutArgs;
    Int                   status;
    // Define pointer to function parameter structure
    rsqrtsp_Params        *fp0;
    XDAS_Int8            *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    // Allocate the InArgs structure as it varies in size
    // (Needs to be changed everytime we make a API call)
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(rsqrtsp_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    // Request contiguous heap memory allocation for the extended input structure
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    // Initialize .size fields for dummy input and output arguments
    uniOutArgs.size = sizeof(uniOutArgs);

    // Set up buffers to pass buffers in and out to alg
    inBufDesc.numBufs  = 1;
    outBufDesc.numBufs = 1;

    // Fill in input/output buffer descriptor parameters and manage ARM cache
    // See wrapper_c6accel_i.h for more details of operation
      CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_z,0,npoints*sizeof(float));
      CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(float));

    // Initialize the extended InArgs structure
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    // Set function Id and parameter pointers for first function call
    CInArgs->fxn[0].FxnID    = MATH_RSQRTSP_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset= sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    // Initialize pointers to function parameters
    fp0 = (rsqrtsp_Params *)((XDAS_Int8*)CInArgs+CInArgs->fxn[0].Param_ptr_offset);

    // Fill in the fields in the parameter structure
    fp0->z_InArrID1 = INBUF0;
    fp0->OutArrID1  = OUTBUF0;
    fp0->n          = npoints;

    // Call the actual algorithm
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        // Free the InArgs structure
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

int C6accel_MATH_rsqrtdp( C6accel_Handle hC6accel,
                         double *ptr_z,
                         double *ptr_r,
                         int    npoints
)
{
    XDM1_BufDesc          inBufDesc;
    XDM1_BufDesc          outBufDesc;
    XDAS_Int32            InArg_Buf_size;
    IC6Accel_InArgs      *CInArgs;
    UNIVERSAL_OutArgs     uniOutArgs;
    Int                   status;
    // Define pointer to function parameter structure
    rsqrtdp_Params        *fp0;
    XDAS_Int8            *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    // Allocate the InArgs structure as it varies in size
    // (Needs to be changed everytime we make a API call)
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(rsqrtdp_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    // Request contiguous heap memory allocation for the extended input structure
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    // Initialize .size fields for dummy input and output arguments
    uniOutArgs.size = sizeof(uniOutArgs);

    // Set up buffers to pass buffers in and out to alg
    inBufDesc.numBufs  = 1;
    outBufDesc.numBufs = 1;

    // Fill in input/output buffer descriptor parameters and manage ARM cache
    // See wrapper_c6accel_i.h for more details of operation
      CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_z,0,npoints*sizeof(double));
      CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(double));

    // Initialize the extended InArgs structure
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    // Set function Id and parameter pointers for first function call
    CInArgs->fxn[0].FxnID    = MATH_RSQRTDP_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset= sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    // Initialize pointers to function parameters
    fp0 = (rsqrtdp_Params *)((XDAS_Int8*)CInArgs+CInArgs->fxn[0].Param_ptr_offset);

    // Fill in the fields in the parameter structure
    fp0->z_InArrID1 = INBUF0;
    fp0->OutArrID1  = OUTBUF0;
    fp0->n          = npoints;

    // Call the actual algorithm
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        // Free the InArgs structure
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

int C6accel_MATH_logsp( C6accel_Handle hC6accel,
                         float *ptr_z,
                         float *ptr_r,
                         int    npoints
)
{
    XDM1_BufDesc          inBufDesc;
    XDM1_BufDesc          outBufDesc;
    XDAS_Int32            InArg_Buf_size;
    IC6Accel_InArgs      *CInArgs;
    UNIVERSAL_OutArgs     uniOutArgs;
    Int                   status;
    // Define pointer to function parameter structure
    logsp_Params        *fp0;
    XDAS_Int8            *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    // Allocate the InArgs structure as it varies in size
    // (Needs to be changed everytime we make a API call)
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(logsp_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    // Request contiguous heap memory allocation for the extended input structure
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    // Initialize .size fields for dummy input and output arguments
    uniOutArgs.size = sizeof(uniOutArgs);

    // Set up buffers to pass buffers in and out to alg
    inBufDesc.numBufs  = 1;
    outBufDesc.numBufs = 1;

    // Fill in input/output buffer descriptor parameters and manage ARM cache
    // See wrapper_c6accel_i.h for more details of operation
      CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_z,0,npoints*sizeof(float));
      CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(float));

    // Initialize the extended InArgs structure
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    // Set function Id and parameter pointers for first function call
    CInArgs->fxn[0].FxnID    = MATH_LOGSP_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset= sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    // Initialize pointers to function parameters
    fp0 = (logsp_Params *)((XDAS_Int8*)CInArgs+CInArgs->fxn[0].Param_ptr_offset);

    // Fill in the fields in the parameter structure
    fp0->z_InArrID1 = INBUF0;
    fp0->OutArrID1  = OUTBUF0;
    fp0->n          = npoints;

    // Call the actual algorithm
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        // Free the InArgs structure
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

int C6accel_MATH_logdp( C6accel_Handle hC6accel,
                         double *ptr_z,
                         double *ptr_r,
                         int    npoints
)
{
    XDM1_BufDesc          inBufDesc;
    XDM1_BufDesc          outBufDesc;
    XDAS_Int32            InArg_Buf_size;
    IC6Accel_InArgs      *CInArgs;
    UNIVERSAL_OutArgs     uniOutArgs;
    Int                   status;
    // Define pointer to function parameter structure
    logdp_Params        *fp0;
    XDAS_Int8            *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    // Allocate the InArgs structure as it varies in size
    // (Needs to be changed everytime we make a API call)
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(logdp_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    // Request contiguous heap memory allocation for the extended input structure
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    // Initialize .size fields for dummy input and output arguments
    uniOutArgs.size = sizeof(uniOutArgs);

    // Set up buffers to pass buffers in and out to alg
    inBufDesc.numBufs  = 1;
    outBufDesc.numBufs = 1;

    // Fill in input/output buffer descriptor parameters and manage ARM cache
    // See wrapper_c6accel_i.h for more details of operation
      CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_z,0,npoints*sizeof(double));
      CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(double));

    // Initialize the extended InArgs structure
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    // Set function Id and parameter pointers for first function call
    CInArgs->fxn[0].FxnID    = MATH_LOGDP_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset= sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    // Initialize pointers to function parameters
    fp0 = (logdp_Params *)((XDAS_Int8*)CInArgs+CInArgs->fxn[0].Param_ptr_offset);

    // Fill in the fields in the parameter structure
    fp0->z_InArrID1 = INBUF0;
    fp0->OutArrID1  = OUTBUF0;
    fp0->n          = npoints;

    // Call the actual algorithm
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        // Free the InArgs structure
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

int C6accel_MATH_expsp( C6accel_Handle hC6accel,
                         float *ptr_z,
                         float *ptr_r,
                         int    npoints
)
{
    XDM1_BufDesc          inBufDesc;
    XDM1_BufDesc          outBufDesc;
    XDAS_Int32            InArg_Buf_size;
    IC6Accel_InArgs      *CInArgs;
    UNIVERSAL_OutArgs     uniOutArgs;
    Int                   status;
    // Define pointer to function parameter structure
    expsp_Params         *fp0;
    XDAS_Int8            *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    // Allocate the InArgs structure as it varies in size
    // (Needs to be changed everytime we make a API call)
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(expsp_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    // Request contiguous heap memory allocation for the extended input structure
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    // Initialize .size fields for dummy input and output arguments
    uniOutArgs.size = sizeof(uniOutArgs);

    // Set up buffers to pass buffers in and out to alg
    inBufDesc.numBufs  = 1;
    outBufDesc.numBufs = 1;

    // Fill in input/output buffer descriptor parameters and manage ARM cache
    // See wrapper_c6accel_i.h for more details of operation
      CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_z,0,npoints*sizeof(float));
      CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(float));

    // Initialize the extended InArgs structure
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    // Set function Id and parameter pointers for first function call
    CInArgs->fxn[0].FxnID    = MATH_EXPSP_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset= sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    // Initialize pointers to function parameters
    fp0 = (expsp_Params *)((XDAS_Int8*)CInArgs+CInArgs->fxn[0].Param_ptr_offset);

    // Fill in the fields in the parameter structure
    fp0->z_InArrID1 = INBUF0;
    fp0->OutArrID1  = OUTBUF0;
    fp0->n          = npoints;

    // Call the actual algorithm
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        // Free the InArgs structure
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

int C6accel_MATH_expdp( C6accel_Handle hC6accel,
                         double *ptr_z,
                         double *ptr_r,
                         int    npoints
)
{
    XDM1_BufDesc          inBufDesc;
    XDM1_BufDesc          outBufDesc;
    XDAS_Int32            InArg_Buf_size;
    IC6Accel_InArgs      *CInArgs;
    UNIVERSAL_OutArgs     uniOutArgs;
    Int                   status;
    // Define pointer to function parameter structure
    expdp_Params         *fp0;
    XDAS_Int8            *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    // Allocate the InArgs structure as it varies in size
    // (Needs to be changed everytime we make a API call)
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(expdp_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    // Request contiguous heap memory allocation for the extended input structure
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    // Initialize .size fields for dummy input and output arguments
    uniOutArgs.size = sizeof(uniOutArgs);

    // Set up buffers to pass buffers in and out to alg
    inBufDesc.numBufs  = 1;
    outBufDesc.numBufs = 1;

    // Fill in input/output buffer descriptor parameters and manage ARM cache
    // See wrapper_c6accel_i.h for more details of operation
      CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_z,0,npoints*sizeof(double));
      CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(double));

    // Initialize the extended InArgs structure
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    // Set function Id and parameter pointers for first function call
    CInArgs->fxn[0].FxnID    = MATH_EXPDP_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset= sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    // Initialize pointers to function parameters
    fp0 = (expdp_Params *)((XDAS_Int8*)CInArgs+CInArgs->fxn[0].Param_ptr_offset);

    // Fill in the fields in the parameter structure
    fp0->z_InArrID1 = INBUF0;
    fp0->OutArrID1  = OUTBUF0;
    fp0->n          = npoints;

    // Call the actual algorithm
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        // Free the InArgs structure
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

int C6accel_MATH_exp2sp( C6accel_Handle hC6accel,
                         float *ptr_z,
                         float *ptr_r,
                         int    npoints
)
{
    XDM1_BufDesc          inBufDesc;
    XDM1_BufDesc          outBufDesc;
    XDAS_Int32            InArg_Buf_size;
    IC6Accel_InArgs      *CInArgs;
    UNIVERSAL_OutArgs     uniOutArgs;
    Int                   status;
    // Define pointer to function parameter structure
    exp2sp_Params         *fp0;
    XDAS_Int8            *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    // Allocate the InArgs structure as it varies in size
    // (Needs to be changed everytime we make a API call)
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(exp2sp_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    // Request contiguous heap memory allocation for the extended input structure
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    // Initialize .size fields for dummy input and output arguments
    uniOutArgs.size = sizeof(uniOutArgs);

    // Set up buffers to pass buffers in and out to alg
    inBufDesc.numBufs  = 1;
    outBufDesc.numBufs = 1;

    // Fill in input/output buffer descriptor parameters and manage ARM cache
    // See wrapper_c6accel_i.h for more details of operation
      CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_z,0,npoints*sizeof(float));
      CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(float));

    // Initialize the extended InArgs structure
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    // Set function Id and parameter pointers for first function call
    CInArgs->fxn[0].FxnID    = MATH_EXP2SP_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset= sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    // Initialize pointers to function parameters
    fp0 = (exp2sp_Params *)((XDAS_Int8*)CInArgs+CInArgs->fxn[0].Param_ptr_offset);

    // Fill in the fields in the parameter structure
    fp0->z_InArrID1 = INBUF0;
    fp0->OutArrID1  = OUTBUF0;
    fp0->n          = npoints;

    // Call the actual algorithm
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        // Free the InArgs structure
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

int C6accel_MATH_exp2dp( C6accel_Handle hC6accel,
                         double *ptr_z,
                         double *ptr_r,
                         int    npoints
)
{
    XDM1_BufDesc          inBufDesc;
    XDM1_BufDesc          outBufDesc;
    XDAS_Int32            InArg_Buf_size;
    IC6Accel_InArgs      *CInArgs;
    UNIVERSAL_OutArgs     uniOutArgs;
    Int                   status;
    // Define pointer to function parameter structure
    exp2dp_Params         *fp0;
    XDAS_Int8            *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    // Allocate the InArgs structure as it varies in size
    // (Needs to be changed everytime we make a API call)
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(exp2dp_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    // Request contiguous heap memory allocation for the extended input structure
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    // Initialize .size fields for dummy input and output arguments
    uniOutArgs.size = sizeof(uniOutArgs);

    // Set up buffers to pass buffers in and out to alg
    inBufDesc.numBufs  = 1;
    outBufDesc.numBufs = 1;

    // Fill in input/output buffer descriptor parameters and manage ARM cache
    // See wrapper_c6accel_i.h for more details of operation
      CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_z,0,npoints*sizeof(double));
      CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(double));

    // Initialize the extended InArgs structure
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    // Set function Id and parameter pointers for first function call
    CInArgs->fxn[0].FxnID    = MATH_EXP2DP_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset= sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    // Initialize pointers to function parameters
    fp0 = (exp2dp_Params *)((XDAS_Int8*)CInArgs+CInArgs->fxn[0].Param_ptr_offset);

    // Fill in the fields in the parameter structure
    fp0->z_InArrID1 = INBUF0;
    fp0->OutArrID1  = OUTBUF0;
    fp0->n          = npoints;

    // Call the actual algorithm
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        // Free the InArgs structure
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

int C6accel_MATH_exp10sp( C6accel_Handle hC6accel,
                         float *ptr_z,
                         float *ptr_r,
                         int    npoints
)
{
    XDM1_BufDesc          inBufDesc;
    XDM1_BufDesc          outBufDesc;
    XDAS_Int32            InArg_Buf_size;
    IC6Accel_InArgs      *CInArgs;
    UNIVERSAL_OutArgs     uniOutArgs;
    Int                   status;
    // Define pointer to function parameter structure
    exp10sp_Params         *fp0;
    XDAS_Int8            *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    // Allocate the InArgs structure as it varies in size
    // (Needs to be changed everytime we make a API call)
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(exp10sp_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    // Request contiguous heap memory allocation for the extended input structure
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    // Initialize .size fields for dummy input and output arguments
    uniOutArgs.size = sizeof(uniOutArgs);

    // Set up buffers to pass buffers in and out to alg
    inBufDesc.numBufs  = 1;
    outBufDesc.numBufs = 1;

    // Fill in input/output buffer descriptor parameters and manage ARM cache
    // See wrapper_c6accel_i.h for more details of operation
      CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_z,0,npoints*sizeof(float));
      CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(float));

    // Initialize the extended InArgs structure
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    // Set function Id and parameter pointers for first function call
    CInArgs->fxn[0].FxnID    = MATH_EXP10SP_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset= sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    // Initialize pointers to function parameters
    fp0 = (exp10sp_Params *)((XDAS_Int8*)CInArgs+CInArgs->fxn[0].Param_ptr_offset);

    // Fill in the fields in the parameter structure
    fp0->z_InArrID1 = INBUF0;
    fp0->OutArrID1  = OUTBUF0;
    fp0->n          = npoints;

    // Call the actual algorithm
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        // Free the InArgs structure
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

int C6accel_MATH_exp10dp( C6accel_Handle hC6accel,
                         double *ptr_z,
                         double *ptr_r,
                         int    npoints
)
{
    XDM1_BufDesc          inBufDesc;
    XDM1_BufDesc          outBufDesc;
    XDAS_Int32            InArg_Buf_size;
    IC6Accel_InArgs      *CInArgs;
    UNIVERSAL_OutArgs     uniOutArgs;
    Int                   status;
    // Define pointer to function parameter structure
    exp10dp_Params         *fp0;
    XDAS_Int8            *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    // Allocate the InArgs structure as it varies in size
    // (Needs to be changed everytime we make a API call)
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(exp10dp_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    // Request contiguous heap memory allocation for the extended input structure
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    // Initialize .size fields for dummy input and output arguments
    uniOutArgs.size = sizeof(uniOutArgs);

    // Set up buffers to pass buffers in and out to alg
    inBufDesc.numBufs  = 1;
    outBufDesc.numBufs = 1;

    // Fill in input/output buffer descriptor parameters and manage ARM cache
    // See wrapper_c6accel_i.h for more details of operation
      CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_z,0,npoints*sizeof(double));
      CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(double));

    // Initialize the extended InArgs structure
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    // Set function Id and parameter pointers for first function call
    CInArgs->fxn[0].FxnID    = MATH_EXP10DP_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset= sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    // Initialize pointers to function parameters
    fp0 = (exp10dp_Params *)((XDAS_Int8*)CInArgs+CInArgs->fxn[0].Param_ptr_offset);

    // Fill in the fields in the parameter structure
    fp0->z_InArrID1 = INBUF0;
    fp0->OutArrID1  = OUTBUF0;
    fp0->n          = npoints;

    // Call the actual algorithm
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        // Free the InArgs structure
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

int C6accel_MATH_sinsp( C6accel_Handle hC6accel,
                         float *ptr_z,
                         float *ptr_r,
                         int    npoints
)
{
    XDM1_BufDesc          inBufDesc;
    XDM1_BufDesc          outBufDesc;
    XDAS_Int32            InArg_Buf_size;
    IC6Accel_InArgs      *CInArgs;
    UNIVERSAL_OutArgs     uniOutArgs;
    Int                   status;
    // Define pointer to function parameter structure
    sinsp_Params         *fp0;
    XDAS_Int8            *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    // Allocate the InArgs structure as it varies in size
    // (Needs to be changed everytime we make a API call)
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(sinsp_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    // Request contiguous heap memory allocation for the extended input structure
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    // Initialize .size fields for dummy input and output arguments
    uniOutArgs.size = sizeof(uniOutArgs);

    // Set up buffers to pass buffers in and out to alg
    inBufDesc.numBufs  = 1;
    outBufDesc.numBufs = 1;

    // Fill in input/output buffer descriptor parameters and manage ARM cache
    // See wrapper_c6accel_i.h for more details of operation
      CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_z,0,npoints*sizeof(float));
      CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(float));

    // Initialize the extended InArgs structure
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    // Set function Id and parameter pointers for first function call
    CInArgs->fxn[0].FxnID    = MATH_SINSP_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset= sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    // Initialize pointers to function parameters
    fp0 = (sinsp_Params *)((XDAS_Int8*)CInArgs+CInArgs->fxn[0].Param_ptr_offset);

    // Fill in the fields in the parameter structure
    fp0->x_InArrID1 = INBUF0;
    fp0->OutArrID1  = OUTBUF0;
    fp0->n          = npoints;

    // Call the actual algorithm
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        // Free the InArgs structure
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

int C6accel_MATH_sindp( C6accel_Handle hC6accel,
                         double *ptr_z,
             double *ptr_r,
                         int    npoints
)
{
    XDM1_BufDesc          inBufDesc;
    XDM1_BufDesc          outBufDesc;
    XDAS_Int32            InArg_Buf_size;
    IC6Accel_InArgs      *CInArgs;
    UNIVERSAL_OutArgs     uniOutArgs;
    Int                   status;
    // Define pointer to function parameter structure
    sindp_Params         *fp0;
    XDAS_Int8            *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    // Allocate the InArgs structure as it varies in size
    // (Needs to be changed everytime we make a API call)
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(sindp_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    // Request contiguous heap memory allocation for the extended input structure
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    // Initialize .size fields for dummy input and output arguments
    uniOutArgs.size = sizeof(uniOutArgs);

    // Set up buffers to pass buffers in and out to alg
    inBufDesc.numBufs  = 1;
    outBufDesc.numBufs = 1;

    // Fill in input/output buffer descriptor parameters and manage ARM cache
    // See wrapper_c6accel_i.h for more details of operation
      CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_z,0,npoints*sizeof(double));
      CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(double));

    // Initialize the extended InArgs structure
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    // Set function Id and parameter pointers for first function call
    CInArgs->fxn[0].FxnID    = MATH_SINDP_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset= sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    // Initialize pointers to function parameters
    fp0 = (sindp_Params *)((XDAS_Int8*)CInArgs+CInArgs->fxn[0].Param_ptr_offset);

    // Fill in the fields in the parameter structure
    fp0->z_InArrID1 = INBUF0;
    fp0->OutArrID1  = OUTBUF0;
    fp0->n          = npoints;

    // Call the actual algorithm
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        // Free the InArgs structure
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

int C6accel_MATH_cossp( C6accel_Handle hC6accel,
                         float *ptr_z,
                         float *ptr_r,
                         int    npoints
)
{
    XDM1_BufDesc          inBufDesc;
    XDM1_BufDesc          outBufDesc;
    XDAS_Int32            InArg_Buf_size;
    IC6Accel_InArgs      *CInArgs;
    UNIVERSAL_OutArgs     uniOutArgs;
    Int                   status;
    // Define pointer to function parameter structure
    cossp_Params         *fp0;
    XDAS_Int8            *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    // Allocate the InArgs structure as it varies in size
    // (Needs to be changed everytime we make a API call)
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(cossp_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    // Request contiguous heap memory allocation for the extended input structure
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    // Initialize .size fields for dummy input and output arguments
    uniOutArgs.size = sizeof(uniOutArgs);

    // Set up buffers to pass buffers in and out to alg
    inBufDesc.numBufs  = 1;
    outBufDesc.numBufs = 1;

    // Fill in input/output buffer descriptor parameters and manage ARM cache
    // See wrapper_c6accel_i.h for more details of operation
      CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_z,0,npoints*sizeof(float));
      CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(float));

    // Initialize the extended InArgs structure
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    // Set function Id and parameter pointers for first function call
    CInArgs->fxn[0].FxnID    = MATH_COSSP_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset= sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    // Initialize pointers to function parameters
    fp0 = (cossp_Params *)((XDAS_Int8*)CInArgs+CInArgs->fxn[0].Param_ptr_offset);

    // Fill in the fields in the parameter structure
    fp0->z_InArrID1 = INBUF0;
    fp0->OutArrID1  = OUTBUF0;
    fp0->n          = npoints;

    // Call the actual algorithm
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    // Free the InArgs structure
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

int C6accel_MATH_cosdp( C6accel_Handle hC6accel,
                         double *ptr_z,
                         double *ptr_r,
                         int    npoints
)
{
    XDM1_BufDesc          inBufDesc;
    XDM1_BufDesc          outBufDesc;
    XDAS_Int32            InArg_Buf_size;
    IC6Accel_InArgs      *CInArgs;
    UNIVERSAL_OutArgs     uniOutArgs;
    Int                   status;
    // Define pointer to function parameter structure
    cosdp_Params         *fp0;
    XDAS_Int8            *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    // Allocate the InArgs structure as it varies in size
    // (Needs to be changed everytime we make a API call)
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(cosdp_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    // Request contiguous heap memory allocation for the extended input structure
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    // Initialize .size fields for dummy input and output arguments
    uniOutArgs.size = sizeof(uniOutArgs);

    // Set up buffers to pass buffers in and out to alg
    inBufDesc.numBufs  = 1;
    outBufDesc.numBufs = 1;

    // Fill in input/output buffer descriptor parameters and manage ARM cache
    // See wrapper_c6accel_i.h for more details of operation
      CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_z,0,npoints*sizeof(double));
      CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(double));

    // Initialize the extended InArgs structure
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    // Set function Id and parameter pointers for first function call
    CInArgs->fxn[0].FxnID    = MATH_COSDP_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset= sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    // Initialize pointers to function parameters
    fp0 = (cosdp_Params *)((XDAS_Int8*)CInArgs+CInArgs->fxn[0].Param_ptr_offset);

    // Fill in the fields in the parameter structure
    fp0->z_InArrID1 = INBUF0;
    fp0->OutArrID1  = OUTBUF0;
    fp0->n          = npoints;

    // Call the actual algorithm
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        // Free the InArgs structure
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

int C6accel_MATH_atansp( C6accel_Handle hC6accel,
                         float *ptr_z,
                         float *ptr_r,
                         int    npoints
)
{
    XDM1_BufDesc          inBufDesc;
    XDM1_BufDesc          outBufDesc;
    XDAS_Int32            InArg_Buf_size;
    IC6Accel_InArgs      *CInArgs;
    UNIVERSAL_OutArgs     uniOutArgs;
    Int                   status;
    // Define pointer to function parameter structure
    atansp_Params         *fp0;
    XDAS_Int8            *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    // Allocate the InArgs structure as it varies in size
    // (Needs to be changed everytime we make a API call)
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(atansp_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    // Request contiguous heap memory allocation for the extended input structure
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    // Initialize .size fields for dummy input and output arguments
    uniOutArgs.size = sizeof(uniOutArgs);

    // Set up buffers to pass buffers in and out to alg
    inBufDesc.numBufs  = 1;
    outBufDesc.numBufs = 1;

    // Fill in input/output buffer descriptor parameters and manage ARM cache
    // See wrapper_c6accel_i.h for more details of operation
      CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_z,0,npoints*sizeof(float));
      CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(float));

    // Initialize the extended InArgs structure
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    // Set function Id and parameter pointers for first function call
    CInArgs->fxn[0].FxnID    = MATH_ATANSP_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset= sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    // Initialize pointers to function parameters
    fp0 = (atansp_Params *)((XDAS_Int8*)CInArgs+CInArgs->fxn[0].Param_ptr_offset);

    // Fill in the fields in the parameter structure
    fp0->z_InArrID1 = INBUF0;
    fp0->OutArrID1  = OUTBUF0;
    fp0->n          = npoints;

    // Call the actual algorithm
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        // Free the InArgs structure
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

int C6accel_MATH_atandp( C6accel_Handle hC6accel,
                         double *ptr_z,
                         double *ptr_r,
                         int    npoints
)
{
    XDM1_BufDesc          inBufDesc;
    XDM1_BufDesc          outBufDesc;
    XDAS_Int32            InArg_Buf_size;
    IC6Accel_InArgs      *CInArgs;
    UNIVERSAL_OutArgs     uniOutArgs;
    Int                   status;
    // Define pointer to function parameter structure
    atandp_Params         *fp0;
    XDAS_Int8            *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    // Allocate the InArgs structure as it varies in size
    // (Needs to be changed everytime we make a API call)
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(atandp_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    // Request contiguous heap memory allocation for the extended input structure
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    // Initialize .size fields for dummy input and output arguments
    uniOutArgs.size = sizeof(uniOutArgs);

    // Set up buffers to pass buffers in and out to alg
    inBufDesc.numBufs  = 1;
    outBufDesc.numBufs = 1;

    // Fill in input/output buffer descriptor parameters and manage ARM cache
    // See wrapper_c6accel_i.h for more details of operation
      CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_z,0,npoints*sizeof(double));
      CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(double));

    // Initialize the extended InArgs structure
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    // Set function Id and parameter pointers for first function call
    CInArgs->fxn[0].FxnID    = MATH_ATANDP_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset= sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    // Initialize pointers to function parameters
    fp0 = (atandp_Params *)((XDAS_Int8*)CInArgs+CInArgs->fxn[0].Param_ptr_offset);

    // Fill in the fields in the parameter structure
    fp0->z_InArrID1 = INBUF0;
    fp0->OutArrID1  = OUTBUF0;
    fp0->n          = npoints;

    // Call the actual algorithm
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        // Free the InArgs structure
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

int C6accel_MATH_recipsp( C6accel_Handle hC6accel,
                         float *ptr_z,
             float *ptr_r,
                         int    npoints
)
{
    XDM1_BufDesc          inBufDesc;
    XDM1_BufDesc          outBufDesc;
    XDAS_Int32            InArg_Buf_size;
    IC6Accel_InArgs      *CInArgs;
    UNIVERSAL_OutArgs     uniOutArgs;
    Int                   status;
    // Define pointer to function parameter structure
    recipsp_Params       *fp0;
    XDAS_Int8            *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    // Allocate the InArgs structure as it varies in size
    // (Needs to be changed everytime we make a API call)
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(recipsp_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    // Request contiguous heap memory allocation for the extended input structure
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    // Initialize .size fields for dummy input and output arguments
    uniOutArgs.size = sizeof(uniOutArgs);

    // Set up buffers to pass buffers in and out to alg
    inBufDesc.numBufs  = 1;
    outBufDesc.numBufs = 1;

    // Fill in input/output buffer descriptor parameters and manage ARM cache
    // See wrapper_c6accel_i.h for more details of operation
      CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_z,0,npoints*sizeof(float));
      CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(float));

    // Initialize the extended InArgs structure
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    // Set function Id and parameter pointers for first function call
    CInArgs->fxn[0].FxnID    = MATH_RECIPF_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset= sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    // Initialize pointers to function parameters
    fp0 = (recipsp_Params *)((XDAS_Int8*)CInArgs+CInArgs->fxn[0].Param_ptr_offset);

    // Fill in the fields in the parameter structure
    fp0->z_InArrID1 = INBUF0;
    fp0->OutArrID1  = OUTBUF0;
    fp0->n          = npoints;

    // Call the actual algorithm
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        // Free the InArgs structure
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

int C6accel_MATH_recipdp( C6accel_Handle hC6accel,
                         double *ptr_z,
             double *ptr_r,
                         int    npoints
)
{
    XDM1_BufDesc          inBufDesc;
    XDM1_BufDesc          outBufDesc;
    XDAS_Int32            InArg_Buf_size;
    IC6Accel_InArgs      *CInArgs;
    UNIVERSAL_OutArgs     uniOutArgs;
    Int                   status;
    // Define pointer to function parameter structure
    recipdp_Params       *fp0;
    XDAS_Int8            *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    // Allocate the InArgs structure as it varies in size
    // (Needs to be changed everytime we make a API call)
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(recipdp_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    // Request contiguous heap memory allocation for the extended input structure
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    // Initialize .size fields for dummy input and output arguments
    uniOutArgs.size = sizeof(uniOutArgs);

    // Set up buffers to pass buffers in and out to alg
    inBufDesc.numBufs  = 1;
    outBufDesc.numBufs = 1;

    // Fill in input/output buffer descriptor parameters and manage ARM cache
    // See wrapper_c6accel_i.h for more details of operation
      CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_z,0,npoints*sizeof(double));
      CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(double));

    // Initialize the extended InArgs structure
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    // Set function Id and parameter pointers for first function call
    CInArgs->fxn[0].FxnID    = MATH_RECIPDP_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset= sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    // Initialize pointers to function parameters
    fp0 = (recipdp_Params *)((XDAS_Int8*)CInArgs+CInArgs->fxn[0].Param_ptr_offset);

    // Fill in the fields in the parameter structure
    fp0->z_InArrID1 = INBUF0;
    fp0->OutArrID1  = OUTBUF0;
    fp0->n          = npoints;

    // Call the actual algorithm
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        // Free the InArgs structure
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

Int  C6accel_MATH_atan2sp(C6accel_Handle hC6accel,
                            float       *ptr_y,
                            float       *ptr_x,
                            float       *ptr_r,
                            int        npoints
)
{

    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int status;
    /* Define pointer to function parameter structure */
    atan2sp_Params              *fp0;
    XDAS_Int8                   *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(atan2sp_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);


    pAlloc=(XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 2;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters */
     CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_y,0,npoints*sizeof(float));
     CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,1,npoints*sizeof(float));
     CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(float));


    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID     = MATH_ATAN2SP_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (atan2sp_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->y_InArrID1 =  INBUF0;
    fp0->x_InArrID2 =  INBUF1;
    fp0->OutArrID1  =  OUTBUF0;
    fp0->n          =  npoints;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

     /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

Int  C6accel_MATH_atan2dp(C6accel_Handle hC6accel,
                            double       *ptr_y,
                            double       *ptr_x,
                            double       *ptr_r,
                            int        npoints
)
{

    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int status;
    /* Define pointer to function parameter structure */
    atan2dp_Params              *fp0;
    XDAS_Int8                   *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(atan2dp_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);


    pAlloc=(XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 2;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters */
     CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_y,0,npoints*sizeof(double));
     CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,1,npoints*sizeof(double));
     CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(double));


    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID     = MATH_ATAN2DP_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (atan2dp_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->y_InArrID1 =  INBUF0;
    fp0->x_InArrID2 =  INBUF1;
    fp0->OutArrID1  =  OUTBUF0;
    fp0->n          =  npoints;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

     /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

Int  C6accel_MATH_powsp(C6accel_Handle hC6accel,
                            float       *ptr_x,
                            float       *ptr_y,
                            float       *ptr_r,
                            int        npoints
)
{

    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int status;
    /* Define pointer to function parameter structure */
    powsp_Params              *fp0;
    XDAS_Int8                   *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(powsp_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);


    pAlloc=(XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 2;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters */
     CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,npoints*sizeof(float));
     CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_y,1,npoints*sizeof(float));
     CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(float));


    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID     = MATH_POWSP_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (powsp_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->x_InArrID1 =  INBUF0;
    fp0->y_InArrID2 =  INBUF1;
    fp0->OutArrID1  =  OUTBUF0;
    fp0->n          =  npoints;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

     /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

Int  C6accel_MATH_powdp(C6accel_Handle hC6accel,
                            double       *ptr_x,
                            double       *ptr_y,
                            double       *ptr_r,
                            int           npoints
)
{

    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int status;
    /* Define pointer to function parameter structure */
    powdp_Params              *fp0;
    XDAS_Int8                   *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(powdp_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);


    pAlloc=(XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 2;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters */
     CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,npoints*sizeof(double));
     CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_y,1,npoints*sizeof(double));
     CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,npoints*sizeof(double));


    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID     = MATH_POWDP_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (powdp_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->x_InArrID1 =  INBUF0;
    fp0->y_InArrID2 =  INBUF1;
    fp0->OutArrID1  =  OUTBUF0;
    fp0->n          =  npoints;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

     /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

Int C6accel_DSPF_sp_fftSPxSP(C6accel_Handle hC6accel,
                                 int   npoints,
                                 float * restrict ptr_x,             /* transformed data reversed */                    /* number of points      */
                                 float * restrict ptr_w,             /* Input twiddle factors */
                                 float * restrict ptr_y,             /* linear transformed data */
                 unsigned char * ptr_brev,
                 int n_min,
                 int offset,
                 int n_max
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int status;
    /* Define pointer to function parameter structure */
    DSPF_sp_fftSPxSP_Params         *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(DSPF_sp_fftSPxSP_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous memeory allocation for the extended input structure */
    pAlloc=(XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 2;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters */
     CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,2*npoints*sizeof(float));
     CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_w,1,2*npoints*sizeof(float));
     CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_brev,2,64*sizeof(unsigned char));
     CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_y,0,2*npoints*sizeof(float));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID     = DSPF_SP_FFTSPXSP_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (DSPF_sp_fftSPxSP_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->x_InArrID1   = INBUF0;
    fp0->N            = npoints;
    fp0->w_InArrID2   = INBUF1;
    fp0->brev_InArrID3= INBUF2;
    fp0->y_OutArrID1  = OUTBUF0;
    fp0->n_min        = n_min;
    fp0->n_max        = n_max;
    fp0->offset       = offset;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
     /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;



}

Int C6accel_DSPF_sp_ifftSPxSP ( C6accel_Handle hC6accel,
                                 int   npoints,
                                 float * restrict ptr_x,             /* transformed data reversed */                    /* number of points      */
                                 float * restrict ptr_w,             /* Input twiddle factors */
                                 float * restrict ptr_y,             /* linear transformed data */
                 unsigned char * ptr_brev,
                 int n_min,
                 int offset,
                 int n_max
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int                         status;
    /* Define pointer to function parameter structure */
    DSPF_sp_ifftSPxSP_Params    *fp0;
    XDAS_Int8                   *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(DSPF_sp_ifftSPxSP_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous memeory allocation for the extended input structure */
    pAlloc=(XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size    = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 3;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters */
     CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,2*npoints*sizeof(float));
     CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_w,1,2*npoints*sizeof(float));
     CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_brev,2,64*sizeof(unsigned char));
     CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_y,0,2*npoints*sizeof(float));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID     = DSPF_SP_IFFTSPXSP_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

     /* Initialize pointers to function parameters */
    fp0 = (DSPF_sp_ifftSPxSP_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->x_InArrID1   = INBUF0;
    fp0->N            = npoints;
    fp0->w_InArrID2   = INBUF1;
    fp0->brev_InArrID3= INBUF2;
    fp0->y_OutArrID1  = OUTBUF0;
    fp0->n_min        = n_min;
    fp0->n_max        = n_max;
    fp0->offset       = offset;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;


}


Int C6accel_DSPF_sp_autocor(C6accel_Handle hC6accel,
                         float * restrict ptr_r,  /* Output array */
                         float * restrict ptr_x,  /*Input array. Must be double-word aligned */
                         int     nx,              /*Length of autocorrelation.*/
                         int     nr )             /* Number of lags */
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int                         status;
    /* Define pointer to function parameter structure */
    DSPF_sp_autocor_Params      *fp0;
    XDAS_Int8                   *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(DSPF_sp_autocor_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous memeory allocation for the extended input structure */
    pAlloc=(XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size    = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters */
     CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,(nx+nr)*sizeof(float));
     CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,nr*sizeof(float));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID     = DSPF_SP_AUTOCOR_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (DSPF_sp_autocor_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->x_InArrID1 = INBUF0;
    fp0->nx         = nx;
    fp0->nr         = nr;
    fp0->r_OutArrID1= OUTBUF0;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);


    RELEASE_CODEC_ENGINE;

    return status;


}


Int C6accel_DSPF_sp_dotprod(C6accel_Handle hC6accel,
                            const float * restrict ptr_x,  /* Output array */
                            const float * restrict ptr_y,  /*Input array. Must be double-word aligned */
                            const float   * ptr_r,
                            const int     nx)              /* Number of elements */

{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int                         status;
    /* Define pointer to function parameter structure */
    DSPF_sp_dotprod_Params          *fp0;
    //int                         outRet[1];
    XDAS_Int8                   *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(DSPF_sp_dotprod_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous memeory allocation for the extended input structure */
    pAlloc=(XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size    = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 2;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,nx*sizeof(float));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_y,1,nx*sizeof(float));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,sizeof(float));

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Initialize the extended InArgs structure */
    CInArgs->fxn[0].FxnID     = DSPF_SP_DOTPROD_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Set function Id and parameter pointers for first function call */
    fp0 = (DSPF_sp_dotprod_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->x_InArrID1       = INBUF0;
    fp0->y_InArrID2       = INBUF1;
    fp0->n                = nx;
    fp0->RetOut_OutArrID1 = OUTBUF0;

    /* Call the actual algorithm */
    status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

    /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

/*void DSPF_sp_mat_mul(float *x1, const int r1, const int c1, float *x2,
    const int c2, float *restrict y)*/
Int C6accel_DSPF_sp_mat_mul( C6accel_Handle hC6accel,
                          float *restrict ptr_x1,  /* Input matrix 1 */
                          int     r1,              /* Number of rows in matrix 1 */
                          int     c1,              /* Number of cols in matrix 1 */
                          float *restrict ptr_x2,  /* Input array. Must be double-word aligned */
                          int     c2,              /* Number of cols for matrix 2 */
                          float   *restrict ptr_y  /* Output matrix */
)
{

    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int status;
    /* Define pointer to function parameter structure */
    DSPF_sp_mat_mul_Params          *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 2;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x1,0,r1*c1*sizeof(float));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x2,1,c1*c2*sizeof(float));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_y,0,r1*c2*sizeof(float));

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size =  sizeof(Fxn_struct)+
                      sizeof(DSPF_sp_mat_mul_Params)+
                      sizeof(CInArgs->size)+
                      sizeof(CInArgs->Num_fxns);

    /* Request contiguous memeory allocation for the extended input structure */
    pAlloc  = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID     = DSPF_SP_MAT_MUL_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

     /* Initialize pointers to function parameters */
    fp0 = (DSPF_sp_mat_mul_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->x1_InArrID1  = INBUF0;
    fp0->x2_InArrID2  = INBUF1;
    fp0->y_OutArrID1  = OUTBUF0;
    fp0->r1           = r1;
    fp0->c1           = c1;
    fp0->c2           = c2;

    /* Call the actual algorithm */
    status  = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

     /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

Int C6accel_DSPF_sp_mat_mul_cplx( C6accel_Handle hC6accel,
                          float *restrict ptr_x1,  /* Input matrix 1 */
                          int     r1,              /* Number of rows in matrix 1 */
                          int     c1,              /* Number of cols in matrix 1 */
                          float *restrict ptr_x2,  /* Input array. Must be double-word aligned */
                          int     c2,              /* Number of cols for matrix 2 */
                          float *restrict ptr_y)  /* Output matrix */
{

    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int status;
    /* Define pointer to function parameter structure */
    DSPF_sp_mat_mul_cplx_Params          *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 2;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x1,0,2*r1*c1*sizeof(float));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x2,1,2*c1*c2*sizeof(float));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_y,0,2*r1*c2*sizeof(float));

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size =  sizeof(Fxn_struct)+
                      sizeof(DSPF_sp_mat_mul_cplx_Params)+
                      sizeof(CInArgs->size)+
                      sizeof(CInArgs->Num_fxns);

    /* Request contiguous memeory allocation for the extended input structure */
    pAlloc  = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID     = DSPF_SP_MAT_MUL_CPLX_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

     /* Initialize pointers to function parameters */
    fp0 = (DSPF_sp_mat_mul_cplx_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->x1_InArrID1  = INBUF0;
    fp0->x2_InArrID2  = INBUF1;
    fp0->y_OutArrID1 = OUTBUF0;
    fp0->r1          = r1;
    fp0->c1          = c1;
    fp0->c2          = c2;

    /* Call the actual algorithm */
    status  = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

     /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

Int C6accel_DSPF_sp_mat_trans( C6accel_Handle hC6accel,
                               float *restrict ptr_x,  /* Input matrix 1 */
                               int     row,              /* Number of rows in matrix 1 */
                               int     col,              /* Number of cols in matrix 1 */
                               float   *restrict ptr_r  /* Output matrix */
                               )
{

    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int status;
    /* Define pointer to function parameter structure */
    DSPF_sp_mat_trans_Params          *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,row*col*sizeof(float));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_r,0,row*col*sizeof(float));

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size =  sizeof(Fxn_struct)+
                      sizeof(DSPF_sp_mat_trans_Params)+
                      sizeof(CInArgs->size)+
                      sizeof(CInArgs->Num_fxns);

    /* Request contiguous memeory allocation for the extended input structure */
    pAlloc  = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;
    
    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID     = DSPF_SP_MAT_TRANS_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

     /* Initialize pointers to function parameters */
    fp0 = (DSPF_sp_mat_trans_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);
    
    /* Fill in the fields in the parameter structure */
    fp0->x_InArrID1  = INBUF0;
    fp0->y_OutArrID1 = OUTBUF0;
    fp0->rows          = row;
    fp0->cols          = col;


    /* Call the actual algorithm */
    status  = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
    
     /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;


}

Int C6accel_DSPF_sp_vecmul( C6accel_Handle hC6accel,
                          float *restrict ptr_x1,  /* Input matrix 1 */
                          float *restrict ptr_x2,  /* Input array. Must be double-word aligned */
                          float *restrict ptr_y,  /* Output matrix */
              const int    n              /* Number of elements*/
)
{

    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int status;
    /* Define pointer to function parameter structure */
    DSPF_sp_vecmul_Params          *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 2;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x1,0,n*sizeof(float));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x2,1,n*sizeof(float));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_y,0,n*sizeof(float));

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size =  sizeof(Fxn_struct)+
                      sizeof(DSPF_sp_vecmul_Params)+
                      sizeof(CInArgs->size)+
                      sizeof(CInArgs->Num_fxns);

    /* Request contiguous memeory allocation for the extended input structure */
    pAlloc  = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID     = DSPF_SP_VECMUL_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

     /* Initialize pointers to function parameters */
    fp0 = (DSPF_sp_vecmul_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->x1_InArrID1  = INBUF0;
    fp0->x2_InArrID2  = INBUF1;
    fp0->y_OutArrID1  = OUTBUF0;
    fp0->n            = n;

    /* Call the actual algorithm */
    status  = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

     /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

Int C6accel_DSPF_sp_vecrecip( C6accel_Handle hC6accel,
                              const float *restrict ptr_x,  /* Input matrix 1 */
                              float *restrict ptr_y,         /* Output matrix */
                  const int    n                 /* Number of elements*/
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int status;
    /* Define pointer to function parameter structure */
    DSPF_sp_vecrecip_Params          *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,n*sizeof(float));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_y,0,n*sizeof(float));

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size =  sizeof(Fxn_struct)+
                      sizeof(DSPF_sp_vecrecip_Params)+
                      sizeof(CInArgs->size)+
                      sizeof(CInArgs->Num_fxns);

    /* Request contiguous memeory allocation for the extended input structure */
    pAlloc  = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID     = DSPF_SP_VECRECIP_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

     /* Initialize pointers to function parameters */
    fp0 = (DSPF_sp_vecrecip_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->x_InArrID1  = INBUF0;
    fp0->y_OutArrID1  = OUTBUF0;
    fp0->n            = n;

    /* Call the actual algorithm */
    status  = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

     /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

Int C6accel_DSPF_sp_vecsum_sq( C6accel_Handle hC6accel,
                              const float *restrict ptr_x,  /* Input matrix 1 */
                              float *restrict ptr_OutRet,   /* Output matrix */
                  const int    n                /* Number of elements*/
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int status;

    /* Define pointer to function parameter structure */
    DSPF_sp_vecsum_sq_Params          *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 1;
    outBufDesc.numBufs = 1;

     /* Fill in input/output buffer descriptor parameters */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,n*sizeof(float));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_OutRet,0,sizeof(float));

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size =  sizeof(Fxn_struct)+
                      sizeof(DSPF_sp_vecsum_sq_Params)+
                      sizeof(CInArgs->size)+
                      sizeof(CInArgs->Num_fxns);

    /* Request contiguous memeory allocation for the extended input structure */
    pAlloc  = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID     = DSPF_SP_VECSUM_SQ_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

     /* Initialize pointers to function parameters */
    fp0 = (DSPF_sp_vecsum_sq_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->x_InArrID1  = INBUF0;
    fp0->OutRet_OutArrID1  = OUTBUF0;
    fp0->n            = n;

    /* Call the actual algorithm */
    status  = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

     /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

Int C6accel_DSPF_sp_w_vec( C6accel_Handle hC6accel,
                          float *restrict ptr_x1,  /* Input matrix 1 */
                          float *restrict ptr_x2,  /* Input array. Must be double-word aligned */
              const float m,
                          float *restrict ptr_y,  /* Output matrix */
              const int    n              /* Number of elements*/
)
{

    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int status;

    /* Define pointer to function parameter structure */
    DSPF_sp_w_vec_Params          *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 2;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x1,0,n*sizeof(float));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x2,1,n*sizeof(float));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_y,0,n*sizeof(float));

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size =  sizeof(Fxn_struct)+
                      sizeof(DSPF_sp_w_vec_Params)+
                      sizeof(CInArgs->size)+
                      sizeof(CInArgs->Num_fxns);

    /* Request contiguous memeory allocation for the extended input structure */
    pAlloc  = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs = (IC6Accel_InArgs *)pAlloc;

    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID     = DSPF_SP_W_VEC_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

     /* Initialize pointers to function parameters */
    fp0 = (DSPF_sp_w_vec_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->x1_InArrID1  = INBUF0;
    fp0->x2_InArrID2  = INBUF1;
    fp0->y_OutArrID1  = OUTBUF0;
    fp0->n            = n;
    fp0->m            = m;

    /* Call the actual algorithm */
    status  = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

     /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

Int C6accel_DSPF_sp_fir_gen( C6accel_Handle hC6accel,
                             float * restrict ptr_x,
                             float * restrict ptr_h,
                             float * restrict ptr_y,
                             int     nh,
                             int     ny
)
{

    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int                         status;
    /* Define pointer to function parameter structure */
    DSPF_sp_fir_gen_Params          *fp0;
    XDAS_Int8                   *pAlloc;

    ACQUIRE_CODEC_ENGINE;
    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 2;
    outBufDesc.numBufs = 1;
    
   /* Fill in input/output buffer descriptor parameters */
      CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,(ny+nh-1)*sizeof(float));
      CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_h,1,nh*sizeof(float));
      CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_y,0,ny*sizeof(float));

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(DSPF_sp_fir_gen_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous memeory allocation for the extended input structure */
    pAlloc=(XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;


    /* Initialize .size fields for dummy input and output arguments */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID     = DSPF_SP_FIR_GEN_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

     /* Initialize pointers to function parameters */
    fp0 = (DSPF_sp_fir_gen_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->x_InArrID1  = INBUF0;
    fp0->h_InArrID2  = INBUF1;
    fp0->y_OutArrID1 = OUTBUF0;
    fp0->nh          = nh;
    fp0->ny          = ny;

    /* Call the actual algorithm */
    status  = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

     /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

Int C6accel_DSPF_sp_fir_cplx( C6accel_Handle hC6accel,
                             float * restrict ptr_x,
                             float * restrict ptr_h,
                             float * restrict ptr_y,
                             int     nh,
                             int     ny
)
{

    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int                         status;
    /* Define pointer to function parameter structure */
    DSPF_sp_fir_cplx_Params     *fp0;
    XDAS_Int8                   *pAlloc;

    ACQUIRE_CODEC_ENGINE;
    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 2;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,2*(ny+nh-1)*sizeof(float));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_h,1,2*nh*sizeof(float));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_y,0,2*ny*sizeof(float));

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(DSPF_sp_fir_cplx_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous memeory allocation for the extended input structure */
    pAlloc=(XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;


    /* Initialize .size fields for dummy input and output arguments */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID     = DSPF_SP_FIR_CPLX_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

     /* Initialize pointers to function parameters */
    fp0 = (DSPF_sp_fir_cplx_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->x_InArrID1  = INBUF0;
    fp0->h_InArrID2  = INBUF1;
    fp0->y_OutArrID1 = OUTBUF0;
    fp0->nh          = nh;
    fp0->ny          = ny;

    /* Call the actual algorithm */
    status  = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

     /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

Int C6accel_DSPF_sp_iir(     C6accel_Handle hC6accel,
                          float * restrict ptr_y1,
                          float * restrict ptr_x,
                          float * restrict ptr_y2,
                          float * restrict ptr_hb,
              float * restrict ptr_ha,
                          int              n
                         )
{

    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int                         status;
    /* Define pointer to function parameter structure */
    DSPF_sp_iir_Params          *fp0;
    XDAS_Int8                   *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 3;
    outBufDesc.numBufs = 2;

    /* Fill in input/output buffer descriptor parameters */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,(n+4)*sizeof(float));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_hb,1,5*sizeof(float));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_ha,2,5*sizeof(float));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_y1,0,(n+4)*sizeof(float));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_y2,1,(n)*sizeof(float));


    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(DSPF_sp_iir_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous memeory allocation for the extended input structure */
    pAlloc=(XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;



   /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= DSPF_SP_IIR_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (DSPF_sp_iir_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->x_InArrID1    = INBUF0;
    fp0->hb_InArrID2   = INBUF1;
    fp0->ha_InArrID3   = INBUF2;
    fp0->y1_OutArrID1  = OUTBUF0;
    fp0->y2_OutArrID2  = OUTBUF1;
    fp0->n             = n;

    /* Call the actual algorithm */
    status  = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

     /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

Int C6accel_DSPF_sp_convol( C6accel_Handle hC6accel,
                             float * restrict ptr_x,
                             float * restrict ptr_h,
                             float * restrict ptr_y,
                             const short     nh,
                             const short     ny
)
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int                         status;
    /* Define pointer to function parameter structure */
    DSPF_sp_convol_Params       *fp0;
    XDAS_Int8                   *pAlloc;

    ACQUIRE_CODEC_ENGINE;
    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 2;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,(ny+nh-1)*sizeof(float));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_h,1,nh*sizeof(float));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_y,0,ny*sizeof(float));

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(DSPF_sp_convol_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous memeory allocation for the extended input structure */
    pAlloc=(XDAS_Int8 *)Memory_alloc(InArg_Buf_size,&wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;


    /* Initialize .size fields for dummy input and output arguments */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID     = DSPF_SP_CONV_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

     /* Initialize pointers to function parameters */
    fp0 = (DSPF_sp_convol_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->x_InArrID1  = INBUF0;
    fp0->h_InArrID2  = INBUF1;
    fp0->y_OutArrID1 = OUTBUF0;
    fp0->nh          = nh;
    fp0->ny          = ny;

    /* Call the actual algorithm */
    status  = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

     /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

Int C6accel_DSPF_sp_biquad(  C6accel_Handle hC6accel,
                             float * restrict ptr_x,
                             float * restrict ptr_b,
                 float * restrict ptr_a,
                 float * restrict ptr_delay,
                 float * restrict ptr_y,
                             const int        n
                         )
{

    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int                         status;
    /* Define pointer to function parameter structure */
    DSPF_sp_biquad_Params       *fp0;
    XDAS_Int8                   *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 4;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_x,0,n*sizeof(float));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_b,1,3*sizeof(float));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_a,2,3*sizeof(float));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_delay,3,2*sizeof(float));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_y,0,n*sizeof(float));


    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(DSPF_sp_biquad_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous memeory allocation for the extended input structure */
    pAlloc=(XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;



   /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= DSPF_SP_BIQUAD_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (DSPF_sp_biquad_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->x_InArrID1    = INBUF0;
    fp0->b_InArrID2    = INBUF1;
    fp0->a_InArrID3    = INBUF2;
    fp0->delay_InArrID4= INBUF3;
    fp0->y_OutArrID1   = OUTBUF0;
    fp0->n             = n;

    /* Call the actual algorithm */
    status  = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

     /* Free the InArgs structure */
    Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);

    RELEASE_CODEC_ENGINE;

    return status;

}

Int C6accel_IMG_ycbcr422sp_to_ycbcr420pl(
    C6accel_Handle hC6accel,
    const unsigned char * y_src,
    const unsigned char * cbcr_src,
    unsigned char * restrict y_dst,
    unsigned char * restrict cb_dst,
    unsigned char * restrict cr_dst,
    unsigned int num_lines,
    unsigned int width,
    unsigned int src_pitch,
    unsigned int dst_y_pitch,
    unsigned int dst_cbcr_pitch
)
{

    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int                         status;
    /* Define pointer to function parameter structure */
    IMG_ycbcr422sp_to_ycbcr420pl_Params *fp0;
    XDAS_Int8                    *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 2;
    outBufDesc.numBufs = 3;

    /* Fill in input/output buffer descriptor parameters */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(y_src,0,num_lines*src_pitch*sizeof(unsigned char));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(cbcr_src,1,num_lines*src_pitch*sizeof(unsigned char));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL( y_dst,0,num_lines*dst_y_pitch*sizeof(unsigned char));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(cb_dst,1,num_lines*dst_cbcr_pitch/2*sizeof(unsigned char));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(cr_dst,2,num_lines*dst_cbcr_pitch/2*sizeof(unsigned char));

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IMG_ycbcr422sp_to_ycbcr420pl_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous memeory allocation for the extended input structure */
    pAlloc=(XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;



   /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

   /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= IMG_YCBCR422SP_TO_YCBCR420PL_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IMG_ycbcr422sp_to_ycbcr420pl_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->y_src_InArrID1    = INBUF0;
    fp0->cbcr_src_InArrID2 = INBUF1;
    fp0->y_dst_OutArrID1   = OUTBUF0;
    fp0->cb_dst_OutArrID2  = OUTBUF1;
    fp0->cr_dst_OutArrID3  = OUTBUF2;
    fp0->src_pitch         = src_pitch;
    fp0->dst_y_pitch       = dst_y_pitch;
    fp0->dst_cbcr_pitch    = dst_cbcr_pitch;
    fp0->num_lines         = num_lines;
    fp0->width             = width;

    /* Call the actual algorithm */
    status  = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);

     /* Free the InArgs structure */
    Memory_contigFree(pAlloc, InArg_Buf_size);

    RELEASE_CODEC_ENGINE;

    return status;

}

Int C6accel_IMG_ycbcr422sp_to_ycbcr422ile(
    C6accel_Handle hC6accel,
    const unsigned char * y_src,    
    const unsigned char * cbcr_src, 
    unsigned char * restrict ycbcr_dst,
    unsigned int num_lines, 
    unsigned int width,
    unsigned int src_pitch, 
    unsigned int dst_ycbcr_pitch
)
{

    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int                         status;
    /* Define pointer to function parameter structure */
    IMG_ycbcr422sp_to_ycbcr422ile_Params *fp0;
    XDAS_Int8                    *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 2;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(y_src,0,num_lines*src_pitch*sizeof(unsigned char));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(cbcr_src,1,num_lines*src_pitch*sizeof(unsigned char));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL( ycbcr_dst,0,num_lines*dst_ycbcr_pitch*sizeof(unsigned char));
    
    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IMG_ycbcr422sp_to_ycbcr422ile_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous memeory allocation for the extended input structure */
    pAlloc=(XDAS_Int8 *)Memory_contigAlloc(InArg_Buf_size, BUFALIGN);
    CInArgs= (IC6Accel_InArgs *)pAlloc;



   /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

   /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= IMG_YCBCR422SP_TO_YCBCR422ILE_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IMG_ycbcr422sp_to_ycbcr422ile_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->y_src_InArrID1    = INBUF0;
    fp0->cbcr_src_InArrID2 = INBUF1;
    fp0->ycbcr_dst_OutArrID1   = OUTBUF0;
    fp0->src_pitch         = src_pitch;
    fp0->dst_ycbcr_pitch   = dst_ycbcr_pitch;
    fp0->num_lines         = num_lines;
    fp0->width             = width;

    /* Call the actual algorithm */
    status  = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
    
     /* Free the InArgs structure */
    Memory_contigFree(pAlloc, InArg_Buf_size);

    RELEASE_CODEC_ENGINE;

    return status;  

}


Int C6accel_IMG_ycbcr422pl_to_ycbcr422sp(
    C6accel_Handle hC6accel,
    const unsigned char * y_dst,    
    const unsigned char * cbcr_dst, 
    unsigned char * restrict y_src,
    unsigned char * restrict cb_src,
    unsigned char * restrict cr_src,
    unsigned int num_lines, 
    unsigned int width,
    unsigned int dst_pitch, 
    unsigned int src_y_pitch,
    unsigned int src_cbcr_pitch
)
{

    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int                         status;
    /* Define pointer to function parameter structure */
    IMG_ycbcr422pl_to_ycbcr422sp_Params *fp0;
    XDAS_Int8                    *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 3;
    outBufDesc.numBufs = 2;

    /* Fill in input/output buffer descriptor parameters */
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(y_dst,0,num_lines*dst_pitch*sizeof(unsigned char));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(cbcr_dst,1,num_lines*dst_pitch*sizeof(unsigned char));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL( y_src,0,num_lines*src_y_pitch*sizeof(unsigned char));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(cb_src,1,num_lines*src_cbcr_pitch*sizeof(unsigned char));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(cr_src,2,num_lines*src_cbcr_pitch*sizeof(unsigned char));
  
    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IMG_ycbcr422pl_to_ycbcr422sp_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous memeory allocation for the extended input structure */
    pAlloc=(XDAS_Int8 *)Memory_contigAlloc(InArg_Buf_size, BUFALIGN);
    CInArgs= (IC6Accel_InArgs *)pAlloc;



   /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

   /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= IMG_YCBCR422PL_TO_YCBCR422SP_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IMG_ycbcr422pl_to_ycbcr422sp_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->y_dst_OutArrID1    = OUTBUF0;
    fp0->cbcr_dst_OutArrID2 = OUTBUF1;
    fp0->y_src_InArrID1 = INBUF0;
    fp0->cb_src_InArrID2    = INBUF1;
    fp0->cr_src_InArrID3    = INBUF2;
    fp0->dst_pitch          = dst_pitch;
    fp0->src_y_pitch        = src_y_pitch;
    fp0->src_cbcr_pitch     = src_cbcr_pitch;
    fp0->num_lines          = num_lines;
    fp0->width              = width;

    /* Call the actual algorithm */
    status  = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
    
     /* Free the InArgs structure */
    Memory_contigFree(pAlloc, InArg_Buf_size);

    RELEASE_CODEC_ENGINE;

    return status;  

}


/* This function defination is added by Pramod */
/*
void C6Accel_VLIB_integralImage8(C6accel_Handle hC6accel, unsigned char * restrict ptr_pIn, unsigned short inCols, unsigned short inRows, unsigned int * 												restrict ptr_pLastLine, unsigned int * restrict ptr_pOut)
Arguments
* hC6accel C6accel Handle
* pIn[inColsxinRows] Pointer to inCols x inRows 8 bit Image.
* inCols No of columns in image.
* inRows No of rows in image
* pLastLine[inColsx1] Pointer to complex 32-bit carry-over buffer.
* pOut[inColsxinRows] Pointer to complex 32-bit output integral image.

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Object classification may be done by calculating image features (such as moments and/or wavelets) on a
region of interest and feeding them to a classifier (such as k-NN or SVM). Integral image is an important
step in calculation of a common type of image features, over-complete Haar wavelets [ 2 ]. Integral image
values may be used as features themselves.


Special Requirements

The arguments pIn, pOut, and pLastLine must be 64-bit aligned. For the fixed-width version the width is
assumed to be 640 pixels.
* pIn is a pointer to an (inCols × inRows) array of unsigned char data.
* pLastLine is a pointer to an (inCols × 1) array of unsigned int data.
* pOut is a pointer to an (inCols × inRows) array of unsigned int data.

*/

int C6accel_VLIB_integralImage8(C6accel_Handle hC6accel,
                          unsigned char * restrict ptr_pIn,             /* array of unsigned char data       */
                          unsigned short inCols,                        /* number of columns                 */
			  unsigned short inRows,			/* number of rows                    */
                          unsigned int * restrict ptr_pLastLine,        /* array of unsigned int data        */
                          unsigned int * restrict ptr_pOut              /* array of unsigned int output data */
#ifdef SUPPORT_ASYNC
                          ,E_CALL_TYPE callType)
#else 
)
#endif
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    Int                         status;

    /* Define pointer to function parameter structure */
    VLIB_integralImage8_Params  *fp0;
    XDAS_Int8                   *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(VLIB_integralImage8_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size    = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs  = 2;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_pIn,0,inCols*inRows*sizeof(char));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_pLastLine,1,inCols*sizeof(int));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_pOut,0,inCols*inRows*sizeof(int));


    /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns = 1;
    CInArgs->size     = InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID     = VLIB_INTEGRALIMAGE_8_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset = sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

     /* Initialize pointers to function parameters */
    fp0 = (VLIB_integralImage8_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->pIn_InArrID1        = INBUF0;
    fp0->inCols              = inCols;
    fp0->inRows              = inRows;
    fp0->pLastLine_InArrID2  = INBUF1;
    fp0->pOut_OutArrID1      = OUTBUF0;

#ifdef SUPPORT_ASYNC
    /* Call the actual algorithm */
    if (callType == ASYNC)
      {

       //Update async structure
       if (c6accelAsyncParams.asyncCallCount!=0)
          { status = UNIVERSAL_EFAIL;
            printf("Async call failed as %d are still pending\n",c6accelAsyncParams.asyncCallCount);
          }
       else
          {c6accelAsyncParams.asyncCallCount++;
           memcpy(&(c6accelAsyncParams.inBufs),&inBufDesc, sizeof (XDM1_BufDesc));
 //           memcpy(&(c6accelAsyncParams.inOutBufs),&inOutBufs, sizeof (XDM1_BufDesc));
           memcpy(&(c6accelAsyncParams.outBufs), &outBufDesc,sizeof(XDM1_BufDesc));
           memcpy(&(c6accelAsyncParams.inArgs), CInArgs,sizeof(UNIVERSAL_InArgs));
           memcpy(&(c6accelAsyncParams.outArgs),&uniOutArgs,sizeof(UNIVERSAL_OutArgs));
           c6accelAsyncParams.pBuf = pAlloc;
           c6accelAsyncParams.pBufSize = InArg_Buf_size;

           status = UNIVERSAL_processAsync(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
           }
      }
     else
       {
#endif
         // Sync call
        status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        /* Free the InArgs structure */
        Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);
#ifdef SUPPORT_ASYNC
        }
#endif 

    RELEASE_CODEC_ENGINE;

    return status;

}

/* This function defination is added by Pramod */
/*
int C6accel_RGB_To_Y(C6accel_Handle hC6accel, unsigned char * restrict ptr_src, unsigned char * restrict ptr_dst, unsigned int count)
Arguments
* hC6accel C6accel Handle
* ptr_src[ ] Input RGB image of count pixels.
* ptr_dst[ ] Output Greyscale image of count pixels.
* count Total pixel count.

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description This routine converts input RGB image to Greyscale Image using following formula

  		Y = 0.299 * R + 0.587 * G + 0.114 * B


*/
int C6accel_RGB_To_Y(C6accel_Handle hC6accel,
                          unsigned char * restrict ptr_src,       /* Pointer to input image in RGB format */
                          unsigned char * restrict ptr_dst,       /* Pointer to Greyscale output image    */
			  unsigned int count			  /* Total number of pixels               */
#ifdef SUPPORT_ASYNC
                          ,E_CALL_TYPE callType)
#else 
)
#endif

{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int status;
    /* Define pointer to function parameter structure */
    IMG_RGB_To_Y_Params      *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(IMG_RGB_To_Y_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_src,0,3*count*sizeof(char));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_dst,0,count*sizeof(short));

     /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= IMG_RGB_TO_Y_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (IMG_RGB_To_Y_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->src_InArrID1= INBUF0;
    fp0->dst_OutArrID1= OUTBUF0;
    fp0->count= count;

#ifdef SUPPORT_ASYNC
    /* Call the actual algorithm */
    if (callType == ASYNC)
      {

       //Update async structure
       if (c6accelAsyncParams.asyncCallCount!=0)
          { status = UNIVERSAL_EFAIL;
            printf("Async call failed as %d are still pending\n",c6accelAsyncParams.asyncCallCount);
          }
       else
          {c6accelAsyncParams.asyncCallCount++;
           memcpy(&(c6accelAsyncParams.inBufs),&inBufDesc, sizeof (XDM1_BufDesc));
 //           memcpy(&(c6accelAsyncParams.inOutBufs),&inOutBufs, sizeof (XDM1_BufDesc));
           memcpy(&(c6accelAsyncParams.outBufs), &outBufDesc,sizeof(XDM1_BufDesc));
           memcpy(&(c6accelAsyncParams.inArgs), CInArgs,sizeof(UNIVERSAL_InArgs));
           memcpy(&(c6accelAsyncParams.outArgs),&uniOutArgs,sizeof(UNIVERSAL_OutArgs));
           c6accelAsyncParams.pBuf = pAlloc;
           c6accelAsyncParams.pBufSize = InArg_Buf_size;

           status = UNIVERSAL_processAsync(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
           }
      }
     else
       {
#endif
         // Sync call
        status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        /* Free the InArgs structure */
        Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);
#ifdef SUPPORT_ASYNC
        }
#endif 

    RELEASE_CODEC_ENGINE;

    return status;

}


/* This function defination is added by Pramod */
/*
int C6accel_RGB_To_Y_IMG_sobel_3x3_8_16(C6accel_Handle hC6accel, unsigned char * restrict ptr_src, unsigned char * restrict ptr_dst, unsigned int count,
					const unsigned char *restrict in, unsigned short *restrict out, short cols, short rows)
Arguments
* hC6accel C6accel Handle
* ptr_src[ ] Input RGB image of count pixels.
* ptr_dst[ ] Output Greyscale image of count pixels.
* count Total pixel count.
* ptr_in[ ] Input Greyscale image.
* ptr_out[ ] Output sobel filtered image.
* cols[ ] Number of columns in image.
* rows[ ] Number of rows in image.

Return value:  API returns status of the function call.
               ==1 Pass
               <0  Fail

Description This routine converts input RGB image to Greyscale Image using formula as described below followed by sobel_3x3_8_16 filter.

  		Y = 0.299 * R + 0.587 * G + 0.114 * B


*/
int C6accel_RGB_To_Y_IMG_sobel_3x3_8_16(C6accel_Handle hC6accel,
                          		unsigned char * restrict ptr_src,       /* Pointer to input image in RGB format */
                          		unsigned char * restrict ptr_dst,       /* Pointer to Greyscale output image    */
			  		unsigned int count,			/* Total number of pixels               */
			  		const unsigned char *restrict in,       /* Input image data   */
                               		unsigned short       *restrict out,     /* Output image data  */
                               		short cols, short rows                  /* Image dimensions   */

#ifdef SUPPORT_ASYNC
                          		,E_CALL_TYPE callType)
#else 
)
#endif
{

    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int status;
    /* Define pointer to function parameter structure */
    IMG_RGB_To_Y_Params       *fp0;
    IMG_sobel_3x3_8_16_Params *fp1;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call).
     In this case we need to consider all the _Params size */
    InArg_Buf_size=  (2*sizeof( Fxn_struct))+
                     sizeof(IMG_RGB_To_Y_Params)+
		     sizeof(IMG_sobel_3x3_8_16_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 2;
    outBufDesc.numBufs = 2; /* I am cheating DSP by telling one in/out buffer as out buffer */

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_src,0,3*count*sizeof(char));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_dst,0,rows* cols *sizeof(char));
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_dst,1,rows*cols*sizeof(char));
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(out,1,rows* cols *sizeof(short));

     /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=2;  //***
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= IMG_RGB_TO_Y_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+ (2 * sizeof(Fxn_struct)); /* 2 is due to fact that we are 															chaining 2 functions    */

    /* Set function Id and parameter pointers for second function call */
    CInArgs->fxn[1].FxnID= IMG_SOBEL_3X3_8_16_FXN_ID;
    CInArgs->fxn[1].Param_ptr_offset=CInArgs->fxn[0].Param_ptr_offset+sizeof(IMG_RGB_To_Y_Params);

    
    /* Initialize pointers to function parameters */
    fp0 = (IMG_RGB_To_Y_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);
    fp1 = (IMG_sobel_3x3_8_16_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[1].Param_ptr_offset);


    /* Fill in the fields in the parameter structure of function 1 */
    fp0->src_InArrID1= INBUF0;
    fp0->dst_OutArrID1= OUTBUF0;
    fp0->count= count;

    /* Fill in the fields in the parameter structure of function 2 */
    fp1->indata_InArrID1= INBUF1;
    fp1->outdata_OutArrID1= OUTBUF1;
    fp1->Col= cols;
    fp1->Row= rows;

#ifdef SUPPORT_ASYNC
    /* Call the actual algorithm */
    if (callType == ASYNC)
      {

       //Update async structure
       if (c6accelAsyncParams.asyncCallCount!=0)
          { status = UNIVERSAL_EFAIL;
            printf("Async call failed as %d are still pending\n",c6accelAsyncParams.asyncCallCount);
          }
       else
          {c6accelAsyncParams.asyncCallCount++;
           memcpy(&(c6accelAsyncParams.inBufs),&inBufDesc, sizeof (XDM1_BufDesc));
 //           memcpy(&(c6accelAsyncParams.inOutBufs),&inOutBufs, sizeof (XDM1_BufDesc));
           memcpy(&(c6accelAsyncParams.outBufs), &outBufDesc,sizeof(XDM1_BufDesc));
           memcpy(&(c6accelAsyncParams.inArgs), CInArgs,sizeof(UNIVERSAL_InArgs));
           memcpy(&(c6accelAsyncParams.outArgs),&uniOutArgs,sizeof(UNIVERSAL_OutArgs));
           c6accelAsyncParams.pBuf = pAlloc;
           c6accelAsyncParams.pBufSize = InArg_Buf_size;

           status = UNIVERSAL_processAsync(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
           }
      }
     else
       {
#endif
         // Sync call
        status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        /* Free the InArgs structure */
        Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);
#ifdef SUPPORT_ASYNC
        }
#endif 

    RELEASE_CODEC_ENGINE;

    return status;

}

/*
void C6Accel_DSP_dft_f(C6accel_Handle hC6accel,
                          unsigned char * restrict ptr_src,       
                          unsigned char * restrict ptr_dst,       
			  int dxtType,				  
			  int nonZeroRows, 
			  )   
Arguments
* hC6accel C6accel 		Handle
* ptr_src[]        		Pointer to input Array
* ptr_dst[]        		Pointer to output arrary   
* dxtType	   		Type of action to take 
* nonZeroRows      		Count of non-zero rows for finding DFT


Return value:  API returns status of the function call.
               ==0 Pass
               <0  Fail

Description This routine computes a complex forward/inverse mixed radix FFT with rounding and digit reversal.
Input data x[ ], output data y[ ]. The output is returned in the separate array
ptr_dst[ ] in normal order. Each complex value is stored with interleaved real and imaginary parts. The code uses a
special ordering of FFT coefficients (also called twiddle factors) and memory accesses to improve performance in the presence of cache.

Special Requirements

In-place computation is not allowed.
The size of the FFT, nx, must be a power of 2 or 4, and 16 = nx = 65536.
The arrays for the complex input data x[ ], complex output data y[ ], and twiddle factors w[ ] must be double-word aligned.
The input and output data are complex, with the real/imaginary components stored in adjacent locations in the array.
*/
int C6accel_DSP_dft_f(C6accel_Handle hC6accel,
                          void * restrict ptr_src,       /* Pointer to input image */
                          void * restrict ptr_dst,       /* Pointer to Greyscale output image    */
			  int dxtType,				  /* Type of action to take */
			  int nonZeroRows 			  /* Count of non-zero rows */
			  
#ifdef SUPPORT_ASYNC
                          		,E_CALL_TYPE callType)
#else 
)
#endif
{
    XDM1_BufDesc                inBufDesc;
    XDM1_BufDesc                outBufDesc;
    XDAS_Int32                  InArg_Buf_size;
    IC6Accel_InArgs             *CInArgs;
    UNIVERSAL_OutArgs           uniOutArgs;
    int status;
    IplImage *tempImage = (IplImage *) ptr_src;
    /* Define pointer to function parameter structure */
    DSP_dft_f_Params      *fp0;
    XDAS_Int8 *pAlloc;

    ACQUIRE_CODEC_ENGINE;

    /* Allocate the InArgs structure as it varies in size
    (Needs to be changed everytime we make a API call)*/
    InArg_Buf_size=  sizeof(Fxn_struct)+
                     sizeof(DSP_dft_f_Params)+
                     sizeof(CInArgs->size)+
                     sizeof(CInArgs->Num_fxns);

    /* Request contiguous heap memory allocation for the extended input structure */
    pAlloc = (XDAS_Int8 *)Memory_alloc(InArg_Buf_size, &wrapperMemParams);
    CInArgs= (IC6Accel_InArgs *)pAlloc;

    /* Initialize .size fields for dummy input and output arguments */
    uniOutArgs.size = sizeof(uniOutArgs);

    /* Set up buffers to pass buffers in and out to alg  */
    inBufDesc.numBufs = 1;
    outBufDesc.numBufs = 1;

    /* Fill in input/output buffer descriptor parameters and manage ARM cache*/
    /* See wrapper_c6accel_i.h for more details of operation                 */
    int size = tempImage->widthStep * tempImage->height;
    CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(tempImage->imageData,0,size);
   
    tempImage = (IplImage *)ptr_dst;
    size = tempImage->widthStep * tempImage->height;
    CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(tempImage->imageData,0,size);

     /* Initialize the extended InArgs structure */
    CInArgs->Num_fxns=1;
    CInArgs->size= InArg_Buf_size;

    /* Set function Id and parameter pointers for first function call */
    CInArgs->fxn[0].FxnID= DSP_DFT_F_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);

    /* Initialize pointers to function parameters */
    fp0 = (DSP_dft_f_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);

    /* Fill in the fields in the parameter structure */
    fp0->src_InArrID1= INBUF0;
    fp0->dst_OutArrID1= OUTBUF0;
    fp0->dxtType= dxtType;
    fp0->nonZeroRows= nonZeroRows;
    memcpy(&(fp0->src),ptr_src,sizeof(IplImage));
    memcpy(&(fp0->dst),ptr_dst,sizeof(IplImage));


#ifdef SUPPORT_ASYNC
    /* Call the actual algorithm */
    if (callType == ASYNC)
      {

       //Update async structure
       if (c6accelAsyncParams.asyncCallCount!=0)
          { status = UNIVERSAL_EFAIL;
            printf("Async call failed as %d are still pending\n",c6accelAsyncParams.asyncCallCount);
          }
       else
          {c6accelAsyncParams.asyncCallCount++;
           memcpy(&(c6accelAsyncParams.inBufs),&inBufDesc, sizeof (XDM1_BufDesc));
 //           memcpy(&(c6accelAsyncParams.inOutBufs),&inOutBufs, sizeof (XDM1_BufDesc));
           memcpy(&(c6accelAsyncParams.outBufs), &outBufDesc,sizeof(XDM1_BufDesc));
           memcpy(&(c6accelAsyncParams.inArgs), CInArgs,sizeof(UNIVERSAL_InArgs));
           memcpy(&(c6accelAsyncParams.outArgs),&uniOutArgs,sizeof(UNIVERSAL_OutArgs));
           c6accelAsyncParams.pBuf = pAlloc;
           c6accelAsyncParams.pBufSize = InArg_Buf_size;

           status = UNIVERSAL_processAsync(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
           }
      }
     else
       {
#endif
         // Sync call
        status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
        /* Free the InArgs structure */
        Memory_free(pAlloc, InArg_Buf_size, &wrapperMemParams);
#ifdef SUPPORT_ASYNC
        }
#endif 

    RELEASE_CODEC_ENGINE;

    return status;
}


/*
 *
 * Function to implemet the async wait for completion of a call made with UNIVERSAL_process_async()
 *
 */
#ifdef SUPPORT_ASYNC
int C6accel_waitAsyncCall(C6accel_Handle hC6accel)
{int Status;

 if (c6accelAsyncParams.asyncCallCount==1)
       {c6accelAsyncParams.asyncCallCount--;

       Status = UNIVERSAL_processWait(hC6accel->hUni, &(c6accelAsyncParams.inBufs), &(c6accelAsyncParams.outBufs), NULL,
        &(c6accelAsyncParams.inArgs), &(c6accelAsyncParams.outArgs), UNIVERSAL_FOREVER);
       if(Status != 0)
	 printf("Error: DSP Sync Error\n");
        /* Free the InArgs structure */
        Memory_free(c6accelAsyncParams.pBuf, c6accelAsyncParams.pBufSize, &wrapperMemParams);

       }
 else
    printf("Failed on waitAsyncCall() count=%d\n",c6accelAsyncParams.asyncCallCount); 

}
#endif 
/*=============================================================================*/
/*                  End of File c6accelw.c                                     */
/*                              Version : 0.0.1                                */
/*=============================================================================*/









