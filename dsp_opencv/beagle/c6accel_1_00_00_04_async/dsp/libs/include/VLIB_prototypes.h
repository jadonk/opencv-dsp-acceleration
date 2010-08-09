/*******************************************************************************
**+--------------------------------------------------------------------------+**
**|                            ****                                          |**
**|                            ****                                          |**
**|                            ******o***                                    |**
**|                      ********_///_****                                   |**
**|                      ***** /_//_/ ****                                   |**
**|                       ** ** (__/ ****                                    |**
**|                           *********                                      |**
**|                            ****                                          |**
**|                            ***                                           |**
**|                                                                          |**
**|         Copyright (c) 2007-2012 Texas Instruments Incorporated           |**
**|                        ALL RIGHTS RESERVED                               |**
**|                                                                          |**
**| Permission to use, copy, modify, or distribute this software,            |**
**| whether in part or in whole, for any purpose is forbidden without        |**
**| a signed licensing agreement and NDA from Texas Instruments              |**
**| Incorporated (TI).                                                       |**
**|                                                                          |**
**| TI makes no representation or warranties with respect to the             |**
**| performance of this computer program, and specifically disclaims         |**
**| any responsibility for any damages, special or consequential,            |**
**| connected with the use of this program.                                  |**
**|                                                                          |**
**+--------------------------------------------------------------------------+**
*******************************************************************************/
 

#ifndef VLIB_PROTO

#define VLIB_PROTO

#ifdef WIN32
#define restrict    
#endif

typedef struct {
	int area;
  	int xsum; 
  	int ysum;

  	int xmin;
  	int ymin;
  	int xmax;
  	int ymax;  
} VLIB_CC;

typedef int VLIB_CCHandle; 

// Background Model Constants
#define IIR_ALPHA_S32         0x00001000    // SQ0.31
#define IIR_ALPHA_S16         0x10       	// SQ0.15
#define CAMERA_NOISE_S32      0x00A00000    // SQ16.15
#define CAMERA_NOISE_S16      0x0A00     	// SQ12.3
#define THRESHOLD_FACTOR_S32  0x31fffff9    // SQ4.27
#define THRESHOLD_FACTOR_S16  0x31ff        // SQ4.11

typedef enum 
{
	VLIB_NO_ERROR = 0,
	VLIB_ERR_INPUT_INVALID = 1,
	VLIB_ERR_INPUT_NEGATIVE = 2,
	VLIB_ERR_INPUT_EXCEEDED_RANGE = 3,
	VLIB_ERR_MEMORY_EXCEEDED_BOUNDARY = 4,
	VLIB_ERR_MEMORY_ALLOCATION_FAILURE = 5,
	VLIB_ERR_MEMORY_POINTER_NULL = 6,
	VLIB_ERR_DMA_FAILURE = 7,
	VLIB_ERR_FILE_OPEN_FAILURE = 8,
	VLIB_ERR_FILE_READ_FAILURE = 9,
	VLIB_ERR_FILE_WRITE_FAILURE = 10,
	VLIB_ERR_FILE_CLOSE_FAILURE = 11,
	VLIB_ERR_FILE_FORMAT_FAILURE = 12,
	VLIB_WARNING_LOW_MEMORY = 13

} VLIB_STATUS;

int VLIB_doublethresholding
(
signed short * restrict pInMag, 
unsigned char *edgeMap, 
unsigned int * restrict strongEdgeListPtr,
int * numStrongEdges,
unsigned short width, 
unsigned short pitch, 
unsigned short height,
unsigned char loThresh,
unsigned char hiThresh,
unsigned int  block_offset
);

int VLIB_edgeRelaxation
(
unsigned char *edgeMap, 
unsigned int * restrict strongEdgeListPtr,
int * numStrongEdges,
unsigned short width
);

int VLIB_houghLineFromList
(
unsigned short * restrict pEdgeMapList, 
unsigned short * restrict pOutHoughSpace, 
unsigned short outBlkWidth,              
unsigned short outBlkHeight,             
unsigned int listSize,
unsigned short thetaRange, 
unsigned short rhoMaxLength,
const signed short *pSIN,
const signed short *pCOS,
unsigned short * restrict ping,
unsigned short * restrict pong,
unsigned short * restrict pang,
unsigned short * restrict peng
);

