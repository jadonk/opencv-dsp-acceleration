/*
 *  ======== beagle_opencv_API.c ========
 *  application for opecvDsp_API
 */
#include <xdc/std.h>
#include <beagle/algos/opencv/iopencv.h>
#include <beagle/algos/opencv/opencv_beagle.h>

#include "beagle_opencv_API.h"
#include "opencvDsp.h"

/* Standard Linux headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <opencv/cv.h>


/*
 *  ======== sobelDsp3x3 ========
 */
void sobelDsp( int s_type, int s_step, char *s_data, int s_rows, int s_cols, int s_channels, int d_type, int d_step, char *d_data, int d_rows, int d_cols, int d_channels, int apertureSize )
{
    OPENCV_Operation operation;

    if (apertureSize < 3 || apertureSize > 7 || (apertureSize % 2 == 0) ) {
	printf( "\n Error: Aperture Size %d not supported.\n",apertureSize);
	return;
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
        return;
    }

    /* Set parameters to be passed to DSP */
    opencvDspSetParams( s_type, s_step, s_data, s_rows, s_cols, s_channels, d_type, d_step, d_data, d_rows, d_cols, d_channels );

    opencvDspCeInit(operation);
}




/*
 *  ======== dftDsp ========
 */
void dftDsp( int s_type, int s_step, char *s_data, int s_rows, int s_cols, int s_channels, int d_type, int d_step, char *d_data, int d_rows, int d_cols, int d_channels )
{
    OPENCV_Operation operation = OPENCV_OPERATION_DFT;

    if (s_type != CV_8UC1 && s_type != CV_8SC1) {
        printf ("Error: Currently only image type of CV_8UC1 and CV_8SC1 are supported.\n");
        return;
    }

    /* Set parameters to be passed to DSP */
    opencvDspSetParams( s_type, s_step, s_data, s_rows, s_cols, s_channels, d_type, d_step, d_data, d_rows, d_cols, d_channels );

    opencvDspCeInit(operation);
}

