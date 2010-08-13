/*
 *  ======== app.c ========
 *  Test application for OPENCV_DSP algorithm.  
 */


#include <stdio.h>
#include <stdlib.h>


/* OpenCV Headerfiles */
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>

#include <beagle/library/opencv/beagle_opencv_API.h> 
#include <beagle/library/opencv/benchmark_time.h>


extern Time_Object sTime;extern unsigned int      time;

/*
 *  ======== main ========
 */
int main(int argc, char *argv[]) 
{   
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
       printf("Following are the all usage:\n");
       printf("./remote_ti_platforms_evm3530_opencv.xv5T i dsp(To test integral-image algorithm using DSP. Input is from webcam). Note: You need to install VLIB to test this.\n");
       printf("./remote_ti_platforms_evm3530_opencv.xv5T i arm(To test integral-image algorithm using ARM. Input is from webcam). \n");
       printf("./remote_ti_platforms_evm3530_opencv.xv5T i test(To test integral-image algorithm using test data given in APP. Input is from webcam). \n");
       printf("./remote_ti_platforms_evm3530_opencv.xv5T d dsp (To test DFT algorithm using DSP) \n");
       printf("./remote_ti_platforms_evm3530_opencv.xv5T d arm (To test DFT algorithm using ARM) \n");
       printf("./remote_ti_platforms_evm3530_opencv.xv5T s tree.avi dsp (To test sobel algorithm for movie clip tree.avi using DSP) \n");
       printf("./remote_ti_platforms_evm3530_opencv.xv5T s tree.avi arm (To test sobel algorithm for movie clip tree.avi using ARM) \n");
       printf("./remote_ti_platforms_evm3530_opencv.xv5T s webcam dsp (To test sobel algorithm for image from webcam using DSP) \n");
       printf("./remote_ti_platforms_evm3530_opencv.xv5T s webcam arm (To test sobel algorithm for image from webcam using ARM) \n");
       printf("./remote_ti_platforms_evm3530_opencv.xv5T rgb2gray (To test RGB to Gray for image from webcam.) \n");
       return (-1);
    }

    if (*argv[1] == 's' && argc < 3) {
       printf( "Usage: ./remote_ti_platforms_evm3530_opencv.xv5T s tree.avi \n");
       printf( "Usage: ./remote_ti_platforms_evm3530_opencv.xv5T s webcam \n");

       return (-1);
    }
	
    


    switch (*argv[1]) {
 
       	case 'i': 
	    switch (*argv[2]) {
                case 'd': { //'d' dor DSP accelerated

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
    			
    			/* create new image for the grayscale version */
    			convFrame = cvCreateImage( cvSize( videoFrame->width, videoFrame->height ), IPL_DEPTH_8U, 1 );

    			/* create sobel filtered image */
    			convOpencvFrame = cvCreateImage( cvSize( convFrame->width+1, convFrame->height+1 ), IPL_DEPTH_32S, 1 );
			
			/* Process the first frame outside the loop*/
			DSP_cvCvtColor(videoFrame,convFrame,CV_RGB2GRAY);
			DSP_cvSyncDSP();			    
    			while ( key != 'q') { 
			          			 
	 		      /* Time to test and benchmark DSP based sobel filter */
			      Time_reset(&sTime);
			      Time_delta(&sTime,&time);
			      /*Find integral image */
			      DSP_cvIntegral(convFrame,convOpencvFrame,NULL,NULL);			      
			      
			      /* get next frame */
			      videoFrame = cvQueryFrame( capture); 
			      if ( !videoFrame) {
	         	         printf("***The End***\n");
	           	         break;
      	      	              }
			      DSP_cvSyncDSP();			      		      
			      /* Do color conversion */
			      DSP_cvCvtColor(videoFrame,convFrame,CV_RGB2GRAY);
			      /* show Image */
			      cvShowImage("video", convOpencvFrame);
			      /* Sync with DSP */					  
			      DSP_cvSyncDSP();
			      Time_delta(&sTime,&time);         
 		              printf("Total execution time = %dus\n",(unsigned int)time);

	      	              key = cvWaitKey( 15 );
       	       		}
			
	       		cvDestroyWindow("video");
    	       		cvReleaseImage(&videoFrame);
    	       		cvReleaseImage(&convFrame);
               		cvReleaseImage(&convOpencvFrame);
			cvReleaseCapture(&capture); 
			}
       	       		break; /* End of sobel test */

		case 'a': { // 'a' for ARM side

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

    			
    			/* create new image for the grayscale version */
    			convFrame = cvCreateImage( cvSize( videoFrame->width, videoFrame->height ), IPL_DEPTH_8U, 1 );

    			/* create sobel filtered image */
    			convOpencvFrame = cvCreateImage( cvSize( convFrame->width+1, convFrame->height+1 ), IPL_DEPTH_32S, 1 );
			
			/* Process the first frame outside the loop*/
			cvCvtColor(videoFrame,convFrame,CV_RGB2GRAY);
			    		    
    			while ( key != 'q') { 
			          			 
	 		      /* Time to test and benchmark DSP based sobel filter */
			      Time_reset(&sTime);
			      Time_delta(&sTime,&time);
			      /*Find integral image */
			      cvIntegral(convFrame,convOpencvFrame,NULL,NULL);
			      
			      /* get next frame */
			      videoFrame = cvQueryFrame( capture); 
			      if ( !videoFrame) {
	         	         printf("***The End***\n");
	           	         break;
      	      	              }
				  
			      /* Do color conversion */
			      cvCvtColor(videoFrame,convFrame,CV_RGB2GRAY);
			      /* show Image */
			      cvShowImage("video", convOpencvFrame);

			      Time_delta(&sTime,&time); 			         
			      printf("Total execution time = %dus\n",(unsigned int)time);

	      	              key = cvWaitKey( 15 );
       	       		}
			
	       		cvDestroyWindow("video");
    	       		cvReleaseImage(&videoFrame);
    	       		cvReleaseImage(&convFrame);
               		cvReleaseImage(&convOpencvFrame); 
			cvReleaseCapture(&capture);
			}
       	       		break; /* End of sobel test */
			
		case 't': { /* This to test the consistency of algorithm */
			/* Start of integral image test */
			dataImage = cvCreateImageHeader(cvSize(16, 4), IPL_DEPTH_8U, 1);
			integralImage = cvCreateImageHeader(cvSize(17, 5), IPL_DEPTH_32S, 1); 
			unsigned char *data = (unsigned char*)cvAlloc(16*4*sizeof(char));
 			unsigned int *sum = (unsigned int *)cvAlloc(17*5*sizeof(int)); 
			memcpy(data, &intdata[0], 16*4*sizeof(char));
			memset(sum, 0, 17*5*sizeof(int));
			dataImage->imageData = ( char *)data;
    			integralImage->imageData = ( char *)sum;
	    		/* DSP based integral */
    			DSP_cvIntegral(dataImage,integralImage,NULL,NULL);
  		    	
    			ptr = (int *)integralImage->imageData;
    			printf(" The integral image is:\n");
    			for (i=0;i<4;i++){
            	    	    for (j=0;j<16;j++){
                        	printf("%d \t ", ptr[i* 16 + j]);
            	    	    }
            	   	    printf("\n");
    			}

			/* Arm based cvIntegral() */
			cvIntegral(dataImage, integralImage,NULL,NULL);
			
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
			}
    			break;   /* End of integral image test */

		default: 
		    printf("Input argument Error:\n Usage :\n./remote_ti_platforms_evm3530_opencv.xv5T i d \n./remote_ti_platforms_evm3530_opencv.xv5T i a\n ./remote_ti_platforms_evm3530_opencv.xv5T i d \n"); 
		
		    break;
		}
		break;                

        /* Start RGB to Y test */
	case 'r': {
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
    			
    		/* create new image for the grayscale version */
    		convFrame = cvCreateImage( cvSize( videoFrame->width, videoFrame->height ), IPL_DEPTH_8U, 1 );

 		unsignedFrame = cvCreateImage( cvSize( videoFrame->width, videoFrame->height ), IPL_DEPTH_8U, 1 );
		 		
    		while ( key != 'q') { 
		    /* Time to test and benchmark DSP based sobel filter */
	            Time_reset(&sTime);
		    Time_delta(&sTime,&time); 
		    /* Process the first frame outside the loop*/			
		    DSP_cvCvtColor(videoFrame,convFrame,CV_RGB2GRAY);
		    	 
		    videoFrame = cvQueryFrame( capture);
		    if ( !videoFrame) {
	               printf("***The End***\n");
	               break;
      	      	    }
		      
		    DSP_cvSyncDSP();
		    cvShowImage("video",convFrame); 		    
		    
		    Time_delta(&sTime,&time); 
		    printf("Total execution time1 = %dus\n",(unsigned int)time);
					      
	      	    key = cvWaitKey( 15 );
			      
       	       	}
			
		
	       	cvDestroyWindow("video");
    	       	cvReleaseImage(&videoFrame);
    	       	cvReleaseImage(&convFrame);
               	}	
       	       	break; 
	        
    	
    	case 's': /* Start of sobel test */
		switch (*argv[2]) {
		    
                    case 't':
		      switch(*argv[3]) {
			case 'd': { //'d' dor DSP accelerated

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
    			
    			    /* create new image for the grayscale version */
    			    convFrame = cvCreateImage( cvSize( videoFrame->width, videoFrame->height ), IPL_DEPTH_8U, 1 );

    			    /* create sobel filtered image */
    			    convOpencvFrame = cvCreateImage( cvSize( convFrame->width, convFrame->height ), IPL_DEPTH_16S, 1 );
			
			    DSP_cvCvtColor(videoFrame,convFrame,CV_RGB2GRAY);
			    DSP_cvSyncDSP();    		
    			    while ( key != 'q') { 
			          			 
	 		          /* Time to test and benchmark DSP based sobel filter */
			          Time_reset(&sTime);
			          Time_delta(&sTime,&time);
				  DSP_cvSobel(convFrame,convOpencvFrame,1,1,3);	

				  /* get next frame */
			          videoFrame = cvQueryFrame( capture); 
			          if ( !videoFrame) {
	         	             printf("***The End***\n");
	           	             break;
      	      	                  }
				  /* Sync with DSP*/		       	      	  
			          DSP_cvSyncDSP();
				  /* Start RGB To Y conversion of next frame */
			          DSP_cvCvtColor(videoFrame,convFrame,CV_RGB2GRAY);
				  
				  /* Display Filtered Image */
				  cvShowImage("video", convOpencvFrame);
 				  /* Sync DSP */	      	       
			          DSP_cvSyncDSP();
				        	                  
			          Time_delta(&sTime,&time);
			          printf("Total execution time = %dus\n",(unsigned int)time);			          

	      	                  key = cvWaitKey( 15 );
       	       		    }
			
	       		cvDestroyWindow("video");
    	       		cvReleaseImage(&videoFrame);
    	       		cvReleaseImage(&convFrame);
               		cvReleaseImage(&convOpencvFrame); 
			}
       	       		break; /* End of sobel test */

			case 'a': { // 'a' for ARM side

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

    			
    			    /* create new image for the grayscale version */
    			    convFrame = cvCreateImage( cvSize( videoFrame->width, videoFrame->height ), IPL_DEPTH_8U, 1 );

    			    /* create sobel filtered image */
    			    convOpencvFrame = cvCreateImage( cvSize( convFrame->width, convFrame->height ), IPL_DEPTH_16S, 1 );
			
			    cvCvtColor(videoFrame,convFrame,CV_RGB2GRAY);   		
    			    while ( key != 'q') { 
			          /* Time to test and benchmark DSP based sobel filter */
			          Time_reset(&sTime);
			          Time_delta(&sTime,&time);
				  cvSobel(convFrame,convOpencvFrame,1,1,3);	

				  /* get next frame */
			          videoFrame = cvQueryFrame( capture); 
			          if ( !videoFrame) {
	         	             printf("***The End***\n");
	           	             break;
      	      	                  }
				  
				  /* Start RGB To Y conversion of next frame */
			          cvCvtColor(videoFrame,convFrame,CV_RGB2GRAY);
				  
				  /* Display Filtered Image */
				  cvShowImage("video", convOpencvFrame);
 				  				        	                  
			          Time_delta(&sTime,&time);
			          printf("Total execution time = %dus\n",(unsigned int)time);			          

	      	                  key = cvWaitKey( 15 );
       	       		    }
			
	       		cvDestroyWindow("video");
    	       		cvReleaseImage(&videoFrame);
    	       		cvReleaseImage(&convFrame);
               		cvReleaseImage(&convOpencvFrame); 
			}
       	       		break; /* End of sobel test */

			default: 
			    printf("Input argument Error:\n Usage :\n./remote_ti_platforms_evm3530_opencv.xv5T s tree.avi d \n./remote_ti_platforms_evm3530_opencv.xv5T s tree.avi a\n"); 
		      }
		      break;
			
		    case 'w':
		      switch(*argv[3]) {
			case 'd': { //'d' dor DSP accelerated

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
    			
    			    /* create new image for the grayscale version */
    			    convFrame = cvCreateImage( cvSize( videoFrame->width, videoFrame->height ), IPL_DEPTH_8U, 1 );

    			    /* create sobel filtered image */
    			    convOpencvFrame = cvCreateImage( cvSize( convFrame->width, convFrame->height ), IPL_DEPTH_16S, 1 );
			    /* Create unsigned image */
 			    unsignedFrame = cvCreateImage( cvSize( videoFrame->width, videoFrame->height ), IPL_DEPTH_8U, 1 );
			
			    /* Process the first frame outside the loop*/			
			    DSP_cvCvtColor(videoFrame,convFrame,CV_RGB2GRAY);
			    DSP_cvSyncDSP();    		
    			    while ( key != 'q') { 
			          			 
	 		          /* Time to test and benchmark DSP based sobel filter */
			          Time_reset(&sTime);
			          Time_delta(&sTime,&time);
				  DSP_cvSobel(convFrame,convOpencvFrame,1,1,3);	
				 			 
				  /* get next frame */
			          videoFrame = cvQueryFrame( capture); 
			          if ( !videoFrame) {
	         	             printf("***The End***\n");
	           	             break;
      	      	                  }
				 
				  /* Sync with DSP*/		       	      	  
			          DSP_cvSyncDSP();				  
				  	
				  /* Start RGB To Y conversion of next frame */
			          DSP_cvCvtColor(videoFrame,convFrame,CV_RGB2GRAY);
				  
				  /* Convert signed image to unsign image for better clarity */
				  cvConvert(convOpencvFrame,unsignedFrame);
				  /* Display Filtered Image */
				  cvShowImage("video", unsignedFrame);

 				  /* Sync DSP */	      	       
			          DSP_cvSyncDSP();
				        	                  
			          Time_delta(&sTime,&time);
			          printf("Total execution time = %dus\n",(unsigned int)time);			          

	      	                  key = cvWaitKey( 15 );
       	       		    }
			
	       		cvDestroyWindow("video");
    	       		cvReleaseImage(&videoFrame);
    	       		cvReleaseImage(&convFrame);
               		cvReleaseImage(&convOpencvFrame); 
			}
       	       		break; /* End of sobel test */

			case 'a': { // 'a' for ARM side

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

    			
    			    /* create new image for the grayscale version */
    			    convFrame = cvCreateImage( cvSize( videoFrame->width, videoFrame->height ), IPL_DEPTH_8U, 1 );

    			    /* create sobel filtered image */
    			    convOpencvFrame = cvCreateImage( cvSize( convFrame->width, convFrame->height ), IPL_DEPTH_16S, 1 );
			    /* Create unsigned image */
 			    unsignedFrame = cvCreateImage( cvSize( videoFrame->width, videoFrame->height ), IPL_DEPTH_8U, 1 );
			
			    /* Process the first frame outside the loop*/
			    cvCvtColor(videoFrame,convFrame,CV_RGB2GRAY);
   		
    			    while ( key != 'q') { 
			          /* Time to test and benchmark DSP based sobel filter */
			          Time_reset(&sTime);
			          Time_delta(&sTime,&time);
				  cvSobel(convFrame,convOpencvFrame,1,1,3);	
				 		  
				  /* get next frame */
			          videoFrame = cvQueryFrame( capture); 
			          if ( !videoFrame) {
	         	             printf("***The End***\n");
	           	             break;
      	      	                  }
				  
				  /* Start RGB To Y conversion of next frame */
			          cvCvtColor(videoFrame,convFrame,CV_RGB2GRAY);
				  
				  /* Convert signed image to unsign image for better clarity */
				  cvConvert(convOpencvFrame,unsignedFrame);

				  /* Display Filtered Image */
				  cvShowImage("video", unsignedFrame);
 				  				        	                  
			          Time_delta(&sTime,&time);
			          printf("Total execution time = %dus\n",(unsigned int)time);			          

	      	                  key = cvWaitKey( 15 );
       	       		    }
			
	       		cvDestroyWindow("video");
    	       		cvReleaseImage(&videoFrame);
    	       		cvReleaseImage(&convFrame);
               		cvReleaseImage(&convOpencvFrame); 
			}
       	       		break; /* End of sobel test */

			default: 
			    printf("Input argument Error:\n Usage :\n./remote_ti_platforms_evm3530_opencv.xv5T s webcam d \n./remote_ti_platforms_evm3530_opencv.xv5T s webcam a\n"); 
		      }
		      break;
  		    
		    default:
		    printf("Input argument Error:\n Usage :\n./remote_ti_platforms_evm3530_opencv.xv5T s tree.avi \n./remote_ti_platforms_evm3530_opencv.xv5T s webcam\n"); 
		    break;
		}
	        break;			
		

	case 'd':
	  switch(*argv[2]) {
	    case 'd': { //'d' dor DSP accelerated
		int row =16;
		int col =16; 
	        floatDataPtr = (float *)cvAlloc(sizeof(float)*row*col);
		floatOutPtr = (float *)cvAlloc(sizeof(float)*row*col);
		dataImage = cvCreateImageHeader(cvSize(col, row), IPL_DEPTH_32F, 2);
		dataImage->imageData = (char *)floatDataPtr;
		integralImage = cvCreateImageHeader(cvSize(col, row), IPL_DEPTH_32F, 2);
		integralImage->imageData = (char *)floatOutPtr;
                
	
		for (i=0; i< row * col * 2; i+=2) {
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
		Time_delta(&sTime,&time);
		DSP_cvSyncDSP();
		
		/* Unnormalize the data */
		cvConvertScale(integralImage,integralImage,32767,0);
		/* As output is scaled down by 4, bring it to unscaled form */
 		cvConvertScale(integralImage,integralImage,4,0);
		
				  
    		
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
		printf("DSP_cvDFT Total execution time = %dus\n",(unsigned int)time);    
 		cvFree(&floatDataPtr);
		cvFree(&floatOutPtr);
		}
		break;
            case 'a': {
		int row =16;
		int col =16; 
	        floatDataPtr = (float *)cvAlloc(sizeof(float)*row*col);
		floatOutPtr = (float *)cvAlloc(sizeof(float)*row*col);
		dataImage = cvCreateImageHeader(cvSize(col, row), IPL_DEPTH_32F, 2);
		dataImage->imageData = (char *)floatDataPtr;
		integralImage = cvCreateImageHeader(cvSize(col, row), IPL_DEPTH_32F, 2);
		integralImage->imageData = (char *)floatOutPtr;
                
	
		for (i=0; i< row * col * 2; i+=2) {
		    tempFloat += 1.0;
		    floatDataPtr[i] = tempFloat;
		    floatDataPtr[i+1] = 0.0;
	        } 
		/* Print the input data for DFT */
		flPtr = (float *)dataImage->imageData;
/*    		printf(" The DFT input is:\n");
    		for (i=0;i<dataImage->height;i++){ 
		    key = 0;
            	    for (j=0;j<dataImage->width * 2;j+=2){
			key++;
                        printf("%f + i%f\t", flPtr[(i*dataImage->width * 2)+j], flPtr[(i*dataImage->width * 2) + j + 1]);
			if ((key % 5) == 0) printf("\n");
            	    }
            	    printf("\n");
    		}
*/		
		Time_reset(&sTime);
		Time_delta(&sTime,&time);		
    		cvDFT(dataImage,integralImage,CV_DXT_FORWARD,0);
				
		Time_delta(&sTime,&time);
				  
    		
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
		printf("cvDFT Total execution time = %dus\n",(unsigned int)time);    
 		cvFree(&floatDataPtr);
		cvFree(&floatOutPtr);
		}
		break;
	    default:
	       printf("Input argument Error:\n Usage :\n./remote_ti_platforms_evm3530_opencv.xv5T d d \n./remote_ti_platforms_evm3530_opencv.xv5T d a\n");
         } // end of latest switch(*argv[3])
         break;
	default:
	      return -1;
    }
       
    DSP_cvEndDSP();
    return 0;

}    
   
