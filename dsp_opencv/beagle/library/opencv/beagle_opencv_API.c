/*
 *  ======== beagle_opencv_API.c ========
 *  application for opecvDsp_API
 */
#include <xdc/std.h>
#include <beagle/algos/opencv/iopencv.h>
//#include <beagle/algos/opencv/opencv_beagle.h>
#include "beagle_opencv_API.h"
#include "opencvDsp.h"

/* Standard Linux headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <opencv/cv.h>


/*
 *  ======== sobelDsp ========
 */

void cvStartDSP()
{   int status;
    status = cvDspInit();
    if (!status) {
       	printf("Error: DSP initilaization Failed. \n");
    }
}

int sobelDsp( int s_type, int s_step, char *s_data, int s_rows, int s_cols, int s_channels, int d_type, int d_step, char *d_data, int d_rows, int d_cols, int d_channels, int apertureSize )
{
    OPENCV_Operation operation;
//    int i;
    int status;

    if (apertureSize < 3 || apertureSize > 7 || (apertureSize % 2 == 0) ) {
	printf( "\n Error: Aperture Size %d not supported.\n",apertureSize);
	return 0;
    }

    if (apertureSize == 3) {
       operation = OPENCV_OPERATION_SOBEL3x3;
    }
    else if (apertureSize == 5) {
       operation = OPENCV_OPERATION_SOBEL5x5;
    }
    else {
       operation = OPENCV_OPERATION_SOBEL7x7;
    }

    if (s_type != CV_8UC1 && s_type != CV_8SC1) {
        printf ("Error: Currently only image type of CV_8UC1 and CV_8SC1 are supported.\n");
        return 0;
    }

//    XDAS_Int16 *tempBuf = (XDAS_Int16 *)malloc(s_rows*s_cols*2);
//    for (i=0; i<(s_cols*s_rows); i++) {
// 	tempBuf[i] = (XDAS_Int16)s_data[i];
// 	}
    /* Set parameters to be passed to DSP */
    opencvDspSetParams( s_type, s_step, s_data, s_rows, s_cols, s_channels, d_type, d_step, d_data, d_rows, d_cols, d_channels );
//    opencvDspSetParams( s_type, s_step, tempBuf, s_rows, s_cols, s_channels, d_type, d_step, d_data, d_rows, d_cols, d_channels );

    status = cvCreateEngine(operation);
    if (!status) 
        return 0;
    else
        return (-1);
 
}




/*
 *  ======== dftDsp ========
 */
int dftDsp( int s_type, int s_step, char *s_data, int s_rows, int s_cols, int s_channels, int d_type, int d_step, char *d_data, int d_rows, int d_cols, int d_channels,OPENCV_Operation operationType )
{   int i,dataLen;
    float *flPtr;
    short *shPtr;
    OPENCV_Operation operation = operationType; //OPENCV_OPERATION_DFT;

    /* For CCS Debug */
//    printf( "Enter:\n");
//    getchar();

    if (s_type != CV_32FC2 && s_type != CV_8UC2) {
        printf ("Error: Currently only image type of CV_8UC2 and CV_32FC2 are supported.\n");
        return 0;
    }
    dataLen = s_rows*s_cols*s_channels;
    flPtr = (float *)s_data;
    for(i=0; i< dataLen; i++) {
	flPtr[i] = flPtr[i] / 32767;
    }
    
    
    /* Set parameters to be passed to DSP */
    opencvDspSetParams( s_type, s_step, s_data, s_rows, s_cols, s_channels, d_type, d_step, d_data, d_rows, d_cols, d_channels );


    cvCreateEngine(operation);

    
    flPtr = (float *)d_data;
        
    for(i=0; i< dataLen; i++) {
	flPtr[i] = flPtr[i] * 32767;
    }
    return (-1);
}

/*
 *  ======== integralDsp ========
 */
int integralDsp( int s_type, int s_step, char *s_data, int s_rows, int s_cols, int s_channels, int d_type, int d_step, char *d_data, int d_rows, int d_cols, int d_channels )
{
    OPENCV_Operation operation = OPENCV_OPERATION_INTEGRAL;

    if ((s_type & 07) > 07 || (s_type & 07) == 2 || (s_type & 07) == 3) {
        printf ("Error: Image Type not supported supported.\n");
        return 0;
    }

    if (d_rows != s_rows + 1 || d_cols != s_cols + 1 ) {
	printf ("Error: Destination matrix or image size must be 1 unit larger in both dimension than source matrix or image.\n");
	return 0;
    }
    /* Set parameters to be passed to DSP */
    opencvDspSetParams( s_type, s_step, s_data, s_rows, s_cols, s_channels, d_type, d_step, d_data, d_rows, d_cols, d_channels );


    cvCreateEngine(operation);
    return (-1);
}

