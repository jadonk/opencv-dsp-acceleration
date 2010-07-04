#ifdef __TI_COMPILER_VERSION__
#pragma     CODE_SECTION(OPENCV_BEAGLE_integral,         ".text:integral")
#endif 

#include <string.h>
#include <xdc/std.h>
#include <ti/xdais/xdas.h>
#include <ti/xdais/dm/iuniversal.h>

#include <dsplib64plus.h>

#include    "iopencv.h"
#include    "opencv_beagle.h"
#include    "opencv_beagle_priv.h"


/*
 *======== OPENCV_BEAGLE_integral ========
 *
 * in = input array of image data
 * out = output array of integral image
 * row = No of rows
 * cols = No of columns
 *
 * Calculates integral of image data stored in vector in. The 
 *  The output result is stored in vector out.
 * 
 *
 */

//union input {
typedef union InputBuf {
      XDAS_Int8 *input_8;
      XDAS_Int32 *input_32;
      float *input_f;
}InputBuf;


//union output {
typedef union OutputBuf {
      XDAS_Int32 *output_32;
      XDAS_Int16 *output_16;
      float *output_f;
}OutputBuf;


Void OPENCV_BEAGLE_integral(IUNIVERSAL_Handle handle, XDAS_Int8 *in, XDAS_Int8 *out)
{
     OPENCV_BEAGLE_Obj *integral = (Void *)handle;
     XDAS_Int32 i, j, k;
     
     XDAS_Int32 channels = integral->s_channels;
     XDAS_Int32 cols = integral->s_cols;
     XDAS_Int32 rows = integral->s_rows;
     XDAS_Int32 size = cols * channels;
     XDAS_Int32 tempVal;

     XDAS_Int32 s_type = integral->s_type & 07;
     XDAS_Int32 d_type = integral->d_type & 07;


     OutputBuf output;//, output2;
     OutputBuf intImage, sqIntImage;
     InputBuf  input;
          
     

     
     if (s_type == 5 ) {
         input.input_f = (float *)in;
         
     } else if (s_type == 4 ) {
        input.input_32 = (XDAS_Int32 *)in;
        
     } else {
	input.input_8 = (XDAS_Int8 *)in;
        
     }

     if (s_type == 5 || s_type == 4) {  /* it implies d_type is also 5 */
         output.output_f = (float *)out;
//	 output2.output_f = (float *)out2;
  	 intImage.output_16 = (XDAS_Int16 *)integral->workBuf;
//	 sqIntImage.output_16 = (XDAS_Int16 *)integral->workBuf2;
     } else if ( s_type == 0 && d_type == 4) {
	 output.output_32 = (XDAS_Int32 *)out;
//	 output2.output_32 = (XDAS_Int32 *)out2;
         intImage.output_32 = (XDAS_Int32 *)integral->workBuf;
//	 sqIntImage.output_32 = (XDAS_Int32 *)integral->workBuf2;
     } else {
         output.output_f = (float *)out;
//	 output2.output_32 = (XDAS_Int32 *)out2;
         intImage.output_16 = (XDAS_Int16 *)integral->workBuf;
//	 sqIntImage.output_32 = (XDAS_Int32 *)integral->workBuf2;
    	 }

     
     /* if input image is float */
     if ( s_type == 5 ) {    /* it implies d_type is also 5 */
       
        DSP_fltoq15( input.input_f, intImage.output_16, rows * cols * channels);
   	        
        tempVal = (cols + 1) * channels;
        for (i=0; i<tempVal; i++) {
            intImage.output_16[i]  = 0;
//	    sqIntImage.output_16[i] = 0;
        }

        tempVal = (rows + 1) * channels;
        for (i=1; i<rows+1; i+=tempVal) {
            intImage.output_16[i]  = 0;
//            sqIntImage.output_16[i] = 0;
        }

        for (i = 1; i< rows+1; i++) {
            for (j=1;j< cols+1; j++) {
                for (k= 0; k< channels; k++) {
                    intImage.output_16[i * size + j * channels + k] = intImage.output_16[i * size + j * channels + k] + intImage.output_16[(i-1) * size + 										j * channels + k] + intImage.output_16[i * size + (j-1) * channels + k] - 										intImage.output_16[(i-1) * size + (j-1) * channels + k];
//     	            sqIntImage.output_16[i * size + j * channels + k] = intImage.output_16[i * size + j * channels + k] * intImage.output_16[i * size + j 											* channels + k];
                }
            }
        }
     
	  
  	DSP_q15tofl( intImage.output_16, output.output_f, (rows+1) * (cols+1) * channels);
//	DSP_q15tofl( sqIntImage.output_16, output2.output_f, rows * cols * channels);

  	return;
    }   


    /* if input image is int output image is float*/
    if ( s_type == 4 ) {    /* it implies d_type is float */
       
           	        
        tempVal = (cols + 1) * channels;
        for (i=0; i<tempVal; i++) {
            intImage.output_16[i]  = 0;
//	    sqIntImage.output_16[i] = 0;
        }

        tempVal = (rows + 1) * channels;
        for (i=1; i<rows+1; i+=tempVal) {
            intImage.output_16[i]  = 0;
//            sqIntImage.output_16[i] = 0;
        }

        for (i = 1; i< rows+1; i++) {
            for (j=1;j< cols+1; j++) {
                for (k= 0; k< channels; k++) {
                    intImage.output_16[i * size + j * channels + k] = (XDAS_Int16)(input.input_32[i * size + j * channels + k] + intImage.output_16[(i-1) * 										size + 	j * channels + k] + intImage.output_16[i * size + (j-1) * channels + k] - 										intImage.output_16[(i-1) * size + (j-1) * channels + k]);
//     	            sqIntImage.output_16[i * size + j * channels + k] = intImage.output_16[i * size + j * channels + k] * intImage.output_16[i * size + j 											* channels + k];
                }
            }
        }
     
	  
  	DSP_q15tofl( intImage.output_16, output.output_f, (rows+1) * (cols+1) * channels);
//	DSP_q15tofl( sqIntImage.output_16, output2.output_f, rows * cols * channels);

  	return;
    }   

    /* if input image is 8-bit output image is int32*/
    if ( s_type == 0 && d_type == 4 ) {    /*  d_type is int */
       
           	        
        tempVal = (cols + 1) * channels;
        for (i=0; i<tempVal; i++) {
            intImage.output_32[i]  = 0;
	    output.output_32[i] = 0;
//	    sqIntImage.output_32[i] = 0;
        }

        size = (cols + 1) * channels;
        for (i=1; i<rows+1; i++) {
            intImage.output_32[i*size]  = 0;
	    output.output_32[i*size] = 0;
//            sqIntImage.output_32[i*size] = 0;
        }

        for (i = 1; i< rows+1; i++) {
            for (j=1;j< cols+1; j++) {
//		tempVal = 0;
                for (k= 0; k< channels; k++) {
                    intImage.output_32[(i * size) + (j * channels) + k] = (XDAS_Int32)input.input_8[((i-1) * (size-channels)) + ((j-1) * channels) + k] + 										intImage.output_32[((i-1) * size) + (j * channels) + k] + intImage.output_32[(i * 										size) + ((j-1) * channels) + k] - intImage.output_32[((i-1) * size) + ((j-1) * 										channels) + k];
		    output.output_32[(i * size) + (j * channels) + k] = intImage.output_32[(i * size) + (j * channels) + k];
//       	            output2.output_32[i * size + j * channels + k] = tempVal * tempVal;
                }
            }
        }
     
	  
  	return;
    }   

    /* if input image is 8 bit output image is float*/
    if ( s_type == 0 && d_type == 5) {    /* it implies d_type is float */
       
           	        
        tempVal = (cols + 1) * channels;
        for (i=0; i<tempVal; i++) {
            intImage.output_16[i]  = 0;
//	    sqIntImage.output_16[i] = 0;
        }

        size = (cols + 1) * channels;
        for (i=1; i<rows+1; i++) {
            intImage.output_16[i*size]  = 0;
//            sqIntImage.output_16[i*size] = 0;
        }

        for (i = 1; i< rows+1; i++) {
            for (j=1;j< cols+1; j++) {
                for (k= 0; k< channels; k++) {
                    intImage.output_16[(i * size) + (j * channels) + k] = (XDAS_Int16)(input.input_8[((i-1) * (size-channels)) + ((j-1) * channels) + k] + 										intImage.output_16[((i-1) * size) + (j * channels) + k] + intImage.output_16[i * 										size + ((j-1) * channels) + k] - intImage.output_16[((i-1) * size) + ((j-1) * 										channels) + k]);
//     	            sqIntImage.output_16[i * size + j * channels + k] = intImage.output_16[i * size + j * channels + k] * intImage.output_16[i * size + j 											* channels + k];
                }
            }
        }
     
	  
  	DSP_q15tofl( intImage.output_16, output.output_f, (rows+1) * (cols+1) * channels);
//	DSP_q15tofl( sqIntImage.output_16, output2.output_f, rows * cols * channels);

  	return;
    }   

}
