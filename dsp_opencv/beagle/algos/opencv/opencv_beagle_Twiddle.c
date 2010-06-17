/*
 *  ======== opencv_beagle_Twiddle.c ========
 *  Implementation of the Twiddle generation. 
 */
#ifdef __TI_COMPILER_VERSION__
#pragma     CODE_SECTION(OPENCV_BEAGLE_generateTwiddle,         ".text:generatetwiddle")
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
 * out = pointer to generated twiddle factor array
 * N = Size of DFT
 *
 * Generates twiddle factor and stores in out. The 
 * The size of DFT is N.
 *
 * 
 *
 */

Void OPENCV_BEAGLE_generateTwiddle( IUNIVERSAL_Handle handle, XDAS_Int32 N, XDAS_Int8 *out)
{
    XDAS_Int16 *output = (XDAS_Int16 *)out;
        
    OPENCV_BEAGLE_gen_twiddle_fft16x16( output, N);
}
