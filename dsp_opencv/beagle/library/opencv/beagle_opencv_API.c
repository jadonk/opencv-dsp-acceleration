/*
 *  ======== beagle_opencv_API.c ========
 *  application for opecvDsp_API
 */
#include <xdc/std.h>
#include "beagle_opencv_API.h"
#include "opencvDsp.h"
#include "iopencvDsp.h"

/* Standard Linux headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <opencv/cv.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== cvStartDSP ========
 */
void DSP_cvStartDSP()
{   int status;
    status = DSP_cvStartDSPProcess();
    if (status == OPENCVDSP_ERR) {
       	printf("Error: DSP initilaization (DSP_cvStartDSPProcess) Failed. \n");
    }
}

/*
 *  ======== cvDFT ========
 */
void DSP_cvSobel( const CvArr* srcarr, CvArr* dstarr, int dx, int dy, int aperture_size ) 
{
    cv::Mat src = cv::cvarrToMat(srcarr), dst = cv::cvarrToMat(dstarr);

 
    CV_Assert( src.size() == dst.size() && src.channels() == dst.channels() &&
        ((src.depth() == CV_8U && (dst.depth() == CV_16S || dst.depth() == CV_32F)) ||
        (src.depth() == CV_32F && dst.depth() == CV_32F)));
    

    if (aperture_size < 3 || aperture_size > 7 || (aperture_size % 2 == 0) ) {
	printf( "\n Error: Aperture Size %d not supported.\n",aperture_size);
	return;
    }

    if(dx != dy && dx != 1) {
	printf("Error: Only 1st order sobel operator is supported. Set dx=1 and dy=1.\n");
	return;
    }
    if(aperture_size == CV_SCHARR) {
	printf("Error: Scharr Filter: Not supported.\n");
	return;
    }
    
    int status;
    DSP_Mat src1;
    DSP_Mat dst1;
    DSP_Flags flags;

    src1 = mat_OpenCV2DSP(src);
    dst1 = mat_OpenCV2DSP(dst);
    flags = FLAGS3_OpenCV2DSP(dx,dy,aperture_size);

    status = DSP_cvSobelProcess(&src1, &dst1, &flags);

    if (status == OPENCVDSP_ERR) 
        printf("Error: DSP Process (DSP_cvSobelProcess) failed. \n");


}




/*
 *  ======== cvDFT ========
 */
void DSP_cvDFT( const CvArr* srcarr, CvArr* dstarr, int flags, int nonzero_rows )
{   
    cv::Mat src = cv::cvarrToMat(srcarr), dst0 = cv::cvarrToMat(dstarr), dst = dst0;
    
    CV_Assert( src.size() == dst.size() );

    int status;
    IplImage *src1;
    IplImage *dst1;
    DSP_Flags _flags;

    src1 = (IplImage *)srcarr;
    
    dst1 = (IplImage *)dstarr;
    
    
    _flags = FLAGS2_OpenCV2DSP(flags, nonzero_rows);
   

    
    status = DSP_cvDFTProcess(src1, dst1, &_flags);
    if (status == OPENCVDSP_ERR) {
        printf("Error: DSP Process (DSP_cvDFTProcess) failed. \n");
	return;
    }
  
    
}

/*
 *  ======== cvMatchTemplate ========
 */

void DSP_cvMatchTemplate( const CvArr* image, const CvArr* templ,
            CvArr* result, int method )
{
           
    int status;
        
    status = DSP_cvMatchTemplateProcess( (void *)image, (void *)templ, result , method );
	    
    if (status == OPENCVDSP_ERR) 
        printf("Error: DSP Process (DSP_cvMatchTemplateProcess) failed. \n");

}

/*
 *  ======== cvIntegral ========
 */

void DSP_cvIntegral( const CvArr* image, CvArr* sumImage,
            CvArr* sumSqImage, CvArr* tiltedSumImage )
{
           
    int status;
        
    status = DSP_cvIntegralProcess( (void *)image, sumImage, sumSqImage , tiltedSumImage );
	    
    if (status == OPENCVDSP_ERR) 
        printf("Error: DSP Process (DSP_cvIntegralProcess) failed. \n");

}


/*
 *  ======== cvIntegral ========
 */
