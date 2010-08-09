/*========================================================================================= */
/* Copyright (c) 2010, Texas Instruments Incorporated                                       */
/*   All rights reserved.                                                                   */
/*                                                                                          */
/*   Name: C6Accel_testfxns.h                                                               */
/*                                                                                          */
/*   Description: File contains function prototypes for test functions                      */
/*                                                                                          */
/*==========================================================================================*/ 

#ifndef C6ACCEL_TESTFXNS_H
#define C6ACCEL_TESTFXNS_H

/* Function prototypes for digital signal processing kernels*/
Int c6accel_test_DSP_FFT(C6accel_Handle hC6accel,unsigned int n);
Int c6accel_test_DSP_IFFT(C6accel_Handle hC6accel,unsigned int n);
Int c6accel_test_DSP_AUTOCOR(C6accel_Handle hC6accel,unsigned int nx,unsigned int nr);
Int c6accel_test_DSP_DOTPROD(C6accel_Handle hC6accel,unsigned int nr);
Int c6accel_test_DSP_MATMUL(C6accel_Handle hC6accel,unsigned int row1, unsigned int col1, 
                            unsigned int col2,unsigned int shift);
Int c6accel_test_DSP_FIR(C6accel_Handle hC6accel, unsigned int noutputs, unsigned int ncoeffs); 
Int c6accel_test_DSP_IIR(C6accel_Handle hC6accel, unsigned int ninputs, unsigned int ncoeffs);


/* Function prototypes for image processing kernel tests */
Int c6accel_test_IMG_sobel(C6accel_Handle hC6accel,unsigned int inputWidth, unsigned int inputHeight);
Int c6accel_test_IMG_histogram(C6accel_Handle hC6accel,unsigned int inputWidth, unsigned int inputHeight);
Int c6accel_test_IMG_conv(C6accel_Handle hC6accel,unsigned int inputWidth, unsigned int inputHeight);
Int c6accel_test_IMG_corr(C6accel_Handle hC6accel,unsigned int inputWidth, unsigned int inputHeight);
Int c6accel_test_IMG_median(C6accel_Handle hC6accel,unsigned int inputWidth, unsigned int inputHeight);
Int c6accel_test_IMG_muls(C6accel_Handle hC6accel,unsigned int inputWidth, unsigned int inputHeight);
Int c6accel_test_IMG_adds(C6accel_Handle hC6accel,unsigned int inputWidth, unsigned int inputHeight);
Int c6accel_test_IMG_subs(C6accel_Handle hC6accel,unsigned int inputWidth, unsigned int inputHeight);
Int c6accel_test_IMG_YC_demux(C6accel_Handle hC6accel,unsigned int inputWidth, unsigned int inputHeight);
int c6accel_test_IMG_YUV422PLtoYUV422SP(C6accel_Handle hC6accel,unsigned int num_lines, unsigned int width,
                                        unsigned int dst_pitch, unsigned int src_y_pitch,unsigned int src_cbcr_pitch);

/* Function prototypes for MATH kernels */
Int c6accel_test_MATH_RTSARITH(C6accel_Handle hC6accel,unsigned int n);
Int c6accel_test_MATH_RTSCONV(C6accel_Handle hC6accel,unsigned int n);
Int c6accel_test_MATH_IQCONV(C6accel_Handle hC6accel,unsigned int n, unsigned int qfmt, unsigned int qfmt2);
Int c6accel_test_MATH_IQMATH(C6accel_Handle hC6accel,unsigned int n, unsigned int qfmt);
Int c6accel_test_MATH_IQARITH(C6accel_Handle hC6accel,unsigned int n, unsigned int qfmt);
Int c6accel_test_MATH_IQTRIG(C6accel_Handle hC6accel,unsigned int n, unsigned int qfmt); 

/* Function prototypes for Floating point DSP kernels */
Int c6accel_test_MATH_RTSFLT(C6accel_Handle hC6accel,unsigned int n) ;
Int c6accel_test_MATH_RTSFLTDP(C6accel_Handle hC6accel,unsigned int n) ;
Int c6accel_test_DSPF_AUTOCOR(C6accel_Handle hC6accel, int Nx, int Nr);
Int c6accel_test_DSPF_CONVOL(C6accel_Handle hC6accel,short nh,short ny);
Int c6accel_test_DSPF_MATFXNS(C6accel_Handle hC6accel, int c1, int r1,int c2 );
Int c6accel_test_DSPF_MAT_MUL_CPLX(C6accel_Handle hC6accel, int c1,int r1, int c2 );
Int c6accel_test_DSPF_MAT_TRANS(C6accel_Handle hC6accel, int r1, int c1 );
Int c6accel_test_DSPF_DOTPRODFXNS(C6accel_Handle hC6accel,unsigned int n);
Int c6accel_test_DSPF_FIR(C6accel_Handle hC6accel,unsigned int ny,unsigned int nh);
Int c6accel_test_DSPF_FIR_CPLX(C6accel_Handle hC6accel, int ny, int nh);
Int c6accel_test_DSPF_IIR(C6accel_Handle hC6accel, int n);
Int c6accel_test_DSPF_VECMUL(C6accel_Handle hC6accel,unsigned int n );
Int c6accel_test_DSPF_VECRECIP(C6accel_Handle hC6accel,unsigned int n );
Int c6accel_test_DSPF_VECSUM_SQ(C6accel_Handle hC6accel,unsigned int n );
Int c6accel_test_DSPF_W_VEC(C6accel_Handle hC6accel,float M, unsigned int n );
Int c6accel_test_DSPF_sp_fftSPxSP(C6accel_Handle hC6accel,int n,int n_min,int offset, int n_max);
Int c6accel_test_DSPF_sp_ifftSPxSP(C6accel_Handle hC6accel,int n,int n_min,int offset, int n_max);
Int c6accel_test_DSPF_BIQUAD(C6accel_Handle hC6accel,unsigned int n);

/* Functions used to validate results */
extern unsigned int compare8bitArrays(unsigned char *pSrc1, unsigned char *pSrc2,unsigned int length);
extern unsigned int compare16bitArrays(unsigned short *pSrc1, unsigned short *pSrc2,unsigned int length);
extern unsigned int compare32bitArrays(unsigned int *pSrc1, unsigned int *pSrc2,unsigned int length);


#endif

/*=============================================================================*/
/*                  End of File C6Accel_testfxns.h                         */
/*                              Version : 0.0.1                                */
/*=============================================================================*/
