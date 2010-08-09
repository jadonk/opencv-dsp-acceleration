/*==================================================================*/
/*  Copyright (c) 2010, Texas Instruments Incorporated              */
/*  All rights reserved.                                            */
/*                                                                  */
/*                                                                  */
/* ======== appMain.h ========                                      */
/*   This is main app header file                                   */
/*                                                                  */
/*                                                                  */
/*  Version: 0.0.1                                                  */
/*==================================================================*/

#ifndef appMain_h_
#define appMain_h_

#include <xdc/std.h>

#define MAX_CODEC_NAME_SIZE     30
#define MAX_ENGINE_NAME_SIZE    30
#define MAX_FILE_NAME_SIZE      100

/* Arguments for app */
typedef struct Args {
    Int   benchmark;
 } Args;

#if defined (__cplusplus)
extern "C" {
#endif

extern Void appMain(Args * args);

#if defined (__cplusplus)
}
#endif

#endif 
