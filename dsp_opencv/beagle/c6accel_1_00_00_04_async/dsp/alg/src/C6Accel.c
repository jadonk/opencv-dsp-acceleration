/*******************************************************************************
**+--------------------------------------------------------------------------+**
 *                                                                            **
 * Copyright (c) 2010, Texas Instruments Incorporated                         **
 * All rights reserved.                                                       **
 *                                                                            **
**+--------------------------------------------------------------------------+**
 *           ======== C6ACCEL.c ========                                      **
 *           Universal "C6ACCEL" xdais algorithm.                             **
 *                                                                            **
 *  This file contains an implementation of the C6Accel xdais algorithm using **
 *  IUNIVERSAL interface defined by xDM.                                      **
*******************************************************************************/

/* Include Files */
#include <xdc/std.h>
#include <string.h>
#include <ti/xdais/dm/iuniversal.h>

/* Header Intrinsic to xdais algorithm*/
#include "C6Accel.h"

#ifdef __TI_COMPILER_VERSION__
/* xDAIS Rule 13 - this #pragma should only apply to TI codegen */
#pragma CODE_SECTION(C6ACCEL_TI_control, ".text:algControl")
#pragma CODE_SECTION(C6ACCEL_TI_process, ".text:algProcess")
#pragma CODE_SECTION(C6ACCEL_TI_initObj, ".text:algInit")
#pragma CODE_SECTION(C6ACCEL_TI_free,    ".text:algFree")
#pragma CODE_SECTION(C6ACCEL_TI_alloc,   ".text:algAlloc")
#endif

/* Application-Algorithm Interface file includes */
#include "iC6Accel_ti.h"
#include "iC6Accel_ti_priv.h"

/* Global function to determine if number is power of two*/
static unsigned short IsPowOfTwo( unsigned long number);

/* Maintain version string for future releases */
#define C6ACCEL_VERSIONSTRING "1.00.00.04"

/* IALGFXN definition */ 
#define IALGFXNS                                                      \
    &C6ACCEL_TI_IALG,         /* module ID */                         \
    NULL,                     /* activate */                          \
    C6ACCEL_TI_alloc,         /* alloc */                             \
    NULL,                     /* control (NULL => no control ops) */  \
    NULL,                     /* deactivate */                        \
    C6ACCEL_TI_free,          /* free */                              \
    C6ACCEL_TI_initObj,       /* init */                              \
    NULL,                     /* moved */                             \
    NULL                      /* numAlloc (NULL => IALG_MAXMEMRECS) */

/****************************************************************************+
 *           ======== C6ACCEL_TI_I C6ACCEL ========
 *  This structure defines TI's implementation of the IUNIVERSAL interface
 *  for the C6ACCEL_TI module.
 ***************************************************************************/
IUNIVERSAL_Fxns C6ACCEL_TI_IC6ACCEL = {
    {IALGFXNS},
    C6ACCEL_TI_process,
    C6ACCEL_TI_control,
};

/***************************************************************************+
 *           ======== C6ACCEL_TI_IALG ========
 *  This structure defines TI's implementation of the IALG interface
 *  for the C6ACCEL_TI module.
 ***************************************************************************/
#ifdef __TI_COMPILER_VERSION__
/* satisfy xDAIS symbol requirement without any overhead */
asm("_C6ACCEL_TI_IALG .set _C6ACCEL_TI_IC6ACCEL");

#else

/***************************************************************************+
 *  We duplicate the structure here to allow this code to be compiled and
 *  run using non-TI toolchains at the expense of unnecessary data space
 *  consumed by the definition below.
 **************************************************************************/
IALG_Fxns C6ACCEL_TI_IALG = {      /* module_vendor_interface */
    IALGFXNS
};

#endif

/*Standard iUniversal params defintion */
IUNIVERSAL_Params UNIVERSAL_TI_PARAMS = {
    sizeof(IUNIVERSAL_Params),       /* size */
};

/**************************************************************************+
 *  ========C6ACCEL_TI_alloc ========
 *  Return a table of memory descriptors that describe the memory needed
 *  to construct our object.
 **************************************************************************/
/* ARGSUSED - this line tells the TI compiler not to warn about unused args. */
Int C6ACCEL_TI_alloc(const IALG_Params *algParams,
    IALG_Fxns **pf, IALG_MemRec memTab[]){
   /* Request memory for my object */
    memTab[0].size = sizeof(C6ACCEL_TI_Obj);
    memTab[0].alignment = 0;
    memTab[0].space = IALG_EXTERNAL;
    memTab[0].attrs = IALG_PERSIST;

    return (1);
}

/*************************************************************************+
 *  ======== C6ACCEL_TI_free ========
 *  Return a table of memory pointers that should be freed.  Note
 *  that this should include *all* memory requested in the
 *  alloc operation above.
 *************************************************************************/
/* ARGSUSED - this line tells the TI compiler not to warn about unused args. */
Int C6ACCEL_TI_free(IALG_Handle handle, IALG_MemRec memTab[]){
    /*
     * Because our internal object size doesn't depend on create
     * params, we can just leverage the algAlloc() call with default
     * (NULL) create params.
     */

    return (C6ACCEL_TI_alloc(NULL, NULL, memTab));
}

/**************************************************************************+
 *  ======== C6ACCEL_TI_initObj ========
 *  Initialize the memory allocated on our behalf (including our object).
 ***************************************************************************/
/* ARGSUSED - this line tells the TI compiler not to warn about unused args. */
Int C6ACCEL_TI_initObj(IALG_Handle handle, const IALG_MemRec memTab[],
    IALG_Handle p, const IALG_Params *algParams){
    const IUNIVERSAL_Params *params = (IUNIVERSAL_Params *)algParams;

    if (params == NULL) {
        params = &UNIVERSAL_TI_PARAMS;
    }
    return (IALG_EOK);
}

/**********************************************************************************+
 *  ======== C6ACCEL_TI_process ========
 Process call that contains the Library functions to be executed in the application
 **********************************************************************************/
