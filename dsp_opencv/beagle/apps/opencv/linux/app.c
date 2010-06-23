/*
 *  ======== app.c ========
 *  Test application for OPENCVTEST algorithm.
 */

#include <stdio.h>
#include <stdlib.h>
#include <xdc/std.h>
#include <ti/xdais/xdas.h>
#include <beagle/library/opencv/beagle_opencv_API.h>


/* OpenCV Headerfiles */
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>
#include <string.h>





OPENCV_Operation operation = OPENCV_OPERATION_SOBEL3x3 ;


/*
 *  ======== main ========
 */
int main(Int argc, String argv[])
{
    CvCapture * pCapture;
    IplImage *pVideoFrame, *convFrame;
    int i;
    int key;
    char filename[50];

    // Initialize video capture
    pCapture = cvCaptureFromCAM(-1);
    if (!pCapture ) {
       fprintf(stderr, "Video Capture Initialization Failed.\n");
       return -1;
    }


    // capture 2 frames
    for (i=0; i<2; i++) {
        pVideoFrame = cvQueryFrame ( pCapture );
        if ( !pVideoFrame ) {
           fprintf(stderr, "Failed to get video frame.\n");
        }
        //save captrue prame as image file
        sprintf(filename, "Frame%d.jpg",i+1);
        if ( !cvSaveImage(filename, pVideoFrame, 0) ) {
           fprintf(stderr, "Save Image %s Failed", filename);
           }
    }   


    pVideoFrame = cvLoadImage( "Frame2.jpg", CV_LOAD_IMAGE_ANYCOLOR );
    
    /* create new image for the grayscale version */

    convFrame = cvCreateImage( cvSize( pVideoFrame->width, pVideoFrame->height ), IPL_DEPTH_8U, 1 );
    IplImage *convOpencvFrame = cvCreateImage( cvSize( convFrame->width, convFrame->height ), IPL_DEPTH_8U, 1 );
    cvCvtColor(pVideoFrame,convFrame,CV_RGB2GRAY);
    
    /* Printing Image characteristic */
    printf("Image width = %d \n", convFrame->width );
    printf("Image height = %d \n", convFrame->height );
    printf("Image widthstep = %d \n", convFrame->widthStep );
    printf("Image imageSize = %d \n", convFrame->imageSize );
    printf("Image nChannels = %d \n", convFrame->nChannels );
    printf("Image nSize = %d \n", convFrame->nSize );
    printf("Image align = %d \n",convFrame->align );

    
    
    switch (operation) {
	   case OPENCV_OPERATION_GEN_TWIDDLE :
		printf("This application does not support twiddle generation. Switch to SVN revision 75. \n");
		break;

	   case OPENCV_OPERATION_DFT :
		printf("This application does not support DFT. Switch to SVN revision 75. \n");
		break;  

    	   case OPENCV_OPERATION_SOBEL3x3 :
	   case OPENCV_OPERATION_SOBEL5x5 :
 	   case OPENCV_OPERATION_SOBEL7x7 :
		cvNamedWindow( "video", 1 );
		
		while ( key != 'q') {
		       
		        pVideoFrame = cvQueryFrame ( pCapture );
           	        cvCvtColor(pVideoFrame,convFrame,CV_RGB2GRAY);
             
                	OPENCVTEST_main( CV_8UC1, convFrame->widthStep, convFrame->imageData, convFrame->height, convFrame->width, convFrame->nChannels, 				CV_8UC1, convOpencvFrame->widthStep, convOpencvFrame->imageData, convOpencvFrame->height, convOpencvFrame->width, 				convOpencvFrame->nChannels,operation );

			/* create a window */
		

			cvShowImage( "video", convOpencvFrame );
			key = cvWaitKey( 10 );
			}
		cvSaveImage("unprocessed.jpg", convFrame, 0);
		cvSaveImage("processed.jpg", convOpencvFrame, 0);
		break;

	   default :
                return(0);
    }
    cvReleaseImage(&convOpencvFrame);
    cvReleaseImage(&convFrame);
    cvDestroyWindow( "video" );
    
    return (0);

}


