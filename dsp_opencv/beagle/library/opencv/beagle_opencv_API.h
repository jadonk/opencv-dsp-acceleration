/*
 *  ======== beagle_opencv_API.h ========
 *  
 */


#ifndef _BEAGLE_OPENCV_API_H
#define _BEAGLE_OPENCV_API_H

#include <opencv/cv.h>

#ifdef __cplusplus
extern "C" {
#endif


/*
 *  ======== API ========
 *  API prototypes.
 */
void DSP_cvStartDSP( void );

void DSP_cvSobel( const CvArr* srcarr, CvArr* dstarr, int dx, int dy, int aperture_size ); 

void DSP_cvDFT( const CvArr* srcarr, CvArr* dstarr, int flags, int nonzero_rows );

void DSP_cvIntegral( const CvArr* image, CvArr* sumImage, CvArr* sumSqImage, CvArr* tiltedSumImage );

void DSP_cvCvtColor( const CvArr* srcarr, CvArr* dstarr, int code );

void DSP_cvCvtColor_cvSobel( const CvArr* srcarr, CvArr* dstarr, int code, const void* srcarr_s, void* dstarr_s, int dx, int dy, int aperture_size );

void DSP_cvSyncDSP( void );

void DSP_cvEndDSP(void);

#ifdef __cplusplus
}
#endif

#endif // _BEAGLE_OPENCV_API_H