/* ARGSUSED - this line tells the TI compiler not to warn about unused args. */
XDAS_Int32 C6ACCEL_TI_process(IUNIVERSAL_Handle handle,
        XDM1_BufDesc *inBufs, XDM1_BufDesc *outBufs, XDM1_BufDesc *inOutBufs,
        IUNIVERSAL_InArgs *inArgs, IUNIVERSAL_OutArgs *outArgs)
{
  const IC6Accel_InArgs *CInArgs =(IC6Accel_InArgs *)inArgs;
  void   *pFnArray;
  Int32 fxnID;
  Int32 i,j;

  /* Mask Variables to extract information from Function IDs */
  Int32 vendorMsk= VENDOR_MASK;
  Int32 fxntypMsk= FXN_TYP_MASK;
  Int32 fxnidMsk= FXN_ID_MASK;

   /* Execute the chain of functions wrapped in the process API call  */
   for( i = 0; i < CInArgs->Num_fxns; i++ ){

   /* The pFnArray is at an offset in the inArgs structure and so does not need cache management*/
   pFnArray = (void*)((XDAS_Int8*)CInArgs + CInArgs->fxn[i].Param_ptr_offset);
   fxnID = CInArgs->fxn[i].FxnID;

   /*Check to see if NULL_INDEX has not been passed*/
   if (fxnID == NULL_INDEX){
       return(IUNIVERSAL_EFXNIDFAIL);
    }
   else{
       /* Pick libraries based on vendor */
       switch ((fxnID & vendorMsk)>>VENDOR_TAG_SHIFT){
          case (TI):
          /* Pick the Library function type (eg : IMGLIB function)  */
          switch (( fxnID & fxntypMsk)>>FXNTYP_TAG_SHIFT){
           /* DSPLIB functions */
             case (DSPLIB_FXN_ID):
             switch ( fxnID & fxnidMsk){
                 case (FFT16x16_FXN_ID):{
                   /* Unmarshal Parameters */
                   DSP_fft16x16_Params *C6ACCEL_TI_DSP_fft16x16_paramPtr;
                   C6ACCEL_TI_DSP_fft16x16_paramPtr= pFnArray;
                   /* Parameter check */
                   if (((C6ACCEL_TI_DSP_fft16x16_paramPtr->w_InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_DSP_fft16x16_paramPtr->x_InArrID2)>INBUF15)|
                       ((C6ACCEL_TI_DSP_fft16x16_paramPtr->y_OutArrID1)>OUTBUF15)|
                       ((C6ACCEL_TI_DSP_fft16x16_paramPtr->nx)<16)|
                       ((C6ACCEL_TI_DSP_fft16x16_paramPtr->nx)>65536)|
                       (IsPowOfTwo(C6ACCEL_TI_DSP_fft16x16_paramPtr->nx) != TRUE)){
                         return(IUNIVERSAL_EPARAMFAIL);
                    }
                    else 
                       /* Call underlying kernel */
                       DSP_fft16x16( (short *)inBufs->descs[C6ACCEL_TI_DSP_fft16x16_paramPtr->w_InArrID1].buf,
                                      C6ACCEL_TI_DSP_fft16x16_paramPtr->nx,
                                     (short *)inBufs->descs[C6ACCEL_TI_DSP_fft16x16_paramPtr->x_InArrID2].buf,
                                     (short *)outBufs->descs[C6ACCEL_TI_DSP_fft16x16_paramPtr->y_OutArrID1].buf);
                  }
                 break;
                 case (IFFT16x16_FXN_ID ):{
                    /* Unmarshal Parameters */  
                    DSP_ifft16x16_Params *C6ACCEL_TI_DSP_ifft16x16_paramPtr;
                    C6ACCEL_TI_DSP_ifft16x16_paramPtr= pFnArray;
                    /* Parameter check */
                    if (((C6ACCEL_TI_DSP_ifft16x16_paramPtr->w_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_DSP_ifft16x16_paramPtr->x_InArrID2)>INBUF15)|
                        ((C6ACCEL_TI_DSP_ifft16x16_paramPtr->y_OutArrID1)>OUTBUF15)|
                        ((C6ACCEL_TI_DSP_ifft16x16_paramPtr->nx)<16)|
                        ((C6ACCEL_TI_DSP_ifft16x16_paramPtr->nx)>65536)|
                        (IsPowOfTwo(C6ACCEL_TI_DSP_ifft16x16_paramPtr->nx) != TRUE)){
                           return(IUNIVERSAL_EPARAMFAIL);
                     }
                     else
                        /* Call underlying kernel */
                        DSP_ifft16x16((short *)inBufs->descs[C6ACCEL_TI_DSP_ifft16x16_paramPtr->w_InArrID1].buf,
                                     C6ACCEL_TI_DSP_ifft16x16_paramPtr->nx,
                                     (short *)inBufs->descs[C6ACCEL_TI_DSP_ifft16x16_paramPtr->x_InArrID2].buf,
                                     (short *)outBufs->descs[C6ACCEL_TI_DSP_ifft16x16_paramPtr->y_OutArrID1].buf);
                   }   
                 break;
                 case (MAT_MUL_FXN_ID ):{
                    /* Unmarshal Parameters */
                    DSP_mat_mul_Params  *C6ACCEL_TI_DSP_mat_mul_paramPtr;
                    C6ACCEL_TI_DSP_mat_mul_paramPtr= pFnArray;
                    /*Parameter check
                    * 1.InArr, OutArr ID checks
                    * 2.Check for constraints on matrix dimensions
                    * 3.Condition to check if col of Input 1 is equal to row of Input 2:
                    * (buf_size Input1)/(buf_size Input2) = (rows of Input1)/(cols of Input 2)
                    */
                    if (((C6ACCEL_TI_DSP_mat_mul_paramPtr->x_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_DSP_mat_mul_paramPtr->y_InArrID2)>INBUF15)|
                        ((C6ACCEL_TI_DSP_mat_mul_paramPtr->r_OutArrID1)>OUTBUF15)|
                        ((float)((inBufs->descs[C6ACCEL_TI_DSP_mat_mul_paramPtr->x_InArrID1].bufSize)/
                        (inBufs->descs[C6ACCEL_TI_DSP_mat_mul_paramPtr->y_InArrID2].bufSize))!=
                        ((float)(C6ACCEL_TI_DSP_mat_mul_paramPtr->r1/C6ACCEL_TI_DSP_mat_mul_paramPtr->c2)))|
                        ((C6ACCEL_TI_DSP_mat_mul_paramPtr->c2)<4)|((C6ACCEL_TI_DSP_mat_mul_paramPtr->c2 )% 4 != 0)|
                        ((C6ACCEL_TI_DSP_mat_mul_paramPtr->c1)<1)|((C6ACCEL_TI_DSP_mat_mul_paramPtr->r1)% 2 != 0)|
                        ((C6ACCEL_TI_DSP_mat_mul_paramPtr->r1)< 8)|((C6ACCEL_TI_DSP_mat_mul_paramPtr->c1)>32767)|
                        ((C6ACCEL_TI_DSP_mat_mul_paramPtr->c2)>32767)|((C6ACCEL_TI_DSP_mat_mul_paramPtr->r1)>32767)){
                          return(IUNIVERSAL_EPARAMFAIL);
                      }
                     else
                        /* Call underlying kernel */
                        DSP_mat_mul((short *)inBufs->descs[C6ACCEL_TI_DSP_mat_mul_paramPtr->x_InArrID1].buf,
                                      C6ACCEL_TI_DSP_mat_mul_paramPtr->r1,
                                      C6ACCEL_TI_DSP_mat_mul_paramPtr->c1,
                                      (short *)inBufs->descs[C6ACCEL_TI_DSP_mat_mul_paramPtr->y_InArrID2].buf,
                                      C6ACCEL_TI_DSP_mat_mul_paramPtr->c2,
                                      (short *)outBufs->descs[C6ACCEL_TI_DSP_mat_mul_paramPtr->r_OutArrID1].buf,
                                      C6ACCEL_TI_DSP_mat_mul_paramPtr->qs);
                    }
                 break;
                 case (FIR_GEN_FXN_ID):{
                    /* Unmarshal Parameters */
                    DSP_fir_gen_Params  *C6ACCEL_TI_DSP_fir_gen_paramPtr;
                    C6ACCEL_TI_DSP_fir_gen_paramPtr = pFnArray;
                    /* Parameter check */
                    if (((C6ACCEL_TI_DSP_fir_gen_paramPtr->x_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_DSP_fir_gen_paramPtr->h_InArrID2)>INBUF15)|
                        ((C6ACCEL_TI_DSP_fir_gen_paramPtr->r_OutArrID1)>OUTBUF15)|
                        ((C6ACCEL_TI_DSP_fir_gen_paramPtr->nr)%4 != 0)| ((C6ACCEL_TI_DSP_fir_gen_paramPtr->nr)<4)){
                           return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else
                       /* Call underlying kernel */
                        DSP_fir_gen((short *)inBufs->descs[C6ACCEL_TI_DSP_fir_gen_paramPtr->x_InArrID1].buf,
                                    (short *)inBufs->descs[C6ACCEL_TI_DSP_fir_gen_paramPtr->h_InArrID2].buf,
                                    (short *)outBufs->descs[C6ACCEL_TI_DSP_fir_gen_paramPtr->r_OutArrID1].buf,
                                    C6ACCEL_TI_DSP_fir_gen_paramPtr->nh,
                                    C6ACCEL_TI_DSP_fir_gen_paramPtr->nr);
                   }
                 break;
                 case (AUTOCOR_FXN_ID):{
                   /* Unmarshal Parameters */
                   DSP_autocor_Params *C6ACCEL_TI_DSP_autocor_paramPtr;
                   C6ACCEL_TI_DSP_autocor_paramPtr = pFnArray;
                   /* Parameter check */
                   if (((C6ACCEL_TI_DSP_autocor_paramPtr->x_InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_DSP_autocor_paramPtr->r_OutArrID1)>OUTBUF15)|
                       ((C6ACCEL_TI_DSP_autocor_paramPtr->nx)%8 != 0)|
                       ((C6ACCEL_TI_DSP_autocor_paramPtr->nr)%4 != 0)){
                         return(IUNIVERSAL_EPARAMFAIL);
                    }
                    else
                     /* Call underlying kernel */
                        DSP_autocor((short *)outBufs->descs[C6ACCEL_TI_DSP_autocor_paramPtr->r_OutArrID1].buf,
                                    (short *)inBufs->descs[C6ACCEL_TI_DSP_autocor_paramPtr->x_InArrID1].buf,
                                     C6ACCEL_TI_DSP_autocor_paramPtr->nx,
                                     C6ACCEL_TI_DSP_autocor_paramPtr->nr);
                  }
                 break;
                 case (FFT32x32_FXN_ID ):{
                    /* Unmarshal Parameters */
                    DSP_fft32x32_Params  *C6ACCEL_TI_DSP_fft32x32_paramPtr;
                    C6ACCEL_TI_DSP_fft32x32_paramPtr= pFnArray;
                    /* Parameter check*/
                    if (((C6ACCEL_TI_DSP_fft32x32_paramPtr->w_InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_DSP_fft32x32_paramPtr->x_InArrID2)>INBUF15)|
                       ((C6ACCEL_TI_DSP_fft32x32_paramPtr->y_OutArrID1)>OUTBUF15)|
                       ((C6ACCEL_TI_DSP_fft32x32_paramPtr->nx)<16)|
                       ((C6ACCEL_TI_DSP_fft32x32_paramPtr->nx)>65536)|
                       (IsPowOfTwo(C6ACCEL_TI_DSP_fft32x32_paramPtr->nx) != TRUE)){
                          return(IUNIVERSAL_EPARAMFAIL);
                    }
                    else
                     /* Call underlying kernel */
                       DSP_fft32x32((int *)inBufs->descs[C6ACCEL_TI_DSP_fft32x32_paramPtr->w_InArrID1].buf,
                                     C6ACCEL_TI_DSP_fft32x32_paramPtr->nx,
                                     (int *)inBufs->descs[C6ACCEL_TI_DSP_fft32x32_paramPtr->x_InArrID2].buf,
                                     (int *)outBufs->descs[C6ACCEL_TI_DSP_fft32x32_paramPtr->y_OutArrID1].buf);
                  } 
                 break;
                 case (IFFT32x32_FXN_ID):{
                    /* Unmarshal Parameters */
                    DSP_ifft32x32_Params *C6ACCEL_TI_DSP_ifft32x32_paramPtr;
                    C6ACCEL_TI_DSP_ifft32x32_paramPtr = pFnArray;
                    /* Parameter check */
                    if (((C6ACCEL_TI_DSP_ifft32x32_paramPtr->w_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_DSP_ifft32x32_paramPtr->x_InArrID2)>INBUF15)|
                        ((C6ACCEL_TI_DSP_ifft32x32_paramPtr->y_OutArrID1)>OUTBUF15)|
                        ((C6ACCEL_TI_DSP_ifft32x32_paramPtr->nx)<16)|
                        ((C6ACCEL_TI_DSP_ifft32x32_paramPtr->nx)>65536)|
                        (IsPowOfTwo(C6ACCEL_TI_DSP_ifft32x32_paramPtr->nx) != TRUE)){
                            return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else
                    /* Call underlying kernel */
                       DSP_ifft32x32((int *)inBufs->descs[C6ACCEL_TI_DSP_ifft32x32_paramPtr->w_InArrID1].buf,
                                      C6ACCEL_TI_DSP_ifft32x32_paramPtr->nx,
                                     (int *)inBufs->descs[C6ACCEL_TI_DSP_ifft32x32_paramPtr->x_InArrID2].buf,
                                     (int *)outBufs->descs[C6ACCEL_TI_DSP_ifft32x32_paramPtr->y_OutArrID1].buf);
                   }
                 break;
                 case (FIR_R4_FXN_ID):{
                    /* Unmarshal Parameters */
                    DSP_fir_r4_Params *C6ACCEL_TI_DSP_fir_r4_paramPtr;
                    C6ACCEL_TI_DSP_fir_r4_paramPtr = pFnArray;
                    /* Parameter check */
                    if (((C6ACCEL_TI_DSP_fir_r4_paramPtr->x_InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_DSP_fir_r4_paramPtr->h_InArrID2)>INBUF15)|
                       ((C6ACCEL_TI_DSP_fir_r4_paramPtr->r_OutArrID1)>OUTBUF15)|
                       ((C6ACCEL_TI_DSP_fir_r4_paramPtr->nh)%4 != 0)|
                       ((C6ACCEL_TI_DSP_fir_r4_paramPtr->nh) < 8)|
                       ((C6ACCEL_TI_DSP_fir_r4_paramPtr->nr)%4 != 0)){
                          return(IUNIVERSAL_EPARAMFAIL);
                    }
                    else
                    /* Call underlying kernel */
                      DSP_fir_r4((short *) inBufs->descs[C6ACCEL_TI_DSP_fir_r4_paramPtr->x_InArrID1].buf,
                                 (short *) inBufs->descs[C6ACCEL_TI_DSP_fir_r4_paramPtr->h_InArrID2].buf,
                                 (short *) outBufs->descs[C6ACCEL_TI_DSP_fir_r4_paramPtr->r_OutArrID1].buf,
                                 C6ACCEL_TI_DSP_fir_r4_paramPtr->nh,
                                 C6ACCEL_TI_DSP_fir_r4_paramPtr->nr);
                   }
                 break;
                 case (FIR_R8_FXN_ID):{
                    /* Unmarshal Parameters */
                    DSP_fir_r8_Params *C6ACCEL_TI_DSP_fir_r8_paramPtr;
                    C6ACCEL_TI_DSP_fir_r8_paramPtr = pFnArray;
                    /* Parameter check*/
                    if (((C6ACCEL_TI_DSP_fir_r8_paramPtr->x_InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_DSP_fir_r8_paramPtr->h_InArrID2)>INBUF15)|
                       ((C6ACCEL_TI_DSP_fir_r8_paramPtr->r_OutArrID1)>OUTBUF15)|
                       ((C6ACCEL_TI_DSP_fir_r8_paramPtr->nh)%8 != 0)|
                       ((C6ACCEL_TI_DSP_fir_r8_paramPtr->nh) < 8)|
                       ((C6ACCEL_TI_DSP_fir_r8_paramPtr->nr)%4 != 0)){
                         return(IUNIVERSAL_EPARAMFAIL);
                    }
                    else
                    /* Call underlying kernel */
                      DSP_fir_r8((short *) inBufs->descs[C6ACCEL_TI_DSP_fir_r8_paramPtr->x_InArrID1].buf,
                                 (short *) inBufs->descs[C6ACCEL_TI_DSP_fir_r8_paramPtr->h_InArrID2].buf,
                                 (short *) outBufs->descs[C6ACCEL_TI_DSP_fir_r8_paramPtr->r_OutArrID1].buf,
                                  C6ACCEL_TI_DSP_fir_r8_paramPtr->nh,
                                  C6ACCEL_TI_DSP_fir_r8_paramPtr->nr);
                  } 
                 break;
                 case (IIR_FXN_ID):{
                    /* Unmarshal Parameters */
                    DSP_iir_Params *C6ACCEL_TI_DSP_iir_paramPtr;
                    C6ACCEL_TI_DSP_iir_paramPtr = pFnArray;
                    /* Parameter check*/
                    if(((C6ACCEL_TI_DSP_iir_paramPtr->h_InArrID2)>INBUF15)|
                       ((C6ACCEL_TI_DSP_iir_paramPtr->b_InArrID3)>INBUF15)|
                       ((C6ACCEL_TI_DSP_iir_paramPtr->x_InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_DSP_iir_paramPtr->Ret_OutArrID1)>OUTBUF15)|
                       ((C6ACCEL_TI_DSP_iir_paramPtr->nh)%8 != 0)|
                       ((C6ACCEL_TI_DSP_iir_paramPtr->nh) < 8)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else
                    /* Call underlying kernel */
                       for (j = 0; j< C6ACCEL_TI_DSP_iir_paramPtr->n ; j++){
                         ((short *)outBufs->descs[C6ACCEL_TI_DSP_iir_paramPtr->Ret_OutArrID1].buf)[j] = DSP_iir(((short *)inBufs->descs[C6ACCEL_TI_DSP_iir_paramPtr->x_InArrID1].buf)[j],
                                                                                         (short *)inBufs->descs[C6ACCEL_TI_DSP_iir_paramPtr->h_InArrID2].buf,
                                                                                          C6ACCEL_TI_DSP_iir_paramPtr->nh,
                                                                                         (short *)inBufs->descs[C6ACCEL_TI_DSP_iir_paramPtr->b_InArrID3].buf);
                     }
                  }
                 break;
                 case (DOTP_SQR_FXN_ID):{
                    /* Unmarshal Parameters */
                    DSP_dotp_sqr_Params *C6ACCEL_TI_DSP_dotp_sqr_paramPtr;
                    C6ACCEL_TI_DSP_dotp_sqr_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_DSP_dotp_sqr_paramPtr->x_InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_DSP_dotp_sqr_paramPtr->y_InArrID2)>INBUF15)|
                       ((C6ACCEL_TI_DSP_dotp_sqr_paramPtr->r_OutArrID1)>OUTBUF15)|
                       ((C6ACCEL_TI_DSP_dotp_sqr_paramPtr->Ret_outArrID2)>OUTBUF15)|
                       ((C6ACCEL_TI_DSP_dotp_sqr_paramPtr->nx)%4 != 0)|
                       ((C6ACCEL_TI_DSP_dotp_sqr_paramPtr->nx) < 12)){
                          return(IUNIVERSAL_EPARAMFAIL);
                     }
                     else
                        /* Call underlying kernel */
                        ((int *)outBufs->descs[C6ACCEL_TI_DSP_dotp_sqr_paramPtr->Ret_outArrID2].buf)[0] =
                        DSP_dotp_sqr(C6ACCEL_TI_DSP_dotp_sqr_paramPtr->G,
                                     (short *)inBufs->descs[C6ACCEL_TI_DSP_dotp_sqr_paramPtr->x_InArrID1].buf,
                                     (short *)inBufs->descs[C6ACCEL_TI_DSP_dotp_sqr_paramPtr->y_InArrID2].buf,
                                     (int *)outBufs->descs[C6ACCEL_TI_DSP_dotp_sqr_paramPtr->r_OutArrID1].buf,
                                     C6ACCEL_TI_DSP_dotp_sqr_paramPtr->nx);
                  }
                 break;
                 case (DOTPROD_FXN_ID):{
                    /* Unmarshal Parameters */
                    DSP_dotprod_Params *C6ACCEL_TI_DSP_dotprod_paramPtr;
                    C6ACCEL_TI_DSP_dotprod_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_DSP_dotprod_paramPtr->x_InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_DSP_dotprod_paramPtr->y_InArrID2)>INBUF15)|
                       ((C6ACCEL_TI_DSP_dotprod_paramPtr->nx)%4 != 0)){
                          return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else
                       /* Call underlying kernel */
                       ((int *)outBufs->descs[C6ACCEL_TI_DSP_dotprod_paramPtr->Ret_outArrID1].buf)[0] =
                       DSP_dotprod((short *)inBufs->descs[C6ACCEL_TI_DSP_dotprod_paramPtr->x_InArrID1].buf,
                                   (short *)inBufs->descs[C6ACCEL_TI_DSP_dotprod_paramPtr->y_InArrID2].buf,
                                    C6ACCEL_TI_DSP_dotprod_paramPtr->nx);
                  } 
                 break;
               #ifdef DEVICE_FLOAT
                 case (F_SP_FFTSPXSP_FXN_ID):{
                    /* Unmarshal Parameters */
                    DSPF_sp_fftSPxSP_Params  *C6ACCEL_TI_DSPF_sp_fftSPxSP_paramPtr;
                    C6ACCEL_TI_DSPF_sp_fftSPxSP_paramPtr = pFnArray;
                    /* Parameter check */
                    if (((C6ACCEL_TI_DSPF_sp_fftSPxSP_paramPtr->w_InArrID2)>INBUF15)|
                        ((C6ACCEL_TI_DSPF_sp_fftSPxSP_paramPtr->x_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_DSPF_sp_fftSPxSP_paramPtr->y_OutArrID1)>OUTBUF15)|
                        ((C6ACCEL_TI_DSPF_sp_fftSPxSP_paramPtr->brev_InArrID3)>INBUF15)|
                        ((C6ACCEL_TI_DSPF_sp_fftSPxSP_paramPtr->N)<8)|
                        ((C6ACCEL_TI_DSPF_sp_fftSPxSP_paramPtr->N)>65536)|
                        (IsPowOfTwo(C6ACCEL_TI_DSPF_sp_fftSPxSP_paramPtr->N) != TRUE)){
                          return(IUNIVERSAL_EPARAMFAIL);
                      }
                     else
                    /* Call underlying kernel */
                       DSPF_sp_fftSPxSP(C6ACCEL_TI_DSPF_sp_fftSPxSP_paramPtr->N,(float *)inBufs->descs[C6ACCEL_TI_DSPF_sp_fftSPxSP_paramPtr->x_InArrID1].buf,
                                     (float *)inBufs->descs[C6ACCEL_TI_DSPF_sp_fftSPxSP_paramPtr->w_InArrID2].buf,
                                     (float *)outBufs->descs[C6ACCEL_TI_DSPF_sp_fftSPxSP_paramPtr->y_OutArrID1].buf,
                                     (unsigned char *)outBufs->descs[C6ACCEL_TI_DSPF_sp_fftSPxSP_paramPtr->brev_InArrID3].buf,
                                     C6ACCEL_TI_DSPF_sp_fftSPxSP_paramPtr->n_min,
                                     C6ACCEL_TI_DSPF_sp_fftSPxSP_paramPtr->offset,
                                     C6ACCEL_TI_DSPF_sp_fftSPxSP_paramPtr->n_max);
                  }
                 break;
                 case (F_SP_IFFTSPXSP_FXN_ID ):{
                    /* Unmarshal Parameters */
                    DSPF_sp_ifftSPxSP_Params *C6ACCEL_TI_DSPF_sp_ifftSPxSP_paramPtr;
                    C6ACCEL_TI_DSPF_sp_ifftSPxSP_paramPtr = pFnArray;
                    /* Parameter check */
                    if (((C6ACCEL_TI_DSPF_sp_ifftSPxSP_paramPtr->w_InArrID2)>INBUF15)|
                        ((C6ACCEL_TI_DSPF_sp_ifftSPxSP_paramPtr->x_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_DSPF_sp_ifftSPxSP_paramPtr->y_OutArrID1)>OUTBUF15)|
                        ((C6ACCEL_TI_DSPF_sp_ifftSPxSP_paramPtr->brev_InArrID3)>INBUF15)|
                        ((C6ACCEL_TI_DSPF_sp_ifftSPxSP_paramPtr->N)<8)|
                        ((C6ACCEL_TI_DSPF_sp_ifftSPxSP_paramPtr->N)>65536)|
                        (IsPowOfTwo(C6ACCEL_TI_DSPF_sp_ifftSPxSP_paramPtr->N) != TRUE)){
                          return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else
                     /* Call underlying kernel */
                       DSPF_sp_ifftSPxSP(C6ACCEL_TI_DSPF_sp_ifftSPxSP_paramPtr->N,(float *)inBufs->descs[C6ACCEL_TI_DSPF_sp_ifftSPxSP_paramPtr->x_InArrID1].buf,
                                     (float *)inBufs->descs[C6ACCEL_TI_DSPF_sp_ifftSPxSP_paramPtr->w_InArrID2].buf,
                                     (float *)outBufs->descs[C6ACCEL_TI_DSPF_sp_ifftSPxSP_paramPtr->y_OutArrID1].buf,
                                     (unsigned char *)outBufs->descs[C6ACCEL_TI_DSPF_sp_ifftSPxSP_paramPtr->brev_InArrID3].buf,
                                     C6ACCEL_TI_DSPF_sp_ifftSPxSP_paramPtr->n_min,
                                     C6ACCEL_TI_DSPF_sp_ifftSPxSP_paramPtr->offset,
                                     C6ACCEL_TI_DSPF_sp_ifftSPxSP_paramPtr->n_max);
                  }
                 break;
                 case (F_SP_AUTOCOR_FXN_ID ):{
                    /* Unmarshal Parameters */
                    DSPF_sp_autocor_Params *C6ACCEL_TI_DSPF_sp_autocor_paramPtr;
                    C6ACCEL_TI_DSPF_sp_autocor_paramPtr = pFnArray;
                    /* Parameter check */
                    if (((C6ACCEL_TI_DSPF_sp_autocor_paramPtr->x_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_DSPF_sp_autocor_paramPtr->r_OutArrID1)>OUTBUF15)|
                        ((C6ACCEL_TI_DSPF_sp_autocor_paramPtr->nr)%4 != 0)|((C6ACCEL_TI_DSPF_sp_autocor_paramPtr->nr)<0)|
                        (IsPowOfTwo(C6ACCEL_TI_DSPF_sp_autocor_paramPtr->nx) != TRUE)|
                        ((C6ACCEL_TI_DSPF_sp_autocor_paramPtr->nx)<(C6ACCEL_TI_DSPF_sp_autocor_paramPtr->nr))){
                          return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else
                     /* Call underlying kernel */
                       DSPF_sp_autocor((float *)outBufs->descs[C6ACCEL_TI_DSPF_sp_autocor_paramPtr->r_OutArrID1].buf,
                                       (float *)inBufs->descs[C6ACCEL_TI_DSPF_sp_autocor_paramPtr->x_InArrID1].buf,
                                       C6ACCEL_TI_DSPF_sp_autocor_paramPtr->nx,
                                       C6ACCEL_TI_DSPF_sp_autocor_paramPtr->nr);
                  }
                 break;
                 case (F_SP_CONV_FXN_ID):{
                    /* Unmarshal Parameters */
                    DSPF_sp_convol_Params *C6ACCEL_TI_DSPF_sp_convol_paramPtr;
                    C6ACCEL_TI_DSPF_sp_convol_paramPtr = pFnArray;
                    /* Parameter check */
                    if (((C6ACCEL_TI_DSPF_sp_convol_paramPtr->x_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_DSPF_sp_convol_paramPtr->h_InArrID2)>INBUF15)|
                        ((C6ACCEL_TI_DSPF_sp_convol_paramPtr->y_OutArrID1)>OUTBUF15)|
                        (IsPowOfTwo(C6ACCEL_TI_DSPF_sp_convol_paramPtr->ny) != TRUE)|
                        ((C6ACCEL_TI_DSPF_sp_convol_paramPtr->ny)<0)|
                        (IsPowOfTwo(C6ACCEL_TI_DSPF_sp_convol_paramPtr->nh) != TRUE)|
                        ((C6ACCEL_TI_DSPF_sp_convol_paramPtr->nh)<0)){
                          return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else
                     /* Call underlying kernel */
                       DSPF_sp_convol((float *)inBufs->descs[C6ACCEL_TI_DSPF_sp_convol_paramPtr->x_InArrID1].buf,
                                      (float *)inBufs->descs[C6ACCEL_TI_DSPF_sp_convol_paramPtr->h_InArrID2].buf,
                                      (float *)outBufs->descs[C6ACCEL_TI_DSPF_sp_convol_paramPtr->y_OutArrID1].buf,
                                       C6ACCEL_TI_DSPF_sp_convol_paramPtr->nh,
                                       C6ACCEL_TI_DSPF_sp_convol_paramPtr->ny);
                  }
                 break;
                 case (F_SP_BIQUAD_FXN_ID):{
                    /* Unmarshal Parameters */
                    DSPF_sp_biquad_Params *C6ACCEL_TI_DSPF_sp_biquad_paramPtr;
                    C6ACCEL_TI_DSPF_sp_biquad_paramPtr = pFnArray;
                    /* Parameter check */
                    if (((C6ACCEL_TI_DSPF_sp_biquad_paramPtr->x_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_DSPF_sp_biquad_paramPtr->a_InArrID3)>INBUF15)|

                        ((C6ACCEL_TI_DSPF_sp_biquad_paramPtr->b_InArrID2)>INBUF15)|
                        ((C6ACCEL_TI_DSPF_sp_biquad_paramPtr->delay_InArrID4)>INBUF15)|
                        ((C6ACCEL_TI_DSPF_sp_biquad_paramPtr->y_OutArrID1)>OUTBUF15)|
                        ((inBufs->descs[C6ACCEL_TI_DSPF_sp_biquad_paramPtr->a_InArrID3].bufSize)!=3*sizeof(float))|
                        ((inBufs->descs[C6ACCEL_TI_DSPF_sp_biquad_paramPtr->b_InArrID2].bufSize)!=3*sizeof(float))|
                        (IsPowOfTwo(C6ACCEL_TI_DSPF_sp_biquad_paramPtr->n) != TRUE)){
                          return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else
                     /* Call underlying kernel */
                       DSPF_sp_biquad((float *)inBufs->descs[C6ACCEL_TI_DSPF_sp_biquad_paramPtr->x_InArrID1].buf,
                                      (float *)inBufs->descs[C6ACCEL_TI_DSPF_sp_biquad_paramPtr->b_InArrID2].buf,
                                      (float *)inBufs->descs[C6ACCEL_TI_DSPF_sp_biquad_paramPtr->a_InArrID3].buf,
                                      (float *)inBufs->descs[C6ACCEL_TI_DSPF_sp_biquad_paramPtr->delay_InArrID4].buf,
                                      (float *)outBufs->descs[C6ACCEL_TI_DSPF_sp_biquad_paramPtr->y_OutArrID1].buf,
                                       C6ACCEL_TI_DSPF_sp_biquad_paramPtr->n);
                  }
                 break;
                 case (F_SP_DOTPROD_FXN_ID):{
                    /* Unmarshal Parameters */
                    DSPF_sp_dotprod_Params *C6ACCEL_TI_DSPF_sp_dotprod_paramPtr;
                    C6ACCEL_TI_DSPF_sp_dotprod_paramPtr = pFnArray;
                    /* Parameter check */
                    if (((C6ACCEL_TI_DSPF_sp_dotprod_paramPtr->x_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_DSPF_sp_dotprod_paramPtr->y_InArrID2)>INBUF15)|
                        ((C6ACCEL_TI_DSPF_sp_dotprod_paramPtr->RetOut_OutArrID1)>OUTBUF15)|
                        (IsPowOfTwo(C6ACCEL_TI_DSPF_sp_dotprod_paramPtr->n) != TRUE)|
                        ((C6ACCEL_TI_DSPF_sp_dotprod_paramPtr->n)<0)){
                          return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else
                     /* Call underlying kernel */
                       ((float *)outBufs->descs[C6ACCEL_TI_DSPF_sp_dotprod_paramPtr->RetOut_OutArrID1].buf)[0]=
                       DSPF_sp_dotprod((float *)inBufs->descs[C6ACCEL_TI_DSPF_sp_dotprod_paramPtr->x_InArrID1].buf,
                                       (float *)inBufs->descs[C6ACCEL_TI_DSPF_sp_dotprod_paramPtr->y_InArrID2].buf,
                                        C6ACCEL_TI_DSPF_sp_dotprod_paramPtr->n);
                  }
                 break;
                 case (F_SP_FIR_GEN_FXN_ID):{
                    /* Unmarshal Parameters */
                    DSPF_sp_fir_gen_Params  *C6ACCEL_TI_DSPF_sp_fir_gen_paramPtr;
                    C6ACCEL_TI_DSPF_sp_fir_gen_paramPtr = pFnArray;
                    /* Parameter check */
                    if (((C6ACCEL_TI_DSPF_sp_fir_gen_paramPtr->x_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_DSPF_sp_fir_gen_paramPtr->h_InArrID2)>INBUF15)|
                        ((C6ACCEL_TI_DSPF_sp_fir_gen_paramPtr->y_OutArrID1)>OUTBUF15)|
                        ((C6ACCEL_TI_DSPF_sp_fir_gen_paramPtr->nh)%4 != 0)| ((C6ACCEL_TI_DSPF_sp_fir_gen_paramPtr->nh)<=0)|
                        ((C6ACCEL_TI_DSPF_sp_fir_gen_paramPtr->ny)%4 != 0)| ((C6ACCEL_TI_DSPF_sp_fir_gen_paramPtr->ny)<=0)){
                           return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else
                       /* Call underlying kernel */
                        DSPF_sp_fir_gen((float *)inBufs->descs[C6ACCEL_TI_DSPF_sp_fir_gen_paramPtr->x_InArrID1].buf,
                                        (float *)inBufs->descs[C6ACCEL_TI_DSPF_sp_fir_gen_paramPtr->h_InArrID2].buf,
                                        (float *)outBufs->descs[C6ACCEL_TI_DSPF_sp_fir_gen_paramPtr->y_OutArrID1].buf,
                                        C6ACCEL_TI_DSPF_sp_fir_gen_paramPtr->nh,
                                        C6ACCEL_TI_DSPF_sp_fir_gen_paramPtr->ny);
                  }
                 break;
                 case (F_SP_FIR_CPLX_FXN_ID):{
                    /* Unmarshal Parameters */
                    DSPF_sp_fir_cplx_Params  *C6ACCEL_TI_DSPF_sp_fir_cplx_paramPtr;
                    C6ACCEL_TI_DSPF_sp_fir_cplx_paramPtr = pFnArray;
                    /* Parameter check */
                    if (((C6ACCEL_TI_DSPF_sp_fir_cplx_paramPtr->x_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_DSPF_sp_fir_cplx_paramPtr->h_InArrID2)>INBUF15)|
                        ((C6ACCEL_TI_DSPF_sp_fir_cplx_paramPtr->y_OutArrID1)>OUTBUF15)|
                        ((inBufs->descs[C6ACCEL_TI_DSPF_sp_fir_cplx_paramPtr->h_InArrID2].bufSize)!=(2*(C6ACCEL_TI_DSPF_sp_fir_cplx_paramPtr->nh)*sizeof(float)))|
                        ((outBufs->descs[C6ACCEL_TI_DSPF_sp_fir_cplx_paramPtr->y_OutArrID1].bufSize)!=(2*(C6ACCEL_TI_DSPF_sp_fir_cplx_paramPtr->ny)*sizeof(float)))|
                        ((C6ACCEL_TI_DSPF_sp_fir_cplx_paramPtr->nh)<=0)|
                        ((C6ACCEL_TI_DSPF_sp_fir_cplx_paramPtr->ny)%2 != 0)| ((C6ACCEL_TI_DSPF_sp_fir_cplx_paramPtr->ny)<=0)){
                           return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else
                       /* Call underlying kernel */
                        DSPF_sp_fir_cplx((float *)(inBufs->descs[C6ACCEL_TI_DSPF_sp_fir_cplx_paramPtr->x_InArrID1].buf+ 2*sizeof(float)*((C6ACCEL_TI_DSPF_sp_fir_cplx_paramPtr->nh)-1)),
                                        (float *)inBufs->descs[C6ACCEL_TI_DSPF_sp_fir_cplx_paramPtr->h_InArrID2].buf,
                                        (float *)outBufs->descs[C6ACCEL_TI_DSPF_sp_fir_cplx_paramPtr->y_OutArrID1].buf,
                                        C6ACCEL_TI_DSPF_sp_fir_cplx_paramPtr->nh,
                                        C6ACCEL_TI_DSPF_sp_fir_cplx_paramPtr->ny);
                  }
                 break;
                 case (F_SP_IIR_FXN_ID):{
                    /* Unmarshal Parameters */
                    DSPF_sp_iir_Params *C6ACCEL_TI_DSPF_sp_iir_paramPtr;
                    C6ACCEL_TI_DSPF_sp_iir_paramPtr = pFnArray;
                    /* Parameter check*/
                    if(((C6ACCEL_TI_DSPF_sp_iir_paramPtr->x_InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_DSPF_sp_iir_paramPtr->hb_InArrID2)>INBUF15)|
                       ((C6ACCEL_TI_DSPF_sp_iir_paramPtr->ha_InArrID3)>INBUF15)|
                       ((C6ACCEL_TI_DSPF_sp_iir_paramPtr->y1_OutArrID1)>OUTBUF15)|
                       ((C6ACCEL_TI_DSPF_sp_iir_paramPtr->y2_OutArrID2)>OUTBUF15)|
                       (outBufs->descs[C6ACCEL_TI_DSPF_sp_iir_paramPtr->y1_OutArrID1].bufSize !=((C6ACCEL_TI_DSPF_sp_iir_paramPtr->n)+4)*sizeof(float))|
                       (outBufs->descs[C6ACCEL_TI_DSPF_sp_iir_paramPtr->y2_OutArrID2].bufSize !=(C6ACCEL_TI_DSPF_sp_iir_paramPtr->n)*sizeof(float))|
                       (inBufs->descs[C6ACCEL_TI_DSPF_sp_iir_paramPtr->x_InArrID1].bufSize !=((C6ACCEL_TI_DSPF_sp_iir_paramPtr->n)+4)*sizeof(float))|
                       (IsPowOfTwo(C6ACCEL_TI_DSPF_sp_iir_paramPtr->n) != TRUE)|
                       ((C6ACCEL_TI_DSPF_sp_iir_paramPtr->n) < 0)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else
                    /* Call underlying kernel */
                        DSPF_sp_iir((float *)outBufs->descs[C6ACCEL_TI_DSPF_sp_iir_paramPtr->y1_OutArrID1].buf,
                                    (float *)inBufs->descs[C6ACCEL_TI_DSPF_sp_iir_paramPtr->x_InArrID1].buf,
                                    (float *)outBufs->descs[C6ACCEL_TI_DSPF_sp_iir_paramPtr->y2_OutArrID2].buf,
                                    (float *)inBufs->descs[C6ACCEL_TI_DSPF_sp_iir_paramPtr->hb_InArrID2].buf,
                                    (float *)inBufs->descs[C6ACCEL_TI_DSPF_sp_iir_paramPtr->ha_InArrID3].buf,
                                    C6ACCEL_TI_DSPF_sp_iir_paramPtr->n);
                     
                  }
                 break;
                 case (F_SP_MAT_MUL_FXN_ID):{
                    /* Unmarshal Parameters */
                    DSPF_sp_mat_mul_Params  *C6ACCEL_TI_DSPF_sp_mat_mul_paramPtr;
                    C6ACCEL_TI_DSPF_sp_mat_mul_paramPtr= pFnArray;
                    /*Parameter check
                    * 1.InArr, OutArr ID checks
                    * 2.Check for constraints on matrix dimensions
                    * 3.Condition to check if col of Input 1 is equal to row of Input 2:
                    * (buf_size Input1)/(buf_size Input2) = (rows of Input1)/(cols of Input 2)
                    */
                    if (((C6ACCEL_TI_DSPF_sp_mat_mul_paramPtr->x1_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_DSPF_sp_mat_mul_paramPtr->x2_InArrID2)>INBUF15)|
                        ((C6ACCEL_TI_DSPF_sp_mat_mul_paramPtr->y_OutArrID1)>OUTBUF15)|
                        ((float)((inBufs->descs[C6ACCEL_TI_DSPF_sp_mat_mul_paramPtr->x1_InArrID1].bufSize)/
                        (inBufs->descs[C6ACCEL_TI_DSPF_sp_mat_mul_paramPtr->x2_InArrID2].bufSize))!=
                        ((float)(C6ACCEL_TI_DSPF_sp_mat_mul_paramPtr->r1/C6ACCEL_TI_DSPF_sp_mat_mul_paramPtr->c2)))){
                          return(IUNIVERSAL_EPARAMFAIL);
                      }
                     else
                        /* Call underlying kernel */
                        DSPF_sp_mat_mul((float *)inBufs->descs[C6ACCEL_TI_DSPF_sp_mat_mul_paramPtr->x1_InArrID1].buf,
                                        C6ACCEL_TI_DSPF_sp_mat_mul_paramPtr->r1,
                                        C6ACCEL_TI_DSPF_sp_mat_mul_paramPtr->c1,
                                        (float *)inBufs->descs[C6ACCEL_TI_DSPF_sp_mat_mul_paramPtr->x2_InArrID2].buf,
                                        C6ACCEL_TI_DSPF_sp_mat_mul_paramPtr->c2,
                                        (float *)outBufs->descs[C6ACCEL_TI_DSPF_sp_mat_mul_paramPtr->y_OutArrID1].buf);
                    }
                 break;
                 case (F_SP_MAT_MUL_CPLX_FXN_ID):{
                    /* Unmarshal Parameters */
                    DSPF_sp_mat_mul_cplx_Params  *C6ACCEL_TI_DSPF_sp_mat_mul_cplx_paramPtr;
                    C6ACCEL_TI_DSPF_sp_mat_mul_cplx_paramPtr= pFnArray;
                    /*Parameter check
                    * 1.InArr, OutArr ID checks
                    * 2.Check for constraints on matrix dimensions
                    * 3.Condition to check if col of Input 1 is equal to row of Input 2:
                    * (buf_size Input1)/(buf_size Input2) = (rows of Input1)/(cols of Input 2)
                    */
                    if (((C6ACCEL_TI_DSPF_sp_mat_mul_cplx_paramPtr->x1_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_DSPF_sp_mat_mul_cplx_paramPtr->x2_InArrID2)>INBUF15)|
                        ((C6ACCEL_TI_DSPF_sp_mat_mul_cplx_paramPtr->y_OutArrID1)>OUTBUF15)|
                        ((float)((inBufs->descs[C6ACCEL_TI_DSPF_sp_mat_mul_cplx_paramPtr->x1_InArrID1].bufSize)/
                        (inBufs->descs[C6ACCEL_TI_DSPF_sp_mat_mul_cplx_paramPtr->x2_InArrID2].bufSize))!=
                        ((float)(C6ACCEL_TI_DSPF_sp_mat_mul_cplx_paramPtr->r1/C6ACCEL_TI_DSPF_sp_mat_mul_cplx_paramPtr->c2)))){
                          return(IUNIVERSAL_EPARAMFAIL);
                      }
                     else
                        /* Call underlying kernel */
                        DSPF_sp_mat_mul_cplx((float *)inBufs->descs[C6ACCEL_TI_DSPF_sp_mat_mul_cplx_paramPtr->x1_InArrID1].buf,
                                        C6ACCEL_TI_DSPF_sp_mat_mul_cplx_paramPtr->r1,
                                        C6ACCEL_TI_DSPF_sp_mat_mul_cplx_paramPtr->c1,
                                        (float *)inBufs->descs[C6ACCEL_TI_DSPF_sp_mat_mul_cplx_paramPtr->x2_InArrID2].buf,
                                        C6ACCEL_TI_DSPF_sp_mat_mul_cplx_paramPtr->c2,
                                        (float *)outBufs->descs[C6ACCEL_TI_DSPF_sp_mat_mul_cplx_paramPtr->y_OutArrID1].buf);
                    }
                 break;
                 case (F_SP_MAT_TRANS_FXN_ID):{
                    /* Unmarshal Parameters */
                    DSPF_sp_mat_trans_Params  *C6ACCEL_TI_DSPF_sp_mat_trans_paramPtr;
                    C6ACCEL_TI_DSPF_sp_mat_trans_paramPtr= pFnArray;
                    /*Parameter check
                    * 1.InArr, OutArr ID checks
                    * 2.Check for constraints on matrix dimensions
                    */
                    if (((C6ACCEL_TI_DSPF_sp_mat_trans_paramPtr->x_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_DSPF_sp_mat_trans_paramPtr->y_OutArrID1)>OUTBUF15)|
                        ((C6ACCEL_TI_DSPF_sp_mat_trans_paramPtr->rows)<2)|
                        ((C6ACCEL_TI_DSPF_sp_mat_trans_paramPtr->cols)<2)){
                          return(IUNIVERSAL_EPARAMFAIL);
                      }
                     else
                        /* Call underlying kernel */
                        DSPF_sp_mat_trans((float *)inBufs->descs[C6ACCEL_TI_DSPF_sp_mat_trans_paramPtr->x_InArrID1].buf,
                                          C6ACCEL_TI_DSPF_sp_mat_trans_paramPtr->rows,
                                          C6ACCEL_TI_DSPF_sp_mat_trans_paramPtr->cols,
                                          (float *)outBufs->descs[C6ACCEL_TI_DSPF_sp_mat_trans_paramPtr->y_OutArrID1].buf);
                   }
                 break;
                 case (F_SP_VECMUL_FXN_ID):{
                    /* Unmarshal Parameters */
                    DSPF_sp_vecmul_Params *C6ACCEL_TI_DSPF_sp_vecmul_paramPtr;
                    C6ACCEL_TI_DSPF_sp_vecmul_paramPtr = pFnArray;
                    /* Parameter check*/
                    if(((C6ACCEL_TI_DSPF_sp_vecmul_paramPtr->x1_InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_DSPF_sp_vecmul_paramPtr->x2_InArrID2)>INBUF15)|
                       ((C6ACCEL_TI_DSPF_sp_vecmul_paramPtr->y_OutArrID1)>OUTBUF15)|
                       (IsPowOfTwo(C6ACCEL_TI_DSPF_sp_vecmul_paramPtr->n) != TRUE)|
                       ((C6ACCEL_TI_DSPF_sp_vecmul_paramPtr->n) < 0)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else
                    /* Call underlying kernel */
                        DSPF_sp_vecmul((float *)inBufs->descs[C6ACCEL_TI_DSPF_sp_vecmul_paramPtr->x1_InArrID1].buf,
                                       (float *)inBufs->descs[C6ACCEL_TI_DSPF_sp_vecmul_paramPtr->x2_InArrID2].buf,
                                       (float *)outBufs->descs[C6ACCEL_TI_DSPF_sp_vecmul_paramPtr->y_OutArrID1].buf,
                                       C6ACCEL_TI_DSPF_sp_vecmul_paramPtr->n);
          
                  }
                 break;
                 case (F_SP_VECRECIP_FXN_ID):{
                    /* Unmarshal Parameters */
                    DSPF_sp_vecrecip_Params *C6ACCEL_TI_DSPF_sp_vecrecip_paramPtr;
                    C6ACCEL_TI_DSPF_sp_vecrecip_paramPtr = pFnArray;
                    /* Parameter check*/
                    if(((C6ACCEL_TI_DSPF_sp_vecrecip_paramPtr->x_InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_DSPF_sp_vecrecip_paramPtr->y_OutArrID1)>OUTBUF15)|
                       (IsPowOfTwo(C6ACCEL_TI_DSPF_sp_vecrecip_paramPtr->n) != TRUE)|
                       ((C6ACCEL_TI_DSPF_sp_vecrecip_paramPtr->n) < 0)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else
                    /* Call underlying kernel */
                        DSPF_sp_vecrecip((float *)inBufs->descs[C6ACCEL_TI_DSPF_sp_vecrecip_paramPtr->x_InArrID1].buf,
                                         (float *)outBufs->descs[C6ACCEL_TI_DSPF_sp_vecrecip_paramPtr->y_OutArrID1].buf,
                                         C6ACCEL_TI_DSPF_sp_vecrecip_paramPtr->n);
          
                  }
                 break;
                 case (F_SP_VECSUM_SQ_FXN_ID):{
                    /* Unmarshal Parameters */
                    DSPF_sp_vecsum_sq_Params *C6ACCEL_TI_DSPF_sp_vecsum_sq_paramPtr;
                    C6ACCEL_TI_DSPF_sp_vecsum_sq_paramPtr = pFnArray;
                    /* Parameter check*/
                    if(((C6ACCEL_TI_DSPF_sp_vecsum_sq_paramPtr->x_InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_DSPF_sp_vecsum_sq_paramPtr->OutRet_OutArrID1)>OUTBUF15)|
                       ((C6ACCEL_TI_DSPF_sp_vecsum_sq_paramPtr->n) < 0)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else
                    /* Call underlying kernel */
                         ((float *)outBufs->descs[C6ACCEL_TI_DSPF_sp_vecsum_sq_paramPtr->OutRet_OutArrID1].buf)[0]=
                         DSPF_sp_vecsum_sq((float *)inBufs->descs[C6ACCEL_TI_DSPF_sp_vecsum_sq_paramPtr->x_InArrID1].buf,
                                            C6ACCEL_TI_DSPF_sp_vecsum_sq_paramPtr->n);
          
                  }
                 break;
                 case (F_SP_W_VEC_FXN_ID):{
                    /* Unmarshal Parameters */
                    DSPF_sp_w_vec_Params *C6ACCEL_TI_DSPF_sp_w_vec_paramPtr;
                    C6ACCEL_TI_DSPF_sp_w_vec_paramPtr = pFnArray;
                    /* Parameter check*/
                    if(((C6ACCEL_TI_DSPF_sp_w_vec_paramPtr->x1_InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_DSPF_sp_w_vec_paramPtr->x2_InArrID2)>INBUF15)|
                       ((C6ACCEL_TI_DSPF_sp_w_vec_paramPtr->y_OutArrID1)>OUTBUF15)|
                       ((C6ACCEL_TI_DSPF_sp_w_vec_paramPtr->n) < 0)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else
                    /* Call underlying kernel */
                         DSPF_sp_w_vec((float *)inBufs->descs[C6ACCEL_TI_DSPF_sp_w_vec_paramPtr->x1_InArrID1].buf,
                                       (float *)inBufs->descs[C6ACCEL_TI_DSPF_sp_w_vec_paramPtr->x2_InArrID2].buf,
                                       C6ACCEL_TI_DSPF_sp_w_vec_paramPtr->m,
                                       (float *)outBufs->descs[C6ACCEL_TI_DSPF_sp_w_vec_paramPtr->y_OutArrID1].buf,
                                       C6ACCEL_TI_DSPF_sp_w_vec_paramPtr->n);
          
                  }
                 break;
               #endif
                 default:
                   /*Error caused due to passing of an invalid ID*/
                      return(IUNIVERSAL_EFXNIDFAIL);
               }
             break;
             case (IMGLIB_FXN_ID):
                /* IMGLIB functions */
               switch ( fxnID & fxnidMsk){
                 case (SOBEL_3X3_8_FXN_ID):{
                    /* Unmarshal Parameters */
                    IMG_sobel_3x3_8_Params *C6ACCEL_TI_IMG_sobel_3x3_8_paramPtr;
                    C6ACCEL_TI_IMG_sobel_3x3_8_paramPtr= pFnArray;
                    /*Parameter check*/
                    if (((C6ACCEL_TI_IMG_sobel_3x3_8_paramPtr->indata_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_IMG_sobel_3x3_8_paramPtr->outdata_OutArrID1)>OUTBUF15)|
                        ((C6ACCEL_TI_IMG_sobel_3x3_8_paramPtr->Col)%2 != 0)|
                        (((C6ACCEL_TI_IMG_sobel_3x3_8_paramPtr->Col)*((C6ACCEL_TI_IMG_sobel_3x3_8_paramPtr->Row)-2))%8 !=0)){
                           return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else
                    /* Call underlying kernel */
                       IMG_sobel_3x3_8((const unsigned char *)inBufs->descs[C6ACCEL_TI_IMG_sobel_3x3_8_paramPtr->indata_InArrID1].buf,
                                       (unsigned char *)outBufs->descs[C6ACCEL_TI_IMG_sobel_3x3_8_paramPtr->outdata_OutArrID1].buf,
                                        C6ACCEL_TI_IMG_sobel_3x3_8_paramPtr->Col,
                                        C6ACCEL_TI_IMG_sobel_3x3_8_paramPtr->Row);
                     }
                 break;
		 /* This case is added by Pramod */
		 case (SOBEL_3X3_8_16_FXN_ID):{
                    /* Unmarshal Parameters */
                    IMG_sobel_3x3_8_16_Params *C6ACCEL_TI_IMG_sobel_3x3_8_16_paramPtr;
                    C6ACCEL_TI_IMG_sobel_3x3_8_16_paramPtr= pFnArray;
                    /*Parameter check*/
                    if (((C6ACCEL_TI_IMG_sobel_3x3_8_16_paramPtr->indata_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_IMG_sobel_3x3_8_16_paramPtr->outdata_OutArrID1)>OUTBUF15)|
                        ((C6ACCEL_TI_IMG_sobel_3x3_8_16_paramPtr->Col)%2 != 0)|
                        (((C6ACCEL_TI_IMG_sobel_3x3_8_16_paramPtr->Col)*((C6ACCEL_TI_IMG_sobel_3x3_8_16_paramPtr->Row)-2))%8 !=0)){
                           return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else
                    /* Call underlying kernel */
                       IMG_sobel_3x3_8_16((const unsigned char *)inBufs->descs[C6ACCEL_TI_IMG_sobel_3x3_8_16_paramPtr->indata_InArrID1].buf,
                                       (unsigned short *)outBufs->descs[C6ACCEL_TI_IMG_sobel_3x3_8_16_paramPtr->outdata_OutArrID1].buf,
                                        C6ACCEL_TI_IMG_sobel_3x3_8_16_paramPtr->Col,
                                        C6ACCEL_TI_IMG_sobel_3x3_8_16_paramPtr->Row);
                     }
                 break;
                 case (SOBEL_3X3_16_FXN_ID):{
                    /* Unmarshal Parameters */
                    IMG_sobel_3x3_16_Params *C6ACCEL_TI_IMG_sobel_3x3_16_paramPtr;
                    C6ACCEL_TI_IMG_sobel_3x3_16_paramPtr = pFnArray;
                    /*Parameter check*/
                    if(((C6ACCEL_TI_IMG_sobel_3x3_16_paramPtr->indata_InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IMG_sobel_3x3_16_paramPtr->outdata_OutArrID1)>OUTBUF15)|
                       ((C6ACCEL_TI_IMG_sobel_3x3_16_paramPtr->Col)%2 != 0)|
                       ((C6ACCEL_TI_IMG_sobel_3x3_16_paramPtr->Row) < 3)|
                       (((C6ACCEL_TI_IMG_sobel_3x3_16_paramPtr->Col)*((C6ACCEL_TI_IMG_sobel_3x3_16_paramPtr->Row)-2))%8 !=0)){
                          return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else
                    /* Call underlying kernel */
                        IMG_sobel_3x3_16((const unsigned short *)inBufs->descs[C6ACCEL_TI_IMG_sobel_3x3_16_paramPtr->indata_InArrID1].buf,
                                         (unsigned short *)outBufs->descs[C6ACCEL_TI_IMG_sobel_3x3_16_paramPtr->outdata_OutArrID1].buf,
                                          C6ACCEL_TI_IMG_sobel_3x3_16_paramPtr->Col,
                                          C6ACCEL_TI_IMG_sobel_3x3_16_paramPtr->Row);
                    }
                 break;
                 case (HISTOGRAM_8_FXN_ID):{
                    /* Unmarshal Parameters */
                    IMG_histogram_8_Params *C6ACCEL_TI_IMG_histogram_8_paramPtr;
                    C6ACCEL_TI_IMG_histogram_8_paramPtr = pFnArray;
                    /*Parameter check*/
                    if (((C6ACCEL_TI_IMG_histogram_8_paramPtr->indata_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_IMG_histogram_8_paramPtr->t_hist_OutArrID1)>OUTBUF15)|
                        ((C6ACCEL_TI_IMG_histogram_8_paramPtr->hist_OutArrID2)>OUTBUF15)|
                        ((C6ACCEL_TI_IMG_histogram_8_paramPtr->n)%8 != 0)|
                         (outBufs->descs[C6ACCEL_TI_IMG_histogram_8_paramPtr->hist_OutArrID2].bufSize<(256*sizeof(short)))|
                         (outBufs->descs[C6ACCEL_TI_IMG_histogram_8_paramPtr->t_hist_OutArrID1].bufSize<(1024*sizeof(short)))){
                             return(IUNIVERSAL_EPARAMFAIL);
                     }
                     else
                     /* Call underlying kernel */
                        IMG_histogram_8((unsigned char *)inBufs->descs[C6ACCEL_TI_IMG_histogram_8_paramPtr->indata_InArrID1].buf,
                                         C6ACCEL_TI_IMG_histogram_8_paramPtr->n,
                                         C6ACCEL_TI_IMG_histogram_8_paramPtr->accumulate,
                                         (short *)outBufs->descs[C6ACCEL_TI_IMG_histogram_8_paramPtr->t_hist_OutArrID1].buf,
                                         (short *)outBufs->descs[C6ACCEL_TI_IMG_histogram_8_paramPtr->hist_OutArrID2].buf);

                    }
                 break;
                 case (HISTOGRAM_16_FXN_ID ):{
                    /* Unmarshal Parameters */
                    IMG_histogram_16_Params *C6ACCEL_TI_IMG_histogram_16_paramPtr;
                    C6ACCEL_TI_IMG_histogram_16_paramPtr = pFnArray;
                    /* Parameter check */
                    if (((C6ACCEL_TI_IMG_histogram_16_paramPtr->in_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_IMG_histogram_16_paramPtr->t_hist_OutArrID1)>OUTBUF15)|
                        ((C6ACCEL_TI_IMG_histogram_16_paramPtr->hist_OutArrID2)>OUTBUF15)|
                        ((C6ACCEL_TI_IMG_histogram_16_paramPtr->n)%8 != 0)|
                        ((C6ACCEL_TI_IMG_histogram_16_paramPtr->img_bits) <1)){
                          return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else
                    /* Call underlying kernel */
                       IMG_histogram_16_c((unsigned short*)inBufs->descs[C6ACCEL_TI_IMG_histogram_16_paramPtr->in_InArrID1].buf,
                                           C6ACCEL_TI_IMG_histogram_16_paramPtr->n,
                                           C6ACCEL_TI_IMG_histogram_16_paramPtr->accumulate,
                                          (short *)outBufs->descs[C6ACCEL_TI_IMG_histogram_16_paramPtr->t_hist_OutArrID1].buf,
                                          (short *)outBufs->descs[C6ACCEL_TI_IMG_histogram_16_paramPtr->hist_OutArrID2].buf,
                                           C6ACCEL_TI_IMG_histogram_16_paramPtr->img_bits);
                    }
                 break;
                 case (MEDIAN_3X3_8_FXN_ID):{
                    /* Unmarshal Parameters */
                    IMG_median_3x3_8_Params *C6ACCEL_TI_IMG_median_3x3_8_paramPtr;
                    C6ACCEL_TI_IMG_median_3x3_8_paramPtr = pFnArray;
                    /*Parameter check*/
                    if(((C6ACCEL_TI_IMG_median_3x3_8_paramPtr->InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IMG_median_3x3_8_paramPtr->OutArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IMG_median_3x3_8_paramPtr->Col)%4 != 0)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else
                    /* Call underlying kernel */
                       IMG_median_3x3_8((unsigned char *)inBufs->descs[C6ACCEL_TI_IMG_median_3x3_8_paramPtr->InArrID1].buf,
                                        C6ACCEL_TI_IMG_median_3x3_8_paramPtr->Col,
                                        (unsigned char *)outBufs->descs[C6ACCEL_TI_IMG_median_3x3_8_paramPtr->OutArrID1].buf);
                    }
                 break;
                 case (MEDIAN_3X3_16_FXN_ID):{
                    /* Unmarshal Parameters */
                    IMG_median_3x3_16_Params *C6ACCEL_TI_IMG_median_3x3_16_paramPtr;
                    C6ACCEL_TI_IMG_median_3x3_16_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_IMG_median_3x3_16_paramPtr->idata_InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IMG_median_3x3_16_paramPtr->odata_OutArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IMG_median_3x3_16_paramPtr->nwidth) < 4)|
                       ((C6ACCEL_TI_IMG_median_3x3_16_paramPtr->nwidth)%4 != 0)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else
                    /* Call underlying kernel */
                       IMG_median_3x3_16((const short *)inBufs->descs[C6ACCEL_TI_IMG_median_3x3_16_paramPtr->idata_InArrID1].buf,
                                          C6ACCEL_TI_IMG_median_3x3_16_paramPtr->nwidth,
                                         (short *)outBufs->descs[C6ACCEL_TI_IMG_median_3x3_16_paramPtr->odata_OutArrID1].buf);
                    }
                  break;
                  case (MEDIAN_3X3_16S_FXN_ID):{
                     /* Unmarshal Parameters */
                     IMG_median_3x3_16s_Params *C6ACCEL_TI_IMG_median_3x3_16s_paramPtr;
                     C6ACCEL_TI_IMG_median_3x3_16s_paramPtr  = pFnArray;
                     /* Parameter check */
                     if (((C6ACCEL_TI_IMG_median_3x3_16s_paramPtr->idata_InArrID1)>INBUF15)|
                         ((C6ACCEL_TI_IMG_median_3x3_16s_paramPtr->odata_OutArrID1)>INBUF15)|
                         ((C6ACCEL_TI_IMG_median_3x3_16s_paramPtr->nwidth) < 4)|
                         ((C6ACCEL_TI_IMG_median_3x3_16s_paramPtr->nwidth) % 4 != 0)){
                           return(IUNIVERSAL_EPARAMFAIL);
                      }
                     else
                     /* Call underlying kernel */
                        IMG_median_3x3_16s((const short *)inBufs->descs[C6ACCEL_TI_IMG_median_3x3_16s_paramPtr->idata_InArrID1].buf,
                                           C6ACCEL_TI_IMG_median_3x3_16s_paramPtr->nwidth,
                                           (short *)outBufs->descs[C6ACCEL_TI_IMG_median_3x3_16s_paramPtr->odata_OutArrID1].buf);
                    }
                  break;
                  case (MEDIAN_3X3_8_FRAME_FXN_ID):{
                     /* Unmarshal Parameters */
                     IMG_median_3x3_8_Frame_Params *C6ACCEL_TI_IMG_median_3x3_8_Frame_paramPtr;
                     C6ACCEL_TI_IMG_median_3x3_8_Frame_paramPtr = pFnArray;
                     /*Parameter check*/
                     if(((C6ACCEL_TI_IMG_median_3x3_8_Frame_paramPtr->InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_IMG_median_3x3_8_Frame_paramPtr->OutArrID1)>INBUF15)|
                        ((C6ACCEL_TI_IMG_median_3x3_8_Frame_paramPtr->Row) <= 0)|
                        ((C6ACCEL_TI_IMG_median_3x3_8_Frame_paramPtr->Col)%4 != 0)){
                          return(IUNIVERSAL_EPARAMFAIL);
                      }
                     else{ // Use Row to loop through image line by line using existing DSP API
                         int r;
                         /* Call underlying kernel */
                         unsigned char *pInChar=(unsigned char*)(inBufs->descs[C6ACCEL_TI_IMG_median_3x3_8_Frame_paramPtr->InArrID1].buf);
                         unsigned char *pOutChar=(unsigned char*)(outBufs->descs[C6ACCEL_TI_IMG_median_3x3_8_Frame_paramPtr->OutArrID1].buf);
                         for(r=0;r<C6ACCEL_TI_IMG_median_3x3_8_Frame_paramPtr->Row;r++){
                             IMG_median_3x3_8(pInChar+r*C6ACCEL_TI_IMG_median_3x3_8_Frame_paramPtr->Col,
                                          C6ACCEL_TI_IMG_median_3x3_8_Frame_paramPtr->Col,
                                          pOutChar+r*C6ACCEL_TI_IMG_median_3x3_8_Frame_paramPtr->Col);
                          }
                      }
                   }
                  break;
                  case (MEDIAN_3X3_16_FRAME_FXN_ID):{
                    /* Unmarshal Parameters */
                    IMG_median_3x3_16_Frame_Params *C6ACCEL_TI_IMG_median_3x3_16_Frame_paramPtr;
                    C6ACCEL_TI_IMG_median_3x3_16_Frame_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_IMG_median_3x3_16_Frame_paramPtr->idata_InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IMG_median_3x3_16_Frame_paramPtr->odata_OutArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IMG_median_3x3_16_Frame_paramPtr->Row) <= 0)|
                       ((C6ACCEL_TI_IMG_median_3x3_16_Frame_paramPtr->nwidth) < 4)|
                       ((C6ACCEL_TI_IMG_median_3x3_16_Frame_paramPtr->nwidth)%4 != 0)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                     else
                      { // Use Row to loop through image line by line using existing DSP API
                        int r;
                        short *pInShort=(short*)(inBufs->descs[C6ACCEL_TI_IMG_median_3x3_16_Frame_paramPtr->idata_InArrID1].buf);
                        short *pOutShort=(short*)(outBufs->descs[C6ACCEL_TI_IMG_median_3x3_16_Frame_paramPtr->odata_OutArrID1].buf);
                        /* Call underlying kernel */
                        for(r=0;r<C6ACCEL_TI_IMG_median_3x3_16_Frame_paramPtr->Row;r++){
                            IMG_median_3x3_16(pInShort+r*C6ACCEL_TI_IMG_median_3x3_16_Frame_paramPtr->nwidth,
                                              C6ACCEL_TI_IMG_median_3x3_16_Frame_paramPtr->nwidth,
                                              pOutShort+r*C6ACCEL_TI_IMG_median_3x3_16_Frame_paramPtr->nwidth);
                          }
                       }
                    }
                  break;
                  case (MEDIAN_3X3_16S_FRAME_FXN_ID):{
                     /* Unmarshal Parameters */
                     IMG_median_3x3_16s_Frame_Params *C6ACCEL_TI_IMG_median_3x3_16s_Frame_paramPtr;
                     C6ACCEL_TI_IMG_median_3x3_16s_Frame_paramPtr  = pFnArray;
                     /* Parameter check */
                     if (((C6ACCEL_TI_IMG_median_3x3_16s_Frame_paramPtr->idata_InArrID1)>INBUF15)|
                         ((C6ACCEL_TI_IMG_median_3x3_16s_Frame_paramPtr->odata_OutArrID1)>INBUF15)|
                         ((C6ACCEL_TI_IMG_median_3x3_16s_Frame_paramPtr->Row) <= 0)|
                         ((C6ACCEL_TI_IMG_median_3x3_16s_Frame_paramPtr->nwidth) < 4)|
                         ((C6ACCEL_TI_IMG_median_3x3_16s_Frame_paramPtr->nwidth) % 4 != 0)){
                           return(IUNIVERSAL_EPARAMFAIL);
                      }
                     else
                       { // Use Row to loop through image line by line using existing DSP API
                        int r;
                        short *pInShort=(short*)(inBufs->descs[C6ACCEL_TI_IMG_median_3x3_16s_Frame_paramPtr->idata_InArrID1].buf);
                        short *pOutShort=(short*)(outBufs->descs[C6ACCEL_TI_IMG_median_3x3_16s_Frame_paramPtr->odata_OutArrID1].buf);
                        /* Call underlying kernel */
                        for(r=0;r<C6ACCEL_TI_IMG_median_3x3_16s_Frame_paramPtr->Row;r++){
                           IMG_median_3x3_16s( pInShort+r*C6ACCEL_TI_IMG_median_3x3_16s_Frame_paramPtr->nwidth,
                                               C6ACCEL_TI_IMG_median_3x3_16s_Frame_paramPtr->nwidth,
                                               pOutShort+r*C6ACCEL_TI_IMG_median_3x3_16s_Frame_paramPtr->nwidth);
                        }
                     }
                   }
                 break;
                 case (CONV_3X3_I8_C8S_FXN_ID):{
                    /* Unmarshal Parameters */
                    IMG_conv_3x3_i8_c8s_Params *C6ACCEL_TI_IMG_conv_3x3_i8_c8s_paramPtr;
                    C6ACCEL_TI_IMG_conv_3x3_i8_c8s_paramPtr = pFnArray;
                    /*Parameter check*/
                    if(((C6ACCEL_TI_IMG_conv_3x3_i8_c8s_paramPtr->in_data_InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IMG_conv_3x3_i8_c8s_paramPtr->out_data_OutArrID1)>OUTBUF15)|
                       ((C6ACCEL_TI_IMG_conv_3x3_i8_c8s_paramPtr->mask_InArrID2)>INBUF15)|
                       ((C6ACCEL_TI_IMG_conv_3x3_i8_c8s_paramPtr->cols)%8 != 0)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else
                    /* Call underlying kernel */
                       IMG_conv_3x3_i8_c8s((unsigned char *)inBufs->descs[C6ACCEL_TI_IMG_conv_3x3_i8_c8s_paramPtr->in_data_InArrID1].buf,
                                           (unsigned char *)outBufs->descs[C6ACCEL_TI_IMG_conv_3x3_i8_c8s_paramPtr->out_data_OutArrID1].buf,
                                           C6ACCEL_TI_IMG_conv_3x3_i8_c8s_paramPtr->cols,
                                           (const char *)inBufs->descs[C6ACCEL_TI_IMG_conv_3x3_i8_c8s_paramPtr->mask_InArrID2].buf,
                                           C6ACCEL_TI_IMG_conv_3x3_i8_c8s_paramPtr->shift);
                     }
                  break;
                  case (CONV_3X3_I16S_C16S_FXN_ID):{
                     /* Unmarshal Parameters */
                     IMG_conv_3x3_i16s_c16s_Params *C6ACCEL_TI_IMG_conv_3x3_i16s_c16s_paramPtr;
                     C6ACCEL_TI_IMG_conv_3x3_i16s_c16s_paramPtr = pFnArray;
                     /* Parameter check */
                     if(((C6ACCEL_TI_IMG_conv_3x3_i16s_c16s_paramPtr->in_data_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_IMG_conv_3x3_i16s_c16s_paramPtr->out_data_OutArrID1)>OUTBUF15)|
                        ((C6ACCEL_TI_IMG_conv_3x3_i16s_c16s_paramPtr->mask_InArrID2)>INBUF15)|
                        ((C6ACCEL_TI_IMG_conv_3x3_i16s_c16s_paramPtr->pitch)<(C6ACCEL_TI_IMG_conv_3x3_i16s_c16s_paramPtr->width))|
                        ((C6ACCEL_TI_IMG_conv_3x3_i16s_c16s_paramPtr->width)< 2)|
                        ((C6ACCEL_TI_IMG_conv_3x3_i16s_c16s_paramPtr->width)%2 != 0)){
                          return(IUNIVERSAL_EPARAMFAIL);
                      }
                      else
                      /* Call underlying kernel */
                         IMG_conv_3x3_i16s_c16s((short *)inBufs->descs[C6ACCEL_TI_IMG_conv_3x3_i16s_c16s_paramPtr->in_data_InArrID1].buf,
                                                (short *)outBufs->descs[C6ACCEL_TI_IMG_conv_3x3_i16s_c16s_paramPtr->out_data_OutArrID1].buf,
                                                C6ACCEL_TI_IMG_conv_3x3_i16s_c16s_paramPtr->width,
                                                C6ACCEL_TI_IMG_conv_3x3_i16s_c16s_paramPtr->pitch,
                                                (const short *)inBufs->descs[C6ACCEL_TI_IMG_conv_3x3_i16s_c16s_paramPtr->mask_InArrID2].buf,
                                                C6ACCEL_TI_IMG_conv_3x3_i16s_c16s_paramPtr->shift);
                  }
                  break;
                  case (CONV_3X3_I8_C8S_FRAME_FXN_ID):{
                     /* Unmarshal Parameters */
                     IMG_conv_3x3_i8_c8s_Frame_Params *C6ACCEL_TI_IMG_conv_3x3_i8_c8s_Frame_paramPtr;
                     C6ACCEL_TI_IMG_conv_3x3_i8_c8s_Frame_paramPtr = pFnArray;
                     /*Parameter check*/
                     if(((C6ACCEL_TI_IMG_conv_3x3_i8_c8s_Frame_paramPtr->in_data_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_IMG_conv_3x3_i8_c8s_Frame_paramPtr->out_data_OutArrID1)>OUTBUF15)|
                        ((C6ACCEL_TI_IMG_conv_3x3_i8_c8s_Frame_paramPtr->mask_InArrID2)>INBUF15)|
                        ((C6ACCEL_TI_IMG_conv_3x3_i8_c8s_Frame_paramPtr->rows)<= 0)|
                        ((C6ACCEL_TI_IMG_conv_3x3_i8_c8s_Frame_paramPtr->cols)%8 != 0)){
                          return(IUNIVERSAL_EPARAMFAIL);
                      }
                     else
                       { // Use Row to loop through image line by line using existing DSP API
                        int r;
                        unsigned char *pInChar=(unsigned char*)inBufs->descs[C6ACCEL_TI_IMG_conv_3x3_i8_c8s_Frame_paramPtr->in_data_InArrID1].buf;
                        unsigned char *pOutChar=(unsigned char*)outBufs->descs[C6ACCEL_TI_IMG_conv_3x3_i8_c8s_Frame_paramPtr->out_data_OutArrID1].buf;
                        /* Call underlying kernel */
                        for(r=0;r<C6ACCEL_TI_IMG_conv_3x3_i8_c8s_Frame_paramPtr->rows;r++){
                            IMG_conv_3x3_i8_c8s(pInChar + r*C6ACCEL_TI_IMG_conv_3x3_i8_c8s_Frame_paramPtr->cols,
                                                pOutChar + r*C6ACCEL_TI_IMG_conv_3x3_i8_c8s_Frame_paramPtr->cols,
                                                C6ACCEL_TI_IMG_conv_3x3_i8_c8s_Frame_paramPtr->cols,
                                               (const char *)inBufs->descs[C6ACCEL_TI_IMG_conv_3x3_i8_c8s_Frame_paramPtr->mask_InArrID2].buf,
                                               C6ACCEL_TI_IMG_conv_3x3_i8_c8s_Frame_paramPtr->shift);
                         }
                      }
                   }
                  break;
                  case (CONV_3X3_I16S_C16S_FRAME_FXN_ID):{
                     /* Unmarshal Parameters */
                     IMG_conv_3x3_i16s_c16s_Frame_Params *C6ACCEL_TI_IMG_conv_3x3_i16s_c16s_Frame_paramPtr;
                     C6ACCEL_TI_IMG_conv_3x3_i16s_c16s_Frame_paramPtr = pFnArray;
                     /* Parameter check */
                     if(((C6ACCEL_TI_IMG_conv_3x3_i16s_c16s_Frame_paramPtr->in_data_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_IMG_conv_3x3_i16s_c16s_Frame_paramPtr->out_data_OutArrID1)>OUTBUF15)|
                        ((C6ACCEL_TI_IMG_conv_3x3_i16s_c16s_Frame_paramPtr->mask_InArrID2)>INBUF15)|
                        ((C6ACCEL_TI_IMG_conv_3x3_i16s_c16s_Frame_paramPtr->pitch)<(C6ACCEL_TI_IMG_conv_3x3_i16s_c16s_Frame_paramPtr->width))|
                        ((C6ACCEL_TI_IMG_conv_3x3_i16s_c16s_Frame_paramPtr->rows)<= 0)|
                        ((C6ACCEL_TI_IMG_conv_3x3_i16s_c16s_Frame_paramPtr->width)< 2)|
                        ((C6ACCEL_TI_IMG_conv_3x3_i16s_c16s_Frame_paramPtr->width)%2 != 0)){
                          return(IUNIVERSAL_EPARAMFAIL);
                      }
                     else
                       { // Use Row to loop through image line by line using existing DSP API
                        int r;
                        short *pInShort=(short*)inBufs->descs[C6ACCEL_TI_IMG_conv_3x3_i16s_c16s_Frame_paramPtr->in_data_InArrID1].buf;
                        short *pOutShort=(short*)outBufs->descs[C6ACCEL_TI_IMG_conv_3x3_i16s_c16s_Frame_paramPtr->out_data_OutArrID1].buf;
                        /* Call underlying kernel */
                        for(r=0;r<C6ACCEL_TI_IMG_conv_3x3_i16s_c16s_Frame_paramPtr->rows;r++){
                            IMG_conv_3x3_i16s_c16s((short *)pInShort+r*C6ACCEL_TI_IMG_conv_3x3_i16s_c16s_Frame_paramPtr->pitch,
                                                  (short *)pOutShort+r*C6ACCEL_TI_IMG_conv_3x3_i16s_c16s_Frame_paramPtr->pitch,
                                                  C6ACCEL_TI_IMG_conv_3x3_i16s_c16s_Frame_paramPtr->width,
                                                  C6ACCEL_TI_IMG_conv_3x3_i16s_c16s_Frame_paramPtr->pitch,
                                                  (const short *)inBufs->descs[C6ACCEL_TI_IMG_conv_3x3_i16s_c16s_Frame_paramPtr->mask_InArrID2].buf,
                                                  C6ACCEL_TI_IMG_conv_3x3_i16s_c16s_Frame_paramPtr->shift);
                          }
                       }
                    }
                 break;
                 case (CORR_3X3_I8_C8_FXN_ID):{
                    /* Unmarshal Parameters */
                    IMG_corr_3x3_i8_c8_Params *C6ACCEL_TI_IMG_corr_3x3_i8_c8_paramPtr;
                    C6ACCEL_TI_IMG_corr_3x3_i8_c8_paramPtr  = pFnArray;
                    /*Parameter check*/
                    if(((C6ACCEL_TI_IMG_corr_3x3_i8_c8_paramPtr->in_InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IMG_corr_3x3_i8_c8_paramPtr->mask_InArrID2)>INBUF15)|
                       ((C6ACCEL_TI_IMG_corr_3x3_i8_c8_paramPtr->out_OutArrID1)>OUTBUF15)|
                       ((C6ACCEL_TI_IMG_corr_3x3_i8_c8_paramPtr->n_out)%4 != 0)|
                       ((C6ACCEL_TI_IMG_corr_3x3_i8_c8_paramPtr->x_dim)%2 != 0)|
                       ((C6ACCEL_TI_IMG_corr_3x3_i8_c8_paramPtr->x_dim)<4)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else
                    /* Call underlying kernel */
                      IMG_corr_3x3_i8_c8((const unsigned char *)inBufs->descs[C6ACCEL_TI_IMG_corr_3x3_i8_c8_paramPtr->in_InArrID1].buf,
                                         (int *)outBufs->descs[C6ACCEL_TI_IMG_corr_3x3_i8_c8_paramPtr->out_OutArrID1].buf,
                                         C6ACCEL_TI_IMG_corr_3x3_i8_c8_paramPtr->n_out,
                                         C6ACCEL_TI_IMG_corr_3x3_i8_c8_paramPtr->x_dim,
                                         (const unsigned char *)inBufs->descs[C6ACCEL_TI_IMG_corr_3x3_i8_c8_paramPtr->mask_InArrID2].buf,
                                         C6ACCEL_TI_IMG_corr_3x3_i8_c8_paramPtr->shift,
                                         C6ACCEL_TI_IMG_corr_3x3_i8_c8_paramPtr->round);
                  }
                 break;
                 case (CORR_3X3_I16S_C16S_FXN_ID):{
                    /* Unmarshal Parameters */
                    IMG_corr_3x3_i16s_c16s_Params *C6ACCEL_TI_IMG_corr_3x3_i16s_c16s_paramPtr;
                    C6ACCEL_TI_IMG_corr_3x3_i16s_c16s_paramPtr  = pFnArray;
                    /*Parameter check*/
                    if(((C6ACCEL_TI_IMG_corr_3x3_i16s_c16s_paramPtr->imgin_InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IMG_corr_3x3_i16s_c16s_paramPtr->mask_InArrID2)>INBUF15)|
                       ((C6ACCEL_TI_IMG_corr_3x3_i16s_c16s_paramPtr->imgout_OutArrID1)>OUTBUF15)|
                       ((C6ACCEL_TI_IMG_corr_3x3_i16s_c16s_paramPtr->pitch)<(C6ACCEL_TI_IMG_corr_3x3_i16s_c16s_paramPtr->width))|
                       ((C6ACCEL_TI_IMG_corr_3x3_i16s_c16s_paramPtr->width)%2 != 0)|
                       ((C6ACCEL_TI_IMG_corr_3x3_i16s_c16s_paramPtr->width)<2)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                     else
                     /* Call underlying kernel */
                        IMG_corr_3x3_i16s_c16s((const short *)inBufs->descs[C6ACCEL_TI_IMG_corr_3x3_i16s_c16s_paramPtr->imgin_InArrID1].buf,
                                               (int *)outBufs->descs[C6ACCEL_TI_IMG_corr_3x3_i16s_c16s_paramPtr->imgout_OutArrID1].buf,
                                               C6ACCEL_TI_IMG_corr_3x3_i16s_c16s_paramPtr->width,
                                               C6ACCEL_TI_IMG_corr_3x3_i16s_c16s_paramPtr->pitch,
                                               (const short *)inBufs->descs[C6ACCEL_TI_IMG_corr_3x3_i16s_c16s_paramPtr->mask_InArrID2].buf,
                                               C6ACCEL_TI_IMG_corr_3x3_i16s_c16s_paramPtr->shift,
                                               C6ACCEL_TI_IMG_corr_3x3_i16s_c16s_paramPtr->round);
                  }
                 break;
                 case (CORR_3X3_I8_C16S_FXN_ID):{
                    /* Unmarshal Parameters */
                    IMG_corr_3x3_i8_c16s_Params *C6ACCEL_TI_IMG_corr_3x3_i8_c16s_paramPtr;
                    C6ACCEL_TI_IMG_corr_3x3_i8_c16s_paramPtr  = pFnArray;
                    /*Parameter check*/
                    if(((C6ACCEL_TI_IMG_corr_3x3_i8_c16s_paramPtr->imgin_InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IMG_corr_3x3_i8_c16s_paramPtr->mask_InArrID2)>INBUF15)|
                       ((C6ACCEL_TI_IMG_corr_3x3_i8_c16s_paramPtr->imgout_OutArrID1)>OUTBUF15)|
                       ((C6ACCEL_TI_IMG_corr_3x3_i8_c16s_paramPtr->pitch)<(C6ACCEL_TI_IMG_corr_3x3_i8_c16s_paramPtr->width))|
                       ((C6ACCEL_TI_IMG_corr_3x3_i8_c16s_paramPtr->width)%2 != 0)|
                       ((C6ACCEL_TI_IMG_corr_3x3_i8_c16s_paramPtr->width)<2)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                     else
                     /* Call underlying kernel */
                       IMG_corr_3x3_i8_c16s((const unsigned char *)inBufs->descs[C6ACCEL_TI_IMG_corr_3x3_i8_c16s_paramPtr->mask_InArrID2].buf,
                                            (int *)outBufs->descs[C6ACCEL_TI_IMG_corr_3x3_i8_c16s_paramPtr->imgout_OutArrID1].buf,
                                            C6ACCEL_TI_IMG_corr_3x3_i8_c16s_paramPtr->width,
                                            C6ACCEL_TI_IMG_corr_3x3_i8_c16s_paramPtr->pitch,
                                            (const short *)inBufs->descs[C6ACCEL_TI_IMG_corr_3x3_i8_c16s_paramPtr->mask_InArrID2].buf);
                  }
                 break;
                 case (CORR_3X3_I8_C8_FRAME_FXN_ID):{
                    /* Unmarshal Parameters */
                    IMG_corr_3x3_i8_c8_Frame_Params *C6ACCEL_TI_IMG_corr_3x3_i8_c8_Frame_paramPtr;
                    C6ACCEL_TI_IMG_corr_3x3_i8_c8_Frame_paramPtr  = pFnArray;
                    /*Parameter check*/
                    if(((C6ACCEL_TI_IMG_corr_3x3_i8_c8_Frame_paramPtr->in_InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IMG_corr_3x3_i8_c8_Frame_paramPtr->mask_InArrID2)>INBUF15)|
                       ((C6ACCEL_TI_IMG_corr_3x3_i8_c8_Frame_paramPtr->out_OutArrID1)>OUTBUF15)|
                       ((C6ACCEL_TI_IMG_corr_3x3_i8_c8_Frame_paramPtr->n_out)%4 != 0)|
                       ((C6ACCEL_TI_IMG_corr_3x3_i8_c8_Frame_paramPtr->rows) <= 0)|
                       ((C6ACCEL_TI_IMG_corr_3x3_i8_c8_Frame_paramPtr->x_dim)%2 != 0)|
                       ((C6ACCEL_TI_IMG_corr_3x3_i8_c8_Frame_paramPtr->x_dim)<4)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                     else{ // Use Row to loop through image line by line using existing DSP API
                          int r;
                          unsigned char *pInChar=(unsigned char*)inBufs->descs[C6ACCEL_TI_IMG_corr_3x3_i8_c8_Frame_paramPtr->in_InArrID1].buf;
                          int *pOutInt          =(int*)outBufs->descs[C6ACCEL_TI_IMG_corr_3x3_i8_c8_Frame_paramPtr->out_OutArrID1].buf;
                          /* Call underlying kernel */
                          for(r=0;r<C6ACCEL_TI_IMG_corr_3x3_i8_c8_Frame_paramPtr->rows;r++){
                             IMG_corr_3x3_i8_c8(pInChar + r*C6ACCEL_TI_IMG_corr_3x3_i8_c8_Frame_paramPtr->x_dim,
                                                pOutInt + r*C6ACCEL_TI_IMG_corr_3x3_i8_c8_Frame_paramPtr->x_dim,
                                                C6ACCEL_TI_IMG_corr_3x3_i8_c8_Frame_paramPtr->n_out,
                                                C6ACCEL_TI_IMG_corr_3x3_i8_c8_Frame_paramPtr->x_dim,
                                                (const unsigned char *)inBufs->descs[C6ACCEL_TI_IMG_corr_3x3_i8_c8_Frame_paramPtr->mask_InArrID2].buf,
                                                C6ACCEL_TI_IMG_corr_3x3_i8_c8_Frame_paramPtr->shift,
                                                C6ACCEL_TI_IMG_corr_3x3_i8_c8_Frame_paramPtr->round);
                          }
                      }
                  }
                 break;
                 case (CORR_3X3_I16S_C16S_FRAME_FXN_ID):{
                     /* Unmarshal Parameters */
                     IMG_corr_3x3_i16s_c16s_Frame_Params *C6ACCEL_TI_IMG_corr_3x3_i16s_c16s_Frame_paramPtr;
                     C6ACCEL_TI_IMG_corr_3x3_i16s_c16s_Frame_paramPtr  = pFnArray;
                     /*Parameter check*/
                     if(((C6ACCEL_TI_IMG_corr_3x3_i16s_c16s_Frame_paramPtr->imgin_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_IMG_corr_3x3_i16s_c16s_Frame_paramPtr->mask_InArrID2)>INBUF15)|
                        ((C6ACCEL_TI_IMG_corr_3x3_i16s_c16s_Frame_paramPtr->imgout_OutArrID1)>OUTBUF15)|
                        ((C6ACCEL_TI_IMG_corr_3x3_i16s_c16s_Frame_paramPtr->pitch)<(C6ACCEL_TI_IMG_corr_3x3_i16s_c16s_Frame_paramPtr->width))|
                        ((C6ACCEL_TI_IMG_corr_3x3_i16s_c16s_Frame_paramPtr->rows) <= 0)|
                        ((C6ACCEL_TI_IMG_corr_3x3_i16s_c16s_Frame_paramPtr->width)%2 != 0)|
                        ((C6ACCEL_TI_IMG_corr_3x3_i16s_c16s_Frame_paramPtr->width)<2)){
                          return(IUNIVERSAL_EPARAMFAIL);
                      }
                      else{ // Use Row to loop through image line by line using existing DSP API
                           int r;
                           short *pInShort=(short*)inBufs->descs[C6ACCEL_TI_IMG_corr_3x3_i16s_c16s_Frame_paramPtr->imgin_InArrID1].buf;
                           int *pOutInt          =(int*)outBufs->descs[C6ACCEL_TI_IMG_corr_3x3_i16s_c16s_Frame_paramPtr->imgout_OutArrID1].buf;
                           /* Call underlying kernel */
                           for(r=0;r<C6ACCEL_TI_IMG_corr_3x3_i16s_c16s_Frame_paramPtr->rows;r++){
                               IMG_corr_3x3_i16s_c16s(pInShort + r * C6ACCEL_TI_IMG_corr_3x3_i16s_c16s_Frame_paramPtr->pitch,
                                                     (int *)pOutInt + r * C6ACCEL_TI_IMG_corr_3x3_i16s_c16s_Frame_paramPtr->pitch,
                                                     C6ACCEL_TI_IMG_corr_3x3_i16s_c16s_Frame_paramPtr->width,
                                                     C6ACCEL_TI_IMG_corr_3x3_i16s_c16s_Frame_paramPtr->pitch,
                                                     (const short *)inBufs->descs[C6ACCEL_TI_IMG_corr_3x3_i16s_c16s_Frame_paramPtr->mask_InArrID2].buf,
                                                     C6ACCEL_TI_IMG_corr_3x3_i16s_c16s_Frame_paramPtr->shift,
                                                     C6ACCEL_TI_IMG_corr_3x3_i16s_c16s_Frame_paramPtr->round);
                            }
                       }
                   }
                 break;
                 case (CORR_3X3_I8_C16S_FRAME_FXN_ID):{
                    /* Unmarshal Parameters */
                    IMG_corr_3x3_i8_c16s_Frame_Params *C6ACCEL_TI_IMG_corr_3x3_i8_c16s_Frame_paramPtr;
                    C6ACCEL_TI_IMG_corr_3x3_i8_c16s_Frame_paramPtr  = pFnArray;
                    /*Parameter check*/
                    if(((C6ACCEL_TI_IMG_corr_3x3_i8_c16s_Frame_paramPtr->imgin_InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IMG_corr_3x3_i8_c16s_Frame_paramPtr->mask_InArrID2)>INBUF15)|
                       ((C6ACCEL_TI_IMG_corr_3x3_i8_c16s_Frame_paramPtr->imgout_OutArrID1)>OUTBUF15)|
                       ((C6ACCEL_TI_IMG_corr_3x3_i8_c16s_Frame_paramPtr->pitch)<(C6ACCEL_TI_IMG_corr_3x3_i8_c16s_Frame_paramPtr->width))|
                       ((C6ACCEL_TI_IMG_corr_3x3_i8_c16s_Frame_paramPtr->width)%2 != 0)|
                       ((C6ACCEL_TI_IMG_corr_3x3_i8_c16s_Frame_paramPtr->rows) <= 0)|
                       ((C6ACCEL_TI_IMG_corr_3x3_i8_c16s_Frame_paramPtr->width)<2)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                     else{ // Use Row to loop through image line by line using existing DSP API
                          int r;
                          unsigned char *pInChar=(unsigned char*)inBufs->descs[C6ACCEL_TI_IMG_corr_3x3_i8_c16s_Frame_paramPtr->mask_InArrID2].buf;
                          int *pOutInt          =(int*)outBufs->descs[C6ACCEL_TI_IMG_corr_3x3_i8_c16s_Frame_paramPtr->imgout_OutArrID1].buf;
                          /* Call underlying kernel */
                          for(r=0;r<C6ACCEL_TI_IMG_corr_3x3_i8_c16s_Frame_paramPtr->rows;r++){
                              IMG_corr_3x3_i8_c16s(pInChar + r*C6ACCEL_TI_IMG_corr_3x3_i8_c16s_Frame_paramPtr->pitch,
                                                   pOutInt + r*C6ACCEL_TI_IMG_corr_3x3_i8_c16s_Frame_paramPtr->pitch,
                                                   C6ACCEL_TI_IMG_corr_3x3_i8_c16s_Frame_paramPtr->width,
                                                   C6ACCEL_TI_IMG_corr_3x3_i8_c16s_Frame_paramPtr->pitch,
                                                   (const short *)inBufs->descs[C6ACCEL_TI_IMG_corr_3x3_i8_c16s_Frame_paramPtr->mask_InArrID2].buf);
                            }
                       }
                   }
                 break;
                 case (MULS_8_FXN_ID):{
                    /* Unmarshal Parameters */
                    IMG_mulS_8_Params *C6ACCEL_TI_IMG_mulS_8_paramPtr;
                    C6ACCEL_TI_IMG_mulS_8_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_IMG_mulS_8_paramPtr->imgR_InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IMG_mulS_8_paramPtr->imgW_OutArrID1)>INBUF15)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else
                    /* Call underlying kernel */
                       IMG_mulS_8((unsigned char *)inBufs->descs[C6ACCEL_TI_IMG_mulS_8_paramPtr->imgR_InArrID1].buf,
                                  (short *)outBufs->descs[C6ACCEL_TI_IMG_mulS_8_paramPtr->imgW_OutArrID1].buf,
                                   C6ACCEL_TI_IMG_mulS_8_paramPtr->constData,
                                   C6ACCEL_TI_IMG_mulS_8_paramPtr->count);
                     }
                 break;
                 case (MULS_16S_FXN_ID):{
                    /* Unmarshal Parameters */
                    IMG_mulS_16s_Params *C6ACCEL_TI_IMG_mulS_16s_paramPtr;
                    C6ACCEL_TI_IMG_mulS_16s_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_IMG_mulS_16s_paramPtr->imgR_InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IMG_mulS_16s_paramPtr->imgW_OutArrID1)>INBUF15)){
                       return(IUNIVERSAL_EPARAMFAIL);
                    }
                    else
                    /* Call underlying kernel */
                      IMG_mulS_16s((short *)inBufs->descs[C6ACCEL_TI_IMG_mulS_16s_paramPtr->imgR_InArrID1].buf,
                                   (int *)outBufs->descs[C6ACCEL_TI_IMG_mulS_16s_paramPtr->imgW_OutArrID1].buf,
                                   (short)C6ACCEL_TI_IMG_mulS_16s_paramPtr->constData,
                                   C6ACCEL_TI_IMG_mulS_16s_paramPtr->count);
                    }
                 break;
                 case (ADDS_8_FXN_ID):{
                    /* Unmarshal Parameters */
                    IMG_addS_8_Params *C6ACCEL_TI_IMG_addS_8_paramPtr;
                    C6ACCEL_TI_IMG_addS_8_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_IMG_addS_8_paramPtr->imgR_InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IMG_addS_8_paramPtr->imgW_OutArrID1)>INBUF15)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else
                    /* Call underlying kernel */
                       IMG_addS_8((char *)inBufs->descs[C6ACCEL_TI_IMG_addS_8_paramPtr->imgR_InArrID1].buf,
                                  (char *)outBufs->descs[C6ACCEL_TI_IMG_addS_8_paramPtr->imgW_OutArrID1].buf,
                                  C6ACCEL_TI_IMG_addS_8_paramPtr->constData,
                                  C6ACCEL_TI_IMG_addS_8_paramPtr->count);
                    }
                 break;
                 case (ADDS_16S_FXN_ID):{ 
                    /* Unmarshal Parameters */   
                    IMG_addS_16s_Params *C6ACCEL_TI_IMG_addS_16s_paramPtr;
                    C6ACCEL_TI_IMG_addS_16s_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_IMG_addS_16s_paramPtr->imgR_InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IMG_addS_16s_paramPtr->imgW_OutArrID1)>INBUF15)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else
                    /* Call underlying kernel */
                       IMG_addS_16s((short *)inBufs->descs[C6ACCEL_TI_IMG_addS_16s_paramPtr->imgR_InArrID1].buf,
                                    (short *)outBufs->descs[C6ACCEL_TI_IMG_addS_16s_paramPtr->imgW_OutArrID1].buf,
                                    C6ACCEL_TI_IMG_addS_16s_paramPtr->constData,
                                    C6ACCEL_TI_IMG_addS_16s_paramPtr->count);
                    }
                 break;
                 case (SUBS_8_FXN_ID):{
                    /* Unmarshal Parameters */
                    IMG_subS_8_Params *C6ACCEL_TI_IMG_subS_8_paramPtr;
                    C6ACCEL_TI_IMG_subS_8_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_IMG_subS_8_paramPtr->imgR_InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IMG_subS_8_paramPtr->imgW_OutArrID1)>INBUF15)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else
                    /* Call underlying kernel */
                      IMG_subS_8((char *)inBufs->descs[C6ACCEL_TI_IMG_subS_8_paramPtr->imgR_InArrID1].buf,
                                 (char *)outBufs->descs[C6ACCEL_TI_IMG_subS_8_paramPtr->imgW_OutArrID1].buf,
                                  C6ACCEL_TI_IMG_subS_8_paramPtr->constData,
                                  C6ACCEL_TI_IMG_subS_8_paramPtr->count);
                    }
                 break;
                 case (SUBS_16S_FXN_ID):{ 
                    /* Unmarshal Parameters */   
                    IMG_subS_16s_Params *C6ACCEL_TI_IMG_subS_16s_paramPtr;
                    C6ACCEL_TI_IMG_subS_16s_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_IMG_subS_16s_paramPtr->imgR_InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IMG_subS_16s_paramPtr->imgW_OutArrID1)>INBUF15)){
                       return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else
                    /* Call underlying kernel */
                       IMG_subS_16s((short *)inBufs->descs[C6ACCEL_TI_IMG_subS_16s_paramPtr->imgR_InArrID1].buf,
                                    (short *)outBufs->descs[C6ACCEL_TI_IMG_subS_16s_paramPtr->imgW_OutArrID1].buf,
                                    C6ACCEL_TI_IMG_subS_16s_paramPtr->constData,
                                    C6ACCEL_TI_IMG_subS_16s_paramPtr->count);
                     }
                 break;
                 case (YC_DEMUX_LE16_16_FXN_ID):{
                    /* Unmarshal Parameters */
                    IMG_yc_demux_le16_16_Params *C6ACCEL_TI_IMG_yc_demux_le16_16_paramPtr;
                    C6ACCEL_TI_IMG_yc_demux_le16_16_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_IMG_yc_demux_le16_16_paramPtr->yc_InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IMG_yc_demux_le16_16_paramPtr->y_OutArrID1)>OUTBUF15)|
                       ((C6ACCEL_TI_IMG_yc_demux_le16_16_paramPtr->cr_OutArrID2)>OUTBUF15)|
                       ((C6ACCEL_TI_IMG_yc_demux_le16_16_paramPtr->cb_OutArrID3)>OUTBUF15)|
                       ((C6ACCEL_TI_IMG_yc_demux_le16_16_paramPtr->n)%16 !=0)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                     else
                     /* Call underlying kernel */
                        IMG_yc_demux_le16_16(C6ACCEL_TI_IMG_yc_demux_le16_16_paramPtr->n,
                                            (const unsigned short *)inBufs->descs[C6ACCEL_TI_IMG_yc_demux_le16_16_paramPtr->yc_InArrID1].buf,
                                            (short *) outBufs->descs[C6ACCEL_TI_IMG_yc_demux_le16_16_paramPtr->y_OutArrID1].buf,
                                            (short *) outBufs->descs[C6ACCEL_TI_IMG_yc_demux_le16_16_paramPtr->cr_OutArrID2].buf,
                                            (short *) outBufs->descs[C6ACCEL_TI_IMG_yc_demux_le16_16_paramPtr->cb_OutArrID3].buf);
                    }
                 break;
                 case (YC_DEMUX_LE8_C_FXN_ID):{
                    /* Unmarshal Parameters */
                    IMG_yc_demux_le8_c_Params *C6ACCEL_TI_IMG_yc_demux_le8_c_paramPtr;
                    C6ACCEL_TI_IMG_yc_demux_le8_c_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_IMG_yc_demux_le8_c_paramPtr->yc_InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IMG_yc_demux_le8_c_paramPtr->y_OutArrID1)>OUTBUF15)|
                       ((C6ACCEL_TI_IMG_yc_demux_le8_c_paramPtr->cr_OutArrID2)>OUTBUF15)|
                       ((C6ACCEL_TI_IMG_yc_demux_le8_c_paramPtr->cb_OutArrID3)>OUTBUF15)|
                       ((C6ACCEL_TI_IMG_yc_demux_le8_c_paramPtr->n)%16 !=0)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else
                    /* Call underlying kernel */
                       IMG_yc_demux_le8_c(C6ACCEL_TI_IMG_yc_demux_le8_c_paramPtr->n,
                                          (const unsigned char *)inBufs->descs[C6ACCEL_TI_IMG_yc_demux_le8_c_paramPtr->yc_InArrID1].buf,
                                          (unsigned char *) outBufs->descs[C6ACCEL_TI_IMG_yc_demux_le8_c_paramPtr->y_OutArrID1].buf,
                                          (unsigned char *) outBufs->descs[C6ACCEL_TI_IMG_yc_demux_le8_c_paramPtr->cr_OutArrID2].buf,
                                          (unsigned char *) outBufs->descs[C6ACCEL_TI_IMG_yc_demux_le8_c_paramPtr->cb_OutArrID3].buf);
                    }
                 break;
                 case (YCBCR422PL_RGB565_FXN_ID):{
                     /* Unmarshal Parameters */
                     IMG_ycbcr422pl_to_rgb565_Params   *C6ACCEL_TI_IMG_ycbcr422pl_to_rgb565_paramPtr;
                     C6ACCEL_TI_IMG_ycbcr422pl_to_rgb565_paramPtr = pFnArray;
                     /* Parameter check */
                     if(((C6ACCEL_TI_IMG_ycbcr422pl_to_rgb565_paramPtr->coeff_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_IMG_ycbcr422pl_to_rgb565_paramPtr->y_data_InArrID2)>INBUF15)|
                        ((C6ACCEL_TI_IMG_ycbcr422pl_to_rgb565_paramPtr->cb_data_InArrID3)>INBUF15)|
                        ((C6ACCEL_TI_IMG_ycbcr422pl_to_rgb565_paramPtr->cr_data_InArrID4)>INBUF15)|
                        ((C6ACCEL_TI_IMG_ycbcr422pl_to_rgb565_paramPtr->rgb_data_OutArrID1)>OUTBUF15)|
                        ((C6ACCEL_TI_IMG_ycbcr422pl_to_rgb565_paramPtr->num_pixels)%8 != 0)){
                          return(IUNIVERSAL_EPARAMFAIL);
                      }
                      else
                      /* Call underlying kernel */
                         IMG_ycbcr422pl_to_rgb565((const short *) inBufs->descs[C6ACCEL_TI_IMG_ycbcr422pl_to_rgb565_paramPtr->coeff_InArrID1].buf,
                                                  (const unsigned char *) inBufs->descs[C6ACCEL_TI_IMG_ycbcr422pl_to_rgb565_paramPtr->y_data_InArrID2].buf,
                                                  (const unsigned char *) inBufs->descs[C6ACCEL_TI_IMG_ycbcr422pl_to_rgb565_paramPtr->cb_data_InArrID3].buf,
                                                  (const unsigned char *) inBufs->descs[C6ACCEL_TI_IMG_ycbcr422pl_to_rgb565_paramPtr->cr_data_InArrID4].buf,
                                                  (unsigned short *) outBufs->descs[C6ACCEL_TI_IMG_ycbcr422pl_to_rgb565_paramPtr->rgb_data_OutArrID1].buf,
                                                  C6ACCEL_TI_IMG_ycbcr422pl_to_rgb565_paramPtr->num_pixels);
                    }
                 break;
                 case (YCBCR422PL16_RGB565_FXN_ID):{
                    /* Unmarshal Parameters */
                    IMG_ycbcr422pl16_to_rgb565_Params *C6ACCEL_TI_IMG_ycbcr422pl16_to_rgb565_paramPtr;
                    C6ACCEL_TI_IMG_ycbcr422pl16_to_rgb565_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_IMG_ycbcr422pl16_to_rgb565_paramPtr->coeff_InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IMG_ycbcr422pl16_to_rgb565_paramPtr->y_data_InArrID2)>INBUF15)|
                       ((C6ACCEL_TI_IMG_ycbcr422pl16_to_rgb565_paramPtr->cb_data_InArrID3)>INBUF15)|
                       ((C6ACCEL_TI_IMG_ycbcr422pl16_to_rgb565_paramPtr->cr_data_InArrID4)>INBUF15)|
                       ((C6ACCEL_TI_IMG_ycbcr422pl16_to_rgb565_paramPtr->rgb_data_OutArrID1)>OUTBUF15)|
                       ((C6ACCEL_TI_IMG_ycbcr422pl16_to_rgb565_paramPtr->num_pixels)%8 != 0)){
                         return(IUNIVERSAL_EPARAMFAIL);
                      }
                     else
                     /* Call underlying kernel */
                        IMG_ycbcr422pl16_to_rgb565_c((const short *) inBufs->descs[C6ACCEL_TI_IMG_ycbcr422pl16_to_rgb565_paramPtr->coeff_InArrID1].buf,
                                                     (short *) inBufs->descs[C6ACCEL_TI_IMG_ycbcr422pl16_to_rgb565_paramPtr->y_data_InArrID2].buf,
                                                     (short *) inBufs->descs[C6ACCEL_TI_IMG_ycbcr422pl16_to_rgb565_paramPtr->cb_data_InArrID3].buf,
                                                     (short *) inBufs->descs[C6ACCEL_TI_IMG_ycbcr422pl16_to_rgb565_paramPtr->cr_data_InArrID4].buf,
                                                     (unsigned short *) outBufs->descs[C6ACCEL_TI_IMG_ycbcr422pl16_to_rgb565_paramPtr->rgb_data_OutArrID1].buf,
                                                     C6ACCEL_TI_IMG_ycbcr422pl16_to_rgb565_paramPtr->num_pixels);
                    }
                 break;
                 case (YUV420PL_TO_RGB565_FXN_ID):{
                    /* Unmarshal Parameters */
                    IMG_yuv420pl_to_rgb565_Params  *C6ACCEL_TI_IMG_yuv420pl_to_rgb565_paramPtr;
                    C6ACCEL_TI_IMG_yuv420pl_to_rgb565_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_IMG_yuv420pl_to_rgb565_paramPtr->coeff_InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IMG_yuv420pl_to_rgb565_paramPtr->y_data_InArrID2)>INBUF15)|
                       ((C6ACCEL_TI_IMG_yuv420pl_to_rgb565_paramPtr->cb_data_InArrID3)>INBUF15)|
                       ((C6ACCEL_TI_IMG_yuv420pl_to_rgb565_paramPtr->cr_data_InArrID4)>INBUF15)|
                       ((C6ACCEL_TI_IMG_yuv420pl_to_rgb565_paramPtr->rgb_data_OutArrID1)>OUTBUF15)|
                       ((C6ACCEL_TI_IMG_yuv420pl_to_rgb565_paramPtr->Height)%2 != 0)|
                       ((C6ACCEL_TI_IMG_yuv420pl_to_rgb565_paramPtr->Width)%8 != 0)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                     else
                     /* Call underlying kernel */
                        IMG_yuv420pl_to_rgb565((const short *) inBufs->descs[C6ACCEL_TI_IMG_yuv420pl_to_rgb565_paramPtr->coeff_InArrID1].buf,
                                               C6ACCEL_TI_IMG_yuv420pl_to_rgb565_paramPtr->Height,
                                               C6ACCEL_TI_IMG_yuv420pl_to_rgb565_paramPtr->Width,
                                               (unsigned char *) inBufs->descs[C6ACCEL_TI_IMG_yuv420pl_to_rgb565_paramPtr->y_data_InArrID2].buf,
                                               (unsigned char *) inBufs->descs[C6ACCEL_TI_IMG_yuv420pl_to_rgb565_paramPtr->cb_data_InArrID3].buf,
                                               (unsigned char *) inBufs->descs[C6ACCEL_TI_IMG_yuv420pl_to_rgb565_paramPtr->cr_data_InArrID4].buf,
                                               (unsigned short *) outBufs->descs[C6ACCEL_TI_IMG_yuv420pl_to_rgb565_paramPtr->rgb_data_OutArrID1].buf);
                   }
                 break;
                 case (YUV420PL16_TO_RGB565_FXN_ID):{
                    /* Unmarshal Parameters */
                    IMG_yuv420pl16_to_rgb565_Params *C6ACCEL_TI_IMG_yuv420pl16_to_rgb565_paramPtr;
                    C6ACCEL_TI_IMG_yuv420pl16_to_rgb565_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_IMG_yuv420pl16_to_rgb565_paramPtr->coeff_InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IMG_yuv420pl16_to_rgb565_paramPtr->y_data_InArrID2)>INBUF15)|
                       ((C6ACCEL_TI_IMG_yuv420pl16_to_rgb565_paramPtr->cb_data_InArrID3)>INBUF15)|
                       ((C6ACCEL_TI_IMG_yuv420pl16_to_rgb565_paramPtr->cr_data_InArrID4)>INBUF15)|
                       ((C6ACCEL_TI_IMG_yuv420pl16_to_rgb565_paramPtr->rgb_data_OutArrID1)>OUTBUF15)|
                       ((C6ACCEL_TI_IMG_yuv420pl16_to_rgb565_paramPtr->Height)%2 != 0)|
                       ((C6ACCEL_TI_IMG_yuv420pl16_to_rgb565_paramPtr->Width)%2 != 0)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                     else
                     /* Call underlying kernel */
                        IMG_yuv420pl16_to_rgb565((const short *) inBufs->descs[C6ACCEL_TI_IMG_yuv420pl16_to_rgb565_paramPtr->coeff_InArrID1].buf,
                                                  C6ACCEL_TI_IMG_yuv420pl16_to_rgb565_paramPtr->Height,
                                                  C6ACCEL_TI_IMG_yuv420pl16_to_rgb565_paramPtr->Width,
                                                  (short *) inBufs->descs[C6ACCEL_TI_IMG_yuv420pl16_to_rgb565_paramPtr->y_data_InArrID2].buf,
                                                  (short *) inBufs->descs[C6ACCEL_TI_IMG_yuv420pl16_to_rgb565_paramPtr->cb_data_InArrID3].buf,
                                                  (short *) inBufs->descs[C6ACCEL_TI_IMG_yuv420pl16_to_rgb565_paramPtr->cr_data_InArrID4].buf,
                                                  (unsigned short *) outBufs->descs[C6ACCEL_TI_IMG_yuv420pl16_to_rgb565_paramPtr->rgb_data_OutArrID1].buf);
                  }
                 break;
                 case (YCBCR422SP_TO_YCBCR420PL_FXN_ID):{
                    /* Unmarshal Parameters */
                    IMG_ycbcr422sp_to_ycbcr420pl_Params  *C6ACCEL_TI_IMG_ycbcr422sp_to_ycbcr420pl_paramPtr;
                    C6ACCEL_TI_IMG_ycbcr422sp_to_ycbcr420pl_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_IMG_ycbcr422sp_to_ycbcr420pl_paramPtr->y_src_InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IMG_ycbcr422sp_to_ycbcr420pl_paramPtr->cbcr_src_InArrID2)>INBUF15)|
                       ((C6ACCEL_TI_IMG_ycbcr422sp_to_ycbcr420pl_paramPtr->y_dst_OutArrID1)>OUTBUF15)|
                       ((C6ACCEL_TI_IMG_ycbcr422sp_to_ycbcr420pl_paramPtr->cb_dst_OutArrID2)>OUTBUF15)|
                       ((C6ACCEL_TI_IMG_ycbcr422sp_to_ycbcr420pl_paramPtr->cr_dst_OutArrID3)>OUTBUF15)|
                       ((C6ACCEL_TI_IMG_ycbcr422sp_to_ycbcr420pl_paramPtr->width)%16 != 0)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                     else
                     /* Call underlying kernel */
                        ycbcr422sp_to_ycbcr420pl((const unsigned char *) inBufs->descs[C6ACCEL_TI_IMG_ycbcr422sp_to_ycbcr420pl_paramPtr->y_src_InArrID1].buf,
                                                 (const unsigned char *) inBufs->descs[C6ACCEL_TI_IMG_ycbcr422sp_to_ycbcr420pl_paramPtr->cbcr_src_InArrID2].buf,
                                                 (unsigned char *) outBufs->descs[C6ACCEL_TI_IMG_ycbcr422sp_to_ycbcr420pl_paramPtr->y_dst_OutArrID1].buf,
                                                 (unsigned char *) outBufs->descs[C6ACCEL_TI_IMG_ycbcr422sp_to_ycbcr420pl_paramPtr->cb_dst_OutArrID2].buf,
                                                 (unsigned char *) outBufs->descs[C6ACCEL_TI_IMG_ycbcr422sp_to_ycbcr420pl_paramPtr->cr_dst_OutArrID3].buf,
                                                 C6ACCEL_TI_IMG_ycbcr422sp_to_ycbcr420pl_paramPtr->num_lines,
                                                 C6ACCEL_TI_IMG_ycbcr422sp_to_ycbcr420pl_paramPtr->width,
                                                 C6ACCEL_TI_IMG_ycbcr422sp_to_ycbcr420pl_paramPtr->src_pitch,
                                                 C6ACCEL_TI_IMG_ycbcr422sp_to_ycbcr420pl_paramPtr->dst_y_pitch,
                                                 C6ACCEL_TI_IMG_ycbcr422sp_to_ycbcr420pl_paramPtr->dst_cbcr_pitch);
                                           
                                               
                  }
                 break;
                 case (YCBCR422PL_TO_YCBCR422SP_FXN_ID):{
                    /* Unmarshal Parameters */
                    IMG_ycbcr422pl_to_ycbcr422sp_Params  *C6ACCEL_TI_IMG_ycbcr422pl_to_ycbcr422sp_paramPtr;
                    C6ACCEL_TI_IMG_ycbcr422pl_to_ycbcr422sp_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_IMG_ycbcr422pl_to_ycbcr422sp_paramPtr->y_src_InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IMG_ycbcr422pl_to_ycbcr422sp_paramPtr->cr_src_InArrID3)>INBUF15)|
                       ((C6ACCEL_TI_IMG_ycbcr422pl_to_ycbcr422sp_paramPtr->cb_src_InArrID2)>INBUF15)|
                       ((C6ACCEL_TI_IMG_ycbcr422pl_to_ycbcr422sp_paramPtr->y_dst_OutArrID1)>OUTBUF15)|
                       ((C6ACCEL_TI_IMG_ycbcr422pl_to_ycbcr422sp_paramPtr->cbcr_dst_OutArrID2)>OUTBUF15)|
                       ((C6ACCEL_TI_IMG_ycbcr422pl_to_ycbcr422sp_paramPtr->width)%16 != 0)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                     else
                     /* Call underlying kernel */
                        ycbcr422pl_to_ycbcr422sp((unsigned char *) outBufs->descs[C6ACCEL_TI_IMG_ycbcr422pl_to_ycbcr422sp_paramPtr->y_dst_OutArrID1].buf,
                                                 (unsigned char *) outBufs->descs[C6ACCEL_TI_IMG_ycbcr422pl_to_ycbcr422sp_paramPtr->cbcr_dst_OutArrID2].buf,
                                                 (unsigned char *) inBufs->descs[C6ACCEL_TI_IMG_ycbcr422pl_to_ycbcr422sp_paramPtr->y_src_InArrID1].buf,
                                                 (unsigned char *) inBufs->descs[C6ACCEL_TI_IMG_ycbcr422pl_to_ycbcr422sp_paramPtr->cb_src_InArrID2].buf,
                                                 (unsigned char *) inBufs->descs[C6ACCEL_TI_IMG_ycbcr422pl_to_ycbcr422sp_paramPtr->cr_src_InArrID3].buf,
                                                 C6ACCEL_TI_IMG_ycbcr422pl_to_ycbcr422sp_paramPtr->num_lines,
                                                 C6ACCEL_TI_IMG_ycbcr422pl_to_ycbcr422sp_paramPtr->width,
                                                 C6ACCEL_TI_IMG_ycbcr422pl_to_ycbcr422sp_paramPtr->dst_pitch,
                                                 C6ACCEL_TI_IMG_ycbcr422pl_to_ycbcr422sp_paramPtr->src_y_pitch,
                                                 C6ACCEL_TI_IMG_ycbcr422pl_to_ycbcr422sp_paramPtr->src_cbcr_pitch);
                                           
                                               
                  }
                 break;
                 case (YCBCR422SP_TO_YCBCR422ILE_FXN_ID):{
                    /* Unmarshal Parameters */
                    IMG_ycbcr422sp_to_ycbcr422ile_Params  *C6ACCEL_TI_IMG_ycbcr422sp_to_ycbcr422ile_paramPtr;
                    C6ACCEL_TI_IMG_ycbcr422sp_to_ycbcr422ile_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_IMG_ycbcr422sp_to_ycbcr422ile_paramPtr->y_src_InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IMG_ycbcr422sp_to_ycbcr422ile_paramPtr->cbcr_src_InArrID2)>INBUF15)|
                       ((C6ACCEL_TI_IMG_ycbcr422sp_to_ycbcr422ile_paramPtr->ycbcr_dst_OutArrID1)>OUTBUF15)|
                       ((C6ACCEL_TI_IMG_ycbcr422sp_to_ycbcr422ile_paramPtr->width)%16 != 0)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                     else
                     /* Call underlying kernel */
                        ycbcr422sp_to_ycbcr422ile((unsigned char *) inBufs->descs[C6ACCEL_TI_IMG_ycbcr422sp_to_ycbcr422ile_paramPtr->y_src_InArrID1].buf,
                                                 (unsigned char *) inBufs->descs[C6ACCEL_TI_IMG_ycbcr422sp_to_ycbcr422ile_paramPtr->cbcr_src_InArrID2].buf,
                                                 (unsigned char *) outBufs->descs[C6ACCEL_TI_IMG_ycbcr422sp_to_ycbcr422ile_paramPtr->ycbcr_dst_OutArrID1].buf,
                                                 C6ACCEL_TI_IMG_ycbcr422sp_to_ycbcr422ile_paramPtr->num_lines,
                                                 C6ACCEL_TI_IMG_ycbcr422sp_to_ycbcr422ile_paramPtr->width,
                                                 C6ACCEL_TI_IMG_ycbcr422sp_to_ycbcr422ile_paramPtr->src_pitch,
                                                 C6ACCEL_TI_IMG_ycbcr422sp_to_ycbcr422ile_paramPtr->dst_ycbcr_pitch);
                                           
                                               
                  }
                 break;
                 default:
                    /*Error caused due to passing of an invalid ID*/
                         return(IUNIVERSAL_EFXNIDFAIL);
                  }
             break;
             case(MATHLIB_FXN_ID):
               /* IQMATHLIB functions */
               switch ( fxnID & fxnidMsk){
                 case(IQNMPY_FXN_ID):{
                    /* Unmarshal Parameters */
                    I32_IQ *inArrRdr1, *inArrRdr2, *outArrRdr;
                    IQNmpy_Params  *C6ACCEL_TI_IQNmpy_paramPtr;
                    C6ACCEL_TI_IQNmpy_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_IQNmpy_paramPtr->InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IQNmpy_paramPtr->InArrID2)>INBUF15)|
                       ((C6ACCEL_TI_IQNmpy_paramPtr->OutArrID1)>OUTBUF15)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else{
                         /* Call underlying kernel */
                         inArrRdr1 = (I32_IQ *)inBufs->descs[C6ACCEL_TI_IQNmpy_paramPtr->InArrID1].buf;
                         inArrRdr2 = (I32_IQ *)inBufs->descs[C6ACCEL_TI_IQNmpy_paramPtr->InArrID2].buf;
                         outArrRdr = (I32_IQ *)outBufs->descs[C6ACCEL_TI_IQNmpy_paramPtr->OutArrID1].buf;
                         for(j=0 ; j < (C6ACCEL_TI_IQNmpy_paramPtr-> n); j++){
                             outArrRdr[j] = _IQNmpy(inArrRdr1[j], inArrRdr2[j], C6ACCEL_TI_IQNmpy_paramPtr->qfmt);
                          }
                       }
                    }
                 break;
                 case(IQNSQRT_FXN_ID):{
                    /* Unmarshal Parameters */
                    I32_IQ *inArrRdr1, *outArrRdr;
                    IQNsqrt_Params  *C6ACCEL_TI_IQNsqrt_paramPtr;
                    C6ACCEL_TI_IQNsqrt_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_IQNsqrt_paramPtr->InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IQNsqrt_paramPtr->OutArrID1)>OUTBUF15)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else{
                         /* Call underlying kernel */
                        inArrRdr1 = (I32_IQ *)inBufs->descs[C6ACCEL_TI_IQNsqrt_paramPtr->InArrID1].buf;
                        outArrRdr = (I32_IQ *)outBufs->descs[C6ACCEL_TI_IQNsqrt_paramPtr->OutArrID1].buf;
                        for(j=0 ; j < (C6ACCEL_TI_IQNsqrt_paramPtr-> n); j++){
                            outArrRdr[j] = _IQNsqrt(inArrRdr1[j], C6ACCEL_TI_IQNsqrt_paramPtr->qfmt);
                         }
                     }
                  }
                 break;
                 case(IQNSIN_FXN_ID):{
                    /* Unmarshal Parameters */
                    I32_IQ *inArrRdr1, *outArrRdr;
                    IQNsin_Params  *C6ACCEL_TI_IQNsin_paramPtr;
                    C6ACCEL_TI_IQNsin_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_IQNsin_paramPtr->InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IQNsin_paramPtr->OutArrID1)>OUTBUF15)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else{
                         /* Call underlying kernel */
                         inArrRdr1 = (I32_IQ *)inBufs->descs[C6ACCEL_TI_IQNsin_paramPtr->InArrID1].buf;
                         outArrRdr = (I32_IQ *)outBufs->descs[C6ACCEL_TI_IQNsin_paramPtr->OutArrID1].buf;
                         for(j=0 ; j < (C6ACCEL_TI_IQNsin_paramPtr-> n); j++){
                             outArrRdr[j] = _IQNsin(inArrRdr1[j], C6ACCEL_TI_IQNsin_paramPtr->qfmt);
                           }
                       }
                   }
                 break;
                 case(IQNCOS_FXN_ID):{
                    /* Unmarshal Parameters */
                    I32_IQ *inArrRdr1, *outArrRdr;
                    IQNcos_Params   *C6ACCEL_TI_IQNcos_paramPtr;
                    C6ACCEL_TI_IQNcos_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_IQNcos_paramPtr->InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IQNcos_paramPtr->OutArrID1)>OUTBUF15)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else{
                         /* Call underlying kernel */
                         inArrRdr1 = (I32_IQ *)inBufs->descs[C6ACCEL_TI_IQNcos_paramPtr->InArrID1].buf;
                         outArrRdr = (I32_IQ *)outBufs->descs[C6ACCEL_TI_IQNcos_paramPtr->OutArrID1].buf;
                         for(j=0 ; j < (C6ACCEL_TI_IQNcos_paramPtr-> n); j++){
                             outArrRdr[j] = _IQNcos(inArrRdr1[j], C6ACCEL_TI_IQNcos_paramPtr->qfmt);
                          }
                      }
                  }
                 break;
                 case(IQNDIV_FXN_ID):{
                    /* Unmarshal Parameters */
                    I32_IQ *inArrRdr1, *inArrRdr2, *outArrRdr;
                    IQNdiv_Params  *C6ACCEL_TI_IQNdiv_paramPtr;
                    C6ACCEL_TI_IQNdiv_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_IQNdiv_paramPtr->num_InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IQNdiv_paramPtr->den_InArrID2)>INBUF15)|
                       ((C6ACCEL_TI_IQNdiv_paramPtr->OutArrID1)>OUTBUF15)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else{
                         /* Call underlying kernel */
                         inArrRdr1 = (I32_IQ *)inBufs->descs[C6ACCEL_TI_IQNdiv_paramPtr->num_InArrID1].buf;
                         inArrRdr2 = (I32_IQ *)inBufs->descs[C6ACCEL_TI_IQNdiv_paramPtr->den_InArrID2].buf;
                         outArrRdr = (I32_IQ *)outBufs->descs[C6ACCEL_TI_IQNdiv_paramPtr->OutArrID1].buf;
                         for(j=0 ; j < (C6ACCEL_TI_IQNdiv_paramPtr->n); j++){
                             outArrRdr[j] = _IQNdiv(inArrRdr1[j], inArrRdr2[j], C6ACCEL_TI_IQNdiv_paramPtr->qfmt);
                          }
                      }
                  }
                 break;
                 case(IQNATAN2_FXN_ID):{
                    /* Unmarshal Parameters */
                    I32_IQ *inArrRdr1, *inArrRdr2, *outArrRdr;
                    IQNatan2_Params *C6ACCEL_TI_IQNatan2_paramPtr;
                    C6ACCEL_TI_IQNatan2_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_IQNatan2_paramPtr->y_InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IQNatan2_paramPtr->x_InArrID2)>INBUF15)|
                       ((C6ACCEL_TI_IQNatan2_paramPtr->OutArrID1)>OUTBUF15)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else{
                         /* Call underlying kernel */
                         inArrRdr1 = (I32_IQ *)inBufs->descs[C6ACCEL_TI_IQNatan2_paramPtr->y_InArrID1].buf;
                         inArrRdr2 = (I32_IQ *)inBufs->descs[C6ACCEL_TI_IQNatan2_paramPtr->x_InArrID2].buf;
                         outArrRdr = (I32_IQ *)outBufs->descs[C6ACCEL_TI_IQNatan2_paramPtr->OutArrID1].buf;
                         for(j=0 ; j < (C6ACCEL_TI_IQNatan2_paramPtr->n); j++){
                             outArrRdr[j] = _IQNatan2(inArrRdr1[j], inArrRdr2[j], C6ACCEL_TI_IQNatan2_paramPtr->qfmt);
                          }
                      }
                  }
                 break;
                 case(IQNEXP_FXN_ID):{
                    /* Unmarshal Parameters */
                    I32_IQ *inArrRdr1, *outArrRdr;
                    IQNexp_Params  *C6ACCEL_TI_IQNexp_paramPtr;
                    C6ACCEL_TI_IQNexp_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_IQNexp_paramPtr->InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IQNexp_paramPtr->OutArrID1)>OUTBUF15)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else{
                         /* Call underlying kernel */
                         inArrRdr1 = (I32_IQ *)inBufs->descs[C6ACCEL_TI_IQNexp_paramPtr->InArrID1].buf;
                         outArrRdr = (I32_IQ *)outBufs->descs[C6ACCEL_TI_IQNexp_paramPtr->OutArrID1].buf;
                         for(j=0 ; j < (C6ACCEL_TI_IQNexp_paramPtr-> n); j++){
                             outArrRdr[j] = _IQNexp(inArrRdr1[j], C6ACCEL_TI_IQNexp_paramPtr->qfmt);
                           }
                       }
                   }
                 break;
                 case(IQNLOG_FXN_ID):{
                    /* Unmarshal Parameters */
                    I32_IQ *inArrRdr1, *outArrRdr;
                    IQNlog_Params *C6ACCEL_TI_IQNlog_paramPtr;
                    C6ACCEL_TI_IQNlog_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_IQNlog_paramPtr->InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IQNlog_paramPtr->OutArrID1)>OUTBUF15)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else{
                         /* Call underlying kernel */
                         inArrRdr1 = (I32_IQ *)inBufs->descs[C6ACCEL_TI_IQNlog_paramPtr->InArrID1].buf;
                         outArrRdr = (I32_IQ *)outBufs->descs[C6ACCEL_TI_IQNlog_paramPtr->OutArrID1].buf;
                         for(j=0 ; j < (C6ACCEL_TI_IQNlog_paramPtr-> n); j++){
                             outArrRdr[j] = _IQNlog(inArrRdr1[j], C6ACCEL_TI_IQNlog_paramPtr->qfmt);
                          }
                      }
                  }
                 break;
                 case(IQNABS_FXN_ID):{
                    /* Unmarshal Parameters */
                    I32_IQ *inArrRdr1, *outArrRdr;
                    IQNabs_Params *C6ACCEL_TI_IQNabs_paramPtr;
                    C6ACCEL_TI_IQNabs_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_IQNabs_paramPtr->InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IQNabs_paramPtr->OutArrID1)>OUTBUF15)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else{
                         /* Call underlying kernel */
                         inArrRdr1 = (I32_IQ *)inBufs->descs[C6ACCEL_TI_IQNabs_paramPtr->InArrID1].buf;
                         outArrRdr = (I32_IQ *)outBufs->descs[C6ACCEL_TI_IQNabs_paramPtr->OutArrID1].buf;
                         for(j=0 ; j < (C6ACCEL_TI_IQNabs_paramPtr-> n); j++){
                              outArrRdr[j] = _IQNabs(inArrRdr1[j]);
                          }
                      }
                  }
                 break;
                 case(IQNRMPY_FXN_ID):{
                    /* Unmarshal Parameters */
                    I32_IQ *inArrRdr1, *inArrRdr2, *outArrRdr;
                    IQNrmpy_Params *C6ACCEL_TI_IQNrmpy_paramPtr;
                    C6ACCEL_TI_IQNrmpy_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_IQNrmpy_paramPtr->InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IQNrmpy_paramPtr->InArrID2)>INBUF15)|
                       ((C6ACCEL_TI_IQNrmpy_paramPtr->OutArrID1)>OUTBUF15)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else{
                         /* Call underlying kernel */
                         inArrRdr1 = (I32_IQ *)inBufs->descs[C6ACCEL_TI_IQNrmpy_paramPtr->InArrID1].buf;
                         inArrRdr2 = (I32_IQ *)inBufs->descs[C6ACCEL_TI_IQNrmpy_paramPtr->InArrID2].buf;
                         outArrRdr = (I32_IQ *)outBufs->descs[C6ACCEL_TI_IQNrmpy_paramPtr->OutArrID1].buf;
                         for(j=0 ; j < (C6ACCEL_TI_IQNrmpy_paramPtr-> n); j++){
                             outArrRdr[j] = _IQNrmpy(inArrRdr1[j], inArrRdr2[j], C6ACCEL_TI_IQNrmpy_paramPtr->qfmt);
                           }
                       }
                  }
                 break;
                 case(IQNRSMPY_FXN_ID):{
                    /* Unmarshal Parameters */
                    I32_IQ *inArrRdr1, *inArrRdr2, *outArrRdr;
                    IQNrsmpy_Params *C6ACCEL_TI_IQNrsmpy_paramPtr;
                    C6ACCEL_TI_IQNrsmpy_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_IQNrsmpy_paramPtr->InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IQNrsmpy_paramPtr->InArrID2)>INBUF15)|
                       ((C6ACCEL_TI_IQNrsmpy_paramPtr->OutArrID1)>OUTBUF15)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else{
                         /* Call underlying kernel */
                         inArrRdr1 = (I32_IQ *)inBufs->descs[C6ACCEL_TI_IQNrsmpy_paramPtr->InArrID1].buf;
                         inArrRdr2 = (I32_IQ *)inBufs->descs[C6ACCEL_TI_IQNrsmpy_paramPtr->InArrID2].buf;
                         outArrRdr = (I32_IQ *)outBufs->descs[C6ACCEL_TI_IQNrsmpy_paramPtr->OutArrID1].buf;
                         for(j=0 ; j < (C6ACCEL_TI_IQNrsmpy_paramPtr-> n); j++){
                              outArrRdr[j] = _IQNrsmpy(inArrRdr1[j], inArrRdr2[j], C6ACCEL_TI_IQNrsmpy_paramPtr->qfmt);
                          }
                      }
                  }
                 break;
                 case(IQNASIN_FXN_ID):{
                    /* Unmarshal Parameters */
                    I32_IQ *inArrRdr1, *outArrRdr;
                    IQNasin_Params *C6ACCEL_TI_IQNasin_paramPtr;
                    C6ACCEL_TI_IQNasin_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_IQNasin_paramPtr->InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IQNasin_paramPtr->OutArrID1)>OUTBUF15)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else{
                         /* Call underlying kernel */
                         inArrRdr1 = (I32_IQ *)inBufs->descs[C6ACCEL_TI_IQNasin_paramPtr->InArrID1].buf;
                         outArrRdr = (I32_IQ *)outBufs->descs[C6ACCEL_TI_IQNasin_paramPtr->OutArrID1].buf;
                         for(j=0 ; j < (C6ACCEL_TI_IQNasin_paramPtr-> n); j++){
                             outArrRdr[j] = _IQNasin(inArrRdr1[j], C6ACCEL_TI_IQNasin_paramPtr->qfmt);
                          }
                      }
                  }
                 break;
                 case(IQNACOS_FXN_ID):{
                    /* Unmarshal Parameters */
                    I32_IQ *inArrRdr1, *outArrRdr;
                    IQNacos_Params  *C6ACCEL_TI_IQNacos_paramPtr;
                    C6ACCEL_TI_IQNacos_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_IQNacos_paramPtr->InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IQNacos_paramPtr->OutArrID1)>OUTBUF15)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else{
                         /* Call underlying kernel */
                         inArrRdr1 = (I32_IQ *)inBufs->descs[C6ACCEL_TI_IQNacos_paramPtr->InArrID1].buf;
                         outArrRdr = (I32_IQ *)outBufs->descs[C6ACCEL_TI_IQNacos_paramPtr->OutArrID1].buf;
                         for(j=0 ; j < (C6ACCEL_TI_IQNacos_paramPtr-> n); j++){
                             outArrRdr[j] = _IQNacos(inArrRdr1[j], C6ACCEL_TI_IQNacos_paramPtr->qfmt);
                          }
                      }
                  }
                 break;
                 case(IQNSINPU_FXN_ID):{
                    /* Unmarshal Parameters */
                    I32_IQ *inArrRdr1, *outArrRdr;
                    IQNsinPU_Params *C6ACCEL_TI_IQNsinPU_paramPtr;
                    C6ACCEL_TI_IQNsinPU_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_IQNsinPU_paramPtr->InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IQNsinPU_paramPtr->OutArrID1)>OUTBUF15)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else{
                        /* Call underlying kernel */
                        inArrRdr1 = (I32_IQ *)inBufs->descs[C6ACCEL_TI_IQNsinPU_paramPtr->InArrID1].buf;
                        outArrRdr = (I32_IQ *)outBufs->descs[C6ACCEL_TI_IQNsinPU_paramPtr->OutArrID1].buf;
                        for(j=0 ; j < (C6ACCEL_TI_IQNsinPU_paramPtr-> n); j++){
                            outArrRdr[j] = _IQNsinPU(inArrRdr1[j], C6ACCEL_TI_IQNsinPU_paramPtr->qfmt);
                         }
                      }
                   }
                 break;
                 case(IQNCOSPU_FXN_ID):{
                    /* Unmarshal Parameters */
                    I32_IQ *inArrRdr1, *outArrRdr;
                    IQNcosPU_Params *C6ACCEL_TI_IQNcosPU_paramPtr;
                    C6ACCEL_TI_IQNcosPU_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_IQNcosPU_paramPtr->InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IQNcosPU_paramPtr->OutArrID1)>OUTBUF15)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else{
                         /* Call underlying kernel */
                         inArrRdr1 = (I32_IQ *)inBufs->descs[C6ACCEL_TI_IQNcosPU_paramPtr->InArrID1].buf;
                         outArrRdr = (I32_IQ *)outBufs->descs[C6ACCEL_TI_IQNcosPU_paramPtr->OutArrID1].buf;
                         for(j=0 ; j < (C6ACCEL_TI_IQNcosPU_paramPtr-> n); j++){
                             outArrRdr[j] = _IQNcosPU(inArrRdr1[j], C6ACCEL_TI_IQNcosPU_paramPtr->qfmt);
                          }
                      }
                  }
                 break;
                 case(IQNATAN2PU_FXN_ID):{
                    /* Unmarshal Parameters */
                    I32_IQ *inArrRdr1, *inArrRdr2, *outArrRdr;
                    IQNatan2PU_Params *C6ACCEL_TI_IQNatan2PU_paramPtr;
                    C6ACCEL_TI_IQNatan2PU_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_IQNatan2PU_paramPtr->y_InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IQNatan2PU_paramPtr->x_InArrID2)>INBUF15)|
                       ((C6ACCEL_TI_IQNatan2PU_paramPtr->OutArrID1)>OUTBUF15)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else{
                         /* Call underlying kernel */
                         inArrRdr1 = (I32_IQ *)inBufs->descs[C6ACCEL_TI_IQNatan2PU_paramPtr->y_InArrID1].buf;
                         inArrRdr2 = (I32_IQ *)inBufs->descs[C6ACCEL_TI_IQNatan2PU_paramPtr->x_InArrID2].buf;
                         outArrRdr = (I32_IQ *)outBufs->descs[C6ACCEL_TI_IQNatan2PU_paramPtr->OutArrID1].buf;
                         for(j=0 ; j < (C6ACCEL_TI_IQNatan2PU_paramPtr->n); j++){
                              outArrRdr[j] = _IQNatan2PU(inArrRdr1[j], inArrRdr2[j], C6ACCEL_TI_IQNatan2PU_paramPtr->qfmt);
                           }
                       }
                   }
                 break;
                 case(IQNMPYIQX_FXN_ID):{
                    /* Unmarshal Parameters */
                    I32_IQ *inArrRdr1, *inArrRdr2, *outArrRdr;
                    IQNmpyIQX_Params *C6ACCEL_TI_IQNmpyIQX_paramPtr;
                    C6ACCEL_TI_IQNmpyIQX_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_IQNmpyIQX_paramPtr->InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IQNmpyIQX_paramPtr->InArrID2)>INBUF15)|
                       ((C6ACCEL_TI_IQNmpyIQX_paramPtr->OutArrID1)>OUTBUF15)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                     else{
                         /* Call underlying kernel */
                         inArrRdr1 = (I32_IQ *)inBufs->descs[C6ACCEL_TI_IQNmpyIQX_paramPtr->InArrID1].buf;
                         inArrRdr2 = (int *)inBufs->descs[C6ACCEL_TI_IQNmpyIQX_paramPtr->InArrID2].buf;
                         outArrRdr = (I32_IQ *)outBufs->descs[C6ACCEL_TI_IQNmpyIQX_paramPtr->OutArrID1].buf;
                         for(j=0 ; j < (C6ACCEL_TI_IQNmpyIQX_paramPtr->n); j++){
                             outArrRdr[j] = _IQNmpyIQx(inArrRdr1[j],
                                                       C6ACCEL_TI_IQNmpyIQX_paramPtr->qfmt1,
                                                       inArrRdr2[j],
                                                       C6ACCEL_TI_IQNmpyIQX_paramPtr->qfmt2,
                                                       C6ACCEL_TI_IQNmpyIQX_paramPtr->qfmt);
                          }
                      }
                  }
                 break;
                 case(IQNMPYI32INT_FXN_ID):{
                     /* Unmarshal Parameters */
                     I32_IQ *inArrRdr1, *inArrRdr2, *outArrRdr;
                     IQNmpyI32int_Params *C6ACCEL_TI_IQNmpyI32int_paramPtr;
                     C6ACCEL_TI_IQNmpyI32int_paramPtr = pFnArray;
                     /* Parameter check */
                     if(((C6ACCEL_TI_IQNmpyI32int_paramPtr->InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_IQNmpyI32int_paramPtr->InArrID2)>INBUF15)|
                        ((C6ACCEL_TI_IQNmpyI32int_paramPtr->OutArrID1)>OUTBUF15)){
                          return(IUNIVERSAL_EPARAMFAIL);
                      }
                     else{
                          /* Call underlying kernel */
                          inArrRdr1 = (I32_IQ *)inBufs->descs[C6ACCEL_TI_IQNmpyI32int_paramPtr->InArrID1].buf;
                          inArrRdr2 = (int *)inBufs->descs[C6ACCEL_TI_IQNmpyI32int_paramPtr->InArrID2].buf;
                          outArrRdr = (I32_IQ *)outBufs->descs[C6ACCEL_TI_IQNmpyI32int_paramPtr->OutArrID1].buf;
                          for(j=0 ; j < (C6ACCEL_TI_IQNmpyI32int_paramPtr->n); j++){
                              outArrRdr[j] = _IQNmpyI32int(inArrRdr1[j], inArrRdr2[j], C6ACCEL_TI_IQNmpyI32int_paramPtr->qfmt);
                           }
                       }
                   }
                 break;
                 case(IQNMPYI32FRAC_FXN_ID):{
                    /* Unmarshal Parameters */
                    I32_IQ *inArrRdr1, *inArrRdr2, *outArrRdr;
                    IQNmpyI32frac_Params *C6ACCEL_TI_IQNmpyI32frac_paramPtr;
                    C6ACCEL_TI_IQNmpyI32frac_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_IQNmpyI32frac_paramPtr->InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IQNmpyI32frac_paramPtr->InArrID2)>INBUF15)|
                       ((C6ACCEL_TI_IQNmpyI32frac_paramPtr->OutArrID1)>OUTBUF15)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else{
                         /* Call underlying kernel */
                         inArrRdr1 = (I32_IQ *)inBufs->descs[C6ACCEL_TI_IQNmpyI32frac_paramPtr->InArrID1].buf;
                         inArrRdr2 = (int *)inBufs->descs[C6ACCEL_TI_IQNmpyI32frac_paramPtr->InArrID2].buf;
                         outArrRdr = (I32_IQ *)outBufs->descs[C6ACCEL_TI_IQNmpyI32frac_paramPtr->OutArrID1].buf;
                         for(j=0 ; j < (C6ACCEL_TI_IQNmpyI32frac_paramPtr->n); j++){
                             outArrRdr[j] = _IQNmpyI32frac(inArrRdr1[j], inArrRdr2[j], C6ACCEL_TI_IQNmpyI32frac_paramPtr->qfmt);
                          }
                     }
                 }
                 break;
                 case(IQNPOW_FXN_ID):{
                    /* Unmarshal Parameters */
                    I32_IQ *inArrRdr1, *inArrRdr2, *outArrRdr;
                    IQNpow_Params *C6ACCEL_TI_IQNpow_paramPtr;
                    C6ACCEL_TI_IQNpow_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_IQNpow_paramPtr->A_InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IQNpow_paramPtr->B_InArrID2)>INBUF15)|
                       ((C6ACCEL_TI_IQNpow_paramPtr->OutArrID1)>OUTBUF15)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else{
                         /* Call underlying kernel */
                         inArrRdr1 = (I32_IQ *)inBufs->descs[C6ACCEL_TI_IQNpow_paramPtr->A_InArrID1].buf;
                         inArrRdr2 = (I32_IQ *)inBufs->descs[C6ACCEL_TI_IQNpow_paramPtr->B_InArrID2].buf;
                         outArrRdr = (I32_IQ *)outBufs->descs[C6ACCEL_TI_IQNpow_paramPtr->OutArrID1].buf;
                         for(j=0 ; j < (C6ACCEL_TI_IQNpow_paramPtr-> n); j++){
                             outArrRdr[j] = _IQNpow(inArrRdr1[j], inArrRdr2[j], C6ACCEL_TI_IQNpow_paramPtr->qfmt);
                          }
                      }
                  }
                 break;
                 case(IQNISQRT_FXN_ID):{
                    /* Unmarshal Parameters */
                    I32_IQ *inArrRdr1, *outArrRdr;
                    IQNisqrt_Params *C6ACCEL_TI_IQNisqrt_paramPtr;
                    C6ACCEL_TI_IQNisqrt_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_IQNisqrt_paramPtr->InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IQNisqrt_paramPtr->OutArrID1)>OUTBUF15)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else{
                        /* Call underlying kernel */
                         inArrRdr1 = (I32_IQ *)inBufs->descs[C6ACCEL_TI_IQNisqrt_paramPtr->InArrID1].buf;
                         outArrRdr = (I32_IQ *)outBufs->descs[C6ACCEL_TI_IQNisqrt_paramPtr->OutArrID1].buf;
                         for(j=0 ; j < (C6ACCEL_TI_IQNisqrt_paramPtr-> n); j++){
                             outArrRdr[j] = _IQNisqrt(inArrRdr1[j], C6ACCEL_TI_IQNisqrt_paramPtr->qfmt);
                           }
                      }
                  }
                 break;
                 case(IQNMAG_FXN_ID):{
                    /* Unmarshal Parameters */
                    I32_IQ *inArrRdr1, *inArrRdr2, *outArrRdr;
                    IQNmag_Params *C6ACCEL_TI_IQNmag_paramPtr;
                    C6ACCEL_TI_IQNmag_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_IQNmag_paramPtr->InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IQNmag_paramPtr->InArrID2)>INBUF15)|
                       ((C6ACCEL_TI_IQNmag_paramPtr->OutArrID1)>OUTBUF15)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else{
                         /* Call underlying kernel */
                         inArrRdr1 = (I32_IQ *)inBufs->descs[C6ACCEL_TI_IQNmag_paramPtr->InArrID1].buf;
                         inArrRdr2 = (I32_IQ *)inBufs->descs[C6ACCEL_TI_IQNmag_paramPtr->InArrID2].buf;
                         outArrRdr = (I32_IQ *)outBufs->descs[C6ACCEL_TI_IQNmag_paramPtr->OutArrID1].buf;
                         for(j=0 ; j < (C6ACCEL_TI_IQNmag_paramPtr-> n); j++){
                             outArrRdr[j] = _IQNmag(inArrRdr1[j], inArrRdr2[j], C6ACCEL_TI_IQNmag_paramPtr->qfmt);
                          }
                      }
                  }
                 break;
                 case(IQNTOF_FXN_ID):{
                    /* Unmarshal Parameters */
                    I32_IQ *inArrRdr1;
                    float *out;
                    IQNtoF_Params *C6ACCEL_TI_IQNtoF_paramPtr;
                    C6ACCEL_TI_IQNtoF_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_IQNtoF_paramPtr->InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IQNtoF_paramPtr->OutArrID1)>OUTBUF15)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else{
                        /* Call underlying kernel */
                         inArrRdr1 = (I32_IQ *)inBufs->descs[C6ACCEL_TI_IQNtoF_paramPtr->InArrID1].buf;
                         out = (float *)outBufs->descs[C6ACCEL_TI_IQNtoF_paramPtr->OutArrID1].buf;
                         for(j=0 ; j < (C6ACCEL_TI_IQNtoF_paramPtr-> n); j++){
                             out[j] = _IQNtoF(inArrRdr1[j],C6ACCEL_TI_IQNtoF_paramPtr->qfmt);
                          }
                      }
                  }
                 break;
                 case(FTOIQN_FXN_ID):{
                    /* Unmarshal Parameters */
                    I32_IQ *outArrRdr;
                    float *in1;
                    FtoIQN_Params *C6ACCEL_TI_FtoIQN_paramPtr;
                    C6ACCEL_TI_FtoIQN_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_FtoIQN_paramPtr->InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_FtoIQN_paramPtr->OutArrID1)>OUTBUF15)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else{
                         /* Call underlying kernel */
                         in1 = (float *)inBufs->descs[C6ACCEL_TI_FtoIQN_paramPtr->InArrID1].buf;
                         outArrRdr = (I32_IQ *)outBufs->descs[C6ACCEL_TI_FtoIQN_paramPtr->OutArrID1].buf;
                         for(j=0 ; j < (C6ACCEL_TI_FtoIQN_paramPtr-> n); j++){
                             outArrRdr[j] = _FtoIQN(in1[j], C6ACCEL_TI_FtoIQN_paramPtr->qfmt);
                           }
                      }
                  }
                 break;
                 case(IQN_FXN_ID):{
                    /* Unmarshal Parameters */
                    I32_IQ *inArrRdr1, *outArrRdr;
                    IQN_Params *C6ACCEL_TI_IQN_paramPtr;
                    C6ACCEL_TI_IQN_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_IQN_paramPtr->InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IQN_paramPtr->OutArrID1)>OUTBUF15)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                     else{
                         /* Call underlying kernel */
                         inArrRdr1 = (I32_IQ *)inBufs->descs[C6ACCEL_TI_IQN_paramPtr->InArrID1].buf;
                         outArrRdr = (I32_IQ *)outBufs->descs[C6ACCEL_TI_IQN_paramPtr->OutArrID1].buf;
                         for(j=0 ; j < (C6ACCEL_TI_IQN_paramPtr-> n); j++){
                             outArrRdr[j] = (I32_IQ)(inArrRdr1[j] * pow((double)2,(double)C6ACCEL_TI_IQN_paramPtr->qfmt));
                           }
                       }
                   }
                 break;
                 case(IQXTOIQY_FXN_ID):{
                    I32_IQ *inArrRdr1, *outArrRdr;
                    /* Unmarshal Parameters */
                    IQXtoIQY_Params *C6ACCEL_TI_IQXtoIQY_paramPtr;
                    C6ACCEL_TI_IQXtoIQY_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_IQXtoIQY_paramPtr->InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IQXtoIQY_paramPtr->OutArrID1)>OUTBUF15)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else{
                         /* Call underlying kernel */
                         inArrRdr1 = (I32_IQ *)inBufs->descs[C6ACCEL_TI_IQXtoIQY_paramPtr->InArrID1].buf;
                         outArrRdr = (I32_IQ *)outBufs->descs[C6ACCEL_TI_IQXtoIQY_paramPtr->OutArrID1].buf;
                         for(j=0 ; j < (C6ACCEL_TI_IQXtoIQY_paramPtr-> n); j++){
                             outArrRdr[j] = _IQXtoIQY(inArrRdr1[j],C6ACCEL_TI_IQXtoIQY_paramPtr->x,C6ACCEL_TI_IQXtoIQY_paramPtr->y);
                           }
                       }
                   }
                 break;
                 case(IQNINT_FXN_ID):{
                    I32_IQ *inArrRdr1, *outArrRdr;
                    /* Unmarshal Parameters */
                    IQNint_Params *C6ACCEL_TI_IQNint_paramPtr;
                    C6ACCEL_TI_IQNint_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_IQNint_paramPtr->InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IQNint_paramPtr->OutArrID1)>OUTBUF15)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else{
                         /* Call underlying kernel */
                         inArrRdr1 = (I32_IQ *)inBufs->descs[C6ACCEL_TI_IQNint_paramPtr->InArrID1].buf;
                         outArrRdr = (I32_IQ *)outBufs->descs[C6ACCEL_TI_IQNint_paramPtr->OutArrID1].buf;
                         for(j=0 ; j < (C6ACCEL_TI_IQNint_paramPtr-> n); j++){
                             outArrRdr[j] = _IQNint(inArrRdr1[j], C6ACCEL_TI_IQNint_paramPtr->qfmt);
                           }
                       }
                   }
                 break;
                 case(IQNFRAC_FXN_ID):{
                    I32_IQ *inArrRdr1, *outArrRdr;
                    /* Unmarshal Parameters */
                    IQNfrac_Params *C6ACCEL_TI_IQNfrac_paramPtr;
                    C6ACCEL_TI_IQNfrac_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_IQNfrac_paramPtr->InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_IQNfrac_paramPtr->OutArrID1)>OUTBUF15)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                     else{
                         /* Call underlying kernel */
                         inArrRdr1 = (I32_IQ *)inBufs->descs[C6ACCEL_TI_IQNfrac_paramPtr->InArrID1].buf;
                         outArrRdr = (I32_IQ *)outBufs->descs[C6ACCEL_TI_IQNfrac_paramPtr->OutArrID1].buf;
                         for(j=0 ; j < (C6ACCEL_TI_IQNfrac_paramPtr-> n); j++){
                             outArrRdr[j] = _IQNfrac(inArrRdr1[j], C6ACCEL_TI_IQNfrac_paramPtr->qfmt);
                           }
                       }
                   }
                 break;
                 /* FASTRTS LIB functions */
                 case (ADDSP_FXN_ID):{
                    float  *in1, *in2, *out;
                    /* Unmarshal Parameters */
                    addsp_i_Params *C6ACCEL_TI_addsp_i_paramPtr;
                    C6ACCEL_TI_addsp_i_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_addsp_i_paramPtr->InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_addsp_i_paramPtr->InArrID2)>INBUF15)|
                       ((C6ACCEL_TI_addsp_i_paramPtr->OutArrID1)>OUTBUF15)){
                         return(IUNIVERSAL_EPARAMFAIL);
                      }
                     else{
                         /* Call underlying kernel */
                         in1 = (float *)inBufs->descs[C6ACCEL_TI_addsp_i_paramPtr->InArrID1].buf;
                         in2 = (float *)inBufs->descs[C6ACCEL_TI_addsp_i_paramPtr->InArrID2].buf;
                         out = (float *)outBufs->descs[C6ACCEL_TI_addsp_i_paramPtr->OutArrID1].buf;
                         for(j=0 ; j < (C6ACCEL_TI_addsp_i_paramPtr-> n); j++){
                            out[j] = addsp_i(in1[j], in2[j]);
                           }
                       }
                   }
                 break;
                 case (SUBSP_FXN_ID):{
                     float  *in1, *in2, *out;
                     /* Unmarshal Parameters */
                     subsp_i_Params *C6ACCEL_TI_subsp_i_paramPtr;
                     C6ACCEL_TI_subsp_i_paramPtr = pFnArray;
                     /* Parameter check */
                     if(((C6ACCEL_TI_subsp_i_paramPtr->InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_subsp_i_paramPtr->InArrID2)>INBUF15)|
                        ((C6ACCEL_TI_subsp_i_paramPtr->OutArrID1)>OUTBUF15)){
                          return(IUNIVERSAL_EPARAMFAIL);
                      }
                      else{
                          /* Call underlying kernel */
                           in1 = (float *)inBufs->descs[C6ACCEL_TI_subsp_i_paramPtr->InArrID1].buf;
                           in2 = (float *)inBufs->descs[C6ACCEL_TI_subsp_i_paramPtr->InArrID2].buf;
                           out = (float *)outBufs->descs[C6ACCEL_TI_subsp_i_paramPtr->OutArrID1].buf;
                           for(j=0 ; j < (C6ACCEL_TI_subsp_i_paramPtr-> n); j++){
                              out[j] = subsp_i(in1[j], in2[j]);
                             }
                         }
                    }
                 break;
                 case (MPYSP_FXN_ID):{
                     float  *in1, *in2, *out;
                     /* Unmarshal Parameters */
                     mpysp_i_Params *C6ACCEL_TI_mpysp_i_paramPtr;
                     C6ACCEL_TI_mpysp_i_paramPtr = pFnArray;
                     /* Parameter check */
                     if(((C6ACCEL_TI_mpysp_i_paramPtr->InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_mpysp_i_paramPtr->InArrID2)>INBUF15)|
                        ((C6ACCEL_TI_mpysp_i_paramPtr->OutArrID1)>OUTBUF15)){
                          return(IUNIVERSAL_EPARAMFAIL);
                      }
                      else{
                           /* Call underlying kernel */
                           in1 = (float *)inBufs->descs[C6ACCEL_TI_mpysp_i_paramPtr->InArrID1].buf;
                           in2 = (float *)inBufs->descs[C6ACCEL_TI_mpysp_i_paramPtr->InArrID2].buf;
                           out = (float *)outBufs->descs[C6ACCEL_TI_mpysp_i_paramPtr->OutArrID1].buf;
                           for(j=0 ; j < (C6ACCEL_TI_mpysp_i_paramPtr-> n); j++){
                              out[j] = mpysp_i(in1[j], in2[j]);
                            }
                        }
                   }
                 break;
                 case (DIVSP_FXN_ID):{
                     float  *in1, *in2, *out;
                     /* Unmarshal Parameters */
                     divsp_i_Params  *C6ACCEL_TI_divsp_i_paramPtr;
                     C6ACCEL_TI_divsp_i_paramPtr = pFnArray;
                     /* Parameter check */
                     if(((C6ACCEL_TI_divsp_i_paramPtr->InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_divsp_i_paramPtr->InArrID2)>INBUF15)|
                        ((C6ACCEL_TI_divsp_i_paramPtr->OutArrID1)>OUTBUF15)){
                          return(IUNIVERSAL_EPARAMFAIL);
                      }
                      else{
                           /* Call underlying kernel */
                           in1 = (float *)inBufs->descs[C6ACCEL_TI_divsp_i_paramPtr->InArrID1].buf;
                           in2 = (float *)inBufs->descs[C6ACCEL_TI_divsp_i_paramPtr->InArrID2].buf;
                           out = (float *)outBufs->descs[C6ACCEL_TI_divsp_i_paramPtr->OutArrID1].buf;
                           for(j=0 ; j < (C6ACCEL_TI_divsp_i_paramPtr-> n); j++){
                              out[j] = divsp_i(in1[j], in2[j]);
                             }
                        }
                   }
                 break;
                 case (RECIPSP_FXN_ID):{
                     float  *in1, *out;
                     /* Unmarshal Parameters */
                     recipsp_i_Params *C6ACCEL_TI_recipsp_i_paramPtr;
                     C6ACCEL_TI_recipsp_i_paramPtr = pFnArray;
                     /* Parameter check */
                     if(((C6ACCEL_TI_recipsp_i_paramPtr->x_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_recipsp_i_paramPtr->OutArrID1)>OUTBUF15)){
                          return(IUNIVERSAL_EPARAMFAIL);
                      }
                     else{
                          /* Call underlying kernel */
                          in1 = (float *)inBufs->descs[C6ACCEL_TI_recipsp_i_paramPtr->x_InArrID1].buf;
                          out = (float *)outBufs->descs[C6ACCEL_TI_recipsp_i_paramPtr->OutArrID1].buf;
                          for(j=0 ; j < (C6ACCEL_TI_recipsp_i_paramPtr-> n); j++){
                              out[j] = recipsp_i(in1[j]);
                           }
                       }
                   }
                 break;
                 case (INTSP_FXN_ID):{
                     I32_IQ *inArrRdr1;
                     float *out;
                     /* Unmarshal Parameters */
                     intsp_i_Params *C6ACCEL_TI_intsp_i_paramPtr;
                     C6ACCEL_TI_intsp_i_paramPtr = pFnArray;
                     /* Parameter check */
                     if(((C6ACCEL_TI_intsp_i_paramPtr->x_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_intsp_i_paramPtr->OutArrID1)>OUTBUF15)){
                          return(IUNIVERSAL_EPARAMFAIL);
                      }
                     else{
                          /* Call underlying kernel */
                         inArrRdr1 = (int *)inBufs->descs[C6ACCEL_TI_intsp_i_paramPtr->x_InArrID1].buf;
                         out = (float *)outBufs->descs[C6ACCEL_TI_intsp_i_paramPtr->OutArrID1].buf;
                         for(j=0 ; j < (C6ACCEL_TI_intsp_i_paramPtr-> n); j++){
                             out[j] = intsp_i(inArrRdr1[j]);
                           }
                       }
                   }
                 break;
                 case (SPINT_FXN_ID):{
                     I32_IQ *outArrRdr;
                     float *in1;
                     /* Unmarshal Parameters */
                     spint_i_Params *C6ACCEL_TI_spint_i_paramPtr;
                     C6ACCEL_TI_spint_i_paramPtr = pFnArray;
                     /* Parameter check */
                     if(((C6ACCEL_TI_spint_i_paramPtr->x_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_spint_i_paramPtr->OutArrID1)>OUTBUF15)){
                          return(IUNIVERSAL_EPARAMFAIL);
                      }
                     else{
                          /* Call underlying kernel */
                         in1 = (float *)inBufs->descs[C6ACCEL_TI_spint_i_paramPtr->x_InArrID1].buf;
                         outArrRdr = (int *)outBufs->descs[C6ACCEL_TI_spint_i_paramPtr->OutArrID1].buf;
                         for(j=0 ; j < (C6ACCEL_TI_spint_i_paramPtr-> n); j++){
                             outArrRdr[j] = spint_i(in1[j]);
                          }
                       }
                   }
                 break;
                 case (SPUINT_FXN_ID):{
                    I32_IQ *outArrRdr;
                    float *in1;
                    /* Unmarshal Parameters */
                    spuint_i_Params *C6ACCEL_TI_spuint_i_paramPtr;
                    C6ACCEL_TI_spuint_i_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_spuint_i_paramPtr->x_InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_spuint_i_paramPtr->OutArrID1)>OUTBUF15)){
                       return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else{
                         /* Call underlying kernel */
                         in1 = (float *)inBufs->descs[C6ACCEL_TI_spuint_i_paramPtr->x_InArrID1].buf;
                         outArrRdr = (int *)outBufs->descs[C6ACCEL_TI_spuint_i_paramPtr->OutArrID1].buf;
                         for(j=0 ; j < (C6ACCEL_TI_spuint_i_paramPtr-> n); j++){
                             outArrRdr[j] = spuint_i(in1[j]);
                          }
                      }
                  }
                 break;
                 case (SQRTSP_FXN_ID):{
                    float  *in1, *out;
                    /* Unmarshal Parameters */
                    sqrtsp_i_Params *C6ACCEL_TI_sqrtsp_i_paramPtr;
                    C6ACCEL_TI_sqrtsp_i_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_sqrtsp_i_paramPtr->x_InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_sqrtsp_i_paramPtr->OutArrID1)>OUTBUF15)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else{
                         /* Call underlying kernel */
                         in1 = (float *)inBufs->descs[C6ACCEL_TI_sqrtsp_i_paramPtr->x_InArrID1].buf;
                         out = (float *)outBufs->descs[C6ACCEL_TI_sqrtsp_i_paramPtr->OutArrID1].buf;
                         for(j=0 ; j < (C6ACCEL_TI_sqrtsp_i_paramPtr-> n); j++){
                             out[j] = sqrtsp_i(in1[j]);
                          }
                      }
                  }
                 break;
                 case (UINTSP_FXN_ID):{
                    I32_IQ *inArrRdr1;
                    float *out;
                    /* Unmarshal Parameters */
                    uintsp_i_Params *C6ACCEL_TI_uintsp_i_paramPtr;
                    C6ACCEL_TI_uintsp_i_paramPtr = pFnArray;
                    /* Parameter check */
                    if(((C6ACCEL_TI_uintsp_i_paramPtr->x_InArrID1)>INBUF15)|
                       ((C6ACCEL_TI_uintsp_i_paramPtr->OutArrID1)>OUTBUF15)){
                         return(IUNIVERSAL_EPARAMFAIL);
                     }
                    else{
                        /* Call underlying kernel */
                         inArrRdr1 = (int *)inBufs->descs[C6ACCEL_TI_uintsp_i_paramPtr->x_InArrID1].buf;
                         out = (float *)outBufs->descs[C6ACCEL_TI_uintsp_i_paramPtr->OutArrID1].buf;
                         for(j=0 ; j < (C6ACCEL_TI_uintsp_i_paramPtr-> n); j++){
                             out[j] = uintsp_i(inArrRdr1[j]);
                          }
                      }
                   }
                 break;
                 #ifdef DEVICE_FLOAT
                 case (ATANSP_FXN_ID):{
                     float  *in1, *out;
                     /* Unmarshal Parameters */
                     atansp_Params *C6ACCEL_TI_atansp_paramPtr;
                     C6ACCEL_TI_atansp_paramPtr = pFnArray;
                     /* Parameter check */
                     if(((C6ACCEL_TI_atansp_paramPtr->z_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_atansp_paramPtr->OutArrID1)>OUTBUF15)){
                        return(IUNIVERSAL_EPARAMFAIL);
                      }
                     else{
                          /* Call underlying kernel */
                          in1 = (float *)inBufs->descs[C6ACCEL_TI_atansp_paramPtr->z_InArrID1].buf;
                          out = (float *)outBufs->descs[C6ACCEL_TI_atansp_paramPtr->OutArrID1].buf;
                          for(j=0 ; j < (C6ACCEL_TI_atansp_paramPtr->n); j++){
                              out[j] = atansp(in1[j]);
                           }
                      }
                  }
                 break;
                 case (COSSP_FXN_ID):{
                        float  *in1, *out;
                        /* Unmarshal Parameters */
                        cossp_Params *C6ACCEL_TI_cossp_paramPtr;
                        C6ACCEL_TI_cossp_paramPtr = pFnArray;
                        /* Parameter check */
                         if(((C6ACCEL_TI_cossp_paramPtr->z_InArrID1)>INBUF15)|
                            ((C6ACCEL_TI_cossp_paramPtr->OutArrID1)>OUTBUF15)){
                             return(IUNIVERSAL_EPARAMFAIL);
                          }
                         else{
                              /* Call underlying kernel */
                              in1 = (float *)inBufs->descs[C6ACCEL_TI_cossp_paramPtr->z_InArrID1].buf;
                              out = (float *)outBufs->descs[C6ACCEL_TI_cossp_paramPtr->OutArrID1].buf;
                              for(j=0 ; j < (C6ACCEL_TI_cossp_paramPtr-> n); j++){
                                out[j] = cossp(in1[j]);
                             }
                         }
                    }
                 break;
                 case (SINSP_FXN_ID):{
                     float  *in1, *out;
                     /* Unmarshal Parameters */
                     sinsp_Params *C6ACCEL_TI_sinsp_paramPtr;
                     C6ACCEL_TI_sinsp_paramPtr = pFnArray;
                     /* Parameter check */
                     if(((C6ACCEL_TI_sinsp_paramPtr->x_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_sinsp_paramPtr->OutArrID1)>OUTBUF15)){
                          return(IUNIVERSAL_EPARAMFAIL);
                      }
                     else{
                          /* Call underlying kernel */
                          in1 = (float *)inBufs->descs[C6ACCEL_TI_sinsp_paramPtr->x_InArrID1].buf;
                          out = (float *)outBufs->descs[C6ACCEL_TI_sinsp_paramPtr->OutArrID1].buf;
                          for(j=0 ; j < (C6ACCEL_TI_sinsp_paramPtr-> n); j++){
                                out[j] = sinsp(in1[j]);
                            }
                        }
                    }
                 break;
                 case (EXPSP_FXN_ID):{
                     float  *in1, *out;
                     /* Unmarshal Parameters */
                     expsp_Params *C6ACCEL_TI_expsp_paramPtr;
                     C6ACCEL_TI_expsp_paramPtr = pFnArray;
                     /* Parameter check */
                     if(((C6ACCEL_TI_expsp_paramPtr->z_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_expsp_paramPtr->OutArrID1)>OUTBUF15)){
                          return(IUNIVERSAL_EPARAMFAIL);
                      }
                     else{
                          /* Call underlying kernel */
                          in1 = (float *)inBufs->descs[C6ACCEL_TI_expsp_paramPtr->z_InArrID1].buf;
                          out = (float *)outBufs->descs[C6ACCEL_TI_expsp_paramPtr->OutArrID1].buf;
                          for(j=0 ; j < (C6ACCEL_TI_expsp_paramPtr->n); j++){
                              out[j] = expsp(in1[j]);
                            }
                        }
                    }
                 break;
                 case (LOGSP_FXN_ID):{
                     float  *in1, *out;
                     /* Unmarshal Parameters */
                     logsp_Params *C6ACCEL_TI_logsp_paramPtr;
                     C6ACCEL_TI_logsp_paramPtr = pFnArray;
                     /* Parameter check */
                     if(((C6ACCEL_TI_logsp_paramPtr->z_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_logsp_paramPtr->OutArrID1)>OUTBUF15)){
                          return(IUNIVERSAL_EPARAMFAIL);
                      }
                     else{
                          /* Call underlying kernel */
                          in1 = (float *)inBufs->descs[C6ACCEL_TI_logsp_paramPtr->z_InArrID1].buf;
                          out = (float *)outBufs->descs[C6ACCEL_TI_logsp_paramPtr->OutArrID1].buf;
                          for(j=0 ; j < (C6ACCEL_TI_logsp_paramPtr-> n); j++){
                              out[j] = logsp(in1[j]);
                           }
                       }
                  }
                 break;
                 case (POWSP_FXN_ID):{
                     float  *in1, *in2, *out;
                     /* Unmarshal Parameters */
                     powsp_Params *C6ACCEL_TI_powsp_paramPtr;
                     C6ACCEL_TI_powsp_paramPtr = pFnArray;
                     /* Parameter check */
                     if(((C6ACCEL_TI_powsp_paramPtr->x_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_powsp_paramPtr->y_InArrID2)>INBUF15)|
                        ((C6ACCEL_TI_powsp_paramPtr->OutArrID1)>OUTBUF15)){
                          return(IUNIVERSAL_EPARAMFAIL);
                      }
                     else{
                          /* Call underlying kernel */
                          in1 = (float *)inBufs->descs[C6ACCEL_TI_powsp_paramPtr->x_InArrID1].buf;
                          in2 = (float *)inBufs->descs[C6ACCEL_TI_powsp_paramPtr->y_InArrID2].buf;
                          out = (float *)outBufs->descs[C6ACCEL_TI_powsp_paramPtr->OutArrID1].buf;
                          for(j=0 ; j < (C6ACCEL_TI_powsp_paramPtr-> n); j++){
                              out[j] = powsp(in1[j],in2[j]);
                           }
                       }
                  }
                 break;
                 case (ATANDP_FXN_ID):{
                     float  *in1, *out;
                     /* Unmarshal Parameters */
                     atandp_Params *C6ACCEL_TI_atandp_paramPtr;
                     C6ACCEL_TI_atandp_paramPtr = pFnArray;
                     /* Parameter check */
                     if(((C6ACCEL_TI_atandp_paramPtr->z_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_atandp_paramPtr->OutArrID1)>OUTBUF15)){
                          return(IUNIVERSAL_EPARAMFAIL);
                      }
                     else{
                          /* Call underlying kernel */
                          in1 = (float *)inBufs->descs[C6ACCEL_TI_atandp_paramPtr->z_InArrID1].buf;
                          out = (float *)outBufs->descs[C6ACCEL_TI_atandp_paramPtr->OutArrID1].buf;
                          for(j=0 ; j < (C6ACCEL_TI_atandp_paramPtr->n); j++){
                              out[j] = atandp(in1[j]);
                           }
                       }
                  }
                 break;
                 case (COSDP_FXN_ID):{
                     float  *in1, *out;
                     /* Unmarshal Parameters */
                     cosdp_Params *C6ACCEL_TI_cosdp_paramPtr;
                     C6ACCEL_TI_cosdp_paramPtr = pFnArray;
                     /* Parameter check */
                     if(((C6ACCEL_TI_cosdp_paramPtr->z_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_cosdp_paramPtr->OutArrID1)>OUTBUF15)){
                          return(IUNIVERSAL_EPARAMFAIL);
                      }
                      else{
                         /* Call underlying kernel */
                         in1 = (float *)inBufs->descs[C6ACCEL_TI_cosdp_paramPtr->z_InArrID1].buf;
                         out = (float *)outBufs->descs[C6ACCEL_TI_cosdp_paramPtr->OutArrID1].buf;
                         for(j=0 ; j < (C6ACCEL_TI_cosdp_paramPtr-> n); j++){
                                out[j] = cosdp(in1[j]);
                           }
                       }
                  }
                 break;
                 case (SINDP_FXN_ID):{
                     float  *in1, *out;
                     /* Unmarshal Parameters */
                     sindp_Params *C6ACCEL_TI_sindp_paramPtr;
                     C6ACCEL_TI_sindp_paramPtr = pFnArray;
                     /* Parameter check */
                     if(((C6ACCEL_TI_sindp_paramPtr->z_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_sindp_paramPtr->OutArrID1)>OUTBUF15)){
                          return(IUNIVERSAL_EPARAMFAIL);
                      }
                     else{
                          /* Call underlying kernel */
                          in1 = (float *)inBufs->descs[C6ACCEL_TI_sindp_paramPtr->z_InArrID1].buf;
                          out = (float *)outBufs->descs[C6ACCEL_TI_sindp_paramPtr->OutArrID1].buf;
                          for(j=0 ; j < (C6ACCEL_TI_sindp_paramPtr-> n); j++){
                              out[j] = sindp(in1[j]);
                           }
                      }
                  }
                 break;
                 case (EXPDP_FXN_ID):{
                     float  *in1, *out;
                     /* Unmarshal Parameters */
                     expdp_Params *C6ACCEL_TI_expdp_paramPtr;
                     C6ACCEL_TI_expdp_paramPtr = pFnArray;
                     /* Parameter check */
                     if(((C6ACCEL_TI_expdp_paramPtr->z_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_expdp_paramPtr->OutArrID1)>OUTBUF15)){
                          return(IUNIVERSAL_EPARAMFAIL);
                      }
                     else{
                          /* Call underlying kernel */
                          in1 = (float *)inBufs->descs[C6ACCEL_TI_expdp_paramPtr->z_InArrID1].buf;
                          out = (float *)outBufs->descs[C6ACCEL_TI_expdp_paramPtr->OutArrID1].buf;
                          for(j=0 ; j < (C6ACCEL_TI_expdp_paramPtr->n); j++){
                              out[j] = expdp(in1[j]);
                            }
                        }
                    }
                 break;
                 case (LOGDP_FXN_ID):{
                     float  *in1, *out;
                     /* Unmarshal Parameters */
                     logdp_Params *C6ACCEL_TI_logdp_paramPtr;
                     C6ACCEL_TI_logdp_paramPtr = pFnArray;
                     /* Parameter check */
                     if(((C6ACCEL_TI_logdp_paramPtr->z_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_logdp_paramPtr->OutArrID1)>OUTBUF15)){
                          return(IUNIVERSAL_EPARAMFAIL);
                      }
                     else{
                          /* Call underlying kernel */
                          in1 = (float *)inBufs->descs[C6ACCEL_TI_logdp_paramPtr->z_InArrID1].buf;
                          out = (float *)outBufs->descs[C6ACCEL_TI_logdp_paramPtr->OutArrID1].buf;
                          for(j=0 ; j < (C6ACCEL_TI_logdp_paramPtr-> n); j++){
                              out[j] = logdp(in1[j]);
                            }
                       }
                  }
                 break;
                 case (POWDP_FXN_ID):{
                     float  *in1,*in2, *out;
                     /* Unmarshal Parameters */
                     powdp_Params *C6ACCEL_TI_powdp_paramPtr;
                     C6ACCEL_TI_powdp_paramPtr = pFnArray;
                     /* Parameter check */
                     if(((C6ACCEL_TI_powdp_paramPtr->x_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_powdp_paramPtr->y_InArrID2)>INBUF15)|
                        ((C6ACCEL_TI_powdp_paramPtr->OutArrID1)>OUTBUF15)){
                          return(IUNIVERSAL_EPARAMFAIL);
                      }
                     else{
                          /* Call underlying kernel */
                          in1 = (float *)inBufs->descs[C6ACCEL_TI_powdp_paramPtr->x_InArrID1].buf;
                          in2 = (float *)inBufs->descs[C6ACCEL_TI_powdp_paramPtr->y_InArrID2].buf;
                          out = (float *)outBufs->descs[C6ACCEL_TI_powdp_paramPtr->OutArrID1].buf;
                          for(j=0 ; j < (C6ACCEL_TI_powdp_paramPtr-> n); j++){
                              out[j] = powdp(in1[j],in2[j]);
                            }
                       }
                  }
                 break;
                 case (RECIPDP_FXN_ID):{
                     float  *in1,*out;
                     /* Unmarshal Parameters */
                     recipdp_Params *C6ACCEL_TI_recipdp_paramPtr;
                     C6ACCEL_TI_recipdp_paramPtr = pFnArray;
                     /* Parameter check */
                     if(((C6ACCEL_TI_recipdp_paramPtr->z_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_recipdp_paramPtr->OutArrID1)>OUTBUF15)){
                          return(IUNIVERSAL_EPARAMFAIL);
                      }
                     else{
                          /* Call underlying kernel */
                          in1 = (float *)inBufs->descs[C6ACCEL_TI_recipdp_paramPtr->z_InArrID1].buf;
                          out = (float *)outBufs->descs[C6ACCEL_TI_recipdp_paramPtr->OutArrID1].buf;
                          for(j=0 ; j < (C6ACCEL_TI_recipdp_paramPtr-> n); j++){
                              out[j] = recipdp(in1[j]);
                            }
                       }
                  }
                 break;
                 case (RSQRTDP_FXN_ID):{
                     float  *in1, *out;
                     /* Unmarshal Parameters */
                     rsqrtdp_Params *C6ACCEL_TI_rsqrtdp_paramPtr;
                     C6ACCEL_TI_rsqrtdp_paramPtr = pFnArray;
                     /* Parameter check */
                     if(((C6ACCEL_TI_rsqrtdp_paramPtr->z_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_rsqrtdp_paramPtr->OutArrID1)>OUTBUF15)){
                          return(IUNIVERSAL_EPARAMFAIL);
                      }
                     else{
                          /* Call underlying kernel */
                          in1 = (float *)inBufs->descs[C6ACCEL_TI_rsqrtdp_paramPtr->z_InArrID1].buf;
                          out = (float *)outBufs->descs[C6ACCEL_TI_rsqrtdp_paramPtr->OutArrID1].buf;
                          for(j=0 ; j < (C6ACCEL_TI_rsqrtdp_paramPtr-> n); j++){
                              out[j] = rsqrtdp(in1[j]);
                            }
                       }
                  }
                 break;
                 case (SQRTDP_FXN_ID):{
                     float  *in1,*out;
                     /* Unmarshal Parameters */
                     sqrtdp_Params *C6ACCEL_TI_sqrtdp_paramPtr;
                     C6ACCEL_TI_sqrtdp_paramPtr = pFnArray;
                     /* Parameter check */
                     if(((C6ACCEL_TI_sqrtdp_paramPtr->z_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_sqrtdp_paramPtr->OutArrID1)>OUTBUF15)){
                          return(IUNIVERSAL_EPARAMFAIL);
                      }
                     else{
                          /* Call underlying kernel */
                          in1 = (float *)inBufs->descs[C6ACCEL_TI_sqrtdp_paramPtr->z_InArrID1].buf;
                          out = (float *)outBufs->descs[C6ACCEL_TI_sqrtdp_paramPtr->OutArrID1].buf;
                          for(j=0 ; j < (C6ACCEL_TI_sqrtdp_paramPtr-> n); j++){
                              out[j] = sqrtdp(in1[j]);
                            }
                       }
                  }
                 break;
                 case (EXP2DP_FXN_ID):{
                     float  *in1,*out;
                     /* Unmarshal Parameters */
                     exp2dp_Params *C6ACCEL_TI_exp2dp_paramPtr;
                     C6ACCEL_TI_exp2dp_paramPtr = pFnArray;
                     /* Parameter check */
                     if(((C6ACCEL_TI_exp2dp_paramPtr->z_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_exp2dp_paramPtr->OutArrID1)>OUTBUF15)){
                          return(IUNIVERSAL_EPARAMFAIL);
                      }
                     else{
                          /* Call underlying kernel */
                          in1 = (float *)inBufs->descs[C6ACCEL_TI_exp2dp_paramPtr->z_InArrID1].buf;
                          out = (float *)outBufs->descs[C6ACCEL_TI_exp2dp_paramPtr->OutArrID1].buf;
                          for(j=0 ; j < (C6ACCEL_TI_exp2dp_paramPtr-> n); j++){
                              out[j] = exp2dp(in1[j]);
                            }
                       }
                  }
                 break;
                 case (EXP10DP_FXN_ID):{
                     float  *in1,*out;
                     /* Unmarshal Parameters */
                     exp10dp_Params *C6ACCEL_TI_exp10dp_paramPtr;
                     C6ACCEL_TI_exp10dp_paramPtr = pFnArray;
                     /* Parameter check */
                     if(((C6ACCEL_TI_exp10dp_paramPtr->z_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_exp10dp_paramPtr->OutArrID1)>OUTBUF15)){
                          return(IUNIVERSAL_EPARAMFAIL);
                      }
                     else{
                          /* Call underlying kernel */
                          in1 = (float *)inBufs->descs[C6ACCEL_TI_exp10dp_paramPtr->z_InArrID1].buf;
                          out = (float *)outBufs->descs[C6ACCEL_TI_exp10dp_paramPtr->OutArrID1].buf;
                          for(j=0 ; j < (C6ACCEL_TI_exp10dp_paramPtr-> n); j++){
                              out[j] = exp10dp(in1[j]);
                            }
                       }
                  }
                 break;
                 case (ATAN2DP_FXN_ID):{
                     float  *in1,*in2, *out;
                     /* Unmarshal Parameters */
                     atan2dp_Params *C6ACCEL_TI_atan2dp_paramPtr;
                     C6ACCEL_TI_atan2dp_paramPtr = pFnArray;
                     /* Parameter check */
                     if(((C6ACCEL_TI_atan2dp_paramPtr->y_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_atan2dp_paramPtr->x_InArrID2)>INBUF15)|
                        ((C6ACCEL_TI_atan2dp_paramPtr->OutArrID1)>OUTBUF15)){
                          return(IUNIVERSAL_EPARAMFAIL);
                      }
                     else{
                          /* Call underlying kernel */
                          in1 = (float *)inBufs->descs[C6ACCEL_TI_atan2dp_paramPtr->y_InArrID1].buf;
                          in2 = (float *)inBufs->descs[C6ACCEL_TI_atan2dp_paramPtr->x_InArrID2].buf;
                          out = (float *)outBufs->descs[C6ACCEL_TI_atan2dp_paramPtr->OutArrID1].buf;
                          for(j=0 ; j < (C6ACCEL_TI_atan2dp_paramPtr-> n); j++){
                              out[j] = atan2dp(in1[j],in2[j]);
                            }
                       }
                  }
                 break;
                 case (RECIPF_FXN_ID):{
                     float  *in1,*out;
                     /* Unmarshal Parameters */
                     recipsp_Params *C6ACCEL_TI_recipsp_paramPtr;
                     C6ACCEL_TI_recipsp_paramPtr = pFnArray;
                     /* Parameter check */
                     if(((C6ACCEL_TI_recipsp_paramPtr->z_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_recipsp_paramPtr->OutArrID1)>OUTBUF15)){
                          return(IUNIVERSAL_EPARAMFAIL);
                      }
                     else{
                          /* Call underlying kernel */
                          in1 = (float *)inBufs->descs[C6ACCEL_TI_recipsp_paramPtr->z_InArrID1].buf;
                          out = (float *)outBufs->descs[C6ACCEL_TI_recipsp_paramPtr->OutArrID1].buf;
                          for(j=0 ; j < (C6ACCEL_TI_recipsp_paramPtr-> n); j++){
                              out[j] = recipsp(in1[j]);
                            }
                       }
                  }
                 break;
                 case (RSQRTSP_FXN_ID):{
                     float  *in1, *out;
                     /* Unmarshal Parameters */
                     rsqrtsp_Params *C6ACCEL_TI_rsqrtsp_paramPtr;
                     C6ACCEL_TI_rsqrtsp_paramPtr = pFnArray;
                     /* Parameter check */
                     if(((C6ACCEL_TI_rsqrtsp_paramPtr->z_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_rsqrtsp_paramPtr->OutArrID1)>OUTBUF15)){
                          return(IUNIVERSAL_EPARAMFAIL);
                      }
                     else{
                          /* Call underlying kernel */
                          in1 = (float *)inBufs->descs[C6ACCEL_TI_rsqrtsp_paramPtr->z_InArrID1].buf;
                          out = (float *)outBufs->descs[C6ACCEL_TI_rsqrtsp_paramPtr->OutArrID1].buf;
                          for(j=0 ; j < (C6ACCEL_TI_rsqrtsp_paramPtr-> n); j++){
                              out[j] = rsqrtsp(in1[j]);
                            }
                       }
                  }
                 break;
                 case (SQRT_FXN_ID):{
                     float  *in1,*out;
                     /* Unmarshal Parameters */
                     sqrtsp_Params *C6ACCEL_TI_sqrtsp_paramPtr;
                     C6ACCEL_TI_sqrtsp_paramPtr = pFnArray;
                     /* Parameter check */
                     if(((C6ACCEL_TI_sqrtsp_paramPtr->z_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_sqrtsp_paramPtr->OutArrID1)>OUTBUF15)){
                          return(IUNIVERSAL_EPARAMFAIL);
                      }
                     else{
                          /* Call underlying kernel */
                          in1 = (float *)inBufs->descs[C6ACCEL_TI_sqrtsp_paramPtr->z_InArrID1].buf;
                          out = (float *)outBufs->descs[C6ACCEL_TI_sqrtsp_paramPtr->OutArrID1].buf;
                          for(j=0 ; j < (C6ACCEL_TI_sqrtsp_paramPtr-> n); j++){
                              out[j] = sqrtsp(in1[j]);
                            }
                       }
                  }
                 break;
                 case (EXP2SP_FXN_ID):{
                     float  *in1,*out;
                     /* Unmarshal Parameters */
                     exp2sp_Params *C6ACCEL_TI_exp2sp_paramPtr;
                     C6ACCEL_TI_exp2sp_paramPtr = pFnArray;
                     /* Parameter check */
                     if(((C6ACCEL_TI_exp2sp_paramPtr->z_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_exp2sp_paramPtr->OutArrID1)>OUTBUF15)){
                          return(IUNIVERSAL_EPARAMFAIL);
                      }
                     else{
                          /* Call underlying kernel */
                          in1 = (float *)inBufs->descs[C6ACCEL_TI_exp2sp_paramPtr->z_InArrID1].buf;
                          out = (float *)outBufs->descs[C6ACCEL_TI_exp2sp_paramPtr->OutArrID1].buf;
                          for(j=0 ; j < (C6ACCEL_TI_exp2sp_paramPtr-> n); j++){
                              out[j] = exp2sp(in1[j]);
                            }
                       }
                  }
                 break;
                 case (EXP10SP_FXN_ID):{
                     float  *in1,*out;
                     /* Unmarshal Parameters */
                     exp10sp_Params *C6ACCEL_TI_exp10sp_paramPtr;
                     C6ACCEL_TI_exp10sp_paramPtr = pFnArray;
                     /* Parameter check */
                     if(((C6ACCEL_TI_exp10sp_paramPtr->z_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_exp10sp_paramPtr->OutArrID1)>OUTBUF15)){
                          return(IUNIVERSAL_EPARAMFAIL);
                      }
                     else{
                          /* Call underlying kernel */
                          in1 = (float *)inBufs->descs[C6ACCEL_TI_exp10sp_paramPtr->z_InArrID1].buf;
                          out = (float *)outBufs->descs[C6ACCEL_TI_exp10sp_paramPtr->OutArrID1].buf;
                          for(j=0 ; j < (C6ACCEL_TI_exp10sp_paramPtr-> n); j++){
                              out[j] = exp10sp(in1[j]);
                            }
                       }
                  }
                 break;
                 case (ATAN2SP_FXN_ID):{
                     float  *in1,*in2, *out;
                     /* Unmarshal Parameters */
                     atan2sp_Params *C6ACCEL_TI_atan2sp_paramPtr;
                     C6ACCEL_TI_atan2sp_paramPtr = pFnArray;
                     /* Parameter check */
                     if(((C6ACCEL_TI_atan2sp_paramPtr->y_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_atan2sp_paramPtr->x_InArrID2)>INBUF15)|
                        ((C6ACCEL_TI_atan2sp_paramPtr->OutArrID1)>OUTBUF15)){
                          return(IUNIVERSAL_EPARAMFAIL);
                      }
                     else{
                          /* Call underlying kernel */
                          in1 = (float *)inBufs->descs[C6ACCEL_TI_atan2sp_paramPtr->y_InArrID1].buf;
                          in2 = (float *)inBufs->descs[C6ACCEL_TI_atan2sp_paramPtr->x_InArrID2].buf;
                          out = (float *)outBufs->descs[C6ACCEL_TI_atan2sp_paramPtr->OutArrID1].buf;
                          for(j=0 ; j < (C6ACCEL_TI_atan2sp_paramPtr-> n); j++){
                              out[j] = atan2sp(in1[j],in2[j]);
                            }
                       }
                  }
                 break;
                 #endif
                 default:
                    /*Error caused due to passing of an invalid ID*/
                    return(IUNIVERSAL_EFXNIDFAIL);
                  }
              break;
              /* Reserved for future purposes */
              case (ANALYTICSLIB_FXN_ID):
                   return(IUNIVERSAL_EFXNIDFAIL);
              case (MEDICALLIB_FXN_ID):
                   return(IUNIVERSAL_EFXNIDFAIL);
              case(PWRCNTRLLIB_FXN_ID):
                   return(IUNIVERSAL_EFXNIDFAIL);
              case(AUDSPECHLIB_FXN_ID):
                   return(IUNIVERSAL_EFXNIDFAIL);