int VLIB_mixtureOfGaussiansS16
(
const unsigned char* restrict Im, 
short* restrict wt, 
short* restrict mu, 
short* restrict var, 
unsigned char* restrict compNum,
unsigned char* restrict updateMask8,
unsigned int* restrict Im_mask, 
const int im_size, 
const short alpha, 
const short rho, 
const short delta, 
const short T, 
const short ini_wt, 
const short ini_var
);

int VLIB_imagePyramid16
(
unsigned short * restrict pIn, 
unsigned short  inCols,
unsigned short  inRows,
unsigned short* restrict pOut
);

int VLIB_imagePyramid8
(
unsigned char * restrict pIn, 
unsigned short  inCols,
unsigned short  inRows,
unsigned char* restrict pOut
);

int VLIB_integralImage16
(
unsigned short* restrict pIn, 
unsigned short  inCols,
unsigned short  inRows,
unsigned int* restrict pLastLine, 
unsigned int* restrict pOut
);

int VLIB_integralImage8
(
unsigned char* restrict pIn, 
unsigned short  inCols,
unsigned short  inRows,
unsigned int* restrict pLastLine,   
unsigned int* restrict pOut
);

int VLIB_mixtureOfGaussiansS32
(
const unsigned char* restrict Im, 
short* restrict wt, 
int* restrict mu, 
int* restrict var, 
unsigned char* restrict compNum,
unsigned char* restrict updateMask8,	
unsigned int* restrict Im_mask, 
const int im_size, 
const short alpha, 
const int rho, 
const int delta, 
const short T, 
const short ini_wt, 
const int ini_var
);

int VLIB_subtractBackgroundS16
(
unsigned int * restrict output32packed,
const unsigned char * restrict newestData, 
const short * restrict currentMean, 
const short * restrict currentVar, 
const short thresholdGlobal, 
const short thresholdFactor,
const unsigned int frameSize
);

int VLIB_subtractBackgroundS32
(
unsigned int * restrict output32packed, 
const unsigned char * restrict newestData, 
const int * restrict currentMean, 
const int * restrict currentVar, 
const int thresholdGlobal, 
const int thresholdFactor,
const unsigned int frameSize
);

int VLIB_updateEWRMeanS16
(
short * restrict currentMean, 
const unsigned char * restrict newestData, 
const unsigned int * restrict foreground, 
const short weight, 
const unsigned int frameSize
);

int VLIB_updateEWRMeanS32
(
int * currentMean, 
unsigned char * newestData, 
unsigned int * foreground, 
unsigned int weight, 
int frameSize
);

int VLIB_updateEWRVarianceS16
(
short * restrict currentVar, 
const short * restrict currentMean, 
const unsigned char * restrict newestData, 
const unsigned int * restrict foreground32packed, 
const short weight, 
const unsigned int frameSize
);

int VLIB_updateEWRVarianceS32
(
int * restrict currentVar, 
const int * restrict currentMean, 
const unsigned char * restrict newestData, 
const unsigned int * restrict foreground32packed, 
const int weight, 
const unsigned int frameSize
);

int VLIB_updateUWRMeanS16
(
short * restrict updatedMean, 
const short * restrict currentMean, 
const unsigned char * restrict newestData, 
const unsigned char * restrict oldestData, 
const unsigned int * restrict newestForeground32packed, 
const unsigned int * restrict oldestForeground32packed, 
const unsigned int pixelCount, 
const unsigned char frameCount
);

int VLIB_updateUWRVarianceS16
(
short * restrict updatedVar, 
const short * restrict previousVar, 
const short * restrict updatedMean, 
const short * restrict previousMean, 
const unsigned char * restrict newestData, 
const unsigned int * restrict newestForeground32packed, 
const unsigned int * restrict oldestForeground32packed, 
const unsigned int pixelCount, 
const unsigned char frameCount
);


void VLIB_erode_bin_square  
(
const unsigned char *restrict in_data, 
unsigned char       *restrict out_data, 
int cols,
int pitch
);

void VLIB_dilate_bin_cross  
(
    const unsigned char *restrict in_data, 
    unsigned char       *restrict out_data, 
    int cols,
    int pitch
);

void VLIB_dilate_bin_mask  
(
    const unsigned char * in_data, 
    unsigned char       * out_data, 
    const char          * mask, 
    int cols,
    int pitch
);

