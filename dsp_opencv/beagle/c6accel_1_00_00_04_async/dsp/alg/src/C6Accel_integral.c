#include<xdc/std.h>
#include <iC6Accel_ti.h>
#include "dsplib64plus.h"


/*
 *======== C6Accel_integral ========
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
      unsigned char *input_8;
      int *input_32;
      float *input_f;
}InputBuf;


//union output {
typedef union OutputBuf {
      int *output_32;
      short *output_16;
      float *output_f;
}OutputBuf;


Void C6ACCEL_TI_integral( unsigned char *src, int srcMatFlag, unsigned int rows, unsigned int cols, unsigned char *sum, int sumMatFlag, unsigned char *sqsum, unsigned char *tilted, unsigned char *workBuf, unsigned char *workBuf2)
{
     
     int i, j, k;
     
     int channels = MAT_CN(srcMatFlag);
     int size = cols * channels;
     int tempVal;

     int s_type = MAT_TYPE(srcMatFlag);
     int d_type = MAT_TYPE(sumMatFlag);


     OutputBuf output;//, output2;
     OutputBuf intImage/*, sqIntImage*/;
     InputBuf  input;
          
     

     
     if (s_type == 5 ) {
         input.input_f = (float *)src;
         
     } else if (s_type == 4 ) {
        input.input_32 = (int *)src;
        
     } else {
	input.input_8 = (unsigned char *)src;
        
     }

     if (s_type == 5 || s_type == 4) {  /* it implies d_type is also 5 */
         output.output_f = (float *)sum;
//	 output2.output_f = (float *)sqsum;
  	 intImage.output_16 = (short *)workBuf;
//	 sqIntImage.output_16 = (short *)workBuf2;
     } else if ( s_type == 0 && d_type == 4) {
	 output.output_32 = (int *)sum;
//	 output2.output_32 = (int *)sqsum;
         intImage.output_32 = (int *)workBuf;
//	 sqIntImage.output_32 = (int *)workBuf2;
     } else {
         output.output_f = (float *)sum;
//	 output2.output_32 = (int *)sqsum;
         intImage.output_16 = (short *)workBuf;
//	 sqIntImage.output_32 = (int *)workBuf2;
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
                    intImage.output_16[i * size + j * channels + k] = (short)(input.input_32[i * size + j * channels + k] + intImage.output_16[(i-1) * 										size + 	j * channels + k] + intImage.output_16[i * size + (j-1) * channels + k] - 										intImage.output_16[(i-1) * size + (j-1) * channels + k]);
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
                    intImage.output_32[(i * size) + (j * channels) + k] = (int)input.input_8[((i-1) * (size-channels)) + ((j-1) * channels) + k] + 										intImage.output_32[((i-1) * size) + (j * channels) + k] + intImage.output_32[(i * 										size) + ((j-1) * channels) + k] - intImage.output_32[((i-1) * size) + ((j-1) * 										channels) + k];
		    output.output_32[(i * size) + (j * channels) + k] = intImage.output_32[(i * size) + (j * channels) + k];
//       	            output2.output_32[i * size + j * channels + k] = tempVal * tempVal;
                }
            }
        }
     
	  
  	return;
    }   

    /* if input image is 8 bit output image is float*/
    if ( s_type == 0 && d_type == 5) {    
       
           	        
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
                    intImage.output_16[(i * size) + (j * channels) + k] = (short)(input.input_8[((i-1) * (size-channels)) + ((j-1) * channels) + k] + 										intImage.output_16[((i-1) * size) + (j * channels) + k] + intImage.output_16[i * 										size + ((j-1) * channels) + k] - intImage.output_16[((i-1) * size) + ((j-1) * 										channels) + k]);
//     	            sqIntImage.output_16[i * size + j * channels + k] = intImage.output_16[i * size + j * channels + k] * intImage.output_16[i * size + j 											* channels + k];
                }
            }
        }
     
	  
  	DSP_q15tofl( intImage.output_16, output.output_f, (rows+1) * (cols+1) * channels);
//	DSP_q15tofl( sqIntImage.output_16, output2.output_f, rows * cols * channels);

  	return;
    }   

}
