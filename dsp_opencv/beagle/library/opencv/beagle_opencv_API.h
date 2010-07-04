/*
 *  ======== beagle_opencv_API.h ========
 *  
 */


#ifndef _BEAGLE_OPENCV_API_H
#define _BEAGLE_OPENCV_API_H


#include <beagle/algos/opencv/iopencv.h>


typedef IOPENCV_Operation  	  OPENCV_Operation;


typedef IOPENCV_OperationTypes    OPENCV_OperationTypes;
/*
 *  ======== API ========
 *  API prototypes.
 */
int sobelDsp( int , int , char *, int , int , int , int , int , char *, int , int , int , int );

int dftDsp( int , int , char *, int , int , int , int , int , char *, int , int , int );

int integralDsp( int , int , char *, int , int , int , int , int , char *, int , int , int );


#endif // _BEAGLE_OPENCV_API_H