void VLIB_dilate_bin_square  
(
    const unsigned char *restrict in_data, 
    unsigned char       *restrict out_data, 
    int cols,
    int pitch
);

void VLIB_dilate_bin_square  
(
    const unsigned char *restrict in_data, 
    unsigned char       *restrict out_data, 
    int cols,
    int pitch
);

void VLIB_erode_bin_mask  
(
    const unsigned char * in_data, 
    unsigned char       * out_data, 
    const char          * mask, 
    int cols,
    int pitch
);


int VLIB_nonMaxSuppress_7x7_S16
(
const short * restrict im,
int width, 
int height,
short thresh,
unsigned char * restrict out
);

int VLIB_harrisScore_7x7
(
const short * restrict gradX, 
const short * restrict gradY,	
int width, 
int height, 
short * restrict outm,
short k,
unsigned char * buffer
);

int VLIB_histogram_1D_Init_U8
(
unsigned char * restrict binEdges, 								  	
const int numBins,
unsigned char * restrict histArray
);

int VLIB_histogram_1D_Init_U16
(
unsigned short * restrict binEdges, 								  	
const int numBins,
unsigned short * restrict histArray 
);

int VLIB_nonMaxSuppress_3x3_S16
(
const short * restrict im, 
int width, 
int height,
short thresh,
unsigned char * restrict out
);


int VLIB_weightedHistogram_1D_U8
(
unsigned char* restrict X, 
const int numX,								   			 
const int numBins,
unsigned short* restrict binWeight,
unsigned char* restrict histArray,
unsigned short* restrict H1,
unsigned short* restrict H2,
unsigned short* restrict H3,
unsigned short* restrict H
);

int VLIB_weightedHistogram_1D_U16_c
(
unsigned short* restrict X, 
const int numX,								   			 
const int numBins,
unsigned short* restrict binWeight,
unsigned short* restrict histArray,
unsigned short* restrict H1,
unsigned short* restrict H
);

int VLIB_weightedHistogram_nD_U16
(
unsigned short* restrict X, 
const int numX,
const int dimX,
unsigned short* binWeight,	
unsigned short* restrict numBins,
unsigned short* restrict normVals,
unsigned short* restrict scrap,
unsigned short* H1,
unsigned short* H
);

int VLIB_histogram_1D_U8
( 
unsigned char* restrict X, 
const int numX,								   			 
const int numBins,
const unsigned short binWeight,
unsigned char* restrict histArray,
unsigned short* restrict H1,
unsigned short* restrict H2,
unsigned short* restrict H3,
unsigned short* restrict H
);

int VLIB_histogram_1D_U16
(
unsigned short* restrict X, 
const int numX,								   			 
const int numBins,
const unsigned short binWeight,
unsigned short* restrict histArray,
unsigned short* restrict H1,
unsigned short* restrict H
);
	

int VLIB_histogram_nD_U16
(
unsigned short* restrict X, 
const int numX,
const int dimX,
unsigned short binWeight,	
unsigned short* restrict numBins,
unsigned short* restrict normVals,
unsigned short* restrict scrap,
unsigned short* H1,
unsigned short* H
);

int VLIB_recursiveFilterHoriz1stOrder
(
unsigned char * out, 
const unsigned char * in, 
const int width,
const int height,
const short weight,
const unsigned char * boundaryLeft,
const unsigned char * boundaryRight,
unsigned char * scratch
);

int VLIB_recursiveFilterVert1stOrder
(
unsigned char *out, 
const unsigned char *in, 
const int width,
const int height,
const short weight,
const unsigned char * boundaryTop,
const unsigned char * boundaryBottom,
unsigned char *scratch
);

int VLIB_trackFeaturesLucasKanade_7x7
(
const unsigned char * restrict im1,
const unsigned char * restrict im2,
const short * restrict gradX,
const short * restrict gradY,
int width, 
int height,	
int nfeatures,
short * restrict x,
short * restrict y,
short * outx,
short * outy,
int max_iters,
const unsigned char * restrict scratch
);

int VLIB_nonMaxSuppress_5x5_S16
(
const short * restrict im, 
int width, 
int height,
short thresh,
unsigned char * restrict out
);

int VLIB_initMeanWithLumaS16
(
short * restrict runningMean,
const unsigned char * restrict lumaFrame, 
const unsigned int pixelCount
);

