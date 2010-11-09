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

void DSP_cvMatchTemplate( const CvArr* image, const CvArr* templ, CvArr* result, int method );

void DSP_cvCvtColor( const CvArr* srcarr, CvArr* dstarr, int code ); 

void DSP_cvCvtColor_cvSobel( const CvArr* srcarr, CvArr* dstarr, int code, const void* srcarr_s, void* dstarr_s, int dx, int dy, int aperture_size );

void DSP_cvMulSpectrums( const CvArr* src1, const CvArr* src2, CvArr* dst, int flags );

void DSP_cvNormalize( const CvArr* src, CvArr* dst, double a, double b, int norm_type, const CvArr* mask );

void DSP_cvRectangle( const CvArr* array, CvPoint pt1, CvPoint pt2, CvScalar color, int thickness, int line_type, int shift );

void DSP_cvMinMaxLoc( const CvArr* src, double* min_val, double* max_val, CvPoint* min_loc, CvPoint* max_loc, const CvArr* mask );

void DSP_cvCopy( const CvArr* src, CvArr* dst, const CvArr* mask );

void DSP_cvZero( CvArr* arr );

void DSP_cvSyncDSP( void );

void DSP_cvEndDSP(void);

#ifdef __cplusplus
}
#endif

#endif // _BEAGLE_OPENCV_API_H
