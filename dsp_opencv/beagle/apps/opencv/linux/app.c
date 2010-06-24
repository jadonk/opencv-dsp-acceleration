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






/*
 *  ======== main ========
 */
int main(Int argc, String argv[])
{
    CvCapture * capture;
    IplImage *videoFrame, *convFrame;
    int i;
    int key;
//    char filename[50];

    // Initialize video capture
    capture = cvCaptureFromCAM(-1);
    if (!capture ) {
       fprintf(stderr, "Video Capture Initialization Failed.\n");
       return -1;
    }

/*
    // capture 2 frames
    for (i=0; i<2; i++) {
        videoFrame = cvQueryFrame ( capture );
        if ( !videoFrame ) {
           fprintf(stderr, "Failed to get video frame.\n");
        }
        //save captrue prame as image file
        sprintf(filename, "Frame%d.jpg",i+1);
        if ( !cvSaveImage(filename, videoFrame, 0) ) {
           fprintf(stderr, "Save Image %s Failed", filename);
           }
    }   


    videoFrame = cvLoadImage( "Frame2.jpg", CV_LOAD_IMAGE_ANYCOLOR );
*/  

    videoFrame = cvQueryFrame ( capture );  
    /* create new image for the grayscale version */
    convFrame = cvCreateImage( cvSize( videoFrame->width, videoFrame->height ), IPL_DEPTH_8U, 1 );
    IplImage *convOpencvFrame = cvCreateImage( cvSize( convFrame->width, convFrame->height ), IPL_DEPTH_16S, 1 );
    cvCvtColor(videoFrame,convFrame,CV_RGB2GRAY);
    
    /* Printing Image characteristic */
    printf("Image width = %d \n", convFrame->width );
    printf("Image height = %d \n", convFrame->height );
    printf("Image widthstep = %d \n", convFrame->widthStep );
    printf("Image imageSize = %d \n", convFrame->imageSize );
    printf("Image nChannels = %d \n", convFrame->nChannels );
    printf("Image nSize = %d \n", convFrame->nSize );
    printf("Image align = %d \n",convFrame->align );

    
    
    cvNamedWindow( "video", 1 );
    i=1;

    while ( key != 'q') {
    
          videoFrame = cvQueryFrame ( capture );
          cvCvtColor(videoFrame,convFrame,CV_RGB2GRAY);
             
       	  sobelDsp( CV_8UC1, convFrame->widthStep, convFrame->imageData, convFrame->height, convFrame->width, convFrame->nChannels, 				CV_16UC1, convOpencvFrame->widthStep, convOpencvFrame->imageData, convOpencvFrame->height, convOpencvFrame->width, 				convOpencvFrame->nChannels,5 );
//	  cvSobel(convFrame,convOpencvFrame,1,1,5);

	  /* create a window */
	  printf( "Frame=%d\n",i++);

	  cvShowImage( "video", convOpencvFrame );
	  key = cvWaitKey( 10 );
    }
    cvSaveImage("unprocessed.pgm", convFrame, 0);
    cvSaveImage("processed.pgm", convOpencvFrame, 0);
		
    
    cvReleaseImage(&convOpencvFrame);
    cvReleaseImage(&convFrame);
    cvReleaseImage(&videoFrame);
    cvReleaseCapture( &capture );
    cvDestroyWindow( "video" );
    
    return (0);

}


