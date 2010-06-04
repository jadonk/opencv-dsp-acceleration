/*
 *  Copyright 2003 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *  
 */
/* "@(#) ReferenceFrameworks 3.30.00.01 10-24-05 (swat-j02)" */
/*
 *  ======== cvDFT_gsoc_filter.c ========
 *  Implementation of the filter operation. 
 */
#ifdef __TI_COMPILER_VERSION__
#pragma     CODE_SECTION(CVDFT_GSOC_filter,         ".text:filter")
#pragma     CODE_SECTION(CVDFT_GSOC_gen,            ".text:filter")
#endif 

#include <string.h>
#include <xdc/std.h>
#include <ti/xdais/xdas.h>
#include <ti/xdais/dm/iuniversal.h>
#include    "icvDFT.h"
#include    "cvDFT_gsoc.h"
#include    "cvDFT_gsoc_priv.h"


/*
 *======== CVDFT_GSOC_gen ========
 *
 * in = input array
 * coeff = coefficient array
 * out = output array
 * nCoeff = number of coefficients (nCoeff >= 5)
 * nout = number of output samples (nout >= 1)
 *
 * Computes a real CVDFT filter (direct-form) using coefficients stored
 * in vector coeff. The real data input is stored in vector in. The filter 
 * output result is stored in vector out. This routine has no memory
 * hits regardless of where in, coeff, and out arrays are located in
 * memory. The filter is nout output samples and nCoeff coefficients.
 *
 * No memory bank hits under any conditions.
 *
 */
static Void CVDFT_GSOC_gen(XDAS_Int16 *in, XDAS_Int16 *coeff, XDAS_Int16 *out, XDAS_Int16 nCoeff, XDAS_Int16 nout)
{
    XDAS_Int16  i, j;
    XDAS_Int32  sum;  
    
    for (j = 0; j < nout; j++) 
    {
        sum = 0;
        for (i = 0; i < nCoeff; i++)
            sum += (XDAS_Int32)(in[i + j]) * (XDAS_Int32)(coeff[i]);
        out[j] = sum >> 15;
    }
}
        
    
/*
 *  ======== CVDFT_GSOC_filter ========
 *  TI's implementation of the filter operation.
 */    
Void CVDFT_GSOC_filter(IUNIVERSAL_Handle handle, Short in[], Short out[])
{
    CVDFT_GSOC_Obj *fir = (Void *)handle;
    Short *src = fir->workBuf;
    Short *dst = out;
    
    /* copy input buffer into working buffer */
    memcpy((Void *)(fir->workBuf + fir->filterLenM1), (Void *)in, fir->frameLen * sizeof (Short));

    /* call fir_gen to filter data */
    CVDFT_GSOC_gen(src, fir->coeff, dst, fir->filterLenM1 + 1, fir->frameLen);

    /* shift filter history to beginning of working buffer for next frame */
    memcpy((Void *)fir->workBuf, (Void *)(fir->workBuf + fir->frameLen), fir->filterLenM1 * sizeof (Short));
}


