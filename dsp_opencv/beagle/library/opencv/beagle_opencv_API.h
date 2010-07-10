/*
 *  ======== beagle_opencv_API.h ========
 *  
 */


#ifndef _BEAGLE_OPENCV_API_H
#define _BEAGLE_OPENCV_API_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum OPENCV_Operation{
        OPERATION_GEN_TWIDDLE = (1 << 0),
        OPERATION_DFT = (1 << 1),
	OPERATION_SOBEL3x3 = (1 << 2),
	OPERATION_SOBEL5x5 = (1 << 3),
	OPERATION_SOBEL7x7 = (1 << 4),
	OPERATION_INTEGRAL = (1 << 5),
        OPERATION_IDFT = (1 << 6),
	OPERATION_DFTROWS = (1 << 7),
	OPERATION_IDFTROWS = (1 << 8),
	OPERATION_SCALE = (1 << 9)
} OPENCV_Operation;

/*
 *  ======== API ========
 *  API prototypes.
 */

void cvStartDSP();

int sobelDsp( int , int , char *, int , int , int , int , int , char *, int , int , int , int );

int dftDsp( int , int , char *, int , int , int , int , int , char *, int , int , int, OPENCV_Operation );

int integralDsp( int , int , char *, int , int , int , int , int , char *, int , int , int );

void cvEndDSP();

#ifdef __cplusplus
}
#endif

#endif // _BEAGLE_OPENCV_API_H
