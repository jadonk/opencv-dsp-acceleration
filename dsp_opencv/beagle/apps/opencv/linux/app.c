/*
 *  ======== app.c ========
 *  Test application for OPENCVTEST algorithm.  
 */

//#include <xdc/std.h>

#include <stdio.h>
#include <stdlib.h>


/* OpenCV Headerfiles */
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>

#include <beagle/library/opencv/beagle_opencv_API.h> 
#include <beagle/library/opencv/benchmark_time.h>

/*
#include <beagle/library/opencv/opencvDsp.h> 
#include <ti/sdo/ce/Engine.h>
#include <ti/sdo/ce/osal/Memory.h>
#include <ti/sdo/ce/CERuntime.h>
#include "../c6accelw/c6accelw.h"

#define ENGINENAME "omap3530"
#define ALGNAME "c6accel"


static String algName      = ALGNAME;
static String engineName   = ENGINENAME;
extern C6accel_Handle hC6accel;
*/

extern Time_Object sTime;extern unsigned int      time;

/*
 *  ======== main ========
 */
int main(int argc, char *argv[]) 
{   //CERuntime_init();
    DSP_cvStartDSP();
    CvCapture * capture;
    IplImage *videoFrame, *convFrame, *convOpencvFrame, *unsignedFrame; 
    IplImage *dataImage, *integralImage;
    int key;
    
    int *ptr;
    float *flPtr;

    int i,j;
    
    float tempFloat=0.0;
    float *floatDataPtr;
    float *floatOutPtr;

    /* Data to test cvIntegral() */
    unsigned char intdata[] = { 151,  57, 116, 170,   9, 247, 208, 140, 150,  60,  88,  77,   4,   6, 162,   6, 
			    	31, 143, 178,   3, 135,  91,  54, 154, 193, 161,  20, 162, 137, 150, 128, 224, 
			    	214, 113,   9,  28,  53, 211,  98, 217, 149, 233, 231, 127, 115, 203, 177,  42, 
			    	62, 155,   3, 103, 127,  16, 135, 131, 211, 158,   9,   2, 106, 227, 249, 255 }; //16 x 4
        
    if ( argc < 2 ) {
       printf( "Usage: ./remote_ti_platforms_evm3530_opencv.xv5T [option] \n");
       printf("option:\ni. integral\ns. sobel\nd. dft\n");
       return (-1);
    }

    if (*argv[1] == 's' && argc < 3) {
       printf( "Usage: ./remote_ti_platforms_evm3530_opencv.xv5T s tree.avi \n");
       printf( "Usage: ./remote_ti_platforms_evm3530_opencv.xv5T s webcam \n");

       return (-1);
    }
	
    


    switch (*argv[1]) {
 
       	case 'i':   /* Start of integral image test */
		dataImage = cvCreateImageHeader(cvSize(16, 4), IPL_DEPTH_8U, 1);
		integralImage = cvCreateImageHeader(cvSize(17, 5), IPL_DEPTH_32S, 1); 
		unsigned char *data = (unsigned char*)cvAlloc(16*4*sizeof(char));
 		unsigned int *sum = (unsigned int *)cvAlloc(17*5*sizeof(int)); 
		memcpy(data, &intdata[0], 16*4*sizeof(char));
		memset(sum, 0, 17*5*sizeof(int));
		dataImage->imageData = ( char *)data;
    		integralImage->imageData = ( char *)sum;

	 	Time_reset(&sTime);
		Time_delta(&sTime,&time);  		    		
    		DSP_cvIntegral(dataImage,integralImage,NULL,NULL);
		DSP_cvSyncDSP();
		Time_delta(&sTime,&time);
		printf("DSP_cvIntegral Total execution time = %dus\n",(unsigned int)time);  
  		    		
    		ptr = (int *)integralImage->imageData;
    		printf(" The integral image is:\n");
    		for (i=0;i<4;i++){
            	    for (j=0;j<16;j++){
                        printf("%d \t ", ptr[i* 16 + j]);
            	    }
            	    printf("\n");
    		}

		Time_reset(&sTime);
		Time_delta(&sTime,&time);
		cvIntegral(dataImage, integralImage,NULL,NULL);
		Time_delta(&sTime,&time);
		printf("cvIntegral Total execution time = %dus\n",(unsigned int)time);

		ptr = (int *)integralImage->imageData;
    		printf(" The integral image is:\n");
    		for (i=0;i<5;i++){
            	    for (j=0;j<17;j++){
                        printf("%d \t ", ptr[i* 17 + j]);
            	    }
            	    printf("\n");
    		}
		cvFree(&dataImage);
		cvFree(&integralImage);
    		break;   /* End of integral image test */

    	
    	case 's': /* Start of sobel test */
		switch (*argv[2]) {
		    
                    case 't':

    			cvNamedWindow( "video", CV_WINDOW_AUTOSIZE );
    
    
    			capture = cvCreateFileCapture(argv[2]);

    			if ( !capture) {
      	   	   	   printf("Error: Video not found\n");
      	   	   	   break;
    			}
        		videoFrame = cvQueryFrame ( capture );
     			if ( !videoFrame) {
      	   	   	   printf("**Error reading video\n");
      		   	break;
    			}

    			/* Printing Image characteristic */
    			printf("Image width = %d \t", videoFrame->width );
    			printf("Image height = %d \t", videoFrame->height );
    			printf("Image widthstep = %d \t", videoFrame->widthStep );
    			printf("Image imageSize = %d \t", videoFrame->imageSize );
    			printf("Image nChannels = %d \t", videoFrame->nChannels );
    			printf("Image nSize = %d \t", videoFrame->nSize );
    			printf("Image align = %d \n",videoFrame->align ); 

    			/* create new image for the grayscale version */
    			convFrame = cvCreateImage( cvSize( videoFrame->width, videoFrame->height ), IPL_DEPTH_8U, 1 );

    			/* create sobel filtered image */
    			convOpencvFrame = cvCreateImage( cvSize( convFrame->width, convFrame->height ), IPL_DEPTH_16S, 1 );
			
			//Time_delta(&sTime,&time);    		
    			while ( key != 'q') { 
			      /* Restet Time for benchmarking */
			      Time_reset(&sTime);
			      Time_delta(&sTime,&time); 
			      /* RGB to Greyscal using native OpenCV */         
	      	      	      cvCvtColor(videoFrame,convFrame,CV_RGB2GRAY);
			      Time_delta(&sTime,&time);
			      printf("cvCvtColor Total execution time = %dus\n",(unsigned int)time);
			      
			      /* Apply sobel filter using native OpenCV library function */
			      Time_delta(&sTime,&time);
			      cvSobel(convFrame,convOpencvFrame,1,1,3);
			      Time_delta(&sTime,&time);
			      printf("cvSobel Total execution time = %dus\n",(unsigned int)time);
			 
	 		      /* Time to test and benchmark DSP based sobel filter */
			      Time_delta(&sTime,&time);
			      DSP_cvSobel(convFrame,convOpencvFrame,1,1,3);	  
			      Time_delta(&sTime,&time);
			      printf("DSP_cvSobel Total execution time = %dus\n",(unsigned int)time); 
			      /* It is important to synch DSP with ARM before the output from DSP is accessed */
			      Time_delta(&sTime,&time);           	      	  
			      DSP_cvSyncDSP();
			      Time_delta(&sTime,&time);
			      printf("DSP_cvSyncDSP Total execution time = %dus\n",(unsigned int)time);

	      	              /* display filtered image */
			      Time_delta(&sTime,&time);
	      	              cvShowImage("video", convOpencvFrame);
			      Time_delta(&sTime,&time);
			      printf("cvShowImage Total execution time = %dus\n",(unsigned int)time);
				
			      /* get next frame */
			      Time_delta(&sTime,&time);
               	              videoFrame = cvQueryFrame( capture); 
			      Time_delta(&sTime,&time);
			      printf("cvQueryFrame Total execution time = %dus\n",(unsigned int)time);	
	      	              if ( !videoFrame) {
	         	         printf("***The End***\n");
	           	         break;
      	      	              }

	      	              key = cvWaitKey( 50 );
       	       		}
			
	       		cvDestroyWindow("video");
    	       		cvReleaseImage(&videoFrame);
    	       		cvReleaseImage(&convFrame);
               		cvReleaseImage(&convOpencvFrame); 
       	       		break; /* End of sobel test */

		    case 'w':

    			cvNamedWindow( "video", CV_WINDOW_AUTOSIZE );
    
    
    			capture = cvCaptureFromCAM(-1);

    			if ( !capture) {
      	   	   	   printf("Error: Video capture initialization failed.\n");
      	   	   	   break;
    			}
        		videoFrame = cvQueryFrame ( capture );
     			if ( !videoFrame) {
      	   	   	   printf("**Error reading from webcam\n");
      		   	break;
    			}

    			/* Printing Image characteristic */
    			printf("Image width = %d \t", videoFrame->width );
    			printf("Image height = %d \t", videoFrame->height );
    			printf("Image widthstep = %d \t", videoFrame->widthStep ); 
    			printf("Image imageSize = %d \t", videoFrame->imageSize );
    			printf("Image nChannels = %d \t", videoFrame->nChannels );
    			printf("Image nSize = %d \t", videoFrame->nSize );
    			printf("Image align = %d \n",videoFrame->align );

    			/* create new image for the grayscale version */
    			convFrame = cvCreateImage( cvSize( videoFrame->width, videoFrame->height ), IPL_DEPTH_8U, 1 );
		

    			/* create sobel filtered image */
    			convOpencvFrame = cvCreateImage( cvSize( convFrame->width, convFrame->height ), IPL_DEPTH_16S, 1 );
			unsignedFrame = cvCreateImage( cvSize( videoFrame->width, videoFrame->height ), IPL_DEPTH_8U, 1 );
    		
    			while ( key != 'q') { 

			      Time_reset(&sTime);
			      Time_delta(&sTime,&time);
          
//	      	      	      DSP_cvCvtColor(videoFrame,convFrame,CV_RGB2GRAY);
//			      DSP_cvSyncDSP();
			      DSP_cvCvtColor_cvSobel(videoFrame,convFrame,CV_RGB2GRAY, convFrame,convOpencvFrame,1,1,3);
			      Time_delta(&sTime,&time);
			      printf("DSP_cvCvtColor_cvSobel Total execution time = %dus\n",(unsigned int)time);
			      DSP_cvSyncDSP();


			      Time_delta(&sTime,&time);
//			      cvCvtColor(videoFrame,convFrame,CV_RGB2GRAY);

			      Time_delta(&sTime,&time);
			      printf("cvCvtColor Total execution time = %dus\n",(unsigned int)time); 
			      Time_delta(&sTime,&time);
//			      cvSobel(convFrame,convOpencvFrame,1,1,3);
			      Time_delta(&sTime,&time);
			      printf("cvSobel Total execution time = %dus\n",(unsigned int)time);
			      Time_delta(&sTime,&time);
//	                      DSP_cvSobel(convFrame,convOpencvFrame,1,1,3);	 
			      Time_delta(&sTime,&time);
			      printf("DSP_cvSobel Total execution time = %dus\n",(unsigned int)time);

			      Time_delta(&sTime,&time);           	      	  
//			      DSP_cvSyncDSP();
			      Time_delta(&sTime,&time);
			      printf("DSP_cvSyncDSP Total execution time = %dus\n",(unsigned int)time); 
 			  
			      // convert singed image to unsigned image for better clarity of image   
			      
			      Time_delta(&sTime,&time);          	      
	  		      cvConvert(convOpencvFrame,unsignedFrame);
			      Time_delta(&sTime,&time);
			      printf("cvConvert Total execution time = %dus\n",(unsigned int)time);
	      	              // display filtered image 

			      Time_delta(&sTime,&time);
			      cvConvert(convOpencvFrame,unsignedFrame);
	      	              cvShowImage("video", unsignedFrame);
//			      cvCvtColor(videoFrame,convFrame,CV_RGB2GRAY);
//			      cvShowImage("video",convFrame);
			      Time_delta(&sTime,&time);
			      printf("cvShowImage Total execution time = %dus\n",(unsigned int)time);
			      Time_delta(&sTime,&time);
               	              videoFrame = cvQueryFrame( capture);
			      Time_delta(&sTime,&time);
			      printf("cvQueryFrame Total execution time = %dus\n",(unsigned int)time);
	      	              if ( !videoFrame) {
	         	         printf("***The End***\n");
	           	         break;
      	      	              }
			      Time_delta(&sTime,&time);
	      	              key = cvWaitKey( 50 );
			      Time_delta(&sTime,&time);
			      printf("cvWaitKey Total execution time = %dus\n",(unsigned int)time);
       	       		}
			
			cvSaveImage("DSP_RGB_Y.jpg",convFrame,0);
	       		cvDestroyWindow("video");
    	       		cvReleaseImage(&videoFrame);
    	       		cvReleaseImage(&convFrame);
               		cvReleaseImage(&convOpencvFrame);
       	       		break; /* End of sobel test */

		    default:
			printf("Operation not supported.\n");
			break;
		}
		break;			
		

	case 'd':
	        floatDataPtr = (float *)cvAlloc(sizeof(float)*512);
		floatOutPtr = (float *)cvAlloc(sizeof(float)*512),
		dataImage = cvCreateImageHeader(cvSize(16, 16), IPL_DEPTH_32F, 2);
		dataImage->imageData = (char *)floatDataPtr;
		integralImage = cvCreateImageHeader(cvSize(16, 16), IPL_DEPTH_32F, 2);
		integralImage->imageData = (char *)floatOutPtr;
                
	
		for (i=0; i< 256 * 2; i+=2) {
		    tempFloat += 1.0;
		    floatDataPtr[i] = tempFloat;
		    floatDataPtr[i+1] = 0.0;
	        } 
		/* Print the input data for DFT */
		flPtr = (float *)dataImage->imageData;
    		printf(" The DFT input is:\n");
    		for (i=0;i<dataImage->height;i++){ 
		    key = 0;
            	    for (j=0;j<dataImage->width * 2;j+=2){
			key++;
                        printf("%f + i%f\t", flPtr[(i*dataImage->width * 2)+j], flPtr[(i*dataImage->width * 2) + j + 1]);
			if ((key % 5) == 0) printf("\n");
            	    }
            	    printf("\n");
    		}

		/* Normalize the input withing range of -1.0 to 1.0 (This is mulitply by (1/32767) */
		cvConvertScale(dataImage,dataImage,0.000030518,0);

		Time_reset(&sTime);
		Time_delta(&sTime,&time);		
    		DSP_cvDFT(dataImage,integralImage,CV_DXT_FORWARD,0);
		DSP_cvSyncDSP();
		/* Unnormalize the data */
		cvConvertScale(integralImage,integralImage,32767,0);
		/* As output is scaled down by 4, bring it to unscaled form */
 		cvConvertScale(integralImage,integralImage,4,0);
		Time_delta(&sTime,&time);
		printf("DSP_cvDFT Total execution time = %dus\n",(unsigned int)time);    		  
    		
		/* Print the output data for DFT */ 		
    		flPtr = (float *)integralImage->imageData;
    		printf(" The DFT output is:\n");
    		for (i=0;i<integralImage->height;i++){
		    key = 0;
            	    for (j=0;j<integralImage->width * 2;j+=2){
			key++;
                        printf("%f + i%f\t", flPtr[(i*integralImage->width * 2)+j], flPtr[(i*integralImage->width * 2) + j + 1]);
			if ((key % 5) == 0) printf("\n");
            	    }
            	    printf("\n");
    		}
 		cvFree(&floatDataPtr);
		cvFree(&floatOutPtr);
		break;
	default:
	      return -1;
    }
       
    DSP_cvEndDSP();
    return 0;

}    
   