int VLIB_initMeanWithLumaS32
(
int * restrict runningMean,
const unsigned char * restrict lumaFrame, 
const unsigned int pixelCount
);


int VLIB_initVarWithConstS16
(
short * restrict runningVar, 
const short constVar, 
const unsigned int pixelCount
);

int VLIB_initVarWithConstS32
(
int * restrict runningVar, 
const int constVar, 
const unsigned int pixelCount
);

int VLIB_gradientV5x5PyramidKernel_8
(
unsigned char* restrict pIn,
short* restrict pB,     
unsigned short  width,      
unsigned short  pitch,      
unsigned short  height, 
unsigned char* restrict pOut
);  

int VLIB_gradientH5x5PyramidKernel_8
(
unsigned char* restrict pIn,  
short* restrict pB,     
unsigned short  width,        
unsigned short  pitch, 
unsigned short  height,        
unsigned char* restrict pOut
);

int VLIB_gauss5x5PyramidKernel_16
( 
unsigned short* restrict pIn,  
unsigned int* restrict pB,     
unsigned short  width,         
unsigned short  pitch,
unsigned short  height,          
unsigned short* restrict pOut
);

int VLIB_gauss5x5PyramidKernel_8
(
unsigned char* restrict pIn, 
unsigned short* restrict pB, 
unsigned short  width,       
unsigned short  pitch,   
unsigned short  height,     
unsigned char* restrict pOut
);


int VLIB_createCCMap8Bit
(
VLIB_CCHandle * handle, 
unsigned char * restrict pOutMap,
const unsigned short  outCols,                        
const unsigned short  outRows
);

int VLIB_getNumCCs
(
VLIB_CCHandle * handle, 
int *numCCs
);

int VLIB_getCCFeatures
(
VLIB_CCHandle * handle, 
VLIB_CC *cc,
short listIndex
);

int VLIB_createConnectedComponentsList
(
VLIB_CCHandle * handle, 
unsigned short  width,
unsigned short  rowsInImg,
int *p32BitPackedFGMask,
int  minBlobArea,
int  connected8Flag
);

int VLIB_initConnectedComponentsList
(
VLIB_CCHandle * handle,
void *pBuf, 
int  bytes
);

int
VLIB_calcConnectedComponentsMaxBufferSize
(
short imgWidth, 
short imgHeight,	
int minArea,
int *maxBytesRequired
);

int 
VLIB_GetSizeOfCCHandle
(
void
);

int VLIB_xyGradientsAndMagnitude
(
unsigned char * restrict pInBlk, 
signed short * restrict pBufGradX,
signed short * restrict pBufGradY,
signed short * restrict pBufMag,
unsigned short width, 
unsigned short height
);

int VLIB_nonMaximumSuppressionCanny(short * restrict pInMag, 
	                            short * restrict pInGradX, 
	                            short * restrict pInGradY, 
	                            unsigned char * restrict pOut,
	                            unsigned short width,
                                unsigned short pitch,
	                            unsigned short height) ;


void VLIB_bhattacharyaDistance_U32
(
unsigned short* restrict X, 
unsigned short* restrict Y, 
int N,
unsigned int* D
);


#define mD_2x4 (2)
#define sD_2x4 (4)

typedef struct VLIB_kalmanFilter_2x4{


	short transition[sD_2x4*sD_2x4];	// QS15 
	// state transition matrix, A

	short errorCov[sD_2x4*sD_2x4];	// QS15
	// a priori error covariance matrix, P

	short predictedErrorCov[sD_2x4*sD_2x4];	// QS15 
	// predicted error covariance matrix, P1
				 	
	short state[sD_2x4];	// QS10.5
	// state of the process, X

	short predictedState[sD_2x4];	// QS10.5
	// predicted state of the process, X1

	short measurement[mD_2x4*sD_2x4];	// QS16 (U8)
	// measurement matrix (relating state to measurement), H

	short processNoiseCov[sD_2x4*sD_2x4];	// QS13.2
	// process noise covariance matrix, Q

	short measurementNoiseCov[mD_2x4*mD_2x4];	// QS16
	// measurement noise covariance matrix, R

	short kalmanGain[sD_2x4*mD_2x4];	// QS0.15
	// Kalman gain, K

	short temp1[sD_2x4*sD_2x4];
	short temp2[sD_2x4*sD_2x4];
	short temp3[sD_2x4*sD_2x4];


} VLIB_kalmanFilter_2x4;


