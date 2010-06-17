/*
 *  ======== opencv_beagle_findDFT.c ========
 *  Implementation of the DFT operation. 
 */
#ifdef __TI_COMPILER_VERSION__
#pragma     CODE_SECTION(OPENCV_BEAGLE_findDFT,         ".text:finddft")
#endif 

#include <string.h>
#include <xdc/std.h>
#include <ti/xdais/xdas.h>
#include <ti/xdais/dm/iuniversal.h>
#include <dsplib64plus.h>
#include <src/DSP_fft16x16/DSP_fft16x16.h>
#include "opencv_beagle_gen_twiddle_fft16x16.h"

#include    "iopencv.h"
#include    "opencv_beagle.h"
#include    "opencv_beagle_priv.h"


/*
 *======== OPENCV_BEAGLE_gen ========
 *
 * in = input array
 * twiddle = dwiddle factors array
 * out = output array
 * N = Size of DFT
 *
 * Computes DFT using twiddle factors stored in vector twiddle. The 
 * data input is stored in vector in. The output result is stored in
 * vector out. The size of DFT is N.
 *
 * 
 *
 */

Void OPENCV_BEAGLE_findDFT(IUNIVERSAL_Handle handle, XDAS_Int32 N, XDAS_Int8 *in, XDAS_Int8 *twiddle, XDAS_Int8 *out)
{
//    float *input_fl = (XDAS_Int32 *)in;
    XDAS_Int16 *input = (XDAS_Int16 *)in;
    XDAS_Int16 *output = (XDAS_Int16 *)out;
    XDAS_Int16 *twid = (XDAS_Int16 *)twiddle;
   
//    need to work on this. 
//    DSP_fltoq15(input_fl, <XDAS_Int16 temp_buffer>, (XDAS_Int16) N)
    
    DSP_fft16x16( twid, N, input, output);
}

