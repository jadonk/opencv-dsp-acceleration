/*==================================================================*/
/*  Copyright (c) 2010, Texas Instruments Incorporated              */
/*  All rights reserved.                                            */
/*                                                                  */
/*                                                                  */
/* ======== c6accelw_i.h ========                                   */
/* This file declares the internal ARM wrapper resources for C6ACCEL*/
/*                                                                  */
/*                                                                  */
/*  Version: 0.0.1                                                  */
/*==================================================================*/

#ifndef WRAPPER_C6ACCEL_I_H
#define WRAPPER_C6ACCEL_I_H

 // These will be static and internal in full implementation
extern Engine_Handle hEngine;
extern UNIVERSAL_Handle hUni;

// Macro to acquire the codec engine - it will pend on a semaphore in full implementation

 #define ACQUIRE_CODEC_ENGINE



 #define RELEASE_CODEC_ENGINE


// In the multicore ARM+DSP devices the wrapper has to manage the shared memory.
 // The ARM user should not have to deal with the fact that the input data needs to be written
 // back to DDR so that the DSP can pick up the valid data. This is done with a cache Writeback and invalidate operation.
 // Similarly the output arrays need to have the cache invalidated so that the ARM will pick up valid data after the DSP
 // has written them to DDR.
 //
 // In the case that the input buffers are filled by DMA and not by the CPU the data will not be in the cache and
 // therefore there is no need to flush the cache. However, to keep it simple for ARM user and minimise the risk of unintended
 // errors the cache management will always be enabled.
 // This has an overhead so a future optimisation will be to control this with a flag.
 //
 // Macro is also responsible for putting same data into the XDM buffers for sending to C6Accel

 #define CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(pBuffer,inputBufferId,size)  \
        inBufDesc.descs[inputBufferId].buf      = (XDAS_Int8 *) pBuffer;               \
        inBufDesc.descs[inputBufferId].bufSize  = size;                                    \
        Memory_cacheWbInv((Ptr)pBuffer, size);

 #define CACHE_WB_INV_INPUT_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(pBuffer,inputOutputBufferId,size)  \
        inOutBufDesc.descs[inputOutputBufferId].buf      = (XDAS_Int8 *) pBuffer;                  \
        inOutBufDesc.descs[inputOutputBufferId].bufSize  = size;                                   \
        Memory_cacheWbInv((Ptr)pBuffer, size);

 #define CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(pBuffer,outputBufferId,size)   \
        outBufDesc.descs[outputBufferId].buf      = (XDAS_Int8 *) pBuffer;             \
        outBufDesc.descs[outputBufferId].bufSize  = size;                                  \
        Memory_cacheInv((Ptr)pBuffer, size);



#endif

