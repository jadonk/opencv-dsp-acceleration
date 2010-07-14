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
    IplImage *videoFrame, *convFrame, *convOpencvFrame, *unsignedFrame;
    IplImage *dataImage, *integralImage;
    int key;
    int status;
    int *ptr;
    float *flPtr;

    int i,j;
    float dftTestInput[512];
    float dftTestOutput[512]; 
    float tempFloat=0.0;


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

    if (getenv("OPENCVDSP") != NULL) {
	if (*getenv("OPENCVDSP") == '1' ) {
	    cvStartDSP();
	}
    }
	
    switch (*argv[1]) {
 
       	case 'i':   /* Start of integral image test */
		dataImage = cvCreateImageHeader(cvSize(5, 7), IPL_DEPTH_8U, 1);
		integralImage = cvCreateImageHeader(cvSize(6, 8), IPL_DEPTH_32S, 1);
		dataImage->imageData = &intdata[0];
    		integralImage->imageData = (char *)&integraldata[0];
		    		
    		cvIntegral(dataImage,integralImage,NULL,NULL);

    		if (!status)
           	   printf("Error:\n");
    		
    		ptr = (int *)integralImage->imageData;
    		printf(" The integral image is:\n");
    		for (i=0;i<8;i++){
            	    for (j=0;j<6;j++){
                        printf("%d \t ", ptr[i* 6 + j]);
            	    }
            	    printf("\n");
    		}

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
    		
    			while ( key != 'q') { 
          
	      	      	      cvCvtColor(videoFrame,convFrame,CV_RGB2GRAY);

	                      cvSobel(convFrame,convOpencvFrame,1,1,7);	  
             	      
	  
	      	              /* display filtered image */
	      	              cvShowImage("video", convOpencvFrame);

               	              videoFrame = cvQueryFrame( capture);
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
          
	      	      	      cvCvtColor(videoFrame,convFrame,CV_RGB2GRAY);

	                      cvSobel(convFrame,convOpencvFrame,1,1,3);	
			  
			      /* convert singed image to unsigned image for better clarity of image */             	      
	  		      cvConvert(convOpencvFrame,unsignedFrame);

	      	              /* display filtered image */
	      	              cvShowImage("video", unsignedFrame);

               	              videoFrame = cvQueryFrame( capture);
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

		    default:
			printf("Operation not supported.\n");
			break;
		}
		break;			
		

	case 'd':
	        
		dataImage = cvCreateImageHeader(cvSize(16, 16), IPL_DEPTH_32F, 2);
		dataImage->imageData = (char *)dftTestInput;
		integralImage = cvCreateImageHeader(cvSize(16, 16), IPL_DEPTH_32F, 2);
		integralImage->imageData = (char *)dftTestOutput;
	
		for (i=0; i< 256 * 2; i+=2) {
		    tempFloat += 1.0;
		    dftTestInput[i] = tempFloat;
		    dftTestInput[i+1] = 0.0;
	        } 
		
    		cvDFT(dataImage,integralImage,CV_DXT_FORWARD,0);

    		if (!status)
           	   printf("Error:\n");
    		
    		flPtr = (float *)integralImage->imageData;
    		printf(" The DFT image is:\n");
    		for (i=0;i<integralImage->height;i++){
		    key = 0;
            	    for (j=0;j<integralImage->width * 2;j+=2){
			key++;
                        printf("%f + i%f\t", flPtr[(i*integralImage->width * 2)+j], flPtr[(i*integralImage->width * 2) + j + 1]);
			if ((key % 5) == 0) printf("\n");
            	    }
            	    printf("\n");
    		}
		break;
	default:
	      return (-1);
    }
      

    if (getenv("OPENCVDSP") != NULL) {
	if (*getenv("OPENCVDSP") == '1' ) {
	    cvEndDSP();
	}
    }
    
    
    return 0;

}    
   
