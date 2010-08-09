#include<xdc/std.h>
#include<iC6Accel_ti.h>
#include<string.h>

#define SHORT_SIZE 2
#define CHANNELS 2

void C6ACCEL_DSP_dft_f(unsigned char *src, unsigned char *dst, unsigned int cols_s,
			unsigned int rows_s, unsigned int optimCols, unsigned int optimRows,
			int srcMatFlag, int dstMatFlag, int dxtType, int nonZeroRows, 
			unsigned char *twiddle8, unsigned char *workBuf1, unsigned char *workBuf2)
{
    int operation = dxtType;
    float *inputFloat = (float *)src;
    float *outputFloat = (float *)dst;
//    short *input = (short *)src;
//    short *output = (short *)dst;

    short *twiddle = (short *)twiddle8;
    short *tempInBuf = (short *)workBuf1;
    short *tempOutBuf = (short *)workBuf2;    
    int *tempInBufWordPtr = (int *)workBuf1;
    int *tempOutBufWordPtr = (int *)workBuf2;
   
   
    int size;
 
    int rows = rows_s;
    int cols = cols_s;
//    int type = dft->s_type & 07;


    int i, j;

    /* First convert the whole block of floating point nos to Q15 */  
    size = rows * cols * CHANNELS;  
    DSP_fltoq15(inputFloat, tempInBuf, size );    

    /* Perform DFT */
    if (rows == 1 || cols == 1) {

       
        if ( operation == DXT_FORWARD ) { /* 1-D forward DFT */

           size = (rows > cols ) ? rows : cols;
           C6ACCEL_TI_gen_twiddle_fft16x16 ( twiddle, size);    
           DSP_fft16x16( twiddle, size, tempInBuf, (short *)tempOutBuf);
           
        }
        else if ( operation == DXT_INVERSE ) { /* 1-D Inverse DFT */

           size = (rows > cols ) ? rows : cols;
           C6ACCEL_TI_gen_twiddle_ifft16x16 ( twiddle, size);    
           DSP_ifft16x16( twiddle, size, tempInBuf, (short *)tempOutBuf);
        }
             
        goto exit;
    }


    /* if 2-D row-wise, perform row-wise DFT */    
    if ( rows > 1 && cols > 1) {
       if (operation == DXT_ROWS) { /* 2-D row-wise forward DFT */
       
          C6ACCEL_TI_gen_twiddle_fft16x16(twiddle, cols);
          for (i = 0; i < rows; i++) {
              /* Find DFT for rows */
              DSP_fft16x16( twiddle, cols, tempInBuf, tempOutBuf);
              tempInBuf += cols * CHANNELS;
              tempOutBuf += cols * CHANNELS;
          }
          tempInBuf  = (short *)workBuf1;
          tempOutBuf = (short *)workBuf2;    
          goto exit;
       }
       else if (operation == DXT_INVERSE_ROWS) { /* 2-D row-wise Inverse DFT  */
         
          C6ACCEL_TI_gen_twiddle_ifft16x16(twiddle, cols);
          for (i = 0; i < rows; i++) {
              /* Find IDFT for rows */
              DSP_ifft16x16( twiddle, cols, tempInBuf, tempOutBuf);
              tempInBuf += cols * CHANNELS;
              tempOutBuf += cols * CHANNELS;
          }
          tempInBuf  = (short *)workBuf1;
          tempOutBuf = (short *)workBuf2;  
          goto exit;
       }
       else if ( operation == DXT_FORWARD) { /* 2-D Forward DFT */
         
          /* Find DFT for rows */
          C6ACCEL_TI_gen_twiddle_fft16x16(twiddle, cols);
          for (i = 0; i < rows; i++) {
              DSP_fft16x16( twiddle, cols, tempInBuf, tempOutBuf);
              tempInBuf += cols * CHANNELS;
              tempOutBuf += cols * CHANNELS;
          }
          tempInBuf = (short *)workBuf1;
          tempOutBuf = (short *)workBuf2;

          /* Do transpose */
          for (i=0; i<cols; i++) {
              for (j=0; j<rows; j++) {
                  *(tempInBufWordPtr+i*rows+j)=*(tempOutBufWordPtr+i+cols*j);
              }
          }
          tempInBufWordPtr = (int *)workBuf1;
          tempOutBufWordPtr = (int *)workBuf2;

          /* Find DFT for cols */
          C6ACCEL_TI_gen_twiddle_fft16x16(twiddle, rows);
          for (i = 0; i < cols; i++) {
              DSP_fft16x16( twiddle, rows, tempInBuf, tempOutBuf);
              tempInBuf += rows * CHANNELS;
              tempOutBuf += rows * CHANNELS;
          }
          tempInBuf = (short *)workBuf1;
          tempOutBuf = (short *)workBuf2;

          /* After transpose output data will be in tempInBuf */
          for (i=0; i<rows; i++) {
              for (j=0; j<cols; j++) {
                  *(tempInBufWordPtr+i*rows+j)=*(tempOutBufWordPtr+i+cols*j);
              }
          }
         
          size = rows * cols * CHANNELS;  
          DSP_q15tofl(tempInBuf, outputFloat, size );    
          return;
       }
       else if ( operation == DXT_INVERSE) { /* 2-D Inverse DFT */
         
          /* find twiddle factors */
          C6ACCEL_TI_gen_twiddle_ifft16x16(twiddle, cols);
          for (i = 0; i < rows; i++) {
              /* Find DFT for rows */
              DSP_ifft16x16( twiddle, cols, tempInBuf, tempOutBuf);
              tempInBuf += cols * CHANNELS;
              tempOutBuf += cols * CHANNELS;
          }
          tempInBuf = (short *)workBuf1;
          tempOutBuf = (short *)workBuf2;

          /* Do the transpose */
          for (i=0; i<cols; i++) {
              for (j=0; j<rows; j++) {
                  *(tempInBufWordPtr+i*rows+j)=*(tempOutBufWordPtr+i+cols*j);
              }
          }
          tempInBufWordPtr = (int *)workBuf1;
          tempOutBufWordPtr = (int *)workBuf2;

          /* find twiddle factors */
          C6ACCEL_TI_gen_twiddle_ifft16x16(twiddle, rows);
          for (i = 0; i < cols; i++) {
              /* Find DFT for cols */
              DSP_ifft16x16( twiddle, rows, tempInBuf, tempOutBuf);
              tempInBuf += rows * CHANNELS;
              tempOutBuf += rows * CHANNELS;
          }
          tempInBuf = (short *)workBuf1;
          tempOutBuf = (short *)workBuf2;
          /* After transpose output data will be in tempInBuf */
          /* Do the transpose */
          for (i=0; i<rows; i++) {
              for (j=0; j<cols; j++) {
                  *(tempInBufWordPtr+i*rows+j)=*(tempOutBufWordPtr+i+cols*j);
              }
          }

          size = rows * cols * CHANNELS;  
          DSP_q15tofl(tempInBuf, outputFloat, size );    
          return;
       }  
    }
exit:
    size = rows_s * cols_s * CHANNELS;  
    DSP_q15tofl(tempOutBuf, outputFloat, size );    
}