#ifdef USE_VLIB
	      case(VLIB_FXN_ID):
     		/* VLIB functions */
                switch ( fxnID & fxnidMsk){
                  case(INTEGRALIMAGE_8_FXN_ID):{
                    /* Unmarshal Parameters */
		    VLIB_integralImage8_Params *C6ACCEL_TI_VLIB_integralImage8_paramPtr;
                    C6ACCEL_TI_VLIB_integralImage8_paramPtr = pFnArray;
                    /* Parameter check */
                    if (((C6ACCEL_TI_VLIB_integralImage8_paramPtr->pIn_InArrID1)>INBUF15)|
                        ((C6ACCEL_TI_VLIB_integralImage8_paramPtr->pLastLine_InArrID2)>INBUF15)|
                        ((C6ACCEL_TI_VLIB_integralImage8_paramPtr->pOut_OutArrID1)>OUTBUF15)){
                          return(IUNIVERSAL_EPARAMFAIL);
                     }
                     else 
                        /* Call underlying kernel */
                        VLIB_integralImage8( (unsigned char*)inBufs->descs[C6ACCEL_TI_VLIB_integralImage8_paramPtr->pIn_InArrID1].buf,
                                       	   C6ACCEL_TI_VLIB_integralImage8_paramPtr->inCols,
					   C6ACCEL_TI_VLIB_integralImage8_paramPtr->inRows,
                                           (unsigned int *)inBufs->descs[C6ACCEL_TI_VLIB_integralImage8_paramPtr->pLastLine_InArrID2].buf,
                                           (unsigned int *)outBufs->descs[C6ACCEL_TI_VLIB_integralImage8_paramPtr->pOut_OutArrID1].buf);
                   }
		  break;
		  default:
	  	    /*Error caused due to passing of an invalid ID*/
		    return(IUNIVERSAL_EFXNIDFAIL);
                }
	        break;