void VLIB_kalmanFilter_2x4_Predict
(
VLIB_kalmanFilter_2x4 * KF
);

void VLIB_kalmanFilter_2x4_Correct
(
VLIB_kalmanFilter_2x4 * KF, 
short * restrict Z, 
short * restrict Residual
);

#define mD_4x6 (4)
#define sD_4x6 (6)

typedef struct VLIB_kalmanFilter_4x6{


	short transition[sD_4x6*sD_4x6];	
	// state transition matrix, A

	short errorCov[sD_4x6*sD_4x6];	
	// a priori error covariance matrix, P

	short predictedErrorCov[sD_4x6*sD_4x6];	
	// predicted error covariance matrix, P1
				 	
	short state[sD_4x6];	
	// state of the process, X

	short predictedState[sD_4x6];	
	// predicted state of the process, X1

	short measurement[mD_4x6*sD_4x6];	
	// measurement matrix (relating state to measurement), H

	short processNoiseCov[sD_4x6*sD_4x6];	
	// process noise covariance matrix, Q

	short measurementNoiseCov[mD_4x6*mD_4x6];	
	// measurement noise covariance matrix, R

	short kalmanGain[sD_4x6*mD_4x6];	
	// Kalman gain, K

	short temp1[sD_4x6*sD_4x6];
	short temp2[sD_4x6*sD_4x6];
	short temp3[sD_4x6*sD_4x6];
	int tempBuffers[mD_4x6*mD_4x6*2];

	int scaleFactor;
	// Scales the matrix M = (H*P1*H' + R) to ensure that its inverse
	//	does not overflow 32 bits.
	//	The scaling is done by right shifting each element of M by the 
	//	quantity assigned to scaleFactor.
	//	The computed inverse is then scaled back to ensure the correct result based on
	//  the identity inv(M) = inv(M/k)/k

} VLIB_kalmanFilter_4x6;

#pragma STRUCT_ALIGN(VLIB_kalmanFilter_4x6, 64);

void matrixInverse_4x4
(
short* output, 
short* source, 
int* buffer, 
short factor
);

void VLIB_kalmanFilter_4x6_Correct
(
VLIB_kalmanFilter_4x6 * KF, 
short * restrict Z, 
short * restrict Residual
);
	
void VLIB_kalmanFilter_4x6_Predict
(
VLIB_kalmanFilter_4x6 * KF
);	

int VLIB_convertUYVYint_to_HSLpl
(
const unsigned char *yc,      
int width,                    
int pitch,                    
int height,
const short coeff[5],         
const unsigned short div_table[510],
unsigned short *restrict H,   
unsigned char  *restrict S,   
unsigned char  *restrict L    
);

int VLIB_convertUYVYint_to_LABpl
(
const unsigned char *yc,      
int width,                    
int pitch,                    
int height,
const short coeff[5],         
float whitePoint[3],          	
float  *restrict L,     
float  *restrict a,     
float  *restrict b      
);


int VLIB_convertUYVYint_to_RGBpl
(
const unsigned char *yc,     
int width,                   
int pitch,                   
int height,
const short coeff[5],        
unsigned char *restrict r,   
unsigned char *restrict g,   
unsigned char *restrict b    
);

int VLIB_convertUYVYint_to_YUVpl
(
const unsigned char *yc,      
int width,                    
int pitch,                   
int height,                  
unsigned char *restrict y,   
unsigned char *restrict cr,  
unsigned char *restrict cb   
);

int VLIB_extractLumaFromUYUV
(
unsigned char* restrict pInUYVY, 
unsigned short  inCols,
unsigned short  inPitch,
unsigned short  inRows,
unsigned char* restrict pOutY
);

int VLIB_convertUYVYsemipl_to_YUVpl
(
const unsigned char *crcb,   
int width,                   
int pitch,                   
int height,                  
unsigned char *restrict cr,  
unsigned char *restrict cb   
);

int VLIB_initUYVYint_to_LABpl_LUT
(
const int d,                 
const short coeff[5],        
const float whitePoint[3],   
unsigned short *Lab          
);


