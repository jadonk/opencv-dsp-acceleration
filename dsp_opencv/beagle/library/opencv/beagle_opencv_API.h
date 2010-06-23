

/*
 *  ======== beagle_opencv_API.h ========
 *  
 */


#ifndef _BEAGLE_OPENCV_API_H
#define _BEAGLE_OPENCV_API_H


#include <beagle/algos/opencv/iopencv.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>


typedef IOPENCV_Operation  	  OPENCV_Operation;


typedef IOPENCV_OperationTypes    OPENCV_OperationTypes;
/*
 *  ======== OPENCVTEST_main ========
 *  
 *  Test Routines Entry Point
 */
void OPENCVTEST_main( int , int , char *, int , int , int , int , int , char *, int , int , int , OPENCV_Operation );


#endif // _BEAGLE_OPENCV_API_H