#endif
                default:
                   return(IUNIVERSAL_EFXNIDFAIL);
            }
          break;
          case (OTHER):
              switch (( fxnID & fxntypMsk)>>FXNTYP_TAG_SHIFT){
              /* Non-TI vendors can add their kernels in this section */
                   case (DSPLIB_FXN_ID):
   		     switch(fxnID & fxnidMsk) {
		       case (DFT_F_FXN_ID): {
			 /* Unmarshal Parameters */
			 DSP_dft_f_Params *C6ACCEL_DSP_dft_f_paramPtr;
                         C6ACCEL_DSP_dft_f_paramPtr = pFnArray;
			 /* Parameter check */
                         if (((C6ACCEL_DSP_dft_f_paramPtr->src_InArrID1)>INBUF15)|
                             ((C6ACCEL_DSP_dft_f_paramPtr->dst_OutArrID1)>OUTBUF15)|
                             ((C6ACCEL_DSP_dft_f_paramPtr->twiddleBuf_OutArrID2)>OUTBUF15)|
			     ((C6ACCEL_DSP_dft_f_paramPtr->pWorkingBuf_OutArrID3)>OUTBUF15)|
			     ((C6ACCEL_DSP_dft_f_paramPtr->pWorkingBuf1_OutArrID4)>OUTBUF15)|
                             (((C6ACCEL_DSP_dft_f_paramPtr->optimCols)<16) && ((C6ACCEL_DSP_dft_f_paramPtr->optimCols) != 1))|
                             ((C6ACCEL_DSP_dft_f_paramPtr->optimCols)>65536)|
			     (((C6ACCEL_DSP_dft_f_paramPtr->optimRows)<16) && ((C6ACCEL_DSP_dft_f_paramPtr->optimRows) != 1))|
			     ((C6ACCEL_DSP_dft_f_paramPtr->optimCols == 1) && (C6ACCEL_DSP_dft_f_paramPtr->optimRows == 1))|
                             ((C6ACCEL_DSP_dft_f_paramPtr->optimRows)>65536)|
			     (IsPowOfTwo(C6ACCEL_DSP_dft_f_paramPtr->optimCols) != TRUE)|
                             (IsPowOfTwo(C6ACCEL_DSP_dft_f_paramPtr->optimRows) != TRUE)){
                               return(IUNIVERSAL_EPARAMFAIL);
                          }
                          else 
                             /* Call underlying kernel */
			    C6ACCEL_DSP_dft_f((unsigned char *)inBufs->descs[C6ACCEL_DSP_dft_f_paramPtr->src_InArrID1].buf,
					      (unsigned char *)outBufs->descs[C6ACCEL_DSP_dft_f_paramPtr->dst_OutArrID1].buf,
					      C6ACCEL_DSP_dft_f_paramPtr->cols,
					      C6ACCEL_DSP_dft_f_paramPtr->rows,
					      C6ACCEL_DSP_dft_f_paramPtr->optimCols,
					      C6ACCEL_DSP_dft_f_paramPtr->optimRows,
					      C6ACCEL_DSP_dft_f_paramPtr->srcMatFlag,
					      C6ACCEL_DSP_dft_f_paramPtr->dstMatFlag,
					      C6ACCEL_DSP_dft_f_paramPtr->dxtType,
					      C6ACCEL_DSP_dft_f_paramPtr->nonZeroRows,
					      (unsigned char *)outBufs->descs[C6ACCEL_DSP_dft_f_paramPtr->twiddleBuf_OutArrID2].buf,
					      (unsigned char *)outBufs->descs[C6ACCEL_DSP_dft_f_paramPtr->pWorkingBuf_OutArrID3].buf,
					      (unsigned char *)outBufs->descs[C6ACCEL_DSP_dft_f_paramPtr->pWorkingBuf1_OutArrID4].buf);
					      
			 }
			 break;
		       default:
			 /* Error caused due to passing of an invalid ID */
			 return(IUNIVERSAL_EFXNIDFAIL);
                       }
		   break;
                   case (IMGLIB_FXN_ID):
		     switch(fxnID & fxnidMsk) {
		       case (RGB_TO_Y_FXN_ID): {
			 /* Unmarshal Parameters */   
                         IMG_RGB_To_Y_Params *C6ACCEL_TI_IMG_RGB_To_Y_paramPtr;
                         C6ACCEL_TI_IMG_RGB_To_Y_paramPtr = pFnArray;
                         /* Parameter check */
                         if(((C6ACCEL_TI_IMG_RGB_To_Y_paramPtr->src_InArrID1)>INBUF15)|
                            ((C6ACCEL_TI_IMG_RGB_To_Y_paramPtr->dst_OutArrID1)>INBUF15)){
                            return(IUNIVERSAL_EPARAMFAIL);
                          }
                         else
                         /* Call underlying kernel */
                            IMG_RGB_To_Y((unsigned char *)inBufs->descs[C6ACCEL_TI_IMG_RGB_To_Y_paramPtr->src_InArrID1].buf,
                                         (unsigned char *)outBufs->descs[C6ACCEL_TI_IMG_RGB_To_Y_paramPtr->dst_OutArrID1].buf,
                                         C6ACCEL_TI_IMG_RGB_To_Y_paramPtr->count);
                          
			}
			break;
		       default:
			 /* Error caused due to passing of an invalid ID */
			 return(IUNIVERSAL_EFXNIDFAIL);
		      }
                   break;     
                   case(MATHLIB_FXN_ID):
                        return(IUNIVERSAL_EFXNIDFAIL);
                   case (ANALYTICSLIB_FXN_ID):
                        return(IUNIVERSAL_EFXNIDFAIL);
                   case (MEDICALLIB_FXN_ID):
                        return(IUNIVERSAL_EFXNIDFAIL);
                   case(PWRCNTRLLIB_FXN_ID):
                        return(IUNIVERSAL_EFXNIDFAIL);
                   case(AUDSPECHLIB_FXN_ID):
                        return(IUNIVERSAL_EFXNIDFAIL);
                   default:
                      return(IUNIVERSAL_EFXNIDFAIL);
                }
	 break;
         default:
                return(IUNIVERSAL_EFXNIDFAIL);
       }
    }
 }

    /* report how we accessed the input buffer */
    for (i=0;i<inBufs->numBufs; i++)
     {
        inBufs->descs[i].accessMask = 0;
        XDM_SETACCESSMODE_READ(inBufs->descs[i].accessMask);
      }
    /* and output buffer */
    for (i=0;i< outBufs->numBufs; i++)
     {
        outBufs->descs[i].accessMask = 0;
        XDM_SETACCESSMODE_WRITE(outBufs->descs[i].accessMask);
      }

    return (IUNIVERSAL_EOK);
}