/*
void DSP_cvIntegral( const CvArr* image, CvArr* sumImage,
            CvArr* sumSqImage, CvArr* tiltedSumImage )
{
    cv::Mat src = cv::cvarrToMat(image), sum = cv::cvarrToMat(sumImage), sum0 = sum;
    cv::Mat sqsum0, sqsum, tilted0, tilted;
    cv::Mat *psqsum = 0, *ptilted = 0;

    if( sumSqImage )
    {   printf("Error: Square sum is not supported.\n");
	return;
        sqsum0 = sqsum = cv::cvarrToMat(sumSqImage);
        psqsum = &sqsum;
    }

    if( tiltedSumImage )
    {	printf("Error: Tilted sum is not supported.\n");
	return;
        tilted0 = tilted = cv::cvarrToMat(tiltedSumImage);
        ptilted = &tilted;
    }
    

    if (sum.rows != src.rows + 1 || sum.cols != src.cols + 1 ) {
	printf ("Error: Destination matrix or image size must be 1 unit larger in both dimension than source matrix or image.\n");
	return;
    }
    
    int status;
    DSP_Mat src_;
    DSP_Mat sum_;
    DSP_Mat sqsum_;
    DSP_Mat tilted_;
    
    src_ = mat_OpenCV2DSP(src);
    sum_ = mat_OpenCV2DSP(sum);
    if(sumSqImage)        
        sqsum_ = mat_OpenCV2DSP(sqsum);
    if(tiltedSumImage)      	
       tilted_ = mat_OpenCV2DSP(tilted);        
    
#if 0 
    // Normalize if float 
    if(src.depth() == CV_32F) {            
        float *flPtr = (float *)src1.data;
        while(flPtr < (float *)src1.dataend) {
	    *flPtr = *flPtr / 32767;
            flPtr++;
        }
    }
#endif

    status = DSP_cvIntegralProcess( &src_, &sum_, sumSqImage ? &sqsum_:NULL, tiltedSumImage ? &tilted_:NULL);
	    
    if (status == OPENCVDSP_ERR) 
        printf("Error: DSP Process failed. \n");

#if 0 
    // unnormalize 
    if(sum.depth() == CV_32F) {
        float *flPtr = (float *)dst1.data;
        while(flPtr < (float *)dst1.dataend) {
	    *flPtr = *flPtr / 32767;
            flPtr++;
        }
    }
#endif

}
*/

/*
 *  ======== cvCvtColor ========
 */
     //This segment does not uses opecvlibrary on DSP
void DSP_CvtColor( const CvArr* srcarr, CvArr* dstarr, int code )
{
    cv::Mat src = cv::cvarrToMat(srcarr);
    cv::Mat dst = cv::cvarrToMat(dstarr);
    
    int status;
    DSP_Mat src1;
    DSP_Mat dst1;
    DSP_Flags flags;
    
    src1 = mat_OpenCV2DSP(src);
    dst1 = mat_OpenCV2DSP(dst);
    flags = FLAGS1_OpenCV2DSP(code);    
    
    switch( code ) {
	//case CV_BGR2GRAY:
        //case CV_BGRA2GRAY:
    	case CV_RGB2GRAY:
    	//case CV_RGBA2GRAY:
            if( ((MAT_CN(src.flags) != 3) && (MAT_CN(src.flags) != 4)) || (MAT_CN(dst.flags) != 1) )
                CV_Error( CV_BadNumChannels,"Incorrect number of channels for this conversion code" );

            status = DSP_CvtColorProcess(&src1, &dst1, &flags);
            break;
	default:
	    printf("DSP: Color conversion code NOT SUPPORTED.\n");
    }
    
    if(status == OPENCVDSP_ERR)
	printf("Error: DSP Process (CvtColor) failed. \n");
	
}


void DSP_cvCvtColor( const CvArr* srcarr, CvArr* dstarr, int code )
{        
    int status;
      
    status = DSP_cvCvtColorProcess((void *)srcarr, dstarr, code);
        
    if(status == OPENCVDSP_ERR)
	printf("Error: DSP Process (DSP_cvCvtColorProcess) failed. \n");
	
}

/* ****** cvMulSpectrums()**************************/

void DSP_cvMulSpectrums( const CvArr* src1, const CvArr* src2, CvArr* dst, int flags )
{        
    int status;    
           
    status = DSP_cvMulSpectrumsProcess((void *)src1, (void *)src2, dst, flags);
        
    if(status == OPENCVDSP_ERR)
	printf("Error: DSP Process (DSP_cvMulSpectrumsProcess) failed. \n");
	
}

/******** cvNormalize()**************************/

void DSP_cvNormalize( const CvArr* src, CvArr* dst, double a, double b, int norm_type, const CvArr* mask )
{        
    int status;    
           
    status = DSP_cvNormalizeProcess((void *)src, dst, a, b, norm_type, (void *)mask);
        
    if(status == OPENCVDSP_ERR)
	printf("Error: DSP Process (cvNormalize) failed. \n");
	
}

