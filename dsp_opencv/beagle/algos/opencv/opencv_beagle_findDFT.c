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
#include "opencv_beagle_gen_twiddle_ifft16x16.h"

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


Void OPENCV_BEAGLE_findDFT(IUNIVERSAL_Handle handle, XDAS_Int32 operation, XDAS_Int8 *in, XDAS_Int8 *out)
{
    OPENCV_BEAGLE_Obj *dft = (Void *)handle;

    float *inputFloat = (float *)in;
    float *outputFloat = (float *)out;
//    XDAS_Int16 *input = (XDAS_Int16 *)in;
    XDAS_Int16 *output = (XDAS_Int16 *)out;

    XDAS_Int16 *twiddle = (XDAS_Int16 *)dft->workBuf3; 
    XDAS_Int16 *tempInBuf = (XDAS_Int16 *)dft->workBuf;
    XDAS_Int16 *tempOutBuf = (XDAS_Int16 *)dft->workBuf2;    
    XDAS_Int32 *tempInBufWordPtr = (XDAS_Int32 *)dft->workBuf;
    XDAS_Int32 *tempOutBufWordPtr = (XDAS_Int32 *)dft->workBuf2;
    
    
    XDAS_Int32 size;
  
    XDAS_Int32 rows = dft->s_rows;
    XDAS_Int32 cols = dft->s_cols;
//    XDAS_Int32 type = dft->s_type & 07;


    XDAS_Int32 i, j;

    /* First convert the whole block of floating point nos to Q15 */  
    size = rows * cols * dft->s_channels;  
    DSP_fltoq15(inputFloat, tempInBuf, size );    

    /* Perform DFT */
    if (rows == 1 || cols == 1) {

	
        if ( operation == OPENCV_OPERATION_DFT ) { /* 1-D forward DFT */

	   size = (rows > cols ) ? rows : cols;
    	   OPENCV_BEAGLE_gen_twiddle_fft16x16 ( twiddle, size);    
    	   DSP_fft16x16( twiddle, size, tempInBuf, (XDAS_Int16 *)tempOutBuf);
  	   
	}
	else if ( operation == OPENCV_OPERATION_IDFT ) { /* 1-D Inverse DFT */

	   size = (rows > cols ) ? rows : cols;
    	   OPENCV_BEAGLE_gen_twiddle_ifft16x16 ( twiddle, size);    
    	   DSP_ifft16x16( twiddle, size, tempInBuf, (XDAS_Int16 *)tempOutBuf);
   	}
             
        goto exit;
    }


    /* if 2-D row-wise, perform row-wise DFT */    
    if ( rows > 1 && cols > 1) {
       if (operation == OPENCV_OPERATION_DFTROWS) { /* 2-D row-wise forward DFT */
	
	  OPENCV_BEAGLE_gen_twiddle_fft16x16(twiddle, cols);
	  for (i = 0; i < rows; i++) {
	      /* Find DFT for rows */
	      DSP_fft16x16( twiddle, cols, tempInBuf, tempOutBuf);
	      tempInBuf += cols * dft->s_channels;
	      tempOutBuf += cols * dft->s_channels;
	  }
	  tempInBuf  = (XDAS_Int16 *)dft->workBuf;
	  tempOutBuf = (XDAS_Int16 *)dft->workBuf2;    
	  goto exit;
       }
       else if (operation == OPENCV_OPERATION_IDFTROWS) { /* 2-D row-wise Inverse DFT  */
	  
  	  OPENCV_BEAGLE_gen_twiddle_ifft16x16(twiddle, cols);
	  for (i = 0; i < rows; i++) {
   	      /* Find IDFT for rows */
	      DSP_ifft16x16( twiddle, cols, tempInBuf, tempOutBuf);
	      tempInBuf += cols * dft->s_channels;
	      tempOutBuf += cols * dft->s_channels;
	  }
	  tempInBuf  = (XDAS_Int16 *)dft->workBuf;
	  tempOutBuf = (XDAS_Int16 *)dft->workBuf2;  
	  goto exit;
       }
       else if ( operation == OPENCV_OPERATION_DFT) { /* 2-D Forward DFT */
	 
	  /* Find DFT for rows */
	  OPENCV_BEAGLE_gen_twiddle_fft16x16(twiddle, cols);
	  for (i = 0; i < rows; i++) {
	      DSP_fft16x16( twiddle, cols, tempInBuf, tempOutBuf);
	      tempInBuf += cols * dft->s_channels;
	      tempOutBuf += cols * dft->s_channels;
	  }
          tempInBuf = (XDAS_Int16 *)dft->workBuf;
	  tempOutBuf = (XDAS_Int16 *)dft->workBuf2;

	  /* Do transpose */
          for (i=0; i<cols; i++) {
      	      for (j=0; j<rows; j++) {
          	  *(tempInBufWordPtr+i*rows+j)=*(tempOutBufWordPtr+i+cols*j);
	      }
	  }
	  tempInBufWordPtr = (XDAS_Int32 *)dft->workBuf;
    	  tempOutBufWordPtr = (XDAS_Int32 *)dft->workBuf2;

	  /* Find DFT for cols */
          OPENCV_BEAGLE_gen_twiddle_fft16x16(twiddle, rows);
	  for (i = 0; i < cols; i++) {
	      DSP_fft16x16( twiddle, rows, tempInBuf, tempOutBuf);
	      tempInBuf += rows * dft->s_channels;
	      tempOutBuf += rows * dft->s_channels;
	  }
          tempInBuf = (XDAS_Int16 *)dft->workBuf;
	  tempOutBuf = (XDAS_Int16 *)dft->workBuf2;

	  /* After transpose output data will be in tempInBuf */
	  for (i=0; i<rows; i++) {
      	      for (j=0; j<cols; j++) {
          	  *(tempInBufWordPtr+i*rows+j)=*(tempOutBufWordPtr+i+cols*j);
	      }
	  }
	  
          size = rows * cols * dft->s_channels;  
    	  DSP_q15tofl(tempInBuf, outputFloat, size );    
          return;
       }
       else if ( operation == OPENCV_OPERATION_IDFT) { /* 2-D Inverse DFT */
	 
	  /* find twiddle factors */
	  OPENCV_BEAGLE_gen_twiddle_ifft16x16(twiddle, cols);
	  for (i = 0; i < rows; i++) {
	      /* Find DFT for rows */
	      DSP_ifft16x16( twiddle, cols, tempInBuf, tempOutBuf);
	      tempInBuf += cols * dft->s_channels;
	      tempOutBuf += cols * dft->s_channels;
	  }
          tempInBuf = (XDAS_Int16 *)dft->workBuf;
	  tempOutBuf = (XDAS_Int16 *)dft->workBuf2;

          /* Do the transpose */
          for (i=0; i<cols; i++) {
      	      for (j=0; j<rows; j++) {
          	  *(tempInBufWordPtr+i*rows+j)=*(tempOutBufWordPtr+i+cols*j);
	      }
	  }
	  tempInBufWordPtr = (XDAS_Int32 *)dft->workBuf;
    	  tempOutBufWordPtr = (XDAS_Int32 *)dft->workBuf2;

	  /* find twiddle factors */
	  OPENCV_BEAGLE_gen_twiddle_ifft16x16(twiddle, rows);
	  for (i = 0; i < cols; i++) {
	      /* Find DFT for cols */
	      DSP_ifft16x16( twiddle, rows, tempInBuf, tempOutBuf);
	      tempInBuf += rows * dft->s_channels;
	      tempOutBuf += rows * dft->s_channels;
	  }
          tempInBuf = (XDAS_Int16 *)dft->workBuf;
	  tempOutBuf = (XDAS_Int16 *)dft->workBuf2;
	  /* After transpose output data will be in tempInBuf */
	  /* Do the transpose */
          for (i=0; i<rows; i++) {
      	      for (j=0; j<cols; j++) {
          	  *(tempInBufWordPtr+i*rows+j)=*(tempOutBufWordPtr+i+cols*j);
	      }
	  }

          size = rows * cols * dft->s_channels;  
    	  DSP_q15tofl(tempInBuf, outputFloat, size );    
          return;
       }   
    }
exit:
    size = dft->s_rows * dft->s_cols * dft->s_channels;  
    DSP_q15tofl(tempOutBuf, outputFloat, size );    
}