/******************************************************************************
 *  ======== C6Accel_TI_control ========
 *****************************************************************************/
/* ARGSUSED - this line tells the TI compiler not to warn about unused args. */
XDAS_Int32 C6ACCEL_TI_control(IUNIVERSAL_Handle handle,
    IUNIVERSAL_Cmd id, IUNIVERSAL_DynamicParams *dynParams,
    IUNIVERSAL_Status *status)
{
    XDAS_Int32 retVal;

    /* validate arguments - this codec only supports "base" xDM. */
    if ((dynParams->size != sizeof(*dynParams)) ||
        (status->size != sizeof(*status))) {
        return (IUNIVERSAL_EUNSUPPORTED);
    }

    /* initialize for the general case where we don't access the data buffer */
    XDM_CLEARACCESSMODE_READ(status->data.descs[0].accessMask);
    XDM_CLEARACCESSMODE_WRITE(status->data.descs[0].accessMask);

    switch (id) {
        case XDM_GETVERSION:
            if ((status->data.descs[0].buf != NULL) &&
                (status->data.descs[0].bufSize >=
                    strlen(C6ACCEL_VERSIONSTRING))) {

                strncpy((char *)status->data.descs[0].buf,
                        C6ACCEL_VERSIONSTRING,
                        strlen(C6ACCEL_VERSIONSTRING));

                /* null terminate the string */
                status->data.descs[0].
                buf[strlen(C6ACCEL_VERSIONSTRING)] = '\0';

                /* strncpy wrote to the data buffer */
                XDM_SETACCESSMODE_WRITE(status->data.descs[0].accessMask);
                retVal = IUNIVERSAL_EOK;
            }
            else {
                retVal = IUNIVERSAL_EFAIL;
            }

        break;

        default:
            /* unsupported cmd */
            retVal = IUNIVERSAL_EFAIL;
        break;
    }

    return (retVal);
}

/****************************************************************************
**                      Algorithm specific Function defintions
*****************************************************************************/
/* Function to check the condition if a number is power of 2         *
 *   This function is useful in checking the N parameter passed to   *
 *   FFT and Inverse FFT functions are power of 2                    */

static U16_IQ IsPowOfTwo( Uword40 x)
{
   return (x != 0) && ((x & (x - 1)) == 0);
}

/****************************************************************************
**                        END OF FILE
*****************************************************************************/
/*
 *  @(#) ti.C6accel; 1, 0, 0,10; 5-4-2010 17:59:00;
 */