/******** cvRectangle()**************************/

void DSP_cvRectangle( const CvArr* array, CvPoint pt1, CvPoint pt2, CvScalar color, int thickness, int line_type, int shift )
{        
    int status;    
           
    status = DSP_cvRectangleProcess(array, pt1, pt2, color, thickness, line_type, shift);
        
    if(status == OPENCVDSP_ERR)
	printf("Error: DSP Process (cvRectangle) failed. \n");
	
}

/******** cvMinMaxLoc()**************************/

void DSP_cvMinMaxLoc( const CvArr* src, double* min_val, double* max_val, CvPoint* min_loc, CvPoint* max_loc, const CvArr* mask )
{
        
    int status;    
           
    status = DSP_cvMinMaxLocProcess((void *)src, min_val, max_val, min_loc, max_loc, (void *)mask);
        
    if(status == OPENCVDSP_ERR)
	printf("Error: DSP Process (cvMinMaxLoc) failed. \n");
	
}

/******** cvCopy()**************************/

void DSP_cvCopy( const CvArr* src, CvArr* dst, const CvArr* mask )
{        
    int status;    
           
    status = DSP_cvCopyProcess((void *)src, dst, (void *)mask);
        
    if(status == OPENCVDSP_ERR)
	printf("Error: DSP Process (cvCopy)failed. \n");
	
}

/******** cvZero()**************************/

void DSP_cvZero( CvArr* arr )
{        
    int status;    
           
    status = DSP_cvZeroProcess( arr );
        
    if(status == OPENCVDSP_ERR)
	printf("Error: DSP Process failed. \n");
	
}
/*
 *  ======== DSP_cvCvtColor_cvSobel ========
 */
void DSP_cvCvtColor_cvSobel( const CvArr* srcarr, CvArr* dstarr, int code, const void* srcarr_s, void* dstarr_s, int dx, int dy, int aperture_size )
{   
    /* convert CvArr to Mat type */
    cv::Mat src = cv::cvarrToMat(srcarr);
    cv::Mat dst = cv::cvarrToMat(dstarr);

    cv::Mat src_s = cv::cvarrToMat(srcarr_s), dst_s = cv::cvarrToMat(dstarr_s);

    /* Check input image type */
    CV_Assert( src_s.size() == dst_s.size() && src_s.channels() == dst_s.channels() &&
        ((src_s.depth() == CV_8U && (dst_s.depth() == CV_16S || dst_s.depth() == CV_32F)) ||
        (src_s.depth() == CV_32F && dst_s.depth() == CV_32F)));
    
    /* Data structure for CvtColor*/
    int status;
    DSP_Mat src1;
    DSP_Mat dst1;
    DSP_Flags flags;

    /* Data structure for Sobel*/
    DSP_Mat src1_s;
    DSP_Mat dst1_s;
    DSP_Flags flags_s;
    
    /* convert OpenCV data structure to OpenCVDSP structure */
    src1 = mat_OpenCV2DSP(src);
    dst1 = mat_OpenCV2DSP(dst);
    flags = FLAGS1_OpenCV2DSP(code);

    src1_s = mat_OpenCV2DSP(src_s);
    dst1_s = mat_OpenCV2DSP(dst_s);
    flags_s = FLAGS3_OpenCV2DSP(dx,dy,aperture_size); 
    
    
    if(code != CV_RGB2GRAY) {
	printf("Error: Currently CV_RGB2GRAY is only supported. \n");
	return;
    }

    if (aperture_size < 3 || aperture_size > 7 || (aperture_size % 2 == 0) ) {
	printf( "Error: Aperture Size %d not supported.\n",aperture_size);
	return;
    }

    if(dx != dy && dx != 1) {
	printf("Error: Only 1st order sobel operator is supported. Set dx=1 and dy=1.\n");
	return;
    }
    if(aperture_size == CV_SCHARR) {
	printf("Error: Scharr Filter: Not supported.\n");
	return;
    }
    /* call DSP side Process */
    status = DSP_cvCvtColor_cvSobelProcess(&src1, &dst1, &flags, &src1_s, &dst1_s, &flags_s);
    if (status == OPENCVDSP_ERR) 
        printf("Error: DSP Process failed. \n");
    
}
/*
 * ======cvSyncDSP=======
 *
 */
void DSP_cvSyncDSP( )
{
    DSP_cvSyncDSPProcess();
}

/*
 *  ======== cvEndDSP ========
 */
void DSP_cvEndDSP()
{
    DSP_cvEndDSPProcess();
}

#ifdef __cplusplus
}
#endif
