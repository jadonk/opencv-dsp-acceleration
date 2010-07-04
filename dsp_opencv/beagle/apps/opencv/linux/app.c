/*
 *  ======== app.c ========
 *  Test application for OPENCVTEST algorithm.
 */

#include <xdc/std.h>

#include <stdio.h>
#include <stdlib.h>


/* OpenCV Headerfiles */
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>

#include <beagle/library/opencv/beagle_opencv_API.h>
#include <beagle/library/opencv/opencvDsp.h>



/*
 *  ======== main ========
 */
int main(int argc, char *argv[])
{
    CvCapture * capture;
    IplImage *videoFrame, *convFrame, *convOpencvFrame;
    
    int key;
    int status;
    char *dsp;

    int i,j;

    ///////////test data for integral//////////

    char intdata[] = 	{1,  2,  5,   1,    2, /* Test Data 7x5 */
			 2,  20, 50,  20,   5,
			 5,  50, 100, 50,   2, 
			 2,  20, 50,  20,   1, 
			 1,  5,  25,  1,    2, 
			 5,  2,  25,  2,    5,
			 2,  1,  5,   2,    1
			};

   int integraldata[] = 	{ 0, 0, 0, 0, 0, 0, /* Resutl Arrary 8x6 */
			          0, 0, 0, 0, 0, 0, 
			   	  0, 0, 0, 0, 0, 0,
			     	  0, 0, 0, 0, 0, 0,
			   	  0, 0, 0, 0, 0, 0, 
			   	  0, 0, 0, 0, 0, 0, 
			   	  0, 0, 0, 0, 0, 0,
			   	  0, 0, 0, 0, 0, 0 
			 	};
    IplImage *dataImage = cvCreateImageHeader(cvSize(5, 7), IPL_DEPTH_8U, 1);
    IplImage *integralImage = cvCreateImageHeader(cvSize(6, 8), IPL_DEPTH_32S, 1);
    dataImage->imageData = &intdata[0];
    integralImage->imageData = (char *)&integraldata[0];
    //dataImage->widthStep = 5;
    //integralImage->widthStep = 6;
    int *ptr;

    if ( argc < 2 ) {
       printf( "Usage: ./remote_ti_platforms_evm3530_opencv.xv5T [option] \n");
       printf("option:\ni. integral\ns. sobel\nd. dft\n");
       return (-1);
    }

    if (*argv[1] == 's' && argc < 3) {
       printf( "Usage: ./remote_ti_platforms_evm3530_opencv.xv5T s tree.avi \n");
       return (-1);
    }
    switch (*argv[1]) {
 
       	case 'i':   /* Start of integral image test */

    		status = cvDspInit();
    		if (!status) {
       	   	   printf("Error: DSP initilaization Failed. \n");
                   return (-1);
    		}

    		status = integralDsp( CV_8UC1, dataImage->widthStep, dataImage->imageData, dataImage->height, dataImage->width, dataImage->nChannels, 				CV_32SC1, integralImage->widthStep, integralImage->imageData, integralImage->height, 							integralImage->width,integralImage->nChannels);
    		//cvIntegral(dataImage,integralImage,NULL,NULL);

    		if (!status)
           	   printf("Error:\n");
    		else printf("Success\n");

    		ptr = (int *)integralImage->imageData;
    		printf(" The integral image is:\n");
    		for (i=0;i<8;i++){
            	    for (j=0;j<6;j++){
                        printf("%d \t ", ptr[i* 6 + j]);
            	    }
            	    printf("\n");
    		}


    		goto exit;   /* End of integral image test */

    	
    	case 's': /* Start of sobel test */

    		cvNamedWindow( "video", CV_WINDOW_AUTOSIZE );
    
    
    		capture = cvCreateFileCapture(argv[2]);

    		if ( !capture) {
      	   	   printf("Error: Video not found\n");
      	   	   goto exit;
    		}
        	videoFrame = cvQueryFrame ( capture );
     		if ( !videoFrame) {
      	   	   printf("**Error reading video\n");
      		   goto exit;
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

    
    		status = cvDspInit();
    		if (!status) {
       	   	   printf("Error: DSP initilaization Failed. \n");
           	   return (-1);
    		}


    		while ( key != 'q') {
          
	      	      cvCvtColor(videoFrame,convFrame,CV_RGB2GRAY);

          
 	              dsp = getenv("OPENCV_DSP");

	              if ( dsp == NULL ) {
	                 cvSobel(convFrame,convOpencvFrame,1,1,7);	     

	      	      }
	              else if (*dsp == '1') {
	                   /* This will will go in place of original sobel function on OpenCV library */   

      	           	   status = sobelDsp( CV_8UC1, convFrame->widthStep, convFrame->imageData, convFrame->height, convFrame->width, 					convFrame->nChannels, CV_16SC1, convOpencvFrame->widthStep, convOpencvFrame->imageData, 					convOpencvFrame->height, convOpencvFrame->width, convOpencvFrame->nChannels,7);
   	              	   if (!status) {
 		              printf("Error.\n");
		              break;
 	                   }
	       
	      	      }
  	      	      else {
	                  cvSobel(convFrame,convOpencvFrame,1,1,3);	  
              	      }
	  
	      	      /* display filtered image */
	      	      cvShowImage("video", convOpencvFrame);

               	      videoFrame = cvQueryFrame( capture);
	      	      if ( !videoFrame) {
	         	 printf("***The End***\n");
	           	 goto exit;
      	      	      }

	      	      key = cvWaitKey( 50 );
       	       }
       	       break; /* End of sobel test */

	default:
	      return (-1);
    }
exit:
    cvEndDsp();
    cvDestroyWindow("video");
    cvReleaseImage(&videoFrame);
    cvReleaseImage(&convFrame);
    cvReleaseImage(&convOpencvFrame);

    return 0;

}    
   