int VLIB_convertUYVYint_to_LABpl_LUT
(
unsigned char * restrict yc,  
int width,                    
int pitch,                    
int height,
int d,
unsigned short * restrict LabExt,           
unsigned short * restrict L,
unsigned short * restrict a,
unsigned short * restrict b
);

int VLIB_convertUYVYpl_to_YUVint
(
const unsigned char *restrict y,    
const unsigned char *restrict cr,   
const unsigned char *restrict cb,   
int width,                          
int pitch,                    
int height,                   
unsigned char *restrict yc          
);


void VLIB_simplex_3D
(
int (*func)(short[], void *), 
short* restrict start,			
short* restrict init_step,		
int MaxIteration,
int EPSILON, 					
short* restrict v,				
int* restrict f,				
short* restrict vr,				
short* restrict ve,				
short* restrict vc,				
short* restrict vm,				
void* addtlArgs,				
short* restrict minPoint,		
int* restrict minValue		   
);
                               
void VLIB_simplex
(
int (*func)(short[], void *), 
short* restrict start,			
short* restrict init_step,		
int N, 						
short N_INV,					
int MaxIteration,
int EPSILON, 					
short* restrict v,				
int* restrict f,				
short* restrict vr,				
short* restrict ve,				
short* restrict vc,				
short* restrict vm,				
void* addtlArgs,				
short* restrict minPoint,		
int* restrict minValue			   
);

void VLIB_legendreMoments
(
const short* restrict Im, 
short* restrict LMoments, 
short* restrict LPoly,  
const int Order,  
const int ImH,
const short Factor1
);

void VLIB_legendreMoments_Init
(
short* LPoly, 
const int Order, 
const int ImH, 
short* Factor
);

void VLIB_normalFlow_16
(
short * imDiff,		
short * Emag, 		
short * Ex,			
short * Ey,			
short * LUT,		
short T,			
int numEls,		
short * normalFlowU,
short * normalFlowV
);

int VLIB_recursiveFilterHoriz1stOrderS16
(
short * out, 
const short * in, 
const int width,
const int height,
const short weight,
const short * boundaryLeft,
const short * boundaryRight,
short * scratch
);
						   				   
int VLIB_recursiveFilterVert1stOrderS16
(
short *out, 
const short *in, 
const int width,
const int height,
const short weight,
const short * boundaryTop,
const short * boundaryBottom,
short * scratch
);
						   		 
int VLIB_extract8bitBackgroundS16
(
const short * restrict BGmodel, 
unsigned char * restrict BGimage,
const unsigned int pixelCount
);
							    
int VLIB_unpackMask32
(
const unsigned int * restrict mask32packed, 
unsigned char * restrict maskImage,
const unsigned int pixelCount
);
					
int VLIB_packMask32
(
const unsigned char * restrict maskImage,
unsigned int * restrict mask32packed, 
const unsigned int pixelCount
);
			  	  
int VLIB_L1DistanceS16
( 
short* restrict X,
short* restrict Y, 
int d,
unsigned int* restrict L1D 
);

int VLIB_disparity_SAD_firstRow
(
const unsigned char *restrict pLeft,
const unsigned char *restrict pRight,
unsigned short *restrict pCost, 
unsigned short *restrict pMinCost, 
unsigned char *restrict pScratch, 
char *restrict pDisparity,  
int displacement, 
int width, 
int pitch,
int windowSize
);

int VLIB_disparity_SAD
(
const unsigned char *restrict pLeft,
const unsigned char *restrict pRight,
unsigned short *restrict pCost, 
unsigned short *restrict pMinCost, 
char *restrict pDisparity,  
int displacement, 
int width, 
int pitch,
int windowSize
);

int VLIB_disparity_SAD16
(
const unsigned short *restrict pLeft,
const unsigned short *restrict pRight,
unsigned short *restrict pCost, 
unsigned short *restrict pMinCost, 
char *restrict pDisparity,  
int displacement, 
int width, 
int pitch,
int windowSize
);

int VLIB_disparity_SAD_firstRow16
(
const unsigned short *restrict pLeft,
const unsigned short *restrict pRight,
unsigned short *restrict pCost, 
unsigned short *restrict minCost, 
unsigned short *restrict pScratch, 
char *restrict pDisparity,  
int displacement, 
int width, 
int pitch,
int windowSize
);

#endif


