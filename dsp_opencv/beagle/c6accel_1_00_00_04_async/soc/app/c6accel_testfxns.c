/*================================================================================*/
/*   Copyright (c) 2010, Texas Instruments Incorporated                           */
/*   All rights reserved.                                                         */
/*                                                                                */
/*   Name: C6Accel_testfxns.c                                                     */
/*                                                                                */
/*   Descriptions:                                                                */
/*   File contains code to test kernels in the C6Accel codec                      */
/*                                                                                */
/*      Version: 0.0.1                                                            */
/*================================================================================*/

// This define uses the new frame based (ie row and col parameters) that are optimised for C6Accel
// as they only request one operation on all rows rather than row operations
#define USE_NEW_FRAME_APIS

/*XDC and codec engine includes*/
#include <xdc/std.h>
#include <ti/sdo/ce/osal/Memory.h>

/* Run Time lib include files: */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

/* Declare MACROS that will be used for benchmarking*/
#include "benchmark.h"

/* Include C6ACCEL headers*/
#include "../c6accelw/c6accelw.h"
#include "c6accel_testfxns.h"
#include "c6accel_test_inputs.h"

/* Create default heap memory configuration for test functions */
static Memory_AllocParams testfxnsMemParams =
{
    Memory_CONTIGHEAP,
    Memory_CACHED,
    Memory_DEFAULTALIGNMENT,
    0
};

/* Global definitions */
/* Number of samples for which FFT needs to be calculated */

/* Number of unique sine waves in input data */
#define NUM_SIN_WAVES 4

/* PI definition*/
#  define PI 3.14159265358979323846

/* Define constant for number of image bits in 16 bit histogram case */
#define IMG_BITS_HISTOGRAM_16   12

/*
    Scale need to be applied (1/2^SCALE) to input data for 16x32 and 32x32
    FFT for their output to match 16x16. This is dure to the inherent
    scaling present in 16x16 kernel
*/
#define SCALE    3

/* Generate a floating point sine wave containing numSinWaves signals with n samples*/
Void generateInput (Int32 numSinWaves, float *x_ref_float, Int32 n);
static short d2s(double d);
int gen_twiddle_fft16x16 (short *w,int n);
static int d2i(double d);
int gen_twiddle_fft32x32(int *w, int n, double scale);
int gen_twiddle_ifft32x32(int *w, int n, double scale);
int gen_twiddle_ifft16x16(short *w, int n);
float randfloat();
float randflt(float min, float max);
int randint();
int randuint(int max);
int randInt(int min, int max);

/*Function declaration for comparison functions to validate result*/
unsigned int compare8bitArrays(unsigned char *pSrc1, unsigned char *pSrc2,unsigned int length);
unsigned int compare16bitArrays(unsigned short *pSrc1, unsigned short *pSrc2,unsigned int length);
unsigned int compare32bitArrays(unsigned int *pSrc1, unsigned int *pSrc2,unsigned int length);
unsigned int comparefloatArrays(float *pSrc1, float *pSrc2,unsigned int length);
unsigned int comparedoubleArrays(double *pSrc1, double *pSrc2,unsigned int length);

/* Intermediate buffers */
extern unsigned char *pSrcBuf_16bpp;
extern unsigned char *pRefBuf_16bpp;
extern unsigned char *pOutBuf_16bpp;
extern unsigned char *pWorkingBuf_16bpp;
extern unsigned char *pWorkingBuf2_16bpp;
#if (PLATFORM == 138)
extern unsigned char *pWorkingBuf3_16bpp;
#endif

/* Define all the test file paths*/
char ref_fft_16x16[]="test_files/fft_16x16_64k_sine_wave_64kHz_sampling_ref.dat";
char ref_fft_32x32[]="test_files/fft_32x32_64k_sine_wave_64kHz_sampling_ref.dat";

/* Filenames for ifft testing*/
char ref_twiddle_fft16x16[]="test_files/ref_twiddle_fft16x16.dat";
char ref_twiddle_ifft16x16[]="test_files/ref_twiddle_ifft16x16.dat";
char ref_twiddle_fft32x32[]="test_files/ref_twiddle_fft32x32.dat";
char ref_twiddle_ifft32x32[]="test_files/ref_twiddle_ifft32x32.dat";
char ref_src_fft32x32[]="test_files/input_64k_source2_combined_sine_wave_64kHz_sampling_ref.dat";
char ref_src_fft[]="test_files/input_64k_source_combined_sine_wave_64kHz_sampling_ref.dat";

char ref_ifft_16x16[]="test_files/ifft_16x16_64k_sine_wave_64kHz_sampling_ref.dat";
char ref_ifft_32x32[]="test_files/ifft_32x32_64k_sine_wave_64kHz_sampling_ref.dat";

/*Filenames for autocorrelation testing*/
char ref_autocor[]="test_files/autocor_64k_sine_wave_64kHz_sampling_ref.dat";
char ref_src_autocor[]="test_files/src_autocor_64k_sine_wave_64kHz_sampling_ref.dat";

/*Filenames for dotprod testing*/
char ref_dotprod[]="test_files/dotprod_ref.dat";
char ref_dotp_sqr[]="test_files/dotp_sqr_ref.dat";

/*Filenames for matrix multiplcation testing*/
char ref_mat_mul[]="test_files/mat_mul_ref.dat";

/*Filenames for fir generation testing*/
char ref_fir_gen[]="test_files/fir_gen_ref.dat";
char ref_fir_r4[]="test_files/fir_r4_ref.dat";
char ref_fir_r8[]="test_files/fir_r8_ref.dat";

/*Filenames for iir filter*/
char ref_iir[]="test_files/fir_iir.dat";

/* Generic 8 bit source file */
char srcFileName[]="test_files/valve_640x480_luma8bpp.pgm";

/* Filenames for histogram testing */
char ref_histogram_8[]="test_files/histogram_8_valve_640x480_luma8bpp_ref.bin";
char ref_histogram_16[]="test_files/histogram_16_valve_640x480_luma8bpp_ref.bin";

/* Filenames for median testing*/
char ref_median_3x3_8[]="test_files/median_3x3_8_valve_640x480_luma8bpp_ref.bin";
char ref_median_3x3_16[]="test_files/median_3x3_16_valve_640x480_luma8bpp_ref.bin";
char ref_median_3x3_16s[]="test_files/median_3x3_16s_valve_640x480_luma8bpp_ref.bin";

/* Filenames for conv testing*/
char ref_conv_3x3_8[]="test_files/conv_3x3_8_valve_640x480_luma8bpp_ref.pgm";
char ref_conv_3x3_16s[]="test_files/conv_3x3_16s_valve_640x480_luma8bpp_ref.dat";

/* Filenames for corr testing*/
char ref_corr_3x3_8[]="test_files/corr_3x3_8_valve_640x480_luma8bpp_ref.pgm";
char ref_corr_3x3_16s[]="test_files/corr_3x3_16s_valve_640x480_luma8bpp_ref.dat";

/* Filenames for Sobel testing */
char ref_sobel_3x3_8[]="test_files/sobel_3x3_8_valve_640x480_luma8bpp_ref.pgm";
char ref_sobel_3x3_16[]="test_files/sobel_3x3_16_valve_640x480_luma8bpp_ref.pgm";

/* Filenames for muls testing */
char ref_muls_8[]="test_files/muls_8_valve_640x480_luma8bpp_ref.pgm";
char ref_muls_16[]="test_files/muls_16_valve_640x480_luma8bpp_ref.pgm";

/* Filenames for adds testing */
char ref_adds_8[]="test_files/adds_8_valve_640x480_luma8bpp_ref.pgm";
char ref_adds_16[]="test_files/adds_16_valve_640x480_luma8bpp_ref.pgm";

/* Filenames for subs testing */
char ref_subs_8[]="test_files/subs_8_valve_640x480_luma8bpp_ref.pgm";
char ref_subs_16[]="test_files/subs_16_valve_640x480_luma8bpp_ref.pgm";

/* Filenames for yc_demux testing */
char src_yuyv[]="test_files/ship.yuv";
char ref_yc_demux_8[]="test_files/yc_demux_8_ship_800x600_luma8bpp_ref_422p.yuv";
char ref_yc_demux_16[]="test_files/yc_demux_16_ship_800x600_luma8bpp_ref_422p.yuv";
char ref_rgb_565[]="test_files/rgb_565_ship_800x600_luma8bpp_ref.rgb";
char ref_rgb_565_yuv16[]="test_files/rgb_565_ship_800x600_luma8bpp_ref_yuv16.rgb";
char ref_yuv420_rgb_565[]="test_files/yuv420_rgb_565_ship_800x600_luma8bpp_ref.rgb";
char ref_yuv420pl16_rgb_565[]="test_files/yuv420pl16_rgb_565_ship_800x600_luma8bpp_ref.rgb";

char ref_input_y_422pl[]="test_files/ref_src_y_422pl.dat";
char ref_input_cb_422pl[]="test_files/ref_src_cb_422pl.dat";
char ref_input_cr_422pl[]="test_files/ref_src_cr_422pl.dat";
char ref_dst_y_422sp[]="test_files/ref_dst_y_422sp.dat";
char ref_dst_cbcr_422sp[]="test_files/ref_dst_cbcr_422sp.dat";
char ref_src_y_422sp[]="test_files/ref_src_y_422sp.dat";
char ref_src_cbcr_422sp[]="test_files/ref_src_cbcr_422sp.dat";
char ref_dst_y_420pl[]="test_files/ref_dst_y_420pl.dat";
char ref_dst_cb_420pl[]="test_files/ref_dst_cb_420pl.dat";
char ref_dst_cr_420pl[]="test_files/ref_dst_cr_420pl.dat";
char ref_dst_ycbcr_422ile[]="test_files/ref_dst_ycbcr_422ile.dat";


/*File name to store ref output for arithmetic rts math operations*/
char ref_addsp_i[]="test_files/addsp_ref.dat";
char ref_subsp_i[]="test_files/subsp_ref.dat";
char ref_mpysp_i[]="test_files/mpysp_ref.dat";
char ref_divsp_i[]="test_files/divsp_ref.dat";
char ref_sqrtsp_i[]="test_files/sqrtsp_ref.dat";
char ref_recipsp_i[]="test_files/recipsp_ref.dat";

/* File name to store ref output for conversion rts functions */
char ref_intsp_i[]="test_files/intsp_ref.dat";
char ref_uintsp_i[]="test_files/uintsp_ref.dat";
char ref_spint_i[]="test_files/spint_ref.dat";
char ref_spuint_i[]="test_files/spuint_ref.dat";

/* File name to store ref output for IQMATH conversion kernels */
char ref_iqn[]="test_files/iqn_ref.dat";
char ref_ftoiqn[]="test_files/ftoiqn_ref.dat";
char ref_iqntof[]="test_files/iqntof_ref.dat";
char ref_iqxtoiqy[]="test_files/iqxtoiqy_ref.dat";
char ref_iqnint[]="test_files/iqnint_ref.dat";
char ref_iqnfrac[]="test_files/iqnfrac_ref.dat";

/*File name to store ref output for IQMATH mathematical kernels*/
char ref_iqnmpy[]="test_files/iqnmpy_ref.dat";
char ref_iqnmpyIQx[]="test_files/iqnmpyIQx_ref.dat";
char ref_iqnrmpy[]="test_files/iqnrmpy_ref.dat";
char ref_iqnrsmpy[]="test_files/iqnrsmpy_ref.dat";
char ref_iqnmpyI32int[]="test_files/iqnmpyI32int_ref.dat";
char ref_iqnmpyI32frac[]="test_files/iqnmpyI32frac_ref.dat";
char ref_iqndiv[]="test_files/iqndiv_ref.dat";

/*File name to store ref output fot Miscellaneous arithmetic kernels*/
char ref_iqnlog[]="test_files/iqnlog_ref.dat";
char ref_iqnabs[]="test_files/iqnabs_ref.dat";
char ref_iqnsqrt[]="test_files/iqnsqrt_ref.dat";
char ref_iqnisqrt[]="test_files/iqnisqrt_ref.dat";
char ref_iqnexp[]="test_files/iqnexp_ref.dat";
char ref_iqnpow[]="test_files/iqnpow_ref.dat";
char ref_iqnmag[]="test_files/iqnmag_ref.dat";

/*File name to store ref output for IQMATH trignometric kernels*/
char ref_iqnsin[]="test_files/iqnsin_ref.dat";
char ref_iqnsinPU[]="test_files/iqnsinPU_ref.dat";
char ref_iqncos[]="test_files/iqncos_ref.dat";
char ref_iqncosPU[]="test_files/iqncosPU_ref.dat";
char ref_iqnatan2[]="test_files/iqnatan2_ref.dat";
char ref_iqnatan2PU[]="test_files/iqnatan2PU_ref.dat";

/* Floating point Kernel test vectors */
char ref_autocor_x[]="test_files/flt_autocor_input_ref.dat";
char ref_autocor_rslt[]="test_files/flt_autocor_rslt_ref.dat";

char ref_biquad_xinput[]="test_files/flt_biquad_xinput_ref.dat";
char ref_biquad_y[]="test_files/flt_biquad_y_ref.dat";
char ref_biquad_ha[]="test_files/flt_biquad_ha_ref.dat";
char ref_biquad_hb[]="test_files/flt_biquad_hb_ref.dat";
char ref_biquad_delay[]="test_files/flt_biquad_delay_ref.dat";

char ref_convol_xinput[]="test_files/flt_convol_xinput_ref.dat";
char ref_convol_rslt[]="test_files/flt_convol_rslt_ref.dat";
char ref_convol_hinput[]="test_files/flt_convol_hinput_ref.dat";

char ref_dotprod_x1[]="test_files/flt_dotprod_x1_ref.dat";
char ref_dotprod_x2[]="test_files/flt_dotprod_x2_ref.dat";
char ref_dotprod_rslt[]="test_files/flt_dotprod_rslt_ref.dat";

char ref_fftSPxSP_xinput[]="test_files/flt_fftSPxSP_xinput_ref.dat";
char ref_fftSPxSP_rslt[]="test_files/flt_fftSPxSP_rslt_ref.dat";
char ref_fftSPxSP_brev[]="test_files/flt_fftSPxSP_brev_ref.dat";
char ref_fftSPxSP_twiddle[]="test_files/flt_fftSPxSP_twiddle_ref.dat";

char ref_fir_cplx_xinput[]="test_files/flt_fir_cplx_xinput_ref.dat";
char ref_fir_cplx_rslt[]="test_files/flt_fir_cplx_rslt_ref.dat";
char ref_fir_cplx_hinput[]="test_files/flt_fir_cplx_hinput_ref.dat";

char ref_fir_gen_xinput[]="test_files/flt_fir_gen_xinput_ref.dat";
char ref_fir_gen_rslt[]="test_files/flt_fir_gen_rslt_ref.dat";
char ref_fir_gen_hinput[]="test_files/flt_fir_gen_hinput_ref.dat";

char ref_ifftSPxSP_xinput[]="test_files/flt_ifftSPxSP_xinput_ref.dat";
char ref_ifftSPxSP_rslt[]="test_files/flt_ifftSPxSP_rslt_ref.dat";
char ref_ifftSPxSP_brev[]="test_files/flt_ifftSPxSP_brev_ref.dat";
char ref_ifftSPxSP_twiddle[]="test_files/flt_ifftSPxSP_twiddle_ref.dat";

char ref_iir_xinput[]="test_files/flt_iir_xinput_ref.dat";
char ref_iir_y1[]="test_files/flt_iir_y1_ref.dat";
char ref_iir_y2[]="test_files/flt_iir_y2_ref.dat";
char ref_iir_ha[]="test_files/flt_iir_ha_ref.dat";
char ref_iir_hb[]="test_files/flt_iir_hb_ref.dat";

char ref_mat_mul_x1[]="test_files/flt_mat_mul_x1_ref.dat";
char ref_mat_mul_x2[]="test_files/flt_mat_mul_x2_ref.dat";
char ref_mat_mul_rslt[]="test_files/flt_mat_mul_rslt_ref.dat";

char ref_mat_mul_cplx_x1[]="test_files/flt_mat_mul_cplx_x1_ref.dat";
char ref_mat_mul_cplx_x2[]="test_files/flt_mat_mul_cplx_x2_ref.dat";
char ref_mat_mul_cplx_rslt[]="test_files/flt_mat_mul_cplx_rslt_ref.dat";

char ref_mat_trans_xinput[]="test_files/flt_mat_trans_xinput_ref.dat";
char ref_mat_trans_rslt[]="test_files/flt_mat_trans_rslt_ref.dat";

char ref_vecmul_x1input[]="test_files/flt_vecmul_x1input_ref.dat";
char ref_vecmul_rslt[]="test_files/flt_vecmul_rslt_ref.dat";
char ref_vecmul_x2input[]="test_files/flt_vecmul_x2input_ref.dat";

char ref_vecrecip_xinput[]="test_files/flt_vecrecip_xinput_ref.dat";
char ref_vecrecip_rslt[]="test_files/flt_vecrecip_rslt_ref.dat";

char ref_vecsum_sq_xinput[]="test_files/flt_vecsum_sq_xinput_ref.dat";
char ref_vecsum_sq_rslt[]="test_files/flt_vecsum_sq_rslt_ref.dat";

char ref_w_vec_x1input[]="test_files/flt_w_vec_x1input_ref.dat";
char ref_w_vec_rslt[]="test_files/flt_w_vec_rslt_ref.dat";
char ref_w_vec_x2input[]="test_files/flt_w_vec_x2input_ref.dat";
/* Single precision RTS functions test vectors */
char ref_atan2sp_x1[]="test_files/flt_atan2sp_x1_ref.dat";
char ref_atan2sp_x2[]="test_files/flt_atan2sp_x2_ref.dat";
char ref_atan2sp_rslt[]="test_files/flt_atan2sp_rslt_ref.dat";

char ref_atansp_x1[]="test_files/flt_atansp_x1_ref.dat";
char ref_atansp_rslt[]="test_files/flt_atansp_rslt_ref.dat";

char ref_cossp_x1[]="test_files/flt_cossp_x1_ref.dat";
char ref_cossp_rslt[]="test_files/flt_cossp_rslt_ref.dat";

char ref_divsp_x1[]="test_files/flt_divsp_x1_ref.dat";
char ref_divsp_rslt[]="test_files/flt_divsp_rslt_ref.dat";

char ref_exp2sp_x1[]="test_files/flt_exp2sp_x1_ref.dat";
char ref_exp2sp_rslt[]="test_files/flt_exp2sp_rslt_ref.dat";

char ref_exp10sp_x1[]="test_files/flt_exp10sp_x1_ref.dat";
char ref_exp10sp_rslt[]="test_files/flt_exp10sp_rslt_ref.dat";

char ref_expsp_x1[]="test_files/flt_expsp_x1_ref.dat";
char ref_expsp_rslt[]="test_files/flt_expsp_rslt_ref.dat";

char ref_log2sp_x1[]="test_files/flt_log2sp_x1_ref.dat";
char ref_log2sp_rslt[]="test_files/flt_log2sp_rslt_ref.dat";

char ref_log10sp_x1[]="test_files/flt_log10sp_x1_ref.dat";
char ref_log10sp_rslt[]="test_files/flt_log10sp_rslt_ref.dat";

char ref_logsp_x1[]="test_files/flt_logsp_x1_ref.dat";
char ref_logsp_rslt[]="test_files/flt_logsp_rslt_ref.dat";

char ref_powsp_x1[]="test_files/flt_powsp_x1_ref.dat";
char ref_powsp_x2[]="test_files/flt_powsp_x2_ref.dat";
char ref_powsp_rslt[]="test_files/flt_powsp_rslt_ref.dat";

char ref_recipsp_x1[]="test_files/flt_recipsp_x1_ref.dat";
char ref_recipsp_rslt[]="test_files/flt_recipsp_rslt_ref.dat";

char ref_rsqrtsp_x1[]="test_files/flt_rsqrtsp_x1_ref.dat";
char ref_rsqrtsp_rslt[]="test_files/flt_rsqrtsp_rslt_ref.dat";

char ref_sinsp_x1[]="test_files/flt_sinsp_x1_ref.dat";
char ref_sinsp_rslt[]="test_files/flt_sinsp_rslt_ref.dat";

char ref_sqrtsp_x1[]="test_files/flt_sqrtsp_x1_ref.dat";
char ref_sqrtsp_rslt[]="test_files/flt_sqrtsp_rslt_ref.dat";

/* Double precision RTS functions test vectors */
char ref_atan2dp_x1[]="test_files/flt_atan2dp_x1_ref.dat";
char ref_atan2dp_x2[]="test_files/flt_atan2dp_x2_ref.dat";
char ref_atan2dp_rslt[]="test_files/flt_atan2dp_rslt_ref.dat";

char ref_atandp_x1[]="test_files/flt_atandp_x1_ref.dat";
char ref_atandp_rslt[]="test_files/flt_atandp_rslt_ref.dat";

char ref_cosdp_x1[]="test_files/flt_cosdp_x1_ref.dat";
char ref_cosdp_rslt[]="test_files/flt_cosdp_rslt_ref.dat";

char ref_divdp_x1[]="test_files/flt_divdp_x1_ref.dat";
char ref_divdp_rslt[]="test_files/flt_divdp_rslt_ref.dat";

char ref_exp2dp_x1[]="test_files/flt_exp2dp_x1_ref.dat";
char ref_exp2dp_rslt[]="test_files/flt_exp2dp_rslt_ref.dat";

char ref_exp10dp_x1[]="test_files/flt_exp10dp_x1_ref.dat";
char ref_exp10dp_rslt[]="test_files/flt_exp10dp_rslt_ref.dat";

char ref_expdp_x1[]="test_files/flt_expdp_x1_ref.dat";
char ref_expdp_rslt[]="test_files/flt_expdp_rslt_ref.dat";

char ref_log2dp_x1[]="test_files/flt_log2dp_x1_ref.dat";
char ref_log2dp_rslt[]="test_files/flt_log2dp_rslt_ref.dat";

char ref_log10dp_x1[]="test_files/flt_log10dp_x1_ref.dat";
char ref_log10dp_rslt[]="test_files/flt_log10dp_rslt_ref.dat";

char ref_logdp_x1[]="test_files/flt_logdp_x1_ref.dat";
char ref_logdp_rslt[]="test_files/flt_logdp_rslt_ref.dat";

char ref_powdp_x1[]="test_files/flt_powdp_x1_ref.dat";
char ref_powdp_x2[]="test_files/flt_powdp_x2_ref.dat";
char ref_powdp_rslt[]="test_files/flt_powdp_rslt_ref.dat";

char ref_recipdp_x1[]="test_files/flt_recipdp_x1_ref.dat";
char ref_recipdp_rslt[]="test_files/flt_recipdp_rslt_ref.dat";

char ref_rsqrtdp_x1[]="test_files/flt_rsqrtdp_x1_ref.dat";
char ref_rsqrtdp_rslt[]="test_files/flt_rsqrtdp_rslt_ref.dat";

char ref_sindp_x1[]="test_files/flt_sindp_x1_ref.dat";
char ref_sindp_rslt[]="test_files/flt_sindp_rslt_ref.dat";

char ref_sqrtdp_x1[]="test_files/flt_sqrtdp_x1_ref.dat";
char ref_sqrtdp_rslt[]="test_files/flt_sqrtdp_rslt_ref.dat";

/*


/*
 * Test for FFT kernels
 */

int c6accel_test_DSP_FFT(C6accel_Handle hC6accel,unsigned int n)
{
    FILE  *refFd,*refFd1;
    float *pFlSineWave;
    short *twiddleTable16;
    short *inputArray16;
    int   *inputArray32;
    int   *twiddleTable32;
    int    i;

    /* Generate N bytes of 16 bit raw sine wave for use in tests
     Reuse pWorkingBuf2_16bpp for this */
    pFlSineWave = (float *)pWorkingBuf2_16bpp;
    generateInput (4, pFlSineWave, n);

    /*Test C6accel_DSP_fft16x16() */

    /*Generate the 16 bit fixed version of sine */
    inputArray16 = (short *)pSrcBuf_16bpp;
    if ((refFd1 = fopen(ref_src_fft,"rb")) == NULL){
       printf("Failed to open refFd on %s\n",ref_src_fft);
       return (0);
       }

    fread(inputArray16,  2*n, sizeof(short), refFd1);
    fclose(refFd1);

    /* Read in the twiddle table, use pWorkingBuf_16bpp for the table */
    twiddleTable16 = (short*)pWorkingBuf_16bpp;

    if ((refFd1 = fopen(ref_twiddle_fft16x16,"rb")) == NULL){
       printf("Failed to open refFd on %s\n",ref_twiddle_fft16x16);
       return (0);
       }
    fread(twiddleTable16,  2*n, sizeof(short), refFd1);
    fclose(refFd1);

    if ((refFd = fopen(ref_fft_16x16,"rb")) == NULL){
       printf("Failed to open refFd on %s\n",ref_fft_16x16);
       return (0);
       }

    /* Clear output arrays*/
    memset (pOutBuf_16bpp,0x00,2*n*sizeof(short));
    memset (pRefBuf_16bpp,0x00,2*n*sizeof(short));

    LOG_STRING("C6accel_DSP_fft16x16(),");
    START_BENCHMARK();

    /* Call the fft16x16 function in C6Accel*/
    C6accel_DSP_fft16x16 (hC6accel,twiddleTable16, n,inputArray16,(short*)pOutBuf_16bpp);

    END_AND_WRITE_BENCHMARK();

    /* read in reference from test file*/
    fread(pRefBuf_16bpp,  2*n, sizeof(short), refFd);
    LOG_STRING("\n");

    /* Validate output between native DSP/ref and C6Accel on 16 bit short level */
    if (compare16bitArrays((unsigned short*)pOutBuf_16bpp, (unsigned short*)pRefBuf_16bpp,2*n) != 1)
         {printf("C6accel_DSP_fft16x16() failed\n");
          return 0;
          }

     fclose(refFd);

    /* Test C6accel_DSP_fft32x32() */
    /* Read in N bytes of 16 bit input signal for tests*/
    /* Generate the 32 bit fixed version of sine */
    inputArray32 = (int *)pSrcBuf_16bpp;

    if ((refFd1 = fopen(ref_src_fft32x32,"rb")) == NULL){
       printf("Failed to open refFd on %s\n",ref_src_fft32x32);
       return (0);
       }

    fread(inputArray32,  2*n, sizeof(int), refFd1);
    fclose(refFd1);

    /* Generate the twiddle table, use pWorkingBuf_16bpp for the table */
      twiddleTable32 = (int *)pWorkingBuf_16bpp;

      if ((refFd1 = fopen(ref_twiddle_fft32x32,"rb")) == NULL){
       printf("Failed to open refFd on %s\n",ref_twiddle_fft32x32);
       return (0);
       }

    fread(twiddleTable32,  2*n, sizeof(int), refFd1);
    fclose(refFd1);

    if ((refFd = fopen(ref_fft_32x32,"rb")) == NULL){
       printf("Failed to open refFd on %s\n",ref_fft_32x32);
       return (0);
      }

     /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,2*n*sizeof(int));
    memset (pRefBuf_16bpp,0x00,2*n*sizeof(int));

    LOG_STRING("C6accel_DSP_fft32x32(),");
    START_BENCHMARK();

    /* Call the fft32x32 in the C6Accel codec */
    C6accel_DSP_fft32x32 (hC6accel,twiddleTable32, n,inputArray32,(int *)pOutBuf_16bpp);
    END_AND_WRITE_BENCHMARK();

   /* read in reference output created on the dsp using the same input */
    fread(pRefBuf_16bpp,  2*n, sizeof(int), refFd);

    LOG_STRING("\n");

    /* Validate output between native DSP/ref and C6Accel on 16 bit short level */
    if (compare32bitArrays((unsigned int*)pOutBuf_16bpp, (unsigned int*)pRefBuf_16bpp,2*n) != 1){
         printf("C6accel_DSP_fft32x32() failed\n");
         return 0;
     }

      fclose(refFd);

    printf("Tests for FFT functions successful\n");
    return (1);
}

/*
 * Test all Inverse FFT kernels in c6accel
 */
int c6accel_test_DSP_IFFT(C6accel_Handle hC6accel,unsigned int n)
{
    FILE  *refFd, *refFd1;
    short *twiddleTable16;
    short *inputArray16;
    int   *inputArray32;
    int   *twiddleTable32;
    double scale = 2147483647.5;

    /* Test C6accel_DSP_ifft16x16() */
    inputArray16 = (short *)pSrcBuf_16bpp;
    if ((refFd1 = fopen(ref_fft_16x16,"rb")) == NULL){
       printf("Failed to open refFd on %s\n",ref_fft_16x16);
       return (0);
     }

    fread(inputArray16,  2*n, sizeof(short), refFd1);
    fclose(refFd1);

    /* Generate the twiddle table, use pWorkingBuf_16bpp for the table*/
    twiddleTable16 = (short*)pWorkingBuf_16bpp;
    gen_twiddle_fft16x16(twiddleTable16, n);

    if ((refFd = fopen(ref_ifft_16x16,"rb")) == NULL){
       printf("Failed to open refFd on %s\n",ref_ifft_16x16);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,2*n*sizeof(short));
    memset (pRefBuf_16bpp,0x00,2*n*sizeof(short));

    LOG_STRING("C6accel_DSP_ifft16x16(),");
    START_BENCHMARK();

    /* Call the ifft16x16 kernel in the codec */
    C6accel_DSP_ifft16x16(hC6accel,twiddleTable16, n,inputArray16,(short*)pOutBuf_16bpp);
    END_AND_WRITE_BENCHMARK();

    /*  read in reference output */
    fread(pRefBuf_16bpp,  2*n, sizeof(short), refFd);

    LOG_STRING("\n");

    /* Validate output between native DSP/ref and C6Accel on 16 bit short level */
    if (compare16bitArrays((unsigned short*)pOutBuf_16bpp, (unsigned short*)pRefBuf_16bpp,2*n) != 1){
         printf("C6accel_DSP_ifft16x16() failed\n");
         return 0;
       }

    fclose(refFd);

   /*Test C6accel_DSP_ifft32x32()*/
    inputArray32 = (int *)pSrcBuf_16bpp;
    if ((refFd1 = fopen(ref_fft_32x32,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_fft_32x32);
       return (0);
       }

    fread(inputArray32,  2*n, sizeof(int), refFd1);
    fclose(refFd1);

    /* Generate the twiddle table, use pWorkingBuf_16bpp for the table */
    twiddleTable32 = (int *)pWorkingBuf_16bpp;
    gen_twiddle_ifft32x32(twiddleTable32, n, scale);

    if ((refFd = fopen(ref_ifft_32x32,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_ifft_32x32);
       return (0);
     }

    /*  Clear output arrays */
    memset (pOutBuf_16bpp,0x00,2*n*sizeof(int));
    memset (pRefBuf_16bpp,0x00,2*n*sizeof(int));

    LOG_STRING("C6accel_DSP_ifft32x32(),");
    START_BENCHMARK();

    /* Call the 32 bit ifft in c6accel */
    C6accel_DSP_ifft32x32(hC6accel,twiddleTable32, n,inputArray32,(int*)pOutBuf_16bpp);

    END_AND_WRITE_BENCHMARK();

    /* read in reference */
    fread(pRefBuf_16bpp,  2*n, sizeof(int), refFd);
    LOG_STRING("\n");

    /* Validate output between native DSP/ref and C6Accel on 16 bit short level*/
    if (compare32bitArrays((unsigned int*)pOutBuf_16bpp, (unsigned int*)pRefBuf_16bpp,2*n) != 1){
         printf("C6accel_DSP_ifft32x32() failed\n");
         return 0;
      }

    fclose(refFd);
    printf("Tests for Invers FFT functions successful\n");
    return (1);
}


/*
 * Test for autocorrelation kernels in c6accel
 */
int c6accel_test_DSP_AUTOCOR(C6accel_Handle hC6accel,unsigned int nx ,unsigned int nr)
{
    FILE  *refFd,*refFd1;
    float *pFlSineWave;
    short *inputArray16;
    int i;

    /* Generate N bytes of 16 bit raw sine wave for use in tests
     Reuse pWorkingBuf2_16bpp for this*/

     if ((refFd1 = fopen(ref_src_autocor,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_src_autocor);
       return (0);
     }

    /* Test C6accel_DSP_autocor(), Generate the 16 bit fixed version of sine */
    inputArray16 = (short *)pSrcBuf_16bpp;
    fread(inputArray16,  nx+nr, sizeof(short), refFd1);
    fclose(refFd1);

    if ((refFd = fopen(ref_autocor,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_autocor);
       return (0);
       }

   /*  Clear output arrays */
    memset (pOutBuf_16bpp,0x00, nr*sizeof(short));
    memset (pRefBuf_16bpp,0x00, nr*sizeof(short));

   /* Make log entry and make benchmarking entries */
    LOG_STRING("C6accel_DSP_autocor(),");
    START_BENCHMARK();

    /* Call autocorrelation function in the C6Accel */
    C6accel_DSP_autocor(hC6accel,(short*)pOutBuf_16bpp,(short*)inputArray16,nx,nr);
    END_AND_WRITE_BENCHMARK();

    /*  read the reference output from file */
    fread(pRefBuf_16bpp,  nr, sizeof(short), refFd);
    LOG_STRING("\n");

    /* Function passed or failed the test */
    if (compare16bitArrays((unsigned short*)pOutBuf_16bpp, (unsigned short*)pRefBuf_16bpp, nr) != 1){
         printf("C6accel_DSP_autocor() failed\n");
         return 0;
     }

    fclose(refFd);
    printf("Test for Auto Correlation function successful\n");
    return (1);
}

/*
 * Test for dot product kernels in c6accel
 */
int c6accel_test_DSP_DOTPROD(C6accel_Handle hC6accel,unsigned int nx)
{
    FILE  *refFd;
    short *mArray16, *nArray16;
    int i, Gout =0;

    /* Generate the 16 bit fixed version of sine */
    mArray16 = (short *)pSrcBuf_16bpp;
    nArray16 = (short *)pWorkingBuf2_16bpp;

    for (i=0;i<nx;i++){
        mArray16[i]     = m[i];
        nArray16[i]     = n[i];
    }

    if ((refFd = fopen(ref_dotprod,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_dotprod);
       return (0);
    }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00, 2*sizeof(int));
    memset (pRefBuf_16bpp,0x00, 2*sizeof(int));

    LOG_STRING("C6accel_DSP_dotprod(),");
    START_BENCHMARK();

    /* Call the dot product function in the C6Accel codec */
    C6accel_DSP_dotprod(hC6accel,(short*)mArray16,(short*)nArray16,(int *)pOutBuf_16bpp,nx);
    END_AND_WRITE_BENCHMARK();

    /* Read reference output from file */
    fread(pRefBuf_16bpp, 1, sizeof(int), refFd);

    LOG_STRING("\n");
    if( *(int *)pRefBuf_16bpp != *(int *)pOutBuf_16bpp ){
      printf("Dot prod failed\n ");
      return 0;
    }

    fclose(refFd);

    /* TEST C6Accel_DSP_dotp_sqr kernel */

    if ((refFd = fopen(ref_dotp_sqr,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_dotprod);
       return (0);
    }

    /* Clear output arrays*/
    memset (pOutBuf_16bpp,0x00, 2*sizeof(int));
    memset (pRefBuf_16bpp,0x00, 2*sizeof(int));

    LOG_STRING("C6accel_DSP_DSP_dotp_sqr(),");
    START_BENCHMARK();

    /* Call the DSP dotp sqr kernel in the C6Accel */
    C6accel_DSP_dotp_sqr(hC6accel,Gout,(short*)mArray16,(short*)nArray16,(int*)(pOutBuf_16bpp+sizeof(Gout)),(int *)pOutBuf_16bpp,nx);

    END_AND_WRITE_BENCHMARK();

    /* Read reference ouput from test files */
      fread(pRefBuf_16bpp, 2, sizeof(int), refFd);

    LOG_STRING("\n");

    /* Compare output from C6Accel with reference output to determine success of the test */
    if((*(int *)pRefBuf_16bpp != *(int *)pOutBuf_16bpp) ||
       (*(int*)(pRefBuf_16bpp+sizeof(Gout))!= *(int*)(pOutBuf_16bpp+sizeof(Gout)))){
      printf("Dot product square failed\n");
      return 0;
    }

    fclose(refFd);

    printf("Tests for DoT Product functions successful\n");
    return (1);
}

/*
 * Test for matrix multiplication kernels in c6accel
 */
int c6accel_test_DSP_MATMUL(C6accel_Handle hC6accel,unsigned int r1,unsigned int c1, unsigned int c2,unsigned int s)
{
    FILE *refFd;
    short *xArray16, *yArray16;
    int i;

    /* Generate the 16 bit input */
    xArray16 = (short *)pSrcBuf_16bpp;
    yArray16 = (short *)pWorkingBuf2_16bpp;

    for (i=0;i<r1*c1;i++){
        xArray16[i]     = x[i];
    }
    for (i=0;i<c2*c1;i++){
        yArray16[i]     = y[i];
    }

    if ((refFd = fopen(ref_mat_mul,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_mat_mul);
       return (0);
       }

    /* Clear output arrays*/
    memset (pOutBuf_16bpp,0x00, r1*c2*sizeof(short));
    memset (pRefBuf_16bpp,0x00, r1*c2*sizeof(short));

    LOG_STRING("C6accel_DSP_mat_mul(),");
    START_BENCHMARK();

    /* Call matrix multiplication kernel in the C6Accel codec*/
    C6accel_DSP_mat_mul(hC6accel,xArray16,r1,c1,yArray16,c2,(short *)pOutBuf_16bpp,s);
    END_AND_WRITE_BENCHMARK();

    /* Read reference input from test file*/
    fread(pRefBuf_16bpp, r1*c2, sizeof(short), refFd);

    LOG_STRING("\n");

    /* Compare the output to the reference output to determine outcome of test */
    if (compare16bitArrays((unsigned short*)pOutBuf_16bpp, (unsigned short*)pRefBuf_16bpp, r1*c2) != 1){
         printf("C6accel_DSP_mat_mul() failed\n");
         return 0;
       }

     fclose(refFd);

     printf("Tests for Matrix multiplication successful\n");
     return (1);
}

/*
 * Test for FIR filter generation kernels in c6accel
 */
int c6accel_test_DSP_FIR(C6accel_Handle hC6accel,unsigned int nr,unsigned int nh)
{
    FILE *refFd;
    short *xArray16, *hArray16;
    int i;
    unsigned int nx = nr+nh-1;

    /* C6accel_DSP_fir_gen() test */
    /* Read 16 bit input */
    xArray16 = (short *)pSrcBuf_16bpp;
    hArray16 = (short *)pWorkingBuf2_16bpp;

    for (i=0;i<nx;i++){
        xArray16[i]     = x[i];
    }
    for (i=0;i<nh;i++){
        hArray16[i]     = h[i];
    }

    /* Open reference file to store reference output */
    if ((refFd = fopen(ref_fir_gen,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_fir_gen);
       return (0);
     }

    /* Clear output arrays before tests begins */
    memset (pOutBuf_16bpp,0x0000, nr);
    memset (pRefBuf_16bpp,0x0000, nr);

    LOG_STRING("C6accel_DSP_fir_gen(),");
    START_BENCHMARK();

    /* Call the DSP_fir_gen function in C6Accel*/
    C6accel_DSP_fir_gen(hC6accel,xArray16, hArray16,(short *)pOutBuf_16bpp,nh,nr);
    END_AND_WRITE_BENCHMARK();

    fread(pRefBuf_16bpp, nr, sizeof(short), refFd);
    LOG_STRING("\n");

    /* Validate Results by comparing output from C6accel call to the reference output */
    if (compare16bitArrays((unsigned short*)pOutBuf_16bpp, (unsigned short*)pRefBuf_16bpp, nr) != 1){
         printf("C6accel_DSP_fir_gen() failed\n");
         return 0;
     }

     fclose(refFd);

    /*Test fir_r4 kernel in c6accel*/
    if ((refFd = fopen(ref_fir_r4,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_fir_r4);
       return (0);
       }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x0000, nr);
    memset (pRefBuf_16bpp,0x0000, nr);

    LOG_STRING("C6accel_DSP_fir_r4(),");
    START_BENCHMARK();

    C6accel_DSP_fir_r4(hC6accel,xArray16, hArray16,(short *)pOutBuf_16bpp,nh,nr);
    END_AND_WRITE_BENCHMARK();

    fread(pRefBuf_16bpp, nr , sizeof(short), refFd);

    LOG_STRING("\n");
    /* Validate results from c6accel with reference output */
    if (compare16bitArrays((unsigned short*)pOutBuf_16bpp, (unsigned short*)pRefBuf_16bpp, nr) != 1){
         printf("C6accel_DSP_fir_r4() failed\n");
         return 0;
       }

     fclose(refFd);

    /* Test fir_r8 kernel in c6accel */
    if ((refFd = fopen(ref_fir_r8,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_fir_r8);
       return (0);
       }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00, nr*sizeof(short));
    memset (pRefBuf_16bpp,0x00, nr*sizeof(short));

    LOG_STRING("C6accel_DSP_fir_r4(),");
    START_BENCHMARK();

    C6accel_DSP_fir_r8(hC6accel,xArray16, hArray16,(short *)pOutBuf_16bpp,nh,nr);
    END_AND_WRITE_BENCHMARK();

    fread(pRefBuf_16bpp, nr, sizeof(short), refFd);
    LOG_STRING("\n");

    /* Validate result from c6accel to reference output */
    if (compare16bitArrays((unsigned short*)pOutBuf_16bpp, (unsigned short*)pRefBuf_16bpp, nr) != 1){
         printf("C6accel_DSP_fir_r8() failed\n");
         return 0;
       }

    fclose(refFd);

    printf("Tests for FIR Filtering functions successful\n");
    return (1);
}

/*
 * Test for IIR filter generation kernels in c6accel
 */
int c6accel_test_DSP_IIR(C6accel_Handle hC6accel,unsigned int Nin,unsigned int nh)
{
    FILE *refFd;
    short *xArray16, *hArray16;
    short *b;
    int i;

    /*  Generate the 16 bit input  */
    xArray16 = (short *)pSrcBuf_16bpp;
    hArray16 = (short *)pWorkingBuf2_16bpp;
    b =(short*)pWorkingBuf_16bpp;

    for (i=0;i<Nin;i++){
        xArray16[i]     = x[i];
    }
    for (i=0;i<nh;i++){
        hArray16[i]     = h[i];
    }

    if ((refFd = fopen(ref_iir,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_iir);
       return (0);
       }
    /* set state vector */
    memset(b,0x00,(nh)*sizeof(short));

    /*  Clear output arrays */
    memset (pOutBuf_16bpp,0x00, Nin*sizeof(short));
    memset (pRefBuf_16bpp,0x00, Nin*sizeof(short));

    LOG_STRING("C6accel_DSP_iir(),");
    START_BENCHMARK();

    /* Call IIR kernel in C6Accel */
    C6accel_DSP_iir(hC6accel,xArray16, hArray16,nh,b,(short *)pOutBuf_16bpp,Nin);

    END_AND_WRITE_BENCHMARK();

    fread(pRefBuf_16bpp, 1, Nin*sizeof(short), refFd);

    LOG_STRING("\n");

    /*Validate the result by comparing result to the reference output */
    if (compare16bitArrays((unsigned short*)pOutBuf_16bpp, (unsigned short*)pRefBuf_16bpp, Nin) != 1){
         printf("C6accel_DSP_iir() failed\n");
         return 0;
       }

     fclose(refFd);

     printf("Test for IIR Filter successful\n");
     return (1);
}


/*
    This function generates the input data and also updates the



    input data arrays used by the various FFT kernels
*/
Void generateInput (Int32 numSinWaves, float *x_ref_float, Int32 n) {
    Int32 i, j;
    float sinWaveIncFreq, sinWaveMag;
    /*
        Based on numSinWave information, create the input data. The
        input data is first created using floating point dataType. The
        floating point data type is then converted to the appropriate
        fixed point notation
    */
    /* Clear the input floating point array */
    for (i = 0; i < n; i++) {
        x_ref_float[2*i] = (float)0.0;
        x_ref_float[2*i + 1] = (float)0.0;
    }

    /* Calculate the incremental freq for each sin wave */
    sinWaveIncFreq = ((float)3.142)/(numSinWaves*(float)1.0);
    /* Calculate the magnitude for each sin wave */
    sinWaveMag = (float)1.0/(numSinWaves * (float)1.0*n);
    /* Create the input array as sum of the various sin wave data */
    for (j = 0; j < numSinWaves; j++) {
        for (i = 0; i < n; i++) {
            x_ref_float[2*i] += sinWaveMag * (float)cos(sinWaveIncFreq*j*i);
            x_ref_float[2*i + 1] = (float) 0.0;
        }
    }


}


/* ======================================================================== */
/*  D2S -- Truncate a 'double' to a 'short', with clamping.                 */
/* ======================================================================== */
static short d2s(double d)
{
    d = floor(0.5 + d);  // Explicit rounding to integer //
    if (d >=  32767.0) return  32767;
    if (d <= -32768.0) return -32768;
    return (short)d;
}


/* ======================================================================== */
/*  GEN_TWIDDLE -- Generate twiddle factors for TI's custom FFTs.           */
/*                                                                          */
/*  USAGE                                                                   */
/*      This routine is called as follows:                                  */
/*                                                                          */
/*          int gen_twiddle_fft16x16(short *w, int n)                       */
/*                                                                          */
/*          short *w      Pointer to twiddle-factor array                   */
/*          int   n       Size of FFT                                       */
/*                                                                          */
/*      The routine will generate the twiddle-factors directly into the     */
/*      array you specify.  The array needs to be approximately 2*N         */
/*      elements long.  (The actual size, which is slightly smaller, is     */
/*      returned by the function.)                                          */
/* ======================================================================== */
int gen_twiddle_fft16x16(short *w, int n)
{
    int i, j, k;
    double M = 32767.5;

    for (j = 1, k = 0; j < n >> 2; j = j << 2) {
        for (i = 0; i < n >> 2; i += j << 1) {
            w[k + 11] =  d2s(M * cos(6.0 * PI * (i + j) / n));
            w[k + 10] =  d2s(M * sin(6.0 * PI * (i + j) / n));
            w[k +  9] =  d2s(M * cos(6.0 * PI * (i    ) / n));
            w[k +  8] =  d2s(M * sin(6.0 * PI * (i    ) / n));

            w[k +  7] = -d2s(M * cos(4.0 * PI * (i + j) / n));
            w[k +  6] = -d2s(M * sin(4.0 * PI * (i + j) / n));
            w[k +  5] = -d2s(M * cos(4.0 * PI * (i    ) / n));
            w[k +  4] = -d2s(M * sin(4.0 * PI * (i    ) / n));

            w[k +  3] =  d2s(M * cos(2.0 * PI * (i + j) / n));
            w[k +  2] =  d2s(M * sin(2.0 * PI * (i + j) / n));
            w[k +  1] =  d2s(M * cos(2.0 * PI * (i    ) / n));
            w[k +  0] =  d2s(M * sin(2.0 * PI * (i    ) / n));

            k += 12;
        }
    }
    return k;
}


/* ======================================================================== */
/*  D2S -- Truncate a 'double' to a 'int',   with clamping.                 */
/* ======================================================================== */
static int d2i(double d)
{
    if (d >=  2147483647.0) return (int)0x7FFFFFFF;
    if (d <= -2147483648.0) return (int)0x80000000;
    return (int)d;
}


/* ======================================================================== */
/*  GEN_TWIDDLE -- Generate twiddle factors for TI's custom FFTs.           */
/*                                                                          */
/*  USAGE                                                                   */
/*      This routine is called as follows:                                  */
/*                                                                          */
/*          int gen_twiddle_fft32x32(short *w, int n, double scale)         */
/*                                                                          */
/*          int    *w     Pointer to twiddle-factor array                   */
/*          int    n      Size of FFT                                       */
/*          double scale  Scale factor to apply to values.                  */
/*                                                                          */
/*      The routine will generate the twiddle-factors directly into the     */
/*      array you specify.  The array needs to be approximately 2*N         */
/*      elements long.  (The actual size, which is slightly smaller, is     */
/*      returned by the function.)                                          */
/* ======================================================================== */
int gen_twiddle_fft32x32(int *w, int n, double scale)
{
    int i, j, k, s=0, t;

    for (j = 1, k = 0; j < n >> 2; j = j << 2, s++) {
        for (i = t=0; i < n >> 2; i += j, t++) {
            w[k +  5] = d2i(scale * cos(6.0 * PI * i / n));
            w[k +  4] = d2i(scale * sin(6.0 * PI * i / n));

            w[k +  3] = d2i(scale * cos(4.0 * PI * i / n));
            w[k +  2] = d2i(scale * sin(4.0 * PI * i / n));

            w[k +  1] = d2i(scale * cos(2.0 * PI * i / n));
            w[k +  0] = d2i(scale * sin(2.0 * PI * i / n));

            k += 6;
        }
    }
    return k;
}

/* ======================================================================== */
/*  GEN_TWIDDLE -- Generate twiddle factors for TI's custom FFTs.           */
/*                                                                          */
/*  USAGE                                                                   */
/*      This routine is called as follows:                                  */
/*                                                                          */
/*          int gen_twiddle_ifft16x16(short *w, int n)                      */
/*                                                                          */
/*          short  *w     Pointer to twiddle-factor array                   */
/*          int    n      Size of FFT                                       */
/*                                                                          */
/*      The routine will generate the twiddle-factors directly into the     */
/*      array you specify.  The array needs to be approximately 2*N         */
/*      elements long.  (The actual size, which is slightly smaller, is     */
/*      returned by the function.)                                          */
/* ======================================================================== */
int gen_twiddle_ifft16x16(short *w, int n)
{
    int i, j, k;
    double M = 32767.5;

    for (j = 1, k = 0; j < n >> 2; j = j << 2) {
        for (i = 0; i < n >> 2; i += j << 1) {
            w[k + 11] =  d2s(M * cos(6.0 * PI * (i + j) / n));
            w[k + 10] = -d2s(M * sin(6.0 * PI * (i + j) / n));
            w[k +  9] =  d2s(M * cos(6.0 * PI * (i    ) / n));
            w[k +  8] = -d2s(M * sin(6.0 * PI * (i    ) / n));

            w[k +  7] = -d2s(M * cos(4.0 * PI * (i + j) / n));
            w[k +  6] =  d2s(M * sin(4.0 * PI * (i + j) / n));
            w[k +  5] = -d2s(M * cos(4.0 * PI * (i    ) / n));
            w[k +  4] =  d2s(M * sin(4.0 * PI * (i    ) / n));

            w[k +  3] =  d2s(M * cos(2.0 * PI * (i + j) / n));
            w[k +  2] = -d2s(M * sin(2.0 * PI * (i + j) / n));
            w[k +  1] =  d2s(M * cos(2.0 * PI * (i    ) / n));
            w[k +  0] = -d2s(M * sin(2.0 * PI * (i    ) / n));

            k += 12;
        }
    }
    return k;
}

/* ======================================================================== */
/*  GEN_TWIDDLE -- Generate twiddle factors for TI's custom FFTs.           */
/*                                                                          */
/*  USAGE                                                                   */
/*      This routine is called as follows:                                  */
/*                                                                          */
/*          int gen_twiddle_ifft32x32(short *w, int n, double scale)         */
/*                                                                          */
/*          int    *w     Pointer to twiddle-factor array                   */
/*          int    n      Size of FFT                                       */
/*          double scale  Scale factor to apply to values.                  */
/*                                                                          */
/*      The routine will generate the twiddle-factors directly into the     */
/*      array you specify.  The array needs to be approximately 2*N         */
/*      elements long.  (The actual size, which is slightly smaller, is     */
/*      returned by the function.)                                          */
/* ======================================================================== */
int gen_twiddle_ifft32x32(int *w, int n, double scale)
{
    int i, j, k, s=0, t;

    for (j = 1, k = 0; j < n >> 2; j = j << 2, s++) {
        for (i = t=0; i < n >> 2; i += j, t++) {
            w[k +  5] = d2i(scale * cos(6.0 * PI * i / n));
            w[k +  4] = d2i(scale * sin(6.0 * PI * i / n));

            w[k +  3] = d2i(scale * cos(4.0 * PI * i / n));
            w[k +  2] = d2i(scale * sin(4.0 * PI * i / n));

            w[k +  1] = d2i(scale * cos(2.0 * PI * i / n));
            w[k +  0] = d2i(scale * sin(2.0 * PI * i / n));

            k += 6;
        }
    }
    return k;
}


/*
 * Test all possible IMG Histogram combinations in this function
 */
int c6accel_test_IMG_histogram(C6accel_Handle hC6accel,unsigned int inputWidth, unsigned int inputHeight)
{
    FILE *srcFd, *refFd;
    unsigned short *p16bitBuf;
    int i;

    /* Open source image file */
    if ((srcFd = fopen(srcFileName,"rb")) == NULL) {
       printf("Failed to open srcFd on %s\n",srcFileName);
       return (0);
       }

    if ((refFd = fopen(ref_histogram_8,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_histogram_8);
       return (0);
       }

    /* Read in source file */
    fread(pSrcBuf_16bpp,inputWidth*inputHeight,1,srcFd);

    /* Clear working and output arrays*/
    memset (pWorkingBuf_16bpp,0x00,1024*sizeof(short));

    memset (pOutBuf_16bpp,0x00,256*sizeof(short));
    memset (pRefBuf_16bpp,0x00,256*sizeof(short));


    LOG_STRING("C6accel_IMG_histogram_8(),");
    START_BENCHMARK();

    /* Call the histogram */
    C6accel_IMG_histogram_8(hC6accel,pSrcBuf_16bpp,inputWidth*inputHeight,1,
            (unsigned short*)pWorkingBuf_16bpp,
            (unsigned short*)pOutBuf_16bpp);

    END_AND_WRITE_BENCHMARK();

    /* reset temp histogram*/
    memset (pWorkingBuf_16bpp,0x00,1024*sizeof(short));

    /* read in reference */
    fread(pRefBuf_16bpp,  (1<<IMG_BITS_HISTOGRAM_16), sizeof(short), refFd);

    LOG_STRING("\n");

    /* Validate output between native DSP/ref and C6Accel on 16 bit short level */
    if (compare16bitArrays((unsigned short*)pOutBuf_16bpp, (unsigned short*)pRefBuf_16bpp,256) != 1)
         {printf("C6accel_IMG_histogram_8() failed\n");
          return 0;
          }

    fclose(refFd);

    /* Test for IMG_histogram_16() */
    /* Open reference output file for 16 bit histogram kernel */
    if ((refFd = fopen(ref_histogram_16,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_histogram_16);
       return (0);
       }

    /* Will reuse 8 bit source file and copy to a 16bpp version with a shift to make it 12 bit data */
    p16bitBuf= (unsigned short *)pWorkingBuf2_16bpp;
    for(i=0;i< inputWidth*inputHeight;i++)
         {p16bitBuf[i] = ((unsigned short)pSrcBuf_16bpp[i])<<(IMG_BITS_HISTOGRAM_16-8);
         }

    /* Clear working and output arrays */
    memset (pWorkingBuf_16bpp,0x00,4*(1<<IMG_BITS_HISTOGRAM_16)*sizeof(short));

    memset (pOutBuf_16bpp,0x00,(1<<IMG_BITS_HISTOGRAM_16)*sizeof(short));
    memset (pRefBuf_16bpp,0x00,(1<<IMG_BITS_HISTOGRAM_16)*sizeof(short));

    LOG_STRING("C6accel_IMG_histogram_16(),");
    START_BENCHMARK();
    /* Call the histogram */
    C6accel_IMG_histogram_16(hC6accel,p16bitBuf,inputWidth*inputHeight,1,
            (short*)pWorkingBuf_16bpp,(short*)pOutBuf_16bpp,IMG_BITS_HISTOGRAM_16 /*img_bits == 12bit data */);
    END_AND_WRITE_BENCHMARK();

    /* reset temp histogram */
    memset (pWorkingBuf_16bpp,0x00,4*(1<<IMG_BITS_HISTOGRAM_16)*sizeof(short));

    /* read in reference */
    fread(pRefBuf_16bpp,  (1<<IMG_BITS_HISTOGRAM_16), sizeof(short), refFd);
    LOG_STRING("\n");

    /* Validate output between native DSP/ref and C6Accel on 16 bit level */
     if (compare16bitArrays((unsigned short*)pOutBuf_16bpp,(unsigned short*)pRefBuf_16bpp,(1<<IMG_BITS_HISTOGRAM_16)) != 1)
         {printf("C6accel_IMG_histogram_16() failed\n");
          return 0;
          }

     fclose(srcFd);
     fclose(refFd);

     printf("Tests for Histogram functions successful\n");
     return (1);
}

/*
 * Test all possible IMG Median combinations in this function
 */
int c6accel_test_IMG_median(C6accel_Handle hC6accel,unsigned int inputWidth, unsigned int inputHeight)
{
    unsigned int                i;
#ifndef USE_NEW_FRAME_APIS
    unsigned int                row;
#endif
    FILE *srcFd, *refFd;

    short* p16sbitBufIn,*p16sbitBufOut;

    /* Test for IMG_median()*/
    /* Open source image file */
    if ((srcFd = fopen(srcFileName,"rb")) == NULL) {
       printf("Failed to open srcFd on %s\n",srcFileName);
       return (0);
       }
    /* Open reference output ref/DSP output file */
    if ((refFd = fopen(ref_median_3x3_8,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_median_3x3_8);
       return (0);
       }

    /* Read in source file */
    fread(pSrcBuf_16bpp,inputWidth*inputHeight,1,srcFd);

    /* Clear working and output arrays */
    memset (pOutBuf_16bpp,0x00,inputWidth*inputHeight);
    memset (pRefBuf_16bpp,0x00,inputWidth*inputHeight);

    LOG_STRING("C6accel_IMG_median_3x3_8(),");
    START_BENCHMARK();

     /*  Call the median filter one line at a time
         The 3x3 array read lines n,n+1,n+2 from input image to calculate output line n.
         Therefore, the last 2 rows of the image cannot be calculated as they read past the original image.
         So only calulate on height-2 rows for valid data.   */
#ifdef USE_NEW_FRAME_APIS
    C6accel_IMG_median_3x3_8_Frame(hC6accel,pSrcBuf_16bpp,inputHeight-2,inputWidth,pOutBuf_16bpp);

#else
    for (row=0;row<(inputHeight-2);row++)
         C6accel_IMG_median_3x3_8(hC6accel,pSrcBuf_16bpp+row*inputWidth,inputWidth,pOutBuf_16bpp+row*inputWidth);
#endif
    END_AND_WRITE_BENCHMARK();

    /* read in reference */
    fread(pRefBuf_16bpp,  inputWidth*(inputHeight-2), sizeof(char), refFd);
    LOG_STRING("\n");


    /* Validate output between native DSP/ref and C6Accel on 8 bit char level */
    if (compare8bitArrays(pOutBuf_16bpp, pRefBuf_16bpp,inputWidth*(inputHeight-2)) != 1)
         {printf("C6accel_IMG_median_3x3_8() failed\n");
          return 0;
          }

     fclose(refFd);

    /* test IMG_median_3x3_16() in c6accel */

    /*Open reference output for 16 bit median kernel */
    if ((refFd = fopen(ref_median_3x3_16,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_median_3x3_16);
       return (0);
       }

    /* Will reuse 8 bit source file and copy to a 16bpp version with a shift to make it 12 bit data */
    p16sbitBufIn= (short *)pWorkingBuf_16bpp;
    p16sbitBufOut= (short *)pOutBuf_16bpp;

    for(i=0;i< inputWidth*inputHeight;i++){
       p16sbitBufIn[i] = ((unsigned short)pSrcBuf_16bpp[i])<<4;
    }

    memset (pOutBuf_16bpp,0x00,inputWidth*inputHeight*sizeof(short));
    memset (pRefBuf_16bpp,0x00,inputWidth*inputHeight*sizeof(short));

    LOG_STRING("C6accel_IMG_median_3x3_16(),");
    START_BENCHMARK();

     /*  Call the median filter one line at a time
         The 3x3 array read lines n,n+1,n+2 from input image to calculate output line n.
         Therefore, the last 2 rows of the image cannot be calculated as they read past the original image.
         So only calulate on height-2 rows for valid data.   */
 #ifdef USE_NEW_FRAME_APIS
       C6accel_IMG_median_3x3_16_Frame(hC6accel,p16sbitBufIn,inputHeight-2,inputWidth,p16sbitBufOut);

 #else
    for (row=0;row<inputHeight-2;row++)
       C6accel_IMG_median_3x3_16(hC6accel,p16sbitBufIn+row*inputWidth,inputWidth,p16sbitBufOut+row*inputWidth);
 #endif
    END_AND_WRITE_BENCHMARK();

    /* read in reference */
    fread(pRefBuf_16bpp, inputWidth*(inputHeight-2), sizeof(short), refFd);
    LOG_STRING("\n");


    /* Validate output between native DSP/ref and C6Accel on 16 bit char level */
    if (compare16bitArrays((unsigned short*)pOutBuf_16bpp, (unsigned short*)pRefBuf_16bpp,inputWidth*(inputHeight-2)) != 1)
         {printf("C6accel_IMG_median_16() failed\n");
          return 0;
          }

    fclose(srcFd);
    fclose(refFd);

    /* Test for IMG_median_3x3_16() in c6accel */
    /* Read reference output for 16 bit signed median kernel in c6accel */
    if ((refFd = fopen(ref_median_3x3_16s,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_median_3x3_16s);
       return (0);
       }

    /* Will reuse 8 bit source file and copy to a 16bpp version with a shift to make it 12 bit data*/
    p16sbitBufIn= (short *)pWorkingBuf_16bpp;
    p16sbitBufOut= (short *)pOutBuf_16bpp;

    for(i=0;i< inputWidth*inputHeight;i++){
       p16sbitBufIn[i] = ((short)pSrcBuf_16bpp[i])<<4;
    }

    memset (pOutBuf_16bpp,0x00,inputWidth*inputHeight*sizeof(short));
    memset (pRefBuf_16bpp,0x00,inputWidth*inputHeight*sizeof(short));

    LOG_STRING("C6accel_IMG_median_3x3_16s(),");
    START_BENCHMARK();

      /* Call the median filter one line at a time
         The 3x3 array read lines n,n+1,n+2 from input image to calculate output line n.
         Therefore, the last 2 rows of the image cannot be calculated as they read past the original image.
         So only calulate on height-2 rows for valid data.   */
#ifdef USE_NEW_FRAME_APIS
         C6accel_IMG_median_3x3_16s_Frame(hC6accel,p16sbitBufIn,inputHeight-2,inputWidth,p16sbitBufOut);

#else
    for (row=0;row<inputHeight-2;row++)
         C6accel_IMG_median_3x3_16s(hC6accel,p16sbitBufIn+row*inputWidth,inputWidth,p16sbitBufOut+row*inputWidth);
#endif
    END_AND_WRITE_BENCHMARK();

    /* read in reference */
    fread(pRefBuf_16bpp, inputWidth*(inputHeight-2), sizeof(short), refFd);

    LOG_STRING("\n");

    /* Validate output between native DSP/ref and C6Accel on 16 bit char level */
    if (compare16bitArrays((unsigned short*)pOutBuf_16bpp, (unsigned short*)pRefBuf_16bpp,inputWidth*(inputHeight-2)) != 1)
         {printf("C6accel_IMG_median_16s() failed\n");
          return 0;
          }

     fclose(refFd);

     printf("Test for Median Filtering functions successful\n");
     return (1);
}



/*
 * Test all possible IMG CONV combinations in this function
 */

const signed char gaussian_3x3_8s_1Q7[]={-1,-1,-1,
                                -1,127,-1,
                                -1,-1,-1};

const unsigned char gaussian_3x3_8_1Q7[]={1,1,1,
                                          1,119,1,
                                          1,1,1};

const short gaussian_3x3_16s_1Q15[]={-256,-256, -256,
                                   -256,32512,-256,
                                   -256,-256, -256};

int c6accel_test_IMG_conv(C6accel_Handle hC6accel,unsigned int inputWidth, unsigned int inputHeight)
{
    unsigned int   row,i;
    FILE           *srcFd, *refFd;
    short          *p16bitInBuf;
    short          *p16bitOutBuf;
    /* Pointer to hold a CMem for the mask*/
    short          *p16GaussianCmem;

    /* Allocate CMEM memory for 3x3 short mask*/
    p16GaussianCmem = Memory_alloc(18, &testfxnsMemParams);

    /* Test IMG_conv_3x3_8() */
    /* Open source file */
    if ((srcFd = fopen(srcFileName,"rb")) == NULL) {
       printf("Failed to open srcFd on %s\n",srcFileName);
       return (0);
     }
    /* OPen reference output file*/
    if ((refFd = fopen(ref_conv_3x3_8,"rb")) == NULL){
       printf("Failed to open refFd on %s\n",ref_conv_3x3_8);
       return (0);
     }

    /* Read in source file*/
    fread(pSrcBuf_16bpp,inputWidth*inputHeight,1,srcFd);

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,inputWidth*inputHeight);
    memset (pRefBuf_16bpp,0x00,inputWidth*inputHeight);

    /* Set Up required Gaussian */
    memcpy( p16GaussianCmem,gaussian_3x3_8s_1Q7,9);

    LOG_STRING("C6accel_IMG_conv_3x3_i8_c8s(),");
    START_BENCHMARK();
    /* Call the conv_codec one line at a time
       The 3x3 array read lines n,n+1,n+2 from input image to calculate output line n.
       Therefore, the last 2 rows of the image cannot be calculated as they read past the original image.
       So only calulate on height-2 rows for valid data.   */

#ifdef USE_NEW_FRAME_APIS
        C6accel_IMG_conv_3x3_i8_c8s_Frame(hC6accel,pSrcBuf_16bpp,pOutBuf_16bpp,inputHeight-2,inputWidth, (char*)p16GaussianCmem,7);
#else

   /*  the 3x3 8 bit mode is line only !!            */
    for(row=0;row<inputHeight-2;row++)
        C6accel_IMG_conv_3x3_i8_c8s(hC6accel,pSrcBuf_16bpp+row*inputWidth,pOutBuf_16bpp+row*inputWidth,inputWidth, (signed char*)p16GaussianCmem,7);
#endif

    END_AND_WRITE_BENCHMARK();

    /* read in reference */
    fread(pRefBuf_16bpp, inputWidth*(inputHeight-2), sizeof(unsigned char), refFd);

    LOG_STRING("\n");

    /* Validate output between native DSP/ref and C6Accel on 8 bit char level
     As undefined border of 2 pixel exists on right hand side and bottom
    */
    for(row=0;row<inputHeight-2;row++){
       if (compare8bitArrays(pOutBuf_16bpp+row*inputWidth, pRefBuf_16bpp+row*inputWidth,inputWidth-2) != 1){
          printf("C6accel_IMG_conv_3x3_i8_c8s() failed on row %d\n",row);
          return 0;
        }
     }

    fclose(refFd);

    /*  IMG_conv_3x3_i16s_c16s()*/
    if ((refFd = fopen(ref_conv_3x3_16s,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_conv_3x3_16s);
       return (0);
       }

    /*  Will reuse 8 bit source file and copy to a 16bpp version with a shift to make it 12 bit data*/
      p16bitInBuf= (short *)pWorkingBuf_16bpp;
      for(i=0;i< inputWidth*inputHeight;i++){
          p16bitInBuf[i] = ((unsigned short)pSrcBuf_16bpp[i])<<4;
       }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,inputWidth*inputHeight*sizeof(short));
    memset (pRefBuf_16bpp,0x00,inputWidth*inputHeight*sizeof(short));

    /*  Set Up required Gaussian */
    memcpy( p16GaussianCmem,gaussian_3x3_16s_1Q15,18);

    LOG_STRING("C6accel_IMG_conv_3x3_i16s_c16s(),");
    START_BENCHMARK();
    /* Call the conv codec, the 16 bit version, width must be <65536 and so just do single lines here
       More optimal would be to do say 80 lines at a time (80*640 = 51200)
       The 3x3 array read lines n,n+1,n+2 from input image to calculate output line n.
       Therefore, the last 2 rows of the image cannot be calculated as they read past the original image.
       So only calulate on height-2 rows for valid data.   */

    p16bitOutBuf = (short*)pOutBuf_16bpp;
#ifdef USE_NEW_FRAME_APIS
    C6accel_IMG_conv_3x3_i16s_c16s_Frame(hC6accel,p16bitInBuf,
                                       p16bitOutBuf,
                                       inputHeight-2,
                                       inputWidth, inputWidth,
                                       p16GaussianCmem, 15);
#else
    /* Call the conv codec, the 16 bit version, width must be <65536 and so just do single lines here
       More optimal would be to do say 80 lines at a time (80*640 = 51200)                             */
    for(row=0;row<(inputHeight-2);row++)
        C6accel_IMG_conv_3x3_i16s_c16s(hC6accel,p16bitInBuf+(row*inputWidth),
                                       p16bitOutBuf+(row*inputWidth),
                                       inputWidth, inputWidth,
                                       p16GaussianCmem, 15);
#endif
    END_AND_WRITE_BENCHMARK();

    /* read in reference */
    fread(pRefBuf_16bpp, inputWidth*(inputHeight-2), sizeof(unsigned short), refFd);

    LOG_STRING("\n");

    /* Validate output between native DSP/ref and C6Accel on 8 bit char level
     As undefined border of 2 pixel exists on right hand side and bottom
    */
    for(row=0;row<inputHeight-2;row++){
      unsigned short *p16Out;
      unsigned short *p16Ref;

      p16Out = (unsigned short*)pOutBuf_16bpp;
      p16Ref = (unsigned short*)pRefBuf_16bpp;

      if (compare16bitArrays(p16Out+row*inputWidth, p16Ref+row*inputWidth,inputWidth-2) != 1) {
        printf("C6accel_IMG_conv_3x3_i16s_c16s() failed on row %d\n",row);
        return 0;
       }
    }

     fclose(srcFd);
     fclose(refFd);

     /*  Release Gaussian CMEM */
     Memory_free(p16GaussianCmem,18,&testfxnsMemParams);

     printf("Tests for Convolution functions successful\n");
     return (1);
}


int c6accel_test_IMG_corr(C6accel_Handle hC6accel,unsigned int inputWidth, unsigned int inputHeight)
{
    unsigned int   i,row;
    FILE           *srcFd, *refFd;
    short          *p16bitBufIn;
    int            *p32bitBufOut;

    /* Pointer to hold a CMem for the mask*/
    short                  *p16GaussianCmem;

    /* Allocate CMEM heap memory for 3x3 short mask*/
     p16GaussianCmem = Memory_alloc(18,&testfxnsMemParams);

    /* Test IMG_corr_3x3_8() kernel */
    /* Open Source Image file */
    if ((srcFd = fopen(srcFileName,"rb")) == NULL) {
       printf("Failed to open srcFd on %s\n",srcFileName);
       return (0);
     }
    if ((refFd = fopen(ref_corr_3x3_8,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_corr_3x3_8);
       return (0);
     }

    /* Read in source file */
    fread(pSrcBuf_16bpp,inputWidth*inputHeight,1,srcFd);

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,inputWidth*inputHeight*sizeof(unsigned int));
    memset (pRefBuf_16bpp,0x00,inputWidth*inputHeight*sizeof(unsigned int));
    /* Set Up required Gaussian */
    memcpy( p16GaussianCmem,gaussian_3x3_8_1Q7,9);

    LOG_STRING("C6accel_IMG_corr_3x3_i8_c8(),");
    START_BENCHMARK();
    /* The 3x3 array read lines n,n+1,n+2 from input image to calculate output line n.
       Therefore, the last 2 rows of the image cannot be calculated as they read past the original image.
       So only calulate on height-2 rows for valid data.   */
    p32bitBufOut=(int*)pOutBuf_16bpp;
#ifdef USE_NEW_FRAME_APIS
       C6accel_IMG_corr_3x3_i8_c8_Frame(hC6accel,pSrcBuf_16bpp,
                                  p32bitBufOut,
                                  inputHeight-2,inputWidth,inputWidth, (unsigned char*)p16GaussianCmem,7,0);
#else
    /* Call the corr codec, width (ie total number of o/p pixels) must be <65536 and so just do single lines here
       More optimal would be to do say 80 lines at a time (80*640 = 51200)
     */
    for(row=0;row<inputHeight-2;row++)
       C6accel_IMG_corr_3x3_i8_c8(hC6accel,pSrcBuf_16bpp+row*inputWidth,
                                  p32bitBufOut+row*inputWidth,
                                  inputWidth,inputWidth, (unsigned char*)p16GaussianCmem,7,0);
#endif
    END_AND_WRITE_BENCHMARK();

    /* read in reference */
    fread(pRefBuf_16bpp, inputWidth*(inputHeight-2), sizeof(unsigned int), refFd);

    LOG_STRING("\n");

    /* Validate output between native DSP/ref and C6Accel on 32 bit int level
       As undefined border of 2 pixel exists on right hand side and bottom
    */
    for(row=0;row<inputHeight-2;row++){
       unsigned int *p32Out;
       unsigned int *p32Ref;

       p32Out = (unsigned int*)pOutBuf_16bpp;
       p32Ref = (unsigned int*)pRefBuf_16bpp;

      if (compare32bitArrays(p32Out+row*inputWidth, p32Ref+row*inputWidth, inputWidth-2) != 1){
         printf("C6accel_IMG_corr_3x3_i8_c8s() failed on row %d\n",row);
         return 0;
       }
    }
    fclose(refFd);

    /* Test IMG_corr_3x3_i16s_c16s() in C6accel*/
    if ((refFd = fopen(ref_corr_3x3_16s,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_corr_3x3_16s);
       return (0);
     }

    /* Will reuse 8 bit source file and copy to a 16bpp version with a shift to make it 12 bit data */
      p16bitBufIn= (short *)pWorkingBuf_16bpp;
      for(i=0;i< inputWidth*inputHeight;i++){
         p16bitBufIn[i] = ((short)pSrcBuf_16bpp[i])<<4;
       }

    /* Clear output arrays*/
    memset (pOutBuf_16bpp,0x00,inputWidth*inputHeight*sizeof(int));
    memset (pRefBuf_16bpp,0x00,inputWidth*inputHeight*sizeof(int));
    /* Set Up required Gaussian */
    memcpy( p16GaussianCmem,gaussian_3x3_16s_1Q15,18);

    LOG_STRING("C6accel_IMG_corr_3x3_i16s_c16s(),");
    START_BENCHMARK();

    /* Call the corr codec, width (ie total number of o/p pixels) must be <65536 and so just do single lines here
       More optimal would be to do say 80 lines at a time (80*640 = 51200)
       The 3x3 array read lines n,n+1,n+2 from input image to calculate output line n.
       Therefore, the last 2 rows of the image cannot be calculated as they read past the original image.
       So only calulate on height-2 rows for valid data.   */
    p32bitBufOut=(int *)pOutBuf_16bpp;
#ifdef USE_NEW_FRAME_APIS
       C6accel_IMG_corr_3x3_i16s_c16s_Frame(hC6accel,p16bitBufIn,
                                      p32bitBufOut,
                                      inputHeight-2,inputWidth, inputWidth,
                                      p16GaussianCmem, 15,0);
#else
    /* Call the corr codec, width (ie total number of o/p pixels) must be <65536 and so just do single lines here
       More optimal would be to do say 80 lines at a time (80*640 = 51200)                                        */
    for(row=0;row<inputHeight-2;row++)
       C6accel_IMG_corr_3x3_i16s_c16s(hC6accel,p16bitBufIn+row*inputWidth,
                                      p32bitBufOut+row*inputWidth,
                                      inputWidth, inputWidth,
                                      p16GaussianCmem, 15,0);
#endif
    END_AND_WRITE_BENCHMARK();
    /* read in reference */
    fread(pRefBuf_16bpp, inputWidth*(inputHeight-2), sizeof(int), refFd);
    LOG_STRING("\n");


    /* Validate output between native DSP/ref and C6Accel on 32 bit int level
     As undefined border of 2 pixel exists on right hand side and bottom
     */
    for(row=0;row<inputHeight-2;row++){
       unsigned int *p32Out;
       unsigned int *p32Ref;

       p32Out = (unsigned int*)pOutBuf_16bpp;
       p32Ref = (unsigned int*)pRefBuf_16bpp;

      if (compare32bitArrays(p32Out+row*inputWidth, p32Ref+row*inputWidth, inputWidth-2) != 1){
         printf("C6accel_IMG_corr_3x3_16s() failed on row %d\n",row);
         return 0;
       }
    }

     fclose(srcFd);
     fclose(refFd);
     // Release Gaussian CMEM
     Memory_free(p16GaussianCmem,18,&testfxnsMemParams);

     printf("Tests for Correlation functions successful\n");
     return (1);
}

/*
 * Test all possible IMG Sobel combinations in this function
 */
int c6accel_test_IMG_sobel(C6accel_Handle hC6accel,unsigned int inputWidth, unsigned int inputHeight)
{
    unsigned int                i,row;
    FILE *srcFd, *refFd;
    unsigned short *p16bitBuf;

    /* Test IMG_sobel_3x3_8() in c6accel */
    if ((srcFd = fopen(srcFileName,"rb")) == NULL) {
       printf("Failed to open srcFd on %s\n",srcFileName);
       return (0);
     }
    if ((refFd = fopen(ref_sobel_3x3_8,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_sobel_3x3_8);
       return (0);
     }

    /* Read in source file */
    fread(pSrcBuf_16bpp,inputWidth*inputHeight,1,srcFd);

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,inputWidth*inputHeight);
    memset (pRefBuf_16bpp,0x00,inputWidth*inputHeight);

    LOG_STRING("C6accel_IMG_sobel_3x3_8(),");
    START_BENCHMARK();

    /* Call the sobel codec to perform edge detection */
    /* Sobel function handles the adjustment 1 line border with undefined pixels internally */
#ifdef SUPPORT_ASYNC
    C6accel_IMG_sobel_3x3_8(hC6accel,pSrcBuf_16bpp,pOutBuf_16bpp,inputWidth, inputHeight, SYNC);
#else
    C6accel_IMG_sobel_3x3_8(hC6accel,pSrcBuf_16bpp,pOutBuf_16bpp,inputWidth, inputHeight);
#endif
    END_AND_WRITE_BENCHMARK();

    /* read in reference */
    fread(pRefBuf_16bpp, inputWidth*inputHeight, sizeof(unsigned char), refFd);
    LOG_STRING("\n");

    /* Validate output between native DSP/ref and C6Accel on 8 bit char level
       As undefined border of 1 pixel exists must test on a line by line basis
     */
    for(row=0;row<inputHeight-2;row++) {
       if (compare8bitArrays(pOutBuf_16bpp+row*inputWidth+1, pRefBuf_16bpp+row*inputWidth+1,inputWidth-2) != 1){
         printf("C6accel_IMG_sobel_3x3_8() failed on row %d\n",row);
         return 0;
        }
     }

#ifdef SUPPORT_ASYNC

    /* Now do same test on ASYNC interface
    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,inputWidth*inputHeight);

    LOG_STRING("C6accel_IMG_sobel_3x3_8_async(),");
    START_BENCHMARK();

    /* Call the sobel codec to perform edge detection */
    /* Sobel function handles the adjustment 1 line border with undefined pixels internally */
    C6accel_IMG_sobel_3x3_8(hC6accel,pSrcBuf_16bpp,pOutBuf_16bpp,inputWidth, inputHeight, ASYNC);
    END_AND_WRITE_BENCHMARK();

    // Now wait for the callback
    LOG_STRING(" ,C6accel_IMG_sobel_3x3_8() wait for callback,");
    START_BENCHMARK();
    C6accel_waitAsyncCall(hC6accel);
    END_AND_WRITE_BENCHMARK();

    LOG_STRING("\n");

    /* Validate output between native DSP/ref and C6Accel on 8 bit char level
       As undefined border of 1 pixel exists must test on a line by line basis
     */
    for(row=0;row<inputHeight-2;row++) {
       if (compare8bitArrays(pOutBuf_16bpp+row*inputWidth+1, pRefBuf_16bpp+row*inputWidth+1,inputWidth-2) != 1){
         printf("C6accel_IMG_sobel_3x3_8() failed on row %d\n",row);
         return 0;
        }
     }

#endif
     fclose(refFd);

    /* Test IMG_sobel_3x3_16() in c6accel */
    if ((refFd = fopen(ref_sobel_3x3_16,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_sobel_3x3_16);
       return (0);
       }

    /* Will reuse 8 bit source file and copy to a 16bpp version with a shift to make it 12 bit data*/
    p16bitBuf= (unsigned short *)pWorkingBuf_16bpp;
    for(i=0;i< inputWidth*inputHeight;i++)
         {p16bitBuf[i] = ((unsigned short)pSrcBuf_16bpp[i])<<4;
         }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,inputWidth*inputHeight*sizeof(short));
    memset (pRefBuf_16bpp,0x00,inputWidth*inputHeight*sizeof(short));

    LOG_STRING("C6accel_IMG_sobel_3x3_16(),");
    START_BENCHMARK();
    /* Call the sobel codec to perform edge detection */
    /* Sobel function handles the adjustment 1 line border with undefined pixels internally */
    C6accel_IMG_sobel_3x3_16(hC6accel,(unsigned short*)pWorkingBuf_16bpp,(unsigned short*)pOutBuf_16bpp,inputWidth, inputHeight);

    END_AND_WRITE_BENCHMARK();

    /* read in reference */
    fread(pRefBuf_16bpp, inputWidth*inputHeight, sizeof(unsigned short), refFd);

    LOG_STRING("\n");

    /* Validate output between native DSP/ref and C6Accel on 16 bit short level
     As undefined border of 1 pixel exists must test on a line by line basis
     */
    for(row=1;row<inputHeight-1;row++){
       unsigned short* p16bitOut, *p16bitRef;
       p16bitOut = (unsigned short*)pOutBuf_16bpp;
       p16bitRef = (unsigned short*)pRefBuf_16bpp;

       if (compare16bitArrays(p16bitOut+row*inputWidth+1, p16bitRef+row*inputWidth+1,inputWidth-2) != 1){
         printf("C6accel_IMG_sobel_3x3_16() failed on row %d\n",row);
         return 0;
        }
     }

     fclose(srcFd);
     fclose(refFd);

     printf("Tests for Sobel Filter successful\n");
     return (1);
}

/*
 * Test all possible IMG muls kernels in c6accel
 */
int c6accel_test_IMG_muls(C6accel_Handle hC6accel,unsigned int inputWidth, unsigned int inputHeight)
{
    unsigned int                i;
    FILE *srcFd, *refFd;
    short *p16bitBuf;

    /* Test IMG_muls_8() in C6accel*/
    if ((srcFd = fopen(srcFileName,"rb")) == NULL) {
       printf("Failed to open srcFd on %s\n",srcFileName);
       return (0);
     }
    if ((refFd = fopen(ref_muls_8,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_muls_8);
       return (0);
     }

    /* Read in source file */
    fread(pSrcBuf_16bpp,inputWidth*inputHeight,1,srcFd);

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,inputWidth*inputHeight);
    memset (pRefBuf_16bpp,0x00,inputWidth*inputHeight);

    LOG_STRING("C6accel_IMG_muls_8(),");
    START_BENCHMARK();
    /* Call the mulS_8 to multiply by 2 */
    C6accel_IMG_mulS_8(hC6accel,pSrcBuf_16bpp,(short*)pOutBuf_16bpp,2,inputWidth*inputHeight);
    END_AND_WRITE_BENCHMARK();

    /* read in reference */
    fread(pRefBuf_16bpp, inputWidth*inputHeight, sizeof(short), refFd);
    LOG_STRING("\n");

    /* Validate output between native DSP/ref and C6Accel on 8 bit char level*/
    if (compare16bitArrays((unsigned short*)pOutBuf_16bpp, (unsigned short*)pRefBuf_16bpp,inputWidth*inputHeight) != 1){
      printf("C6accel_IMG_muls_8() failed\n");
      return 0;
    }
    fclose(refFd);

    /*Test the IMG_muls_16() in c6accel*/
    if ((refFd = fopen(ref_muls_16,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_muls_16);
       return (0);
     }

    /* Will reuse 8 bit source file and copy to a 16bpp version with a shift to make it 12 bit data */
    p16bitBuf= (short *)pWorkingBuf_16bpp;
    for(i=0;i< inputWidth*inputHeight;i++)
         {p16bitBuf[i] = ((short)pSrcBuf_16bpp[i])<<4;
         }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,inputWidth*inputHeight*sizeof(int));
    memset (pRefBuf_16bpp,0x00,inputWidth*inputHeight*sizeof(int));

    LOG_STRING("C6accel_IMG_muls_16(),");
    START_BENCHMARK();

    /* Call the muls to multiply by 2 */
    C6accel_IMG_mulS_16s(hC6accel,p16bitBuf,(int*)pOutBuf_16bpp,2,inputWidth*inputHeight);

    END_AND_WRITE_BENCHMARK();

    /* read in reference */
    fread(pRefBuf_16bpp, inputWidth*inputHeight, sizeof(int), refFd);

    LOG_STRING("\n");

    /* Validate output between native DSP/ref and C6Accel on 16 bit short level */
    if (compare32bitArrays((unsigned int*)pOutBuf_16bpp,(unsigned int*) pRefBuf_16bpp,inputWidth*inputHeight) != 1){
       printf("C6accel_IMG_muls_16() failed\n");
       return 0;
     }

     fclose(srcFd);
     fclose(refFd);

     printf("Test for Image multiplication functions successful\n");
     return (1);
}

/*
 * Test all possible IMG adds combinations in this function
 */
int c6accel_test_IMG_adds(C6accel_Handle hC6accel,unsigned int inputWidth, unsigned int inputHeight)
{
    unsigned int                i;
    FILE *srcFd, *refFd;
    short *p16bitBuf,*p16bitInBuf;

    /* Test IMG_adds_8() in c6accel */
    /* Open source image file */
    if ((srcFd = fopen(srcFileName,"rb")) == NULL) {
       printf("Failed to open srcFd on %s\n",srcFileName);
       return (0);
    }
    if ((refFd = fopen(ref_adds_8,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_adds_8);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,inputWidth*inputHeight);
    memset (pRefBuf_16bpp,0x00,inputWidth*inputHeight);

    /* Inplace Adding a constant to an image Source = OutBuf
      Read in source file to output buffer
     */
    fread(pOutBuf_16bpp,inputWidth*inputHeight,1,srcFd);

    /* Read in source file */
    fread(pSrcBuf_16bpp,inputWidth*inputHeight,1,srcFd);

    LOG_STRING("C6accel_IMG_adds_8(),");
    START_BENCHMARK();

    /* Call the addS_8 to add 1 */
    C6accel_IMG_addS_8(hC6accel,(char*)pOutBuf_16bpp,(char*)pOutBuf_16bpp,1,inputWidth*inputHeight);
    END_AND_WRITE_BENCHMARK();

    /*  read in reference output */
    fread(pRefBuf_16bpp, inputWidth*inputHeight, sizeof(char), refFd);
    LOG_STRING("\n");

    /* Validate output between native DSP/ref and C6Accel on 8 bit char level */
    if (compare8bitArrays(pOutBuf_16bpp, pRefBuf_16bpp,inputWidth*inputHeight) != 1){

      printf("C6accel_IMG_adds_8() failed\n");
      return 0;
    }
    fclose(refFd);

    /* Test IMG_adds_16() in c6accel */
    /* Open reference ouput file */
    if ((refFd = fopen(ref_adds_16,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_adds_16);
       return (0);
    }

    /* Clear output arrays*/
    memset (pOutBuf_16bpp,0x00,inputWidth*inputHeight*sizeof(short));
    memset (pRefBuf_16bpp,0x00,inputWidth*inputHeight*sizeof(short));

    /* Will reuse 8 bit source file and copy to a 16bpp version with a shift to make it 12 bit data*/
      p16bitBuf= (short *)pWorkingBuf_16bpp;
    /*Enable inplace processing */
    p16bitInBuf= (short *)pOutBuf_16bpp;
    for(i=0;i< inputWidth*inputHeight;i++){
       p16bitBuf[i] = ((short)pSrcBuf_16bpp[i])<<4;
       p16bitInBuf[i] = p16bitBuf[i];
    }

    LOG_STRING("C6accel_IMG_adds_16(),");
    START_BENCHMARK();
    /* Call the adds_16 to add 1*/
    C6accel_IMG_addS_16s(hC6accel,p16bitInBuf,(short*)pOutBuf_16bpp,1,inputWidth*inputHeight);
    END_AND_WRITE_BENCHMARK();

    /* read in reference file */
    fread(pRefBuf_16bpp, inputWidth*inputHeight, sizeof(short), refFd);
    LOG_STRING("\n");

    /* Validate output between native DSP/ref and C6Accel on 16 bit short level */
    if (compare16bitArrays((unsigned short*)pOutBuf_16bpp,(unsigned short*) pRefBuf_16bpp,inputWidth*inputHeight) != 1){
      printf("C6accel_IMG_adds_16() failed\n");
      return 0;
    }

    fclose(srcFd);
    fclose(refFd);

    printf("Tests for Image addition functions successful\n");
    return (1);
}

/*
 * Test all possible IMG subs combinations in this function
 */
int c6accel_test_IMG_subs(C6accel_Handle hC6accel,unsigned int inputWidth, unsigned int inputHeight)
{
    unsigned int                i;
    FILE *srcFd, *refFd;
    short *p16bitBuf,*p16bitInBuf;

    /* Test IMG_subs_8() in c6accel */
    /*Open source image file */
    if ((srcFd = fopen(srcFileName,"rb")) == NULL) {
       printf("Failed to open srcFd on %s\n",srcFileName);
       return (0);
     }
    if ((refFd = fopen(ref_subs_8,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_subs_8);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,inputWidth*inputHeight);
    memset (pRefBuf_16bpp,0x00,inputWidth*inputHeight);

    /* Inplace Adding a constant to an image Source = OutBuf
       Read in source file to output buffer
     */
    fread(pOutBuf_16bpp,inputWidth*inputHeight,1,srcFd);

    /* Read in source file */
    fread(pSrcBuf_16bpp,inputWidth*inputHeight,1,srcFd);

    LOG_STRING("C6accel_IMG_subs_8(),");
    START_BENCHMARK();
    /* Call the subS_8 to sub 1 */
    C6accel_IMG_subS_8(hC6accel,(char*)pOutBuf_16bpp,(char*)pOutBuf_16bpp,1,inputWidth*inputHeight);
    END_AND_WRITE_BENCHMARK();

    /* read in reference output file */
    fread(pRefBuf_16bpp, inputWidth*inputHeight, sizeof(short), refFd);
    LOG_STRING("\n");

    /* Validate output between native DSP/ref and C6Accel on 8 bit char level */
    if (compare8bitArrays(pOutBuf_16bpp, pRefBuf_16bpp,inputWidth*inputHeight) != 1){
       printf("C6accel_IMG_subs_8() failed\n");
       return 0;
     }

    fclose(refFd);

    /* Test IMG_subs_16() in c6accel */
    if ((refFd = fopen(ref_subs_16,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_subs_16);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,inputWidth*inputHeight*sizeof(short));
    memset (pRefBuf_16bpp,0x00,inputWidth*inputHeight*sizeof(short));

    /* Will reuse 8 bit source file and copy to a 16bpp version with a shift to make it 12 bit data */
    p16bitBuf= (short *)pWorkingBuf_16bpp;

    /*Enable inplace processing*/
    p16bitInBuf= (short *)pOutBuf_16bpp;
    for(i=0;i< inputWidth*inputHeight;i++){
       p16bitBuf[i] = ((short)pSrcBuf_16bpp[i])<<4;
       p16bitInBuf[i] = p16bitBuf[i];
     }

    LOG_STRING("C6accel_IMG_subs_16s(),");
    START_BENCHMARK();
    /* Call the subs_16 to add 1*/
    C6accel_IMG_subS_16s(hC6accel,p16bitInBuf,(short*)pOutBuf_16bpp,1,inputWidth*inputHeight);
    END_AND_WRITE_BENCHMARK();

    /* read in reference ouput from file */
    fread(pRefBuf_16bpp, inputWidth*inputHeight, sizeof(short), refFd);
    LOG_STRING("\n");

    /* Validate output between native DSP/ref and C6Accel on 16 bit short level */
    if (compare16bitArrays((unsigned short*)pOutBuf_16bpp,(unsigned short*) pRefBuf_16bpp,inputWidth*inputHeight) != 1){
       printf("C6accel_IMG_subs_16() failed\n");
       return 0;
    }

    fclose(srcFd);
    fclose(refFd);

    printf("Tests for Image substraction functions successful\n");
    return (1);
}

/*
 * Test all possible IMG yuv demux + PL420 to RGB combinations in this function
 */

const short yuv2rgb_coeff[] = { 0x2000, 0x2BDD, -0x0AC5, -0x1658, 0x3770 };

int c6accel_test_IMG_YC_demux(C6accel_Handle hC6accel,unsigned int inputWidth, unsigned int inputHeight)
{
    unsigned int                i,j;
    FILE *srcFd, *refFd;
    short *p16bitBuf;
    unsigned char *pY, *pCb, *pCr;
    short *psY, *psCb, *psCr;

    int tempInt;
    unsigned int *pInt;
    unsigned char *pChar;

    /* Create a CMem heap array that can be translated for DSP */
    short *pYUV2RGB_Cmem;
    pYUV2RGB_Cmem = Memory_alloc(5*sizeof(short),&testfxnsMemParams);

    /* Test IMG_YC_demux() kenrel in c6accel */
    if ((srcFd = fopen(src_yuyv,"rb")) == NULL) {
       printf("Failed to open srcFd on %s\n",src_yuyv);
       return (0);
    }
    if ((refFd = fopen(ref_yc_demux_8,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_yc_demux_8);
       return (0);
    }

    /* Read in source file which is Bigendian 422 CbY1CrY0*/
    fread(pSrcBuf_16bpp,inputWidth*inputHeight,sizeof(short),srcFd);

    /* and swap to LE 422 Y0CrY1Cb*/
    pChar = pSrcBuf_16bpp;
    pInt = (unsigned int*)pSrcBuf_16bpp;
    for(i=0;i<inputWidth*inputHeight/2;i++){
       //Cb
       tempInt = (*pChar++) << 24;
       //Y1
       tempInt |= *pChar++ ;
       //Cr
       tempInt |=(*pChar++) << 8;
       //Y0
       tempInt |=(*pChar++)<< 16;

       *pInt++ = tempInt;
     }


    /* Clear output arrays
       split each to hold 3 separate arrays of chars
       y[h*w]
       cb[h*w/2]
       cr[h*w/2]
    */
    memset (pOutBuf_16bpp,0x00,inputWidth*inputHeight*2*sizeof(char));
    memset (pRefBuf_16bpp,0x00,inputWidth*inputHeight*2*sizeof(char));
    LOG_STRING("C6accel_IMG_yc_demux_le8_c(),");

    pY = pOutBuf_16bpp;
    pCb = pY + inputWidth*inputHeight;
    pCr = pCb + (inputWidth*inputHeight)/2;
    START_BENCHMARK();
    /* Call the yc_demux_be8 to generate 422*/
    C6accel_IMG_yc_demux_le8_8(hC6accel,inputWidth*inputHeight,(unsigned char*)pSrcBuf_16bpp,pY, pCr, pCb);
    END_AND_WRITE_BENCHMARK();

    /* read in reference output */
    fread(pRefBuf_16bpp, inputWidth*inputHeight*2, sizeof(char), refFd);
    LOG_STRING("\n");

    /* Validate output between native DSP/ref and C6Accel on 8 bit char level */
    if (compare8bitArrays(pOutBuf_16bpp, pRefBuf_16bpp,inputWidth*inputHeight*2) != 1){
      printf("C6accel_IMG_yc_demux_le8_c() failed\n");
      return 0;
    }

    fclose(refFd);

    /* Now we have YUV422 planar in memory will use for 422 to RGB function */
    if ((refFd = fopen(ref_rgb_565,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_rgb_565);
       return (0);
     }

    /* Pick up 422Pl source buffer from reference buffer of previous step*/
    memcpy(pWorkingBuf_16bpp,pRefBuf_16bpp,inputWidth*inputHeight*2*sizeof(char));
    /*Copy YUV2RGB to cmem pointer */
    memcpy(pYUV2RGB_Cmem,yuv2rgb_coeff,5*sizeof(short));

    /* Clear output arrays for 16 bit rgb */
    memset (pOutBuf_16bpp,0x00,inputWidth*inputHeight*sizeof(short));
    memset (pRefBuf_16bpp,0x00,inputWidth*inputHeight*sizeof(short));

    pY = pWorkingBuf_16bpp;
    pCb = pY + inputWidth*inputHeight;
    pCr = pCb + (inputWidth*inputHeight)/2;

    LOG_STRING("C6accel_IMG_yuv422pl_to_rgb565(),");
    START_BENCHMARK();

    /* Call the _ycbcr422pl_to_rgb565 as we have 422P data in memory */
    C6accel_IMG_ycbcr422pl_to_rgb565(hC6accel,pYUV2RGB_Cmem,pY, pCb, pCr,(unsigned short*)pOutBuf_16bpp,inputHeight*inputWidth);
    END_AND_WRITE_BENCHMARK();

    /* read in reference output */
    fread(pRefBuf_16bpp, inputWidth*inputHeight, sizeof(short), refFd);
    LOG_STRING("\n");

    /* Validate output between native DSP/ref and C6Accel on 16 bit short level */
    if (compare16bitArrays((unsigned short*)pOutBuf_16bpp,(unsigned short*) pRefBuf_16bpp,inputWidth*inputHeight) != 1){
      printf("C6accel_IMG_yuv422pl_to_rgb565() failed\n");
      return 0;
    }

    fclose(refFd);

    /*Test for 420pltoRGB565 kernel in c6accel */
    /* Open reference output file */
    if ((refFd = fopen(ref_yuv420_rgb_565,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_rgb_565);
       return (0);
     }

     /* Now we have YUV422 planar in memory will use for 422 to RGB function
       Convert the 422pl inputs from previous test to 420pl data
       by subsampling the chrominance data horizontally by 2
     */

     /*Do in place subsampling of Cb and Cr */
     for (i=0,j=0;i<inputWidth*inputHeight/4;i++,j+=2){
         *(pCb+i)= *(pCb+j);
         *(pCr+i)= *(pCr+j);
        }

     /* Clear output arrays for 16 bit rgb */
     memset (pOutBuf_16bpp,0x00,inputWidth*inputHeight*sizeof(short));
     memset (pRefBuf_16bpp,0x00,inputWidth*inputHeight*sizeof(short));

     LOG_STRING("C6accel_IMG_yuv420pl_to_rgb565(),");
     START_BENCHMARK();

     /* Call the _yun420pl_to_rgb565 as we have 420P data in memory */
     C6accel_IMG_yuv420pl_to_rgb565(hC6accel,pYUV2RGB_Cmem,inputHeight,inputWidth,pY, pCb, pCr,(unsigned short*)pOutBuf_16bpp);
     END_AND_WRITE_BENCHMARK();

    /* read in reference */
    fread(pRefBuf_16bpp, inputWidth*inputHeight, sizeof(short), refFd);
    LOG_STRING("\n");

    /* Validate output between native DSP/ref and C6Accel on 16 bit short level */
    if (compare16bitArrays((unsigned short*)pOutBuf_16bpp,(unsigned short*) pRefBuf_16bpp,inputWidth*inputHeight) != 1){
      printf("C6accel_IMG_yuv420pl_to_rgb565() failed\n");
      return 0;
    }

    fclose(refFd);

    /* Test IMG_yc_demux_le16_16() kernel in c6accel */
    if ((refFd = fopen(ref_yc_demux_16,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_yc_demux_16);
       return (0);
    }

    /* Will reuse 8 bit 422planar source file and copy to a 16bpp YUYV version*/
    p16bitBuf= (short *)pWorkingBuf_16bpp;
    for(i=0;i< inputWidth*inputHeight*2;i++){
       p16bitBuf[i] = ((short)pSrcBuf_16bpp[i]);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,inputWidth*inputHeight*2*sizeof(short));
    memset (pRefBuf_16bpp,0x00,inputWidth*inputHeight*2*sizeof(short));

    LOG_STRING("C6accel_IMG_yc_demux_le16_16(),");
    psY = (short*)pOutBuf_16bpp;
    psCb = psY + inputWidth*inputHeight;
    psCr = psCb + (inputWidth*inputHeight)/2;
    START_BENCHMARK();

    /* Call the yc_demux_le16_16 */
    C6accel_IMG_yc_demux_le16_16(hC6accel,inputWidth*inputHeight,(unsigned short*)p16bitBuf,psY, psCr, psCb);
    END_AND_WRITE_BENCHMARK();

    /* read in reference output */
    fread(pRefBuf_16bpp, inputWidth*inputHeight*2, sizeof(short), refFd);
    LOG_STRING("\n");

    /* Validate output between native DSP/ref and C6Accel on 16 bit short level */
    if (compare16bitArrays((unsigned short*)pOutBuf_16bpp,(unsigned short*) pRefBuf_16bpp,inputWidth*inputHeight*2) != 1){
       printf("C6accel_IMG_yc_demux_le16_16() failed\n");
       return 0;
     }

     fclose(srcFd);
     fclose(refFd);

     /* Now we have YUV422 16 bit planar in memory will use for 422 to RGB function */
    if ((refFd = fopen(ref_rgb_565_yuv16,"rb")) == NULL){
       printf("Failed to open refFd on %s\n",ref_rgb_565_yuv16);
       return (0);
     }

    /* Pick up source buffer from reference buffer of previous step*/
    memcpy(pSrcBuf_16bpp,pRefBuf_16bpp,inputWidth*inputHeight*sizeof(short)*2);

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,inputWidth*inputHeight*sizeof(short));
    memset (pRefBuf_16bpp,0x00,inputWidth*inputHeight*sizeof(short));

    psY = (short*)pSrcBuf_16bpp;
    psCb = psY + inputWidth*inputHeight;
    psCr = psCb + (inputWidth*inputHeight)/2;

    LOG_STRING("C6accel_IMG_yuv422pl16_to_rgb565(),");
    START_BENCHMARK();

    /* Call the _ycbcr422pl_to_rgb565 as we have 422P data in memory */
    C6accel_IMG_ycbcr422pl16_to_rgb565(hC6accel,pYUV2RGB_Cmem,psY, psCb, psCr,(unsigned short*)pOutBuf_16bpp,inputHeight*inputWidth);
    END_AND_WRITE_BENCHMARK();

    /* read in reference output from file */
    fread(pRefBuf_16bpp, inputWidth*inputHeight, sizeof(short), refFd);
    LOG_STRING("\n");

    /* Validate output between native DSP/ref and C6Accel on 16 bit short level */
    if (compare16bitArrays((unsigned short*)pOutBuf_16bpp,(unsigned short*) pRefBuf_16bpp,inputWidth*inputHeight) != 1){
      printf("C6accel_IMG_yuv420pl16_to_rgb565() failed\n");
      return 0;
    }
    fclose(refFd);

    if ((refFd = fopen(ref_yuv420pl16_rgb_565,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_yuv420pl16_rgb_565);
       return (0);
     }

    /* Now we have YUV422 planar in memory will use for 422 to RGB function
    Convert the 422pl inputs from previous test to 420pl data
    by subsampling the chrominance data horizontally by 2
    */
    /* Do in place subsampling of Cb and Cr */
    for (i=0,j=0;i<inputWidth*inputHeight/4;i++,j+=2){
         *(psCb+i)= *(psCb+j);
         *(psCr+i)= *(psCr+j);
     }

    /* Clear output arrays for 16 bit rgb */
    memset (pOutBuf_16bpp,0x00,inputWidth*inputHeight*sizeof(short));
    memset (pRefBuf_16bpp,0x00,inputWidth*inputHeight*sizeof(short));

    LOG_STRING("C6accel_IMG_yuv420pl16_to_rgb565(),");
    START_BENCHMARK();

    /* Call the _yun420pl_to_rgb565 as we have 420P data in memory */
    C6accel_IMG_yuv420pl16_to_rgb565(hC6accel,pYUV2RGB_Cmem,inputHeight,inputWidth,psY, psCb, psCr,(unsigned short*)pOutBuf_16bpp);
    END_AND_WRITE_BENCHMARK();

    /* read in reference output*/
    fread(pRefBuf_16bpp, inputWidth*inputHeight, sizeof(short), refFd);
    LOG_STRING("\n");

    /* Validate output between native DSP/ref and C6Accel on 16 bit short level */
    if (compare16bitArrays((unsigned short*)pOutBuf_16bpp,(unsigned short*) pRefBuf_16bpp,inputWidth*inputHeight) != 1){
      printf("C6accel_IMG_yuv420pl16_to_rgb565() failed\n");
      return 0;
     }
    fclose(refFd);

    /* Free YUV2RGB CMEM array*/
    Memory_free(pYUV2RGB_Cmem,sizeof(short)*5,&testfxnsMemParams);
    printf("Tests for Color Space Conversion functions successful\n");
    return (1);
}

int c6accel_test_IMG_YUV422PLtoYUV422SP(C6accel_Handle hC6accel,unsigned int num_lines, 
    unsigned int width,
    unsigned int dst_pitch, 
    unsigned int src_y_pitch,
    unsigned int src_cbcr_pitch)
{
    unsigned int                i,j;
    FILE *srcFd, *refFd;
    short *p16bitBuf;
    unsigned char *y_dst, *cbcr_dst;
    unsigned char *input_y, *input_cb, *input_cr;
    int status;

      
    /* Create a CMem heap array that can be translated for DSP */
    unsigned char *pCbCr_Cmem;
    pCbCr_Cmem = Memory_alloc(num_lines*dst_pitch*sizeof(unsigned char),&testfxnsMemParams);
    
    input_y = (unsigned char *)pSrcBuf_16bpp;
    input_cb = (unsigned char *)pWorkingBuf_16bpp;
    input_cr = (unsigned char *)pWorkingBuf2_16bpp;
    y_dst=(unsigned char *)pRefBuf_16bpp;
    /* Test IMG_YC_demux() kenrel in c6accel */
    if ((refFd = fopen(ref_input_y_422pl,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_input_y_422pl);
        
     }
    fread(input_y, num_lines*src_y_pitch, sizeof(unsigned char), refFd);
    fclose(refFd);

    if ((refFd = fopen(ref_input_cb_422pl,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_input_cb_422pl);
         
      }
     fread(input_cb, num_lines*src_cbcr_pitch, sizeof(unsigned char), refFd);
     fclose(refFd);

     if ((refFd = fopen(ref_input_cr_422pl,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_input_cr_422pl);
         
      }
      fread(input_cr, num_lines*src_cbcr_pitch, sizeof(unsigned char), refFd);
      fclose(refFd);

   
    memset (pOutBuf_16bpp,0x00,num_lines*dst_pitch*sizeof(unsigned char));
    memset (pRefBuf_16bpp,0x00,num_lines*dst_pitch*sizeof(unsigned char));
    LOG_STRING("C6accel_IMG_ycbcr422pl_to_ycbcr422sp(),");
    y_dst = (unsigned char *)pOutBuf_16bpp;
    cbcr_dst = (unsigned char *)pCbCr_Cmem;
    START_BENCHMARK();
    
    /* Call the ycbcr422pl_to_ycbcr422sp*/
    status = C6accel_IMG_ycbcr422pl_to_ycbcr422sp( hC6accel,y_dst, cbcr_dst,input_y,input_cb,input_cr,num_lines,width,dst_pitch,src_y_pitch,src_cbcr_pitch);
    END_AND_WRITE_BENCHMARK();

    if ((refFd = fopen(ref_dst_cbcr_422sp,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_dst_cbcr_422sp);
         
     }
    /* read in reference output */
    fread(pRefBuf_16bpp, num_lines*dst_pitch, sizeof(unsigned char), refFd);
    LOG_STRING("\n");

    /* Validate output between native DSP/ref and C6Accel on 8 bit char level */
    if (compare8bitArrays(pCbCr_Cmem, pRefBuf_16bpp,num_lines*dst_pitch) != 1){
      printf("C6accel_IMG_ycbcr422pl_to_ycbcr422sp() failed\n");
      return 0;
    }
    printf("C6accel_IMG_ycbcr422pl_to_ycbcr422sp() successful\n");
    fclose(refFd);
    /* Free YUV2RGB CMEM array*/
    Memory_free(pCbCr_Cmem,sizeof(unsigned char)*num_lines*dst_pitch,&testfxnsMemParams);
    return 1;
}

int c6accel_test_IMG_YUV422SPtoYUV422ILE(C6accel_Handle hC6accel,unsigned int num_lines, 
    unsigned int width,
    unsigned int src_pitch, 
    unsigned int dst_ycbcr_pitch
)
{
    unsigned int                i,j;
    FILE *srcFd, *refFd;
    short *p16bitBuf;
    unsigned char *ycbcr_dst;
    unsigned char *src_y, *src_cbcr;
     
    src_y = (unsigned char *)pSrcBuf_16bpp;
    src_cbcr = (unsigned char *)pWorkingBuf_16bpp;
    ycbcr_dst=(unsigned char *)pRefBuf_16bpp;
    /* Test IMG_YC_demux() kenrel in c6accel */
     if ((refFd = fopen(ref_src_y_422sp,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_src_y_422sp);
         
     }
     fread(src_y, num_lines*src_pitch, sizeof(unsigned char), refFd);
     fclose(refFd);

     if ((refFd = fopen(ref_src_cbcr_422sp,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_src_cbcr_422sp);
         
      }
     fread(src_cbcr,num_lines*src_pitch, sizeof(unsigned char), refFd);
     fclose(refFd);
       
    memset (pOutBuf_16bpp,0x00,num_lines*dst_ycbcr_pitch*sizeof(unsigned char));
    memset (pRefBuf_16bpp,0x00,num_lines*dst_ycbcr_pitch*sizeof(unsigned char));
    LOG_STRING("C6accel_IMG_ycbcr422pl_to_ycbcr422sp(),");
    ycbcr_dst = (unsigned char *)pOutBuf_16bpp;
    
    START_BENCHMARK();
    /* Call the ycbcr422sp_to_ycbcr422ile*/
    C6accel_IMG_ycbcr422sp_to_ycbcr422ile( hC6accel,src_y,src_cbcr,ycbcr_dst,num_lines,width,src_pitch,dst_ycbcr_pitch);
    END_AND_WRITE_BENCHMARK();

    if ((refFd = fopen(ref_dst_ycbcr_422ile,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_dst_ycbcr_422ile);
         
     }

    /* read in reference output */
    fread(pRefBuf_16bpp, num_lines*dst_ycbcr_pitch, sizeof(unsigned char), refFd);
    LOG_STRING("\n");

    /* Validate output between native DSP/ref and C6Accel on 8 bit char level */
    if (compare8bitArrays(pOutBuf_16bpp, pRefBuf_16bpp,num_lines*dst_ycbcr_pitch) != 1){
      printf("C6accel_IMG_ycbcr422sp_to_ycbcr422ile() failed\n");
      return 0;
    }
    printf("C6accel_IMG_ycbcr422sp_to_ycbcr422ile() successful\n");
    fclose(refFd);
    return 1;
}

int c6accel_test_IMG_YUV422SPtoYUV420PL(C6accel_Handle hC6accel,unsigned int num_lines, 
    unsigned int width,
    unsigned int src_pitch, 
    unsigned int dst_y_pitch,
    unsigned int dst_cbcr_pitch
)
{
    unsigned int            i,j;
    FILE *srcFd, *refFd;
    short *p16bitBuf;
    int status;
    unsigned char *src_y,*src_cbcr;
    unsigned char *y_dst, *cb_dst, *cr_dst;
    /* Create a CMem heap array that can be translated for DSP */
    unsigned char *pCr_Cmem;
    pCr_Cmem = Memory_alloc(num_lines*dst_cbcr_pitch*sizeof(unsigned char),&testfxnsMemParams);
    src_y = (unsigned char *)pSrcBuf_16bpp;
    src_cbcr = (unsigned char *)pWorkingBuf_16bpp;
    y_dst = (unsigned char *)pWorkingBuf2_16bpp;
    cb_dst = (unsigned char *)pOutBuf_16bpp;
    cr_dst = (unsigned char *)pCr_Cmem;

     /* Test IMG_YC_demux() kenrel in c6accel */
     if ((refFd = fopen(ref_src_y_422sp,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_src_y_422sp);
         
     }
     fread(src_y, num_lines*src_pitch, sizeof(unsigned char), refFd);
     fclose(refFd);

     if ((refFd = fopen(ref_src_cbcr_422sp,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_src_cbcr_422sp);
         
      }
     fread(src_cbcr,num_lines*src_pitch, sizeof(unsigned char), refFd);
     fclose(refFd);
       
    
    memset (pRefBuf_16bpp,0x00,num_lines*dst_cbcr_pitch*sizeof(unsigned char));
    LOG_STRING("C6accel_IMG_ycbcr422pl_to_ycbcr422sp(),");
    
    START_BENCHMARK();
    /* Call the ycbcr422sp_to_ycbcr422ile*/
    status=C6accel_IMG_ycbcr422sp_to_ycbcr420pl( hC6accel,src_y,src_cbcr,y_dst,cb_dst,cr_dst,num_lines,width,src_pitch,dst_y_pitch,dst_cbcr_pitch);
   
    END_AND_WRITE_BENCHMARK();

    if ((refFd = fopen(ref_dst_cb_420pl,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_dst_cb_420pl);
         
     }
     fread(pRefBuf_16bpp, dst_cbcr_pitch, sizeof(unsigned char), refFd);
 
    LOG_STRING("\n");

    /* Validate output between native DSP/ref and C6Accel on 8 bit char level */
    if (compare8bitArrays(pOutBuf_16bpp, pRefBuf_16bpp,num_lines*dst_cbcr_pitch/2) != 1){
      printf("C6accel_IMG_ycbcr422sp_to_ycbcr420pl() failed\n");
      return 0;
    }
    printf("C6accel_IMG_ycbcr422sp_to_ycbcr420pl successful\n");
    fclose(refFd);
    return 1;
}

/*
 * Test function for arithmetic rts functions in this function
 */
int c6accel_test_MATH_RTSARITH(C6accel_Handle hC6accel,unsigned int n)
{
 FILE *refFd;
 float *in1;
 float *in2;
 float *out;
 int i;
  /* Generate N bytes of 16 bit raw sine wave for use in tests
     Reuse pWorkingBuf2_16bpp for this
   */
    in1 = (float *)pWorkingBuf2_16bpp;
    in2 = (float *)pSrcBuf_16bpp;
    out = (float *)pRefBuf_16bpp;

   /*Read inputs */
    for (i=0;i<n;i++){
        in1[i] = xin[i];
        in2[i] = yin[i];
    }
    /* Open reference output files */
    if ((refFd = fopen(ref_addsp_i,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_addsp_i);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(float));
    memset (pRefBuf_16bpp,0x00,n*sizeof(float));

    LOG_STRING("C6accel_MATH_addsp(),");
    START_BENCHMARK();

    C6accel_MATH_addsp_i(hC6accel,in1,in2,(float *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();

    /* read in reference*/
    fread(pRefBuf_16bpp,  n, sizeof(float), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level*/
    if (comparefloatArrays((float *)pOutBuf_16bpp, (float *)pRefBuf_16bpp,n) != 1){
       printf("C6accel_MATH_addsp() failed\n");
       return 0;
     }

    fclose(refFd);

    /* Test subsp_i kernel in c6accel */
    if ((refFd = fopen(ref_subsp_i,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_subsp_i);
       return (0);
     }

    /* Clear output arrays*/
    memset (pOutBuf_16bpp,0x00,n*sizeof(float));
    memset (pRefBuf_16bpp,0x00,n*sizeof(float));

    LOG_STRING("C6accel_MATH_subsp(),");
    START_BENCHMARK();

    C6accel_MATH_subsp_i(hC6accel,in1,in2,(float *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();

    /* read in reference*/
    fread(pRefBuf_16bpp,  n, sizeof(float), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level*/
    if (comparefloatArrays((float *)pOutBuf_16bpp, (float *)pRefBuf_16bpp,n) != 1){
      printf("C6accel_MATH_subsp() failed\n");
      return 0;
    }

    fclose(refFd);

   /* Test mpysp_i kernel in c6accel */
   /* Open reference output file */
    if ((refFd = fopen(ref_mpysp_i,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_mpysp_i);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(float));
    memset (pRefBuf_16bpp,0x00,n*sizeof(float));

    LOG_STRING("C6accel_MATH_mpysp(),");
    START_BENCHMARK();

    C6accel_MATH_mpysp_i(hC6accel,in1,in2,(float *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();

    /* read in reference*/
    fread(pRefBuf_16bpp,  n, sizeof(float), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level*/
    if (comparefloatArrays((float *)pOutBuf_16bpp, (float *)pRefBuf_16bpp,n) != 1){
      printf("C6accel_MATH_mpysp() failed\n");
      return 0;
    }

    fclose(refFd);

    /* Test divsp_i kernel in c6accel */
    if ((refFd = fopen(ref_divsp_i,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_divsp_i);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(float));
    memset (pRefBuf_16bpp,0x00,n*sizeof(float));

    LOG_STRING("C6accel_MATH_divsp(),");
    START_BENCHMARK();

    C6accel_MATH_divsp_i(hC6accel,in1,in2,(float *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();

    /* read in reference */
    fread(pRefBuf_16bpp,  n, sizeof(float), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level */
    if (comparefloatArrays((float *)pOutBuf_16bpp, (float *)pRefBuf_16bpp,n) != 1){
       printf("C6accel_MATH_divsp() failed\n");
       return 0;
     }

    fclose(refFd);

    /* Test recipsp_i kernel in c6accel */
    if ((refFd = fopen(ref_recipsp_i,"rb")) == NULL) {
      printf("Failed to open refFd on %s\n",ref_recipsp_i);
      return (0);
    }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(float));
    memset (pRefBuf_16bpp,0x00,n*sizeof(float));

    LOG_STRING("C6accel_MATH_recipsp(),");
    START_BENCHMARK();

    C6accel_MATH_recipsp_i(hC6accel,in1,(float *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();
    /* read in reference */
    fread(pRefBuf_16bpp,  n, sizeof(float), refFd);
    LOG_STRING("\n");

     /* Validate output between native MATH/ref and C6Accel on 16 bit short level */
    if (comparefloatArrays((float *)pOutBuf_16bpp, (float *)pRefBuf_16bpp,n) != 1){
       printf("C6accel_MATH_recipsp() failed\n");
       return 0;
     }

     close(refFd);

    /* Test sqrtsp_i kernel in c6accel */
    if ((refFd = fopen(ref_sqrtsp_i,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_sqrtsp_i);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(float));
    memset (pRefBuf_16bpp,0x00,n*sizeof(float));

    LOG_STRING("C6accel_MATH_sqrtsp(),");
    START_BENCHMARK();

    C6accel_MATH_sqrtsp_i(hC6accel,in1,(float *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();

    /* read in reference */
    fread(pRefBuf_16bpp,  n, sizeof(float), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level */
    if (comparefloatArrays((float *)pOutBuf_16bpp, (float *)pRefBuf_16bpp,n) != 1){
      printf("C6accel_MATH_sqrtsp() failed\n");
      return 0;
    }

    fclose(refFd);

    printf("Test for Arithmetic RTS functions successful\n");
    return 1;
}

/*
 * Test function for conversion rts kernels in this function
 */
int c6accel_test_MATH_RTSCONV(C6accel_Handle hC6accel,unsigned int n)
{
 FILE  *refFd;
 int   *in1, *intOut;
 unsigned int *uin1, *uintOut;
 float *out, *fltIn;
 int i;

    /* Use Contiguous working buffers for inputs */
    in1 = (int *)pWorkingBuf2_16bpp;
    out = (float *)pRefBuf_16bpp;

    for (i=0;i<n;i++){
        in1[i] = zin[i];
     }

    if ((refFd = fopen(ref_intsp_i,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_intsp_i);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(float));
    memset (pRefBuf_16bpp,0x00,n*sizeof(float));

    LOG_STRING("C6accel_MATH_intsp(),");
    START_BENCHMARK();

    C6accel_MATH_intsp_i(hC6accel,(int *)in1,(float *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();

    /* read in reference */
    fread(pRefBuf_16bpp,  n, sizeof(float), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level*/
    if (comparefloatArrays((float *)pOutBuf_16bpp, (float *)pRefBuf_16bpp,n) != 1){
      printf("C6accel_MATH_intsp() failed\n");
      return 0;
    }
    fclose(refFd);

    /* Test uintsp kernel in c6accel */
    /* Read input using working buffers */
    uin1 = (unsigned int *)pWorkingBuf2_16bpp;
    out = (float *)pRefBuf_16bpp;

    for (i=0;i<n;i++){
        uin1[i] = win[i];
     }
    /* Open reference output from file */
    if ((refFd = fopen(ref_uintsp_i,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_uintsp_i);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(float));
    memset (pRefBuf_16bpp,0x00,n*sizeof(float));

    LOG_STRING("C6accel_MATH_uintsp(),");
    START_BENCHMARK();

    C6accel_MATH_uintsp_i(hC6accel,(unsigned int *)uin1,(float *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();
    /* read in reference */
    fread(pRefBuf_16bpp,  n, sizeof(float), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level*/
    if (comparefloatArrays((float *)pOutBuf_16bpp, (float *)pRefBuf_16bpp,n) != 1){
       printf("C6accel_MATH_uintsp() failed\n");
       return 0;
    }
    fclose(refFd);

    /* Test spint kernel in c6accel */
    fltIn = (float *)pWorkingBuf2_16bpp;
    intOut = (int *)pRefBuf_16bpp;

    for (i=0;i<n;i++){
        fltIn[i] = xin[i];
     }

    /*Open reference output file*/
    if ((refFd = fopen(ref_spint_i,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_spint_i);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(int));
    memset (pRefBuf_16bpp,0x00,n*sizeof(int));

    LOG_STRING("C6accel_MATH_spint(),");
    START_BENCHMARK();

    C6accel_MATH_spint_i(hC6accel,(float *)fltIn,(int *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();

    /* read in reference output from file */
    fread(pRefBuf_16bpp,  n, sizeof(int), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level*/
    if (compare32bitArrays((unsigned int *)pOutBuf_16bpp, (unsigned int *)pRefBuf_16bpp,n) != 1){
       printf("C6accel_MATH_spint() failed\n");
       return 0;
     }
    fclose(refFd);

    /* Test spuint kernel in c6accel*/
    fltIn = (float *)pWorkingBuf2_16bpp;
    uintOut = (unsigned int *)pRefBuf_16bpp;

    for (i=0;i<n;i++){
        fltIn[i] = xin[i];
    }
    /* open reference output */
    if ((refFd = fopen(ref_spuint_i,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_spuint_i);
       return (0);
       }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(int));
    memset (pRefBuf_16bpp,0x00,n*sizeof(int));

    LOG_STRING("C6accel_MATH_spuint(),");
    START_BENCHMARK();

    C6accel_MATH_spuint_i(hC6accel,(float *)fltIn,(unsigned int *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();

    /* read in reference */
    fread(pRefBuf_16bpp,  n, sizeof(int), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level */
    if (compare32bitArrays((unsigned int *)pOutBuf_16bpp, (unsigned int *)pRefBuf_16bpp,n) != 1){
      printf("C6accel_MATH_spuint() failed\n");
      return 0;
     }
    fclose(refFd);

    printf("Tests for Conversion RTS functions successful\n");
    return 1;
}

/*
 * Test function for IQMath conversion kernels in this function
 */
int c6accel_test_MATH_IQCONV(C6accel_Handle hC6accel,unsigned int n, unsigned int qfmt, unsigned int qfmt2)
{
 FILE  *refFd;
 int *IQ_Out, *IQ_In;
 int *in1, *intOut;
 float *fltOut, *fltIn;
 int i;

    /* Reuse pWorkingBuf2_16bpp for this*/
    fltIn = (float *)pWorkingBuf2_16bpp;
    IQ_Out = (int *)pRefBuf_16bpp;

    /* Set input vectors */
    for (i=0;i<n;i++){
        fltIn[i] = xin[i];
     }
    /* open reference output file */
    if ((refFd = fopen(ref_ftoiqn,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_ftoiqn);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(int));
    memset (pRefBuf_16bpp,0x00,n*sizeof(int));

    LOG_STRING("C6accel_MATH_FTOIQ(),");
    START_BENCHMARK();

    C6accel_MATH_FTOIQN(hC6accel,(float *)fltIn,qfmt,(int *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();

    /* read in reference Output */
    fread(pRefBuf_16bpp,  n, sizeof(int), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level */
    if (compare32bitArrays((unsigned int *)pOutBuf_16bpp, (unsigned int *)pRefBuf_16bpp,n) != 1){
       printf("C6accel_MATH_FtoIQN() failed\n");
       return 0;
    }
   fclose(refFd);

   /* Reuse pWorkingBuf2_16bpp for this */
    in1 = (int *)pWorkingBuf2_16bpp;
    IQ_Out = (int *)pRefBuf_16bpp;

    for (i=0;i<n;i++){
        in1[i] = zin[i];
     }

    if ((refFd = fopen(ref_iqn,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_iqn);
       return (0);
    }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(int));
    memset (pRefBuf_16bpp,0x00,n*sizeof(int));

    LOG_STRING("C6accel_MATH_IQN(),");
    START_BENCHMARK();

    C6accel_MATH_IQN(hC6accel,(int *)in1,qfmt,(int *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();

    /* read in reference */
    fread(pRefBuf_16bpp,  n, sizeof(int), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level*/
    if (compare32bitArrays((unsigned int *)pOutBuf_16bpp, (unsigned int *)pRefBuf_16bpp,n) != 1){
      printf("C6accel_MATH_IQN() failed\n");
      return 0;
    }
    fclose(refFd);

   /* Test IQ format to float conversion kernel */
   /* Reuse pWorkingBuf2_16bpp for this*/
    IQ_In = (int *)pWorkingBuf2_16bpp;
    fltOut = (float *)pRefBuf_16bpp;

    for (i=0;i<n;i++){
        IQ_In[i] = IQ16_in1[i];
     }

    /*Open reference output file */
    if ((refFd = fopen(ref_iqntof,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_iqntof);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(float));
    memset (pRefBuf_16bpp,0x00,n*sizeof(float));

    LOG_STRING("C6accel_MATH_IQNTOF(),");
    START_BENCHMARK();

    C6accel_MATH_IQNtoF(hC6accel,(int *)IQ_In,qfmt,(float *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();

    /* read in reference */
    fread(pRefBuf_16bpp,  n, sizeof(float), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level*/
    if (compare32bitArrays((unsigned int *)pOutBuf_16bpp, (unsigned int *)pRefBuf_16bpp,n) != 1)
         {printf("C6accel_MATH_IQNTOF() failed\n");
          return 0;
          }
    fclose(refFd);

    /*Test for iq to int format conversion kernel */
    /* Reuse pWorkingBuf2_16bpp for this */
    IQ_In = (int *)pWorkingBuf2_16bpp;
    intOut = (int *)pRefBuf_16bpp;

    for (i=0;i<n;i++){
        IQ_In[i] = IQ16_in1[i];
     }

    if ((refFd = fopen(ref_iqnint,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_iqnint);
       return (0);
       }

    /* Clear output arrays*/
    memset (pOutBuf_16bpp,0x00,n*sizeof(int));
    memset (pRefBuf_16bpp,0x00,n*sizeof(int));

    LOG_STRING("C6accel_MATH_IQNint(),");
    START_BENCHMARK();

    C6accel_MATH_IQNint(hC6accel,(int *)IQ_In,qfmt,(int *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();

    /* read in reference */
    fread(pRefBuf_16bpp,  n, sizeof(int), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level*/
    if (compare32bitArrays((unsigned int *)pOutBuf_16bpp, (unsigned int *)pRefBuf_16bpp,n) != 1){
       printf("C6accel_MATH_IQNint() failed\n");
       return 0;
     }
    fclose(refFd);

    /* Test for iq to fraction conversion kernel */
    /* Reuse pWorkingBuf2_16bpp for this */
    IQ_In = (int *)pWorkingBuf2_16bpp;
    IQ_Out =(int *)pRefBuf_16bpp;

    for (i=0;i<n;i++){
        IQ_In[i] = IQ16_in1[i];
     }
    /* Open reference output file */
    if ((refFd = fopen(ref_iqnfrac,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_iqnfrac);
       return (0);
       }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(int));
    memset (pRefBuf_16bpp,0x00,n*sizeof(int));

    LOG_STRING("C6accel_MATH_IQNfrac(),");
    START_BENCHMARK();

    C6accel_MATH_IQNfrac(hC6accel,(int *)IQ_In,qfmt,(int *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();
    /* read in reference */
    fread(pRefBuf_16bpp,  n, sizeof(int), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level*/
    if (compare32bitArrays((unsigned int *)pOutBuf_16bpp, (unsigned int *)pRefBuf_16bpp,n) != 1){
       printf("C6accel_MATH_IQNfrac() failed\n");
       return 0;
     }
    fclose(refFd);

   /* Test for IQXtoIQY conversion kernel in c6accel*/
   /* Reuse pWorkingBuf2_16bpp for this */
    IQ_In = (int *)pWorkingBuf2_16bpp;
    IQ_Out =(int *)pRefBuf_16bpp;

    for (i=0;i<n;i++){
        IQ_In[i] = IQ16_in1[i];
     }
    /* Open reference output file */
    if ((refFd = fopen(ref_iqxtoiqy,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_iqxtoiqy);
       return (0);
       }

    /* Clear output arrays*/
    memset (pOutBuf_16bpp,0x00,n*sizeof(int));
    memset (pRefBuf_16bpp,0x00,n*sizeof(int));

    LOG_STRING("C6accel_MATH_IQXtoIQY(),");
    START_BENCHMARK();

    C6accel_MATH_IQXtoIQY(hC6accel,(int *)IQ_In,qfmt,qfmt2,(int *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();
    /* read in reference*/
    fread(pRefBuf_16bpp,  n, sizeof(int), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level*/
    if (compare32bitArrays((unsigned int *)pOutBuf_16bpp, (unsigned int *)pRefBuf_16bpp,n) != 1)
         {printf("C6accel_MATH_IQNfrac() failed\n");
          return 0;
          }
     fclose(refFd);

    printf("Tests for IQmath Conversion functions successful\n");
    return 1;
}

/*
 * Test function for IQMATH math functions in this function
 */
int c6accel_test_MATH_IQMATH(C6accel_Handle hC6accel,unsigned int n, unsigned int qfmt)
{
 FILE  *refFd;
 int *IQ_Out, *IQ_In1, *IQ_In2;
 int i;

    /* Reuse pWorkingBuf2_16bpp for this*/
    IQ_In1 = (int *)pSrcBuf_16bpp;
    IQ_Out = (int *)pRefBuf_16bpp;

    for (i=0;i<n;i++){
        IQ_In1[i] = IQ16_in1[i];
     }
    /* Open reference output file */
    if ((refFd = fopen(ref_iqnlog,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_iqnlog);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(int));
    memset (pRefBuf_16bpp,0x00,n*sizeof(int));

    LOG_STRING("C6accel_MATH_IQNlog(),");
    START_BENCHMARK();

    C6accel_MATH_IQNlog(hC6accel,(int *)IQ_In1,(int *)pOutBuf_16bpp,qfmt,n);
    END_AND_WRITE_BENCHMARK();

    /* read in reference*/
    fread(pRefBuf_16bpp,  n, sizeof(int), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level*/
    if (compare32bitArrays((unsigned int *)pOutBuf_16bpp, (unsigned int *)pRefBuf_16bpp,n) != 1){
      printf("C6accel_MATH_IQNlog() failed\n");
      return 0;
    }
    fclose(refFd);

    /* Test for IQabs kernel in c6accel */
    /* Reuse pWorkingBuf2_16bpp for this */
    IQ_In1 = (int *)pSrcBuf_16bpp;
    IQ_Out = (int *)pRefBuf_16bpp;

    for (i=0;i<n;i++){
        IQ_In1[i] = IQ16_in2[i];
     }
    /* Open reference output file */
    if ((refFd = fopen(ref_iqnabs,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_iqnabs);
       return (0);
    }

    /* Clear output arrays*/
    memset (pOutBuf_16bpp,0x00,n*sizeof(int));
    memset (pRefBuf_16bpp,0x00,n*sizeof(int));

    LOG_STRING("C6accel_MATH_IQNabs(),");
    START_BENCHMARK();

    C6accel_MATH_IQNabs(hC6accel,(int *)IQ_In1,(int *)pOutBuf_16bpp,qfmt,n);
    END_AND_WRITE_BENCHMARK();
    /* read in reference */
    fread(pRefBuf_16bpp,  n, sizeof(int), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level */
    if (compare32bitArrays((unsigned int *)pOutBuf_16bpp, (unsigned int *)pRefBuf_16bpp,n) != 1){
       printf("C6accel_MATH_IQNabs() failed\n");
       return 0;
     }
     fclose(refFd);


    /* Test for IQsqrt kernelin c6accel */
    /* Reuse pWorkingBuf2_16bpp for reading the inputs */
    IQ_In1 = (int *)pSrcBuf_16bpp;
    IQ_Out = (int *)pRefBuf_16bpp;

    for (i=0;i<n;i++){
        IQ_In1[i] = IQ16_in5[i];
     }

    if ((refFd = fopen(ref_iqnsqrt,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_iqnsqrt);
       return (0);
       }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(int));
    memset (pRefBuf_16bpp,0x00,n*sizeof(int));

    LOG_STRING("C6accel_MATH_IQNsqrt(),");
    START_BENCHMARK();

    C6accel_MATH_IQNsqrt(hC6accel,(int *)IQ_In1,(int *)pOutBuf_16bpp,qfmt,n);
    END_AND_WRITE_BENCHMARK();
    /* read in reference */
    fread(pRefBuf_16bpp,  n, sizeof(int), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level */
    if (compare32bitArrays((unsigned int *)pOutBuf_16bpp, (unsigned int *)pRefBuf_16bpp,n) != 1){
       printf("C6accel_MATH_IQNsqrt() failed\n");
       return 0;
    }
     fclose(refFd);

    /* Test IQMATH inverse sqrt in c6accel */
    /* Reuse pWorkingBuf2_16bpp for this*/
    IQ_In1 = (int *)pSrcBuf_16bpp;
    IQ_Out = (int *)pRefBuf_16bpp;

    for (i=0;i<n;i++){
        IQ_In1[i] = IQ16_in5[i];
     }

    /* Open refernce output file */
    if ((refFd = fopen(ref_iqnisqrt,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_iqnisqrt);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(int));
    memset (pRefBuf_16bpp,0x00,n*sizeof(int));

    LOG_STRING("C6accel_MATH_IQNisqrt(),");
    START_BENCHMARK();

    C6accel_MATH_IQNisqrt(hC6accel,(int *)IQ_In1,(int *)pOutBuf_16bpp,qfmt,n);
    END_AND_WRITE_BENCHMARK();
    /* read in reference */
    fread(pRefBuf_16bpp,  n, sizeof(int), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level */
    if (compare32bitArrays((unsigned int *)pOutBuf_16bpp, (unsigned int *)pRefBuf_16bpp,n) != 1)
         {printf("C6accel_MATH_IQNisqrt() failed\n");
          return 0;
          }
    fclose(refFd);

    /* Test IQMath exponential function in c6accel */
    /* Reuse pWorkingBuf2_16bpp for this */
    IQ_In1 = (int *)pSrcBuf_16bpp;
    IQ_Out = (int *)pRefBuf_16bpp;

    for (i=0;i<n;i++){
        IQ_In1[i] = IQ16_in3[i];
     }

    /* Open reference test output file */
    if ((refFd = fopen(ref_iqnexp,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_iqnexp);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(int));
    memset (pRefBuf_16bpp,0x00,n*sizeof(int));

    LOG_STRING("C6accel_MATH_IQNexp(),");
    START_BENCHMARK();

    C6accel_MATH_IQNexp(hC6accel,(int *)IQ_In1,(int *)pOutBuf_16bpp,qfmt,n);
    END_AND_WRITE_BENCHMARK();
    /* read in reference */
    fread(pRefBuf_16bpp,  n, sizeof(int), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level */
    if (compare32bitArrays((unsigned int *)pOutBuf_16bpp, (unsigned int *)pRefBuf_16bpp,n) != 1){
      printf("C6accel_MATH_IQNexp() failed\n");
      return 0;
    }

    fclose(refFd);

    /* Test for power function in IQMath */
    /*  Reuse pWorkingBuf2_16bpp to read inputs */
    IQ_In1 = (int *)pSrcBuf_16bpp;
    IQ_In2 = (int *)pWorkingBuf2_16bpp;
    IQ_Out = (int *)pRefBuf_16bpp;

    for (i=0;i<n;i++){
        IQ_In1[i] = IQ16_in3[i];
        IQ_In2[i] = IQ16_in4[i];
     }

    /* Open reference test output file */
    if ((refFd = fopen(ref_iqnpow,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_iqnpow);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(int));
    memset (pRefBuf_16bpp,0x00,n*sizeof(int));

    LOG_STRING("C6accel_MATH_IQNpow(),");
    START_BENCHMARK();

    C6accel_MATH_IQNpow(hC6accel,(int *)IQ_In1,(int *)IQ_In2,(int *)pOutBuf_16bpp,qfmt,n);
    END_AND_WRITE_BENCHMARK();
    /* read in reference */
    fread(pRefBuf_16bpp,  n, sizeof(int), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level */
    if (compare32bitArrays((unsigned int *)pOutBuf_16bpp, (unsigned int *)pRefBuf_16bpp,n) != 1){
      printf("C6accel_MATH_IQNpow() failed\n");
      return 0;
    }
    fclose(refFd);

    /* Test for IQNmag kernel in c6accel */
    /* Reuse pWorkingBuf2_16bpp for this */
    IQ_In1 = (int *)pSrcBuf_16bpp;
    IQ_In2 = (int *)pWorkingBuf2_16bpp;
    IQ_Out = (int *)pRefBuf_16bpp;

    for (i=0;i<n;i++){
        IQ_In1[i] = IQ16_in3[i];
        IQ_In2[i] = IQ16_in4[i];
     }
    /* Open reference ouptu file */
    if ((refFd = fopen(ref_iqnmag,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_iqnmag);
       return (0);
       }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(int));
    memset (pRefBuf_16bpp,0x00,n*sizeof(int));

    LOG_STRING("C6accel_MATH_IQNmag(),");
    START_BENCHMARK();

    C6accel_MATH_IQNmag(hC6accel,(int *)IQ_In1,(int *)IQ_In2,(int *)pOutBuf_16bpp,qfmt,n);
    END_AND_WRITE_BENCHMARK();
    /* read in reference */
    fread(pRefBuf_16bpp,  n, sizeof(int), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level */
    if (compare32bitArrays((unsigned int *)pOutBuf_16bpp, (unsigned int *)pRefBuf_16bpp,n) != 1){
      printf("C6accel_MATH_IQNmag() failed\n");
      return 0;
    }
    fclose(refFd);

   printf("Tests for Mathematical IQmath functions successful\n");
   return 1;
}

/*
 * Test function for arithmetic IQMath kernel.
 */
int c6accel_test_MATH_IQARITH(C6accel_Handle hC6accel,unsigned int n, unsigned int qfmt)
{
 FILE  *refFd;
 int *IQ_Out, *IQ_In1, *IQ_In2;
 int i;
 int *intIn, *intOut;

  /* Test for IQMath multiplication kernel */
  /* Reuse pWorkingBuf2_16bpp for this     */
    IQ_In1 = (int *)pSrcBuf_16bpp;
    IQ_In2 = (int *)pWorkingBuf2_16bpp;
    IQ_Out = (int *)pRefBuf_16bpp;


    for (i=0;i<n;i++){
        IQ_In1[i] = IQ16_in3[i];
        IQ_In2[i] = IQ16_in4[i];
     }
    /* Open reference ouput file */
    if ((refFd = fopen(ref_iqnmpy,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_iqnmpy);
       return (0);
       }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(int));
    memset (pRefBuf_16bpp,0x00,n*sizeof(int));


    LOG_STRING("C6accel_MATH_IQNmpy(),");
    START_BENCHMARK();

    C6accel_MATH_IQNmpy(hC6accel,(int *)IQ_In1,(int *)IQ_In2,(int *)pOutBuf_16bpp,qfmt,n);

    END_AND_WRITE_BENCHMARK();
    /* read in reference */
    fread(pRefBuf_16bpp,  n, sizeof(int), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level */
    if (compare32bitArrays((unsigned int *)pOutBuf_16bpp, (unsigned int *)pRefBuf_16bpp,n) != 1){
      printf("C6accel_MATH_IQNmpy() failed\n");
      return 0;
    }
    fclose(refFd);

    /*Test for IQNmpyIQx in c6accel*/
    /* Reuse pWorkingBuf2_16bpp for this*/
    IQ_In1 = (int *)pSrcBuf_16bpp;
    IQ_In2 = (int *)pWorkingBuf2_16bpp;
    IQ_Out = (int *)pRefBuf_16bpp;

    /*Ser input vectors*/
    for (i=0;i<n;i++){
        IQ_In1[i] = IQ24_in[i];
        IQ_In2[i] = IQ12_in[i];
     }

    /*Open reference ouput file */
    if ((refFd = fopen(ref_iqnmpyIQx,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_iqnmpyIQx);
       return (0);
       }

    /* Clear output arrays*/
    memset (pOutBuf_16bpp,0x00,n*sizeof(int));
    memset (pRefBuf_16bpp,0x00,n*sizeof(int));

    LOG_STRING("C6accel_MATH_IQNmpyIQX(),");
    START_BENCHMARK();

    C6accel_MATH_IQNmpyIQx(hC6accel,(int *)IQ_In1,24,(int *)IQ_In2,12,(int *)pOutBuf_16bpp,qfmt,n);
    END_AND_WRITE_BENCHMARK();
    /* read in reference*/
    fread(pRefBuf_16bpp,  n, sizeof(int), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level */
    if (compare32bitArrays((unsigned int *)pOutBuf_16bpp, (unsigned int *)pRefBuf_16bpp,n) != 1){
      printf("C6accel_MATH_IQNmpyIQX() failed\n");
      return 0;
    }
    fclose(refFd);

    /* Test IQNrmpy kernel in c6accel */
    /* Reuse pWorkingBuf2_16bpp for this*/
    IQ_In1 = (int *)pSrcBuf_16bpp;
    IQ_In2 = (int *)pWorkingBuf2_16bpp;
    IQ_Out = (int *)pRefBuf_16bpp;

    /* Set test input */
    for (i=0;i<n;i++){
        IQ_In1[i] = IQ16_in3[i];
        IQ_In2[i] = IQ16_in4[i];
     }

    /*Open reference output file */
    if ((refFd = fopen(ref_iqnrmpy,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_iqnrmpy);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(int));
    memset (pRefBuf_16bpp,0x00,n*sizeof(int));

    LOG_STRING("C6accel_MATH_IQNrmpy(),");
    START_BENCHMARK();

    C6accel_MATH_IQNrmpy(hC6accel,(int *)IQ_In1,(int *)IQ_In2,(int *)pOutBuf_16bpp,qfmt,n);
    END_AND_WRITE_BENCHMARK();
    /* read in reference*/
    fread(pRefBuf_16bpp,  n, sizeof(int), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level */
    if (compare32bitArrays((unsigned int *)pOutBuf_16bpp, (unsigned int *)pRefBuf_16bpp,n) != 1){
      printf("C6accel_MATH_IQNrmpy() failed\n");
      return 0;
     }
     fclose(refFd);

    /*Test IQNrsmpy in c6accel */
    /* Reuse pWorkingBuf2_16bpp for this */
    IQ_In1 = (int *)pSrcBuf_16bpp;
    IQ_In2 = (int *)pWorkingBuf2_16bpp;
    IQ_Out = (int *)pRefBuf_16bpp;

    /* Set test input vectors */
    for (i=0;i<n;i++){
        IQ_In1[i] = IQ16_in3[i];
        IQ_In2[i] = IQ16_in4[i];
     }

    /* Open reference output file */
    if ((refFd = fopen(ref_iqnrsmpy,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_iqnrsmpy);
       return (0);
     }

     /* Clear output arrays*/
    memset (pOutBuf_16bpp,0x00,n*sizeof(int));
    memset (pRefBuf_16bpp,0x00,n*sizeof(int));

    LOG_STRING("C6accel_MATH_IQNrsmpy(),");
    START_BENCHMARK();

    C6accel_MATH_IQNrsmpy(hC6accel,(int *)IQ_In1,(int *)IQ_In2,(int *)pOutBuf_16bpp,qfmt,n);
    END_AND_WRITE_BENCHMARK();
    /* read in reference */
    fread(pRefBuf_16bpp,  n, sizeof(int), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level*/
    if (compare32bitArrays((unsigned int *)pOutBuf_16bpp, (unsigned int *)pRefBuf_16bpp,n) != 1){
       printf("C6accel_MATH_IQNrsmpy() failed\n");
       return 0;
     }
    fclose(refFd);

    /* Reuse pWorkingBuf2_16bpp for this */
    IQ_In1 = (int *)pSrcBuf_16bpp;
    intIn = (int *)pWorkingBuf2_16bpp;
    intOut = (int *)pRefBuf_16bpp;

    /*Setup input test vectors */
    for (i=0;i<n;i++){
        IQ_In1[i] = IQ16_in3[i];
        intIn[i] = win[i];
     }
    /* Open reference test output file */
    if ((refFd = fopen(ref_iqnmpyI32int,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_iqnmpyI32int);
       return (0);
       }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(int));
    memset (pRefBuf_16bpp,0x00,n*sizeof(int));

    LOG_STRING("C6accel_MATH_IQNmpyI32int(),");
    START_BENCHMARK();

    C6accel_MATH_IQNmpyI32int(hC6accel,(int *)IQ_In1,(int *)intIn,(int *)pOutBuf_16bpp,qfmt,n);
    END_AND_WRITE_BENCHMARK();
    /* read in reference */
    fread(pRefBuf_16bpp,  n, sizeof(int), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level */
    if (compare32bitArrays((unsigned int *)pOutBuf_16bpp, (unsigned int *)pRefBuf_16bpp,n) != 1){
      printf("C6accel_MATH_IQNmpyI32int() failed\n");
      return 0;
     }
    fclose(refFd);

    /*Test IQNmpyI32frac kernel in c6accel */
    /* Reuse pWorkingBuf2_16bpp for this */
    IQ_In1 = (int *)pSrcBuf_16bpp;
    intIn = (int *)pWorkingBuf2_16bpp;
    IQ_Out = (int *)pRefBuf_16bpp;

    /*Set test input vectors  */
    for (i=0;i<n;i++){
        IQ_In1[i] = IQ16_in3[i];
        intIn[i] = win[i];
     }

    /* open test output file */
    if ((refFd = fopen(ref_iqnmpyI32frac,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_iqnmpyI32frac);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(int));
    memset (pRefBuf_16bpp,0x00,n*sizeof(int));

    LOG_STRING("C6accel_MATH_IQNmpyI32frac(),");
    START_BENCHMARK();

    C6accel_MATH_IQNmpyI32frac(hC6accel,(int *)IQ_In1,(int *)intIn,(int *)pOutBuf_16bpp,qfmt,n);
    END_AND_WRITE_BENCHMARK();
    /* read in reference */
    fread(pRefBuf_16bpp,  n, sizeof(int), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level */
    if (compare32bitArrays((unsigned int *)pOutBuf_16bpp, (unsigned int *)pRefBuf_16bpp,n) != 1){
       printf("C6accel_MATH_IQNmpyI32frac() failed\n");
       return 0;
    }

    fclose(refFd);

    /* Test for IQmath divion kernel */
    /* Reuse pWorkingBuf2_16bpp for this */
    IQ_In1 = (int *)pSrcBuf_16bpp;
    IQ_In2 = (int *)pWorkingBuf2_16bpp;
    IQ_Out = (int *)pRefBuf_16bpp;
    /*Set test input vectors */
    for (i=0;i<n;i++){
        IQ_In1[i] = IQ16_in3[i];
        IQ_In2[i] = IQ16_in4[i];
     }
    /* Open reference output file  */
    if ((refFd = fopen(ref_iqndiv,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_iqndiv);
       return (0);
       }

   /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(int));
    memset (pRefBuf_16bpp,0x00,n*sizeof(int));

    LOG_STRING("C6accel_MATH_IQNdiv(),");
    START_BENCHMARK();

    C6accel_MATH_IQNdiv(hC6accel,(int *)IQ_In1,(int *)IQ_In2,(int *)pOutBuf_16bpp,qfmt,n);
    END_AND_WRITE_BENCHMARK();

    /*read in reference*/
    fread(pRefBuf_16bpp,  n, sizeof(int), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level */
    if (compare32bitArrays((unsigned int *)pOutBuf_16bpp, (unsigned int *)pRefBuf_16bpp,n) != 1){
       printf("C6accel_MATH_IQNdiv() failed\n");
       return 0;
     }
    fclose(refFd);

    printf("Tests for Arithmetic IQmath functions successful\n");
    return 1;
}

/*
 * Test function for trignometric kernels in c6accel
 */
int c6accel_test_MATH_IQTRIG(C6accel_Handle hC6accel,unsigned int n, unsigned int qfmt)
{
 FILE  *refFd;
 int *IQ_Out, *IQ_In1, *IQ_In2;
 int i;

   /* Test for IQmath trignometric function */
   /* Reuse pWorkingBuf2_16bpp for this */
    IQ_In1 = (int *)pSrcBuf_16bpp;
    IQ_Out = (int *)pRefBuf_16bpp;

   /*Set tet input vectors */
    for (i=0;i<n;i++){
        IQ_In1[i] = IQ16_in5[i];
     }

    /* Open reference output file */
    if ((refFd = fopen(ref_iqnsin,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_iqnsin);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(int));
    memset (pRefBuf_16bpp,0x00,n*sizeof(int));

    LOG_STRING("C6accel_MATH_IQNsin(),");
    START_BENCHMARK();

    C6accel_MATH_IQNsin(hC6accel,(int *)IQ_In1,(int *)pOutBuf_16bpp,qfmt,n);
    END_AND_WRITE_BENCHMARK();
    /* read in reference */
    fread(pRefBuf_16bpp,  n, sizeof(int), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level*/
    if (compare32bitArrays((unsigned int *)pOutBuf_16bpp, (unsigned int *)pRefBuf_16bpp,n) != 1){
      printf("C6accel_MATH_IQNsin() failed\n");
      return 0;
    }
    fclose(refFd);

    /*Test for IQmathcos kernel */
    /* Reuse pWorkingBuf2_16bpp for this*/
    IQ_In1 = (int *)pSrcBuf_16bpp;
    IQ_Out = (int *)pRefBuf_16bpp;

    /* Set test input vectors */
    for (i=0;i<n;i++){
        IQ_In1[i] = IQ16_in5[i];
     }
    /* Open reference output file */

    if ((refFd = fopen(ref_iqncos,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_iqncos);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(int));
    memset (pRefBuf_16bpp,0x00,n*sizeof(int));

    LOG_STRING("C6accel_MATH_IQNcos(),");
    START_BENCHMARK();

    C6accel_MATH_IQNcos(hC6accel,(int *)IQ_In1,(int *)pOutBuf_16bpp,qfmt,n);
    END_AND_WRITE_BENCHMARK();
    /* read in reference */
    fread(pRefBuf_16bpp,  n, sizeof(int), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level*/
    if (compare32bitArrays((unsigned int *)pOutBuf_16bpp, (unsigned int *)pRefBuf_16bpp,n) != 1){
       printf("C6accel_MATH_IQNcos() failed\n");
       return 0;
    }
    fclose(refFd);

    /*Test for IQsinPU kernel */
   /* Reuse pWorkingBuf2_16bpp for this*/
    IQ_In1 = (int *)pSrcBuf_16bpp;
    IQ_Out = (int *)pRefBuf_16bpp;
    /* Set test input vectors */
    for (i=0;i<n;i++){
        IQ_In1[i] = IQ16_in5[i];
     }
    /* Open reference test output vectors*/
    if ((refFd = fopen(ref_iqnsinPU,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_iqnsinPU);
       return (0);
       }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(int));
    memset (pRefBuf_16bpp,0x00,n*sizeof(int));

    LOG_STRING("C6accel_MATH_IQNsinPU(),");
    START_BENCHMARK();

    C6accel_MATH_IQNsinPU(hC6accel,(int *)IQ_In1,(int *)pOutBuf_16bpp,qfmt,n);
    END_AND_WRITE_BENCHMARK();
    /* read in reference*/
    fread(pRefBuf_16bpp,  n, sizeof(int), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level*/
    if (compare32bitArrays((unsigned int *)pOutBuf_16bpp, (unsigned int *)pRefBuf_16bpp,n) != 1) {
       printf("C6accel_MATH_IQNsinPU() failed\n");
       return 0;
     }
     fclose(refFd);

    /* Reuse pWorkingBuf2_16bpp for this*/
    IQ_In1 = (int *)pSrcBuf_16bpp;
    IQ_Out = (int *)pRefBuf_16bpp;

    /* Set test input vectors */
    for (i=0;i<n;i++){
        IQ_In1[i] = IQ16_in5[i];
     }

    /* Open reference test vector file */
    if ((refFd = fopen(ref_iqncosPU,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_iqncosPU);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(int));
    memset (pRefBuf_16bpp,0x00,n*sizeof(int));

    LOG_STRING("C6accel_MATH_IQNcosPU(),");
    START_BENCHMARK();

    C6accel_MATH_IQNcosPU(hC6accel,(int *)IQ_In1,(int *)pOutBuf_16bpp,qfmt,n);
    END_AND_WRITE_BENCHMARK();
    /* read in reference */
    fread(pRefBuf_16bpp,  n, sizeof(int), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level */
    if (compare32bitArrays((unsigned int *)pOutBuf_16bpp, (unsigned int *)pRefBuf_16bpp,n) != 1){
       printf("C6accel_MATH_IQNcosPU() failed\n");
       return 0;
     }
    fclose(refFd);

    /* Test IQatan2 kernel in c6accel */
   /* Reuse pWorkingBuf2_16bpp for settingup the input vectors */
    IQ_In1 = (int *)pSrcBuf_16bpp;
    IQ_In2 = (int *)pWorkingBuf2_16bpp;
    IQ_Out = (int *)pRefBuf_16bpp;

    /* Setup test input vectors */
    for (i=0;i<n;i++){
        IQ_In1[i] = IQ16_in3[i];
        IQ_In2[i] = IQ16_in4[i];
     }

    /* Open reference test output file */
    if ((refFd = fopen(ref_iqnatan2,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_iqnatan2);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(int));
    memset (pRefBuf_16bpp,0x00,n*sizeof(int));

    LOG_STRING("C6accel_MATH_IQNatan2(),");
    START_BENCHMARK();

    C6accel_MATH_IQNatan2(hC6accel,(int *)IQ_In1,(int *)IQ_In2,(int *)pOutBuf_16bpp,qfmt,n);
    END_AND_WRITE_BENCHMARK();
    /* read in reference */
    fread(pRefBuf_16bpp,  n, sizeof(int), refFd);
    LOG_STRING("\n");

    /*  Validate output between native MATH/ref and C6Accel on 16 bit short level*/
    if (compare32bitArrays((unsigned int *)pOutBuf_16bpp, (unsigned int *)pRefBuf_16bpp,n) != 1){
       printf("C6accel_MATH_IQNatan2() failed\n");
       return 0;
     }
    fclose(refFd);

    /* Test IQatan2PU kernel in C6accel */
    /* Reuse pWorkingBuf2_16bpp for setting up test inputs */
    IQ_In1 = (int *)pSrcBuf_16bpp;
    IQ_In2 = (int *)pWorkingBuf2_16bpp;
    IQ_Out = (int *)pRefBuf_16bpp;

    /* Setup input test vectors */
    for (i=0;i<n;i++){
        IQ_In1[i] = IQ16_in3[i];
        IQ_In2[i] = IQ16_in4[i];
     }
    /* Open test reference output file */
    if ((refFd = fopen(ref_iqnatan2PU,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_iqnatan2PU);
       return (0);
    }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(int));
    memset (pRefBuf_16bpp,0x00,n*sizeof(int));

    LOG_STRING("C6accel_MATH_IQNatan2PU(),");
    START_BENCHMARK();

    C6accel_MATH_IQNatan2PU(hC6accel,(int *)IQ_In1,(int *)IQ_In2,(int *)pOutBuf_16bpp,qfmt,n);
    END_AND_WRITE_BENCHMARK();
    /* read in reference */
    fread(pRefBuf_16bpp,  n, sizeof(int), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level */
    if (compare32bitArrays((unsigned int *)pOutBuf_16bpp, (unsigned int *)pRefBuf_16bpp,n) != 1){
       printf("C6accel_MATH_IQNatan2PU() failed\n");
       return 0;
     }
    fclose(refFd);

    printf("Tests for Trignometric IQmath functions successful\n");
    return 1;
}

#if (PLATFORM == 138)
/* Test for Floating point kernels */
/*
 * Test function for arithmetic rts single precision functions in this function
 */
int c6accel_test_MATH_RTSFLT(C6accel_Handle hC6accel,unsigned int n)
{
 FILE *refFd;
 float *in1;
 float *in2;
 float *out;
 int i;

/* Generate N bytes of 16 bit raw sine wave for use in tests
     Reuse pWorkingBuf2_16bpp for this
   */
    in1 = (float *)pSrcBuf_16bpp;
    in2 = (float *)pWorkingBuf2_16bpp;
    out = (float *)pRefBuf_16bpp;

   /*Read inputs */

   if ((refFd = fopen(ref_cossp_x1,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_cossp_x1);
         }
    fread(in1, n, sizeof(float), refFd);
    fclose(refFd);

    /* Open reference output files */
    if ((refFd = fopen(ref_cossp_rslt,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_cossp_rslt);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(float));
    memset (pRefBuf_16bpp,0x00,n*sizeof(float));

    LOG_STRING("C6accel_MATH_cossp(),");
    START_BENCHMARK();

    C6accel_MATH_cossp(hC6accel,in1,(float *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();

    /* read in reference*/
    fread(pRefBuf_16bpp,  n, sizeof(float), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level*/
    if (comparefloatArrays((float *)pOutBuf_16bpp, (float *)pRefBuf_16bpp,n) != 1){
       printf("C6accel_MATH_cossp() failed\n");
       return 0;
     }

    fclose(refFd);

   /*Read inputs */
   if ((refFd = fopen(ref_sinsp_x1,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_sinsp_x1);
         }
    fread(in1, n, sizeof(float), refFd);
    fclose(refFd);

    /* Open reference output files */
    if ((refFd = fopen(ref_sinsp_rslt,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_sinsp_rslt);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(float));
    memset (pRefBuf_16bpp,0x00,n*sizeof(float));

    LOG_STRING("C6accel_MATH_sinsp(),");
    START_BENCHMARK();

    C6accel_MATH_sinsp(hC6accel,in1,(float *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();

    /* read in reference*/
    fread(pRefBuf_16bpp,  n, sizeof(float), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level*/
    if (comparefloatArrays((float *)pOutBuf_16bpp, (float *)pRefBuf_16bpp,n) != 1){
       printf("C6accel_MATH_sinsp() failed\n");
       return 0;
     }

    fclose(refFd);

    /*Read inputs */
   if ((refFd = fopen(ref_atansp_x1,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_atansp_x1);
         }
    fread(in1, n, sizeof(float), refFd);
    fclose(refFd);

    /* Open reference output files */
    if ((refFd = fopen(ref_atansp_rslt,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_atansp_rslt);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(float));
    memset (pRefBuf_16bpp,0x00,n*sizeof(float));

    LOG_STRING("C6accel_MATH_atansp(),");
    START_BENCHMARK();

    C6accel_MATH_atansp(hC6accel,in1,(float *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();

    /* read in reference*/
    fread(pRefBuf_16bpp,  n, sizeof(float), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level*/
    if (comparefloatArrays((float *)pOutBuf_16bpp, (float *)pRefBuf_16bpp,n) != 1){
       printf("C6accel_MATH_atansp() failed\n");
       return 0;
     }

    fclose(refFd);

    /*Read inputs */
   if ((refFd = fopen(ref_logsp_x1,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_logsp_x1);
         }
    fread(in1, n, sizeof(float), refFd);
    fclose(refFd);

    /* Open reference output files */
    if ((refFd = fopen(ref_logsp_rslt,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_logsp_rslt);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(float));
    memset (pRefBuf_16bpp,0x00,n*sizeof(float));

    LOG_STRING("C6accel_MATH_logsp(),");
    START_BENCHMARK();

    C6accel_MATH_logsp(hC6accel,in1,(float *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();

    /* read in reference*/
    fread(pRefBuf_16bpp,  n, sizeof(float), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level*/
    if (comparefloatArrays((float *)pOutBuf_16bpp, (float *)pRefBuf_16bpp,n) != 1){
       printf("C6accel_MATH_logsp() failed\n");
       return 0;
     }

    fclose(refFd);

   /*Read inputs */
   if ((refFd = fopen(ref_expsp_x1,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_expsp_x1);
         }
    fread(in1, n, sizeof(float), refFd);
    fclose(refFd);

    /* Open reference output files */
    if ((refFd = fopen(ref_expsp_rslt,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_expsp_rslt);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(float));
    memset (pRefBuf_16bpp,0x00,n*sizeof(float));

    LOG_STRING("C6accel_MATH_expsp(),");
    START_BENCHMARK();

    C6accel_MATH_expsp(hC6accel,in1,(float *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();

    /* read in reference*/
    fread(pRefBuf_16bpp,  n, sizeof(float), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level*/
    if (comparefloatArrays((float *)pOutBuf_16bpp, (float *)pRefBuf_16bpp,n) != 1){
       printf("C6accel_MATH_expsp() failed\n");
       return 0;
     }

    fclose(refFd);

    /*Read inputs */
   if ((refFd = fopen(ref_exp2sp_x1,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_exp2sp_x1);
         }
    fread(in1, n, sizeof(float), refFd);
    fclose(refFd);

    /* Open reference output files */
    if ((refFd = fopen(ref_exp2sp_rslt,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_exp2sp_rslt);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(float));
    memset (pRefBuf_16bpp,0x00,n*sizeof(float));

    LOG_STRING("C6accel_MATH_exp2sp(),");
    START_BENCHMARK();

    C6accel_MATH_exp2sp(hC6accel,in1,(float *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();

    /* read in reference*/
    fread(pRefBuf_16bpp,  n, sizeof(float), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level*/
    if (comparefloatArrays((float *)pOutBuf_16bpp, (float *)pRefBuf_16bpp,n) != 1){
       printf("C6accel_MATH_exp2sp() failed\n");
       return 0;
     }

    fclose(refFd);

    /*Read inputs */
   if ((refFd = fopen(ref_exp10sp_x1,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_exp10sp_x1);
         }
    fread(in1, n, sizeof(float), refFd);
    fclose(refFd);

    /* Open reference output files */
    if ((refFd = fopen(ref_exp10sp_rslt,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_exp10sp_rslt);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(float));
    memset (pRefBuf_16bpp,0x00,n*sizeof(float));

    LOG_STRING("C6accel_MATH_exp10sp(),");
    START_BENCHMARK();

    C6accel_MATH_exp10sp(hC6accel,in1,(float *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();

    /* read in reference*/
    fread(pRefBuf_16bpp,  n, sizeof(float), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level*/
    if (comparefloatArrays((float *)pOutBuf_16bpp, (float *)pRefBuf_16bpp,n) != 1){
       printf("C6accel_MATH_exp10sp() failed\n");
       return 0;
     }

    fclose(refFd);

    /*Read inputs */
   if ((refFd = fopen(ref_recipsp_x1,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_recipsp_x1);
         }
    fread(in1, n, sizeof(float), refFd);
    fclose(refFd);

    /* Open reference output files */
    if ((refFd = fopen(ref_recipsp_rslt,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_recipsp_rslt);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(float));
    memset (pRefBuf_16bpp,0x00,n*sizeof(float));

    LOG_STRING("C6accel_MATH_recipsp(),");
    START_BENCHMARK();

    C6accel_MATH_recipsp(hC6accel,in1,(float *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();

    /* read in reference*/
    fread(pRefBuf_16bpp,  n, sizeof(float), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level*/
    if (comparefloatArrays((float *)pOutBuf_16bpp, (float *)pRefBuf_16bpp,n) != 1){
       printf("C6accel_MATH_recipsp() failed\n");
       return 0;
     }

    fclose(refFd);

   /*Read inputs */
   if ((refFd = fopen(ref_sqrtsp_x1,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_sqrtsp_x1);
         }
    fread(in1, n, sizeof(float), refFd);
    fclose(refFd);

    /* Open reference output files */
    if ((refFd = fopen(ref_sqrtsp_rslt,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_sqrtsp_rslt);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(float));
    memset (pRefBuf_16bpp,0x00,n*sizeof(float));

    LOG_STRING("C6accel_MATH_sqrtsp(),");
    START_BENCHMARK();

    C6accel_MATH_sqrtsp(hC6accel,in1,(float *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();

    /* read in reference*/
    fread(pRefBuf_16bpp,  n, sizeof(float), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level*/
    if (comparefloatArrays((float *)pOutBuf_16bpp, (float *)pRefBuf_16bpp,n) != 1){
       printf("C6accel_MATH_sqrtsp() failed\n");
       return 0;
     }
    fclose(refFd);

   /*Read inputs */
   if ((refFd = fopen(ref_rsqrtsp_x1,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_rsqrtsp_x1);
         }
    fread(in1, n, sizeof(float), refFd);
    fclose(refFd);

    /* Open reference output files */
    if ((refFd = fopen(ref_rsqrtsp_rslt,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_rsqrtsp_rslt);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(float));
    memset (pRefBuf_16bpp,0x00,n*sizeof(float));

    LOG_STRING("C6accel_MATH_rsqrtsp(),");
    START_BENCHMARK();

    C6accel_MATH_rsqrtsp(hC6accel,in1,(float *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();

    /* read in reference*/
    fread(pRefBuf_16bpp,  n, sizeof(float), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level*/
    if (comparefloatArrays((float *)pOutBuf_16bpp, (float *)pRefBuf_16bpp,n) != 1){
       printf("C6accel_MATH_rsqrtsp() failed\n");
       return 0;
     }

    fclose(refFd);

   /*Read inputs */
   if ((refFd = fopen(ref_atan2sp_x1,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_atan2sp_x1);
         }
    fread(in1, n, sizeof(float), refFd);
    fclose(refFd);

    if ((refFd = fopen(ref_atan2sp_x2,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_atan2sp_x2);
         }
    fread(in2, n, sizeof(float), refFd);
    fclose(refFd);

    /* Open reference output files */
    if ((refFd = fopen(ref_atan2sp_rslt,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_atan2sp_rslt);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(float));
    memset (pRefBuf_16bpp,0x00,n*sizeof(float));

    LOG_STRING("C6accel_MATH_atan2sp(),");
    START_BENCHMARK();

    C6accel_MATH_atan2sp(hC6accel,(float *)in1,(float *)in2,(float *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();

    /* read in reference*/
    fread(pRefBuf_16bpp,  n, sizeof(float), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level*/
    if (comparefloatArrays((float *)pOutBuf_16bpp, (float *)pRefBuf_16bpp,n) != 1){
       printf("C6accel_MATH_atan2sp() failed\n");
       return 0;
     }

    fclose(refFd);

    /*Read inputs */
   if ((refFd = fopen(ref_powsp_x1,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_powsp_x1);
         }
    fread(in1, n, sizeof(float), refFd);
    fclose(refFd);

    if ((refFd = fopen(ref_powsp_x2,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_powsp_x2);
         }
    fread(in2, n, sizeof(float), refFd);
    fclose(refFd);

    /* Open reference output files */
    if ((refFd = fopen(ref_powsp_rslt,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_powsp_rslt);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(float));
    memset (pRefBuf_16bpp,0x00,n*sizeof(float));

    LOG_STRING("C6accel_MATH_powsp(),");
    START_BENCHMARK();

    C6accel_MATH_powsp(hC6accel,(float *)in1,(float *)in2,(float *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();

    /* read in reference*/
    fread(pRefBuf_16bpp,  n, sizeof(float), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level*/
    if (comparefloatArrays((float *)pOutBuf_16bpp, (float *)pRefBuf_16bpp,n) != 1){
       printf("C6accel_MATH_powsp() failed\n");
       return 0;
     }

    fclose(refFd);

    printf("Tests for Floating point RTS single precision functions successful\n");
    return 1;
}

/*

 * Test function for arithmetic rts double precision functions in this function
 */
int c6accel_test_MATH_RTSFLTDP(C6accel_Handle hC6accel,unsigned int n)
{
 FILE *refFd;
 double *in1;
 double *in2;
 double *out;
 int i;

/* Generate N bytes of 16 bit raw sine wave for use in tests

     Reuse pWorkingBuf2_16bpp for this
   */
    in1 = (double *)pSrcBuf_16bpp;
    in2 = (double *)pWorkingBuf2_16bpp;
    out = (double *)pRefBuf_16bpp;

   /*Read inputs */

   if ((refFd = fopen(ref_cosdp_x1,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_cosdp_x1);
         }
    fread(in1, n, sizeof(double), refFd);
    fclose(refFd);

    /* Open reference output files */
    if ((refFd = fopen(ref_cosdp_rslt,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_cosdp_rslt);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(double));
    memset (pRefBuf_16bpp,0x00,n*sizeof(double));

    LOG_STRING("C6accel_MATH_cosdp(),");
    START_BENCHMARK();

    C6accel_MATH_cosdp(hC6accel,in1,(double *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();

    /* read in reference*/
    fread(pRefBuf_16bpp,  n, sizeof(double), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level*/
    if (comparedoubleArrays((double *)pOutBuf_16bpp, (double *)pRefBuf_16bpp,n) != 1){
       printf("C6accel_MATH_cosdp() failed\n");
       return 0;
     }

    fclose(refFd);

   /*Read inputs */
   if ((refFd = fopen(ref_sindp_x1,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_sindp_x1);
         }
    fread(in1, n, sizeof(double), refFd);
    fclose(refFd);

    /* Open reference output files */
    if ((refFd = fopen(ref_sindp_rslt,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_sindp_rslt);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(double));
    memset (pRefBuf_16bpp,0x00,n*sizeof(double));

    LOG_STRING("C6accel_MATH_sindp(),");
    START_BENCHMARK();

    C6accel_MATH_sindp(hC6accel,in1,(double *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();

    /* read in reference*/
    fread(pRefBuf_16bpp,  n, sizeof(double), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level*/
    if (comparedoubleArrays((double *)pOutBuf_16bpp, (double *)pRefBuf_16bpp,n) != 1){
       printf("C6accel_MATH_sindp() failed\n");
       return 0;
     }

    fclose(refFd);

    /*Read inputs */
   if ((refFd = fopen(ref_atandp_x1,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_atandp_x1);
         }
    fread(in1, n, sizeof(double), refFd);
    fclose(refFd);

    /* Open reference output files */
    if ((refFd = fopen(ref_atandp_rslt,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_atandp_rslt);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(double));
    memset (pRefBuf_16bpp,0x00,n*sizeof(double));

    LOG_STRING("C6accel_MATH_atandp(),");
    START_BENCHMARK();

    C6accel_MATH_atandp(hC6accel,in1,(double *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();

    /* read in reference*/
    fread(pRefBuf_16bpp,  n, sizeof(double), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level*/
    if (comparedoubleArrays((double *)pOutBuf_16bpp, (double *)pRefBuf_16bpp,n) != 1){
       printf("C6accel_MATH_atandp() failed\n");
       return 0;
     }

    fclose(refFd);

    /*Read inputs */
   if ((refFd = fopen(ref_logdp_x1,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_logdp_x1);
         }
    fread(in1, n, sizeof(double), refFd);
    fclose(refFd);

    /* Open reference output files */
    if ((refFd = fopen(ref_logdp_rslt,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_logdp_rslt);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(double));
    memset (pRefBuf_16bpp,0x00,n*sizeof(double));

    LOG_STRING("C6accel_MATH_logdp(),");
    START_BENCHMARK();

    C6accel_MATH_logdp(hC6accel,in1,(double *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();

    /* read in reference*/
    fread(pRefBuf_16bpp,  n, sizeof(double), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level*/
    if (comparedoubleArrays((double *)pOutBuf_16bpp, (double *)pRefBuf_16bpp,n) != 1){
       printf("C6accel_MATH_logdp() failed\n");
       return 0;
     }

    fclose(refFd);

   /*Read inputs */
   if ((refFd = fopen(ref_expdp_x1,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_expdp_x1);
         }
    fread(in1, n, sizeof(double), refFd);
    fclose(refFd);

    /* Open reference output files */
    if ((refFd = fopen(ref_expdp_rslt,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_expdp_rslt);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(double));
    memset (pRefBuf_16bpp,0x00,n*sizeof(double));

    LOG_STRING("C6accel_MATH_expdp(),");
    START_BENCHMARK();

    C6accel_MATH_expdp(hC6accel,in1,(double *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();

    /* read in reference*/
    fread(pRefBuf_16bpp,  n, sizeof(double), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level*/
    if (comparedoubleArrays((double *)pOutBuf_16bpp, (double *)pRefBuf_16bpp,n) != 1){
       printf("C6accel_MATH_expdp() failed\n");
       return 0;
     }

    fclose(refFd);

    /*Read inputs */
   if ((refFd = fopen(ref_exp2dp_x1,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_exp2dp_x1);
         }
    fread(in1, n, sizeof(double), refFd);
    fclose(refFd);

    /* Open reference output files */
    if ((refFd = fopen(ref_exp2dp_rslt,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_exp2dp_rslt);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(double));
    memset (pRefBuf_16bpp,0x00,n*sizeof(double));

    LOG_STRING("C6accel_MATH_exp2dp(),");
    START_BENCHMARK();

    C6accel_MATH_exp2dp(hC6accel,in1,(double *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();

    /* read in reference*/
    fread(pRefBuf_16bpp,  n, sizeof(double), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level*/
    if (comparedoubleArrays((double *)pOutBuf_16bpp, (double *)pRefBuf_16bpp,n) != 1){
       printf("C6accel_MATH_exp2dp() failed\n");
       return 0;
     }

    fclose(refFd);

    /*Read inputs */
   if ((refFd = fopen(ref_exp10dp_x1,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_exp10dp_x1);
         }
    fread(in1, n, sizeof(double), refFd);
    fclose(refFd);

    /* Open reference output files */
    if ((refFd = fopen(ref_exp10dp_rslt,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_exp10dp_rslt);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(double));
    memset (pRefBuf_16bpp,0x00,n*sizeof(double));

    LOG_STRING("C6accel_MATH_exp10dp(),");
    START_BENCHMARK();

    C6accel_MATH_exp10dp(hC6accel,in1,(double *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();

    /* read in reference*/
    fread(pRefBuf_16bpp,  n, sizeof(double), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level*/
    if (comparedoubleArrays((double *)pOutBuf_16bpp, (double *)pRefBuf_16bpp,n) != 1){
       printf("C6accel_MATH_exp10dp() failed\n");
       return 0;
     }

    fclose(refFd);

   /*Read inputs */
   if ((refFd = fopen(ref_sqrtdp_x1,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_sqrtdp_x1);
         }
    fread(in1, n, sizeof(double), refFd);
    fclose(refFd);

    /* Open reference output files */
    if ((refFd = fopen(ref_sqrtdp_rslt,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_sqrtdp_rslt);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(double));
    memset (pRefBuf_16bpp,0x00,n*sizeof(double));

    LOG_STRING("C6accel_MATH_sqrtdp(),");
    START_BENCHMARK();

    C6accel_MATH_sqrtdp(hC6accel,in1,(double *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();

    /* read in reference*/
    fread(pRefBuf_16bpp,  n, sizeof(double), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level*/
    if (comparedoubleArrays((double *)pOutBuf_16bpp, (double *)pRefBuf_16bpp,n) != 1){
       printf("C6accel_MATH_sqrtdp() failed\n");
       return 0;
     }
    fclose(refFd);

   /*Read inputs */
   if ((refFd = fopen(ref_rsqrtdp_x1,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_rsqrtdp_x1);
         }
    fread(in1, n, sizeof(double), refFd);
    fclose(refFd);

    /* Open reference output files */
    if ((refFd = fopen(ref_rsqrtdp_rslt,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_rsqrtdp_rslt);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(double));
    memset (pRefBuf_16bpp,0x00,n*sizeof(double));

    LOG_STRING("C6accel_MATH_rsqrtdp(),");
    START_BENCHMARK();

    C6accel_MATH_rsqrtdp(hC6accel,in1,(double *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();

    /* read in reference*/
    fread(pRefBuf_16bpp,  n, sizeof(double), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level*/
    if (comparedoubleArrays((double *)pOutBuf_16bpp, (double *)pRefBuf_16bpp,n) != 1){
       printf("C6accel_MATH_rsqrtdp() failed\n");
       return 0;
     }

    fclose(refFd);

   /*Read inputs */
   if ((refFd = fopen(ref_atan2dp_x1,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_atan2dp_x1);
         }
    fread(in1, n, sizeof(double), refFd);
    fclose(refFd);

    if ((refFd = fopen(ref_atan2dp_x2,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_atan2dp_x2);
         }
    fread(in2, n, sizeof(double), refFd);
    fclose(refFd);

    /* Open reference output files */
    if ((refFd = fopen(ref_atan2dp_rslt,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_atan2dp_rslt);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(double));
    memset (pRefBuf_16bpp,0x00,n*sizeof(double));

    LOG_STRING("C6accel_MATH_atan2dp(),");
    START_BENCHMARK();

    C6accel_MATH_atan2dp(hC6accel,(double *)in1,(double *)in2,(double *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();

    /* read in reference*/
    fread(pRefBuf_16bpp,  n, sizeof(double), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level*/
    if (comparedoubleArrays((double *)pOutBuf_16bpp, (double *)pRefBuf_16bpp,n) != 1){
       printf("C6accel_MATH_atan2dp() failed\n");
       return 0;
     }

    fclose(refFd);

    /*Read inputs */
   if ((refFd = fopen(ref_powdp_x1,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_powdp_x1);
         }
    fread(in1, n, sizeof(double), refFd);
    fclose(refFd);

    if ((refFd = fopen(ref_powdp_x2,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_powdp_x2);

         }
    fread(in2, n, sizeof(double), refFd);
    fclose(refFd);

    /* Open reference output files */
    if ((refFd = fopen(ref_powdp_rslt,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_powdp_rslt);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,n*sizeof(double));
    memset (pRefBuf_16bpp,0x00,n*sizeof(double));

    LOG_STRING("C6accel_MATH_powdp(),");
    START_BENCHMARK();

    C6accel_MATH_powdp(hC6accel,(double *)in1,(double *)in2,(double *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();

    /* read in reference*/
    fread(pRefBuf_16bpp,  n, sizeof(double), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level*/
    if (comparedoubleArrays((double *)pOutBuf_16bpp, (double *)pRefBuf_16bpp,n) != 1){
       printf("C6accel_MATH_powdp() failed\n");
       return 0;
     }

    fclose(refFd);

    printf("Tests for Floating point RTS double precision functions successful\n");
    return 1;
}

/*
 * Test function for floating point autocorrelation function
*/
int c6accel_test_DSPF_AUTOCOR(C6accel_Handle hC6accel, int Nx, int Nr)
{
 FILE *refFd;
 float *in1;
 int i;

/* Initialize Source and Output buffers to contiguous memory allocated   */
    in1 = (float *)pSrcBuf_16bpp;
    
   /*Read inputs */
   if ((refFd = fopen(ref_autocor_x,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_autocor_x);
         }
    fread(in1, Nx+Nr, sizeof(float), refFd);
    fclose(refFd);

    /* Open reference output files */
    if ((refFd = fopen(ref_autocor_rslt,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_autocor_rslt);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,Nr*sizeof(float));
    memset (pRefBuf_16bpp,0x00,Nr*sizeof(float));

    LOG_STRING("C6accel_DSPF_autocor(),");
    START_BENCHMARK();

    C6accel_DSPF_sp_autocor(hC6accel,(float *)pOutBuf_16bpp,(float *)in1,Nx, Nr);

    END_AND_WRITE_BENCHMARK();

    /* read in reference*/
    fread(pRefBuf_16bpp,  Nr, sizeof(float), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on float level*/
    if (comparefloatArrays((float *)pOutBuf_16bpp, (float *)pRefBuf_16bpp,Nr) != 1){
       printf("C6accel_DSPF_autocor() failed\n");
       return 0;
     }
    
    fclose(refFd);

    printf("Tests for Floating point DSP autocorrelation functions successful\n");
    return 1;
}

/*
 * Test function for floating point convolution function
*/
int c6accel_test_DSPF_CONVOL(C6accel_Handle hC6accel,short nh,short ny)
{
 FILE *refFd;
 float *inX;
 float  *inH;
 int i;

/* Initialize Source and Output buffers to contiguous memory allocated   */
    inX = (float *)pSrcBuf_16bpp;
    inH = (float *)pWorkingBuf2_16bpp;
    
   /*Read inputs */
   if ((refFd = fopen(ref_convol_xinput,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_convol_xinput);
         }
    fread(inX, nh+ny-1, sizeof(float), refFd);
    fclose(refFd);
   
    if ((refFd = fopen(ref_convol_hinput,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_convol_hinput);
         }
    fread(inH, nh, sizeof(float), refFd);
    fclose(refFd);
   
    /* Open reference output files */
    if ((refFd = fopen(ref_convol_rslt,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_convol_rslt);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,ny*sizeof(float));
    memset (pRefBuf_16bpp,0x00,ny*sizeof(float));

    LOG_STRING("C6accel_DSPF_convol(),");
    START_BENCHMARK();

    C6accel_DSPF_sp_convol(hC6accel,(float *)inX,(float *)inH,(float *)pOutBuf_16bpp,(short)nh, (short)ny);
    END_AND_WRITE_BENCHMARK();

    /* read in reference*/
    fread(pRefBuf_16bpp,  ny, sizeof(float), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on float level*/
    if (comparefloatArrays((float *)pOutBuf_16bpp, (float *)pRefBuf_16bpp,ny) != 1){
       printf("C6accel_DSPF_convol() failed\n");
       return 0;
     }

    fclose(refFd);

    printf("Tests for Floating point DSP convolution functions successful\n");
    return 1;
}


/*
 * Test function for floating point Matrix multiplication functions
*/
int c6accel_test_DSPF_MATFXNS(C6accel_Handle hC6accel, int c1, int r1, int c2 )
{
 FILE *refFd;
 float *inX1;
 float  *inX2;
 float *out;
 int i;
    
    memset (pSrcBuf_16bpp,0x00, c1*r1*sizeof(float));
    memset (pWorkingBuf2_16bpp,0x00, c1*c2*sizeof(float));
/* Initialize Source and Output buffers to contiguous memory allocated   */
    inX1 = (float *)pSrcBuf_16bpp;
    inX2 = (float *)pWorkingBuf2_16bpp;
    out = (float *)pRefBuf_16bpp;

   /*Read inputs */
   if ((refFd = fopen(ref_mat_mul_x1,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_mat_mul_x1);
         }
    fread(inX1, c1*r1, sizeof(float), refFd);
    fclose(refFd);
    
    if ((refFd = fopen(ref_mat_mul_x2,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_mat_mul_x2);
         }
    fread(inX2, c1*c2, sizeof(float), refFd);
    fclose(refFd);
    
    /* Open reference output files */
    if ((refFd = fopen(ref_mat_mul_rslt,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_mat_mul_rslt);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00, r1*c2*sizeof(float));
    memset (pRefBuf_16bpp,0x00, r1*c2*sizeof(float));

    LOG_STRING("C6accel_DSPF_sp_mat_mul(),");
    START_BENCHMARK();

    C6accel_DSPF_sp_mat_mul(hC6accel,inX1,r1, c1,inX2,c2,(float *)pOutBuf_16bpp);
    END_AND_WRITE_BENCHMARK();

    /* read in reference*/
    fread(pRefBuf_16bpp,  r1*c2, sizeof(float), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level*/
    if (comparefloatArrays((float *)pOutBuf_16bpp, (float *)pRefBuf_16bpp, r1*c2) != 1){
       printf("C6accel_DSPF_mat_mul() failed\n");
       return 0;
     }

    fclose(refFd);



    printf("Tests for Floating point DSP matrix functions successful\n");
    return 1;
}


// Test for Matrix Multiplication of complex matrices
int c6accel_test_DSPF_MAT_MUL_CPLX(C6accel_Handle hC6accel,int c1,int r1,int c2 )
{
 FILE *refFd;
 float *inX1;
 float  *inX2;
 float *out;
 int i;

/* Initialize Source and Output buffers to contiguous memory allocated   */
    inX1 = (float *)pSrcBuf_16bpp;
    inX2 = (float *)pWorkingBuf2_16bpp;
    out = (float *)pRefBuf_16bpp;

// Test for Matrix Multiplication of complex matrices
    /*Read inputs */
   if ((refFd = fopen(ref_mat_mul_cplx_x1,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_mat_mul_cplx_x1);
         }
    fread(inX1, 2*c1*r1, sizeof(float), refFd);
    fclose(refFd);
    
    if ((refFd = fopen(ref_mat_mul_cplx_x2,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_mat_mul_cplx_x2);
         }
    fread(inX2, 2*c1*c2, sizeof(float), refFd);
    fclose(refFd);
    
    /* Open reference output files */
    if ((refFd = fopen(ref_mat_mul_cplx_rslt,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_mat_mul_cplx_rslt);
       return (0);
     }
    
    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00,2* r1*c2*sizeof(float));
    memset (pRefBuf_16bpp,0x00, 2*r1*c2*sizeof(float));

    LOG_STRING("C6accel_DSPF_sp_mat_mul_cplx(),");
    START_BENCHMARK();

    C6accel_DSPF_sp_mat_mul_cplx(hC6accel,inX1,r1, c1,inX2,c2,(float *)pOutBuf_16bpp);
    END_AND_WRITE_BENCHMARK();

    /* read in reference*/
    fread(pRefBuf_16bpp,  2*r1*c2, sizeof(float), refFd);
    LOG_STRING("\n");
    
    /* Validate output between native MATH/ref and C6Accel on float level*/
    if (comparefloatArrays((float *)pOutBuf_16bpp, (float *)pRefBuf_16bpp,2*r1*c2) != 1){
       printf("C6accel_DSPF_mat_mul_cplx() failed\n");
       return 0;
     }

    fclose(refFd);

    printf("Tests for Floating point DSP Matrix Multiplication functions successful\n");
    return 1;
}
/*
 * Test function for floating point Matrix transpose
*/
int c6accel_test_DSPF_MAT_TRANS(C6accel_Handle hC6accel, int r1, int c1 )
{
 FILE *refFd;
 float *inX1;
 int i;

/* Initialize Source and Output buffers to contiguous memory allocated   */
    inX1 = (float *)pSrcBuf_16bpp;
    
   // Test for Matrix Transpose of complex matrices
    /*Read inputs */
   if ((refFd = fopen(ref_mat_trans_xinput,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_mat_trans_xinput);
         }
    fread(inX1, c1*r1, sizeof(float), refFd);
    fclose(refFd);

     /* Open reference output files */
    if ((refFd = fopen(ref_mat_trans_rslt,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_mat_trans_rslt);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00, r1*c1*sizeof(float));
    memset (pRefBuf_16bpp,0x00, r1*c1*sizeof(float));

    LOG_STRING("C6accel_DSPF_sp_trans(),");
    START_BENCHMARK();

    C6accel_DSPF_sp_mat_trans(hC6accel,inX1,r1, c1,(float *)pOutBuf_16bpp);
    END_AND_WRITE_BENCHMARK();

    /* read in reference*/
    fread(pRefBuf_16bpp,  r1*c1, sizeof(float), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on float level*/
    if (comparefloatArrays((float *)pOutBuf_16bpp, (float *)pRefBuf_16bpp,r1*c1) != 1){
       printf("C6accel_DSPF_mat_trans() failed\n");
       return 0;
     }

    fclose(refFd);

    printf("Test for Floating point DSP matrix transpose function successful\n");
    return 1;
}

/*
 * Test function for floating point vector dot product functions
*/
int c6accel_test_DSPF_DOTPRODFXNS(C6accel_Handle hC6accel,unsigned int n)
{
 FILE *refFd;
 float *inX1;
 float  *inX2;
 float *out;
 int i;

/* Initialize Source and Output buffers to contiguous memory allocated   */
    inX1 = (float *)pSrcBuf_16bpp;
    inX2 = (float *)pWorkingBuf2_16bpp;
    out = (float *)pRefBuf_16bpp;

   /*Read inputs */
   if ((refFd = fopen(ref_dotprod_x1,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_dotprod_x1);
         }
    fread(inX1, n, sizeof(float), refFd);
    fclose(refFd);

    if ((refFd = fopen(ref_dotprod_x2,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_dotprod_x2);
         }
    fread(inX2, n, sizeof(float), refFd);
    fclose(refFd);

    /* Open reference output files */
    if ((refFd = fopen(ref_dotprod_rslt,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_dotprod_rslt);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00, n*sizeof(float));
    memset (pRefBuf_16bpp,0x00, n*sizeof(float));

    LOG_STRING("C6accel_DSPF_sp_dotprod(),");
    START_BENCHMARK();

    C6accel_DSPF_sp_dotprod(hC6accel,inX1,inX2,(float *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();

    /* read in reference*/
    fread(pRefBuf_16bpp,  1, sizeof(float), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level*/
    if (comparefloatArrays((float *)pOutBuf_16bpp, (float *)pRefBuf_16bpp,1) != 1){
       printf("C6accel_DSPF_sp_dotprod() failed\n");
       return 0;
     }

    fclose(refFd);

    printf("Test for Floating point DSP vector dot product function executed successful\n");
    return 1;
}


/*
 * Test for FIR filter generation kernels in c6accel
 */
int c6accel_test_DSPF_FIR(C6accel_Handle hC6accel,unsigned int ny,unsigned int nh)
{
    FILE *refFd;
    float *inX, *inH;
    int i;
    unsigned int nx = ny+nh-1;

    /* C6accel_DSP_fir_gen() test */
    /* Read 16 bit input */
    inX= (float *)pSrcBuf_16bpp;
    inH = (float *)pWorkingBuf2_16bpp;

   /*Read inputs */
   if ((refFd = fopen(ref_fir_gen_xinput,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_fir_gen_xinput);
         }
    fread(inX, nx, sizeof(float), refFd);
    fclose(refFd);

    if ((refFd = fopen(ref_fir_gen_hinput,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_fir_gen_hinput);
         }
    fread(inH, nh, sizeof(float), refFd);
    fclose(refFd);

    /* Open reference file to store reference output */
    if ((refFd = fopen(ref_fir_gen_rslt,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_fir_gen_rslt);
       return (0);
     }

    /* Clear output arrays before tests begins */
    memset (pOutBuf_16bpp,0x0000, ny);
    memset (pRefBuf_16bpp,0x0000, ny);

    LOG_STRING("C6accel_DSPF_sp_fir_gen(),");
    START_BENCHMARK();

    /* Call the DSP_fir_gen function in C6Accel*/
    C6accel_DSPF_sp_fir_gen(hC6accel,inX, inH,(float *)pOutBuf_16bpp,nh,ny);
    END_AND_WRITE_BENCHMARK();

    fread(pRefBuf_16bpp, ny, sizeof(float), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on 16 bit short level*/
    if (comparefloatArrays((float *)pOutBuf_16bpp, (float *)pRefBuf_16bpp,ny) != 1){
       printf("C6accel_DSPF_sp_fir_gen() failed\n");
       return 0;
     }
    printf("Test for Floating point DSP FIR filter successful\n");
     fclose(refFd);
     return 1;
}

int c6accel_test_DSPF_FIR_CPLX(C6accel_Handle hC6accel, int ny, int nh)
{
    FILE *refFd;
    float *inX, *inH;
    int i,status;
    unsigned int nx = ny+nh-1;

    /* C6accel_DSP_fir_gen() test */
    /* Read 16 bit input */
    inX= (float *)pSrcBuf_16bpp;
    inH = (float *)pWorkingBuf2_16bpp;

    /* C6accel_DSP_fir_cplx() test */

   /*Read inputs */
   if ((refFd = fopen(ref_fir_cplx_xinput,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_fir_cplx_xinput);
         }
    fread(inX, 2*nx, sizeof(float), refFd);
    fclose(refFd);

    if ((refFd = fopen(ref_fir_cplx_hinput,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_fir_cplx_hinput);
         }
    fread(inH, 2*nh, sizeof(float), refFd);
    fclose(refFd);

    /* Open reference file to store reference output */
    if ((refFd = fopen(ref_fir_cplx_rslt,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_fir_cplx_rslt);
       return (0);
     }

    /* Clear output arrays before tests begins */
    memset (pOutBuf_16bpp,0x0000, 2*ny);
    memset (pRefBuf_16bpp,0x0000, 2*ny);

    LOG_STRING("C6accel_DSPF_sp_fir_cplx(),");
    START_BENCHMARK();

    /* Call the DSP_fir_cplx function in C6Accel*/
    status = C6accel_DSPF_sp_fir_cplx(hC6accel,(float *)inX, inH,(float *)pOutBuf_16bpp,nh,ny);
    END_AND_WRITE_BENCHMARK();

    fread(pRefBuf_16bpp, 2*ny, sizeof(float), refFd);
    LOG_STRING("\n");

    /* Validate output between native MATH/ref and C6Accel on float level*/
    if (comparefloatArrays((float *)pOutBuf_16bpp, (float *)pRefBuf_16bpp,2*ny) != 1){
       printf("C6accel_DSPF_sp_fir_cplx() failed\n");
       return 0;
     }

    fclose(refFd);

    printf("Test for Floating point Complex FIR Filtering successful\n");
    return (1);
}

/*
 * Test for IIR filter generation kernels in c6accel
 */
int c6accel_test_DSPF_IIR(C6accel_Handle hC6accel, int n)
{
    FILE *refFd;
    float *inX ;
    float *hb, *ha;
    int i,status;
    float *y2;

    /*  Generate the 16 bit input  */
    inX = (float *)pSrcBuf_16bpp;
    ha = (float *)pWorkingBuf2_16bpp;
    hb =(float*)pWorkingBuf_16bpp;
    y2 = (float*)pWorkingBuf3_16bpp;

     /*Read inputs */
    if ((refFd = fopen(ref_iir_xinput,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_iir_xinput);
         }
    fread(inX, n+4, sizeof(float), refFd);
    fclose(refFd);

    if ((refFd = fopen(ref_iir_ha,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_iir_ha);
         }
    fread(ha, 5, sizeof(float), refFd);
    fclose(refFd);

    if ((refFd = fopen(ref_iir_hb,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_iir_hb);
         }
    fread(hb, 5, sizeof(float), refFd);
    fclose(refFd);


    if ((refFd = fopen(ref_iir_y1,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_iir_y1);
       return (0);
      }

    /*  Clear output arrays */
    memset (pOutBuf_16bpp,0x00, n*sizeof(float));
    memset (pRefBuf_16bpp,0x00, n*sizeof(float));

    LOG_STRING("C6accel_DSPF_sp_iir(),");
    START_BENCHMARK();

    /* Call IIR kernel in C6Accel */
    status = C6accel_DSPF_sp_iir(hC6accel,(float *)pOutBuf_16bpp,inX,y2,hb,ha,n);
   
    END_AND_WRITE_BENCHMARK();

    fread(pRefBuf_16bpp, n+4,sizeof(float), refFd);

    LOG_STRING("\n");

   /* Validate output between native MATH/ref and C6Accel on float level*/
    if (comparefloatArrays((float *)pOutBuf_16bpp, (float *)pRefBuf_16bpp,n+4) != 1){
       printf("C6accel_DSPF_sp_iir() failed\n");
       return 0;
     }

     fclose(refFd);

     printf("Tests for Floating point DSP IIR Filter successful\n");
     return (1);
}

/*
 * Test function for floating point vector multiplication functions
*/
int c6accel_test_DSPF_VECMUL(C6accel_Handle hC6accel,unsigned int n )
{
 FILE *refFd;
 float *inX1;
 float  *inX2;
 float *out;
 int i;

/* Initialize Source and Output buffers to contiguous memory allocated   */
    inX1 = (float *)pSrcBuf_16bpp;
    inX2 = (float *)pWorkingBuf2_16bpp;
    out = (float *)pRefBuf_16bpp;

   /*Read inputs */
   if ((refFd = fopen(ref_vecmul_x1input,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_vecmul_x1input);
         }
    fread(inX1, n, sizeof(float), refFd);
    fclose(refFd);

    if ((refFd = fopen(ref_vecmul_x2input,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_vecmul_x2input);
         }
    fread(inX2, n, sizeof(float), refFd);
    fclose(refFd);

    /* Open reference output files */
    if ((refFd = fopen(ref_vecmul_rslt,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_vecmul_rslt);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00, n*sizeof(float));
    memset (pRefBuf_16bpp,0x00, n*sizeof(float));

    LOG_STRING("C6accel_DSPF_sp_vecmul(),");
    START_BENCHMARK();

    C6accel_DSPF_sp_vecmul(hC6accel,inX1,inX2,(float *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();

    /* read in reference*/
    fread(pRefBuf_16bpp, n, sizeof(float), refFd);
    LOG_STRING("\n");

    /* Validate output between native DSP/ref and C6Accel on 16 bit short level*/
    if (comparefloatArrays((float *)pOutBuf_16bpp, (float *)pRefBuf_16bpp, n) != 1){
       printf("C6accel_DSPF_vecmul() failed\n");
       return 0;
     }

    fclose(refFd);

     printf("Test for Floating point DSP vector multiplication successful\n");
    return 1;
}

/*
 * Test function for floating point vector multiplication functions
*/
int c6accel_test_DSPF_VECRECIP(C6accel_Handle hC6accel,unsigned int n )
{
 FILE *refFd;
 float *inX1;
 float *out;
 int i;

/* Initialize Source and Output buffers to contiguous memory allocated   */
    inX1 = (float *)pSrcBuf_16bpp;
    out = (float *)pRefBuf_16bpp;


 //Test Vector sum function in C6accel
    /*Read inputs */
   if ((refFd = fopen(ref_vecrecip_xinput,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_vecrecip_xinput);
         }
    fread(inX1, n, sizeof(float), refFd);
    fclose(refFd);


    /* Open reference output files */
    if ((refFd = fopen(ref_vecrecip_rslt,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_vecrecip_rslt);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00, n*sizeof(float));
    memset (pRefBuf_16bpp,0x00, n*sizeof(float));

    LOG_STRING("C6accel_DSPF_sp_vecrecip(),");
    START_BENCHMARK();

    C6accel_DSPF_sp_vecrecip(hC6accel,inX1,(float *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();

    /* read in reference*/
    fread(pRefBuf_16bpp, n, sizeof(float), refFd);
    LOG_STRING("\n");

    /* Validate output between native DSP/ref and C6Accel on 16 bit short level*/
    if (comparefloatArrays((float *)pOutBuf_16bpp, (float *)pRefBuf_16bpp, n) != 1){
       printf("C6accel_DSPF_vecrecip() failed\n");
       return 0;
     }

    fclose(refFd);
    printf("Test for Floating point DSP vector reciprocal function successful\n");
    return 1;
}


/*
 * Test function for floating point vector sum of squares functions
*/
int c6accel_test_DSPF_VECSUM_SQ(C6accel_Handle hC6accel,unsigned int n )
{
 FILE *refFd;
 float *inX1;
 float *out;
 int i;

/* Initialize Source and Output buffers to contiguous memory allocated   */
    inX1 = (float *)pSrcBuf_16bpp;
    out = (float *)pRefBuf_16bpp;


 //Test Vector sum function in C6accel
    /*Read inputs */
   if ((refFd = fopen(ref_vecsum_sq_xinput,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_vecsum_sq_xinput);
         }
    fread(inX1, n, sizeof(float), refFd);
    fclose(refFd);


    /* Open reference output files */
    if ((refFd = fopen(ref_vecsum_sq_rslt,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_vecsum_sq_rslt);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00, n*sizeof(float));
    memset (pRefBuf_16bpp,0x00, n*sizeof(float));

    LOG_STRING("C6accel_DSPF_sp_vecsum_sq(),");
    START_BENCHMARK();

    C6accel_DSPF_sp_vecsum_sq(hC6accel,inX1,(float *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();

    /* read in reference*/
    fread(pRefBuf_16bpp, n, sizeof(float), refFd);
    LOG_STRING("\n");

    /* Validate output between native DSP/ref and C6Accel on 16 bit short level*/
    if (comparefloatArrays((float *)pOutBuf_16bpp, (float *)pRefBuf_16bpp, n) != 1){
       printf("C6accel_DSPF_vecsum_sq() failed\n");
       return 0;
     }

    fclose(refFd);
    printf("Test for Floating point DSP vector reciprocal function successful\n");
    return 1;
}

/*
 * Test function for floating point weighted vector sum functions
*/
int c6accel_test_DSPF_W_VEC(C6accel_Handle hC6accel,float M, unsigned int n )
{
 FILE *refFd;
 float *inX1;
 float *inX2;
 float *out;
 int i;

/* Initialize Source and Output buffers to contiguous memory allocated   */
    inX1 = (float *)pSrcBuf_16bpp;
    out = (float *)pRefBuf_16bpp;
    inX2 = (float *)pWorkingBuf2_16bpp;


 //Test Vector sum function in C6accel
    /*Read inputs */
   if ((refFd = fopen(ref_w_vec_x1input,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_w_vec_x1input);
         }
    fread(inX1, n, sizeof(float), refFd);
    fclose(refFd);

    /*Read inputs */
   if ((refFd = fopen(ref_w_vec_x2input,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_w_vec_x2input);
         }
    fread(inX2, n, sizeof(float), refFd);
    fclose(refFd);

     /* Open reference output files */
    if ((refFd = fopen(ref_w_vec_rslt,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_w_vec_rslt);
       return (0);
     }

    /* Clear output arrays */
    memset (pOutBuf_16bpp,0x00, n*sizeof(float));
    memset (pRefBuf_16bpp,0x00, n*sizeof(float));

    LOG_STRING("C6accel_DSPF_sp_w_vec(),");
    START_BENCHMARK();

    C6accel_DSPF_sp_w_vec(hC6accel,inX1,inX2,M,(float *)pOutBuf_16bpp,n);
    END_AND_WRITE_BENCHMARK();

    /* read in reference*/
    fread(pRefBuf_16bpp, n, sizeof(float), refFd);
    LOG_STRING("\n");

    /* Validate output between native DSP/ref and C6Accel on 16 bit short level */
    if (comparefloatArrays((float *)pOutBuf_16bpp, (float *)pRefBuf_16bpp, n) != 1){
       printf("C6accel_DSPF_w_vec() failed\n");
       return 0;
     }

    fclose(refFd);
    printf("Test for Floating point DSP vector weighted sum function successful\n");
    return 1;
}

int c6accel_test_DSPF_sp_fftSPxSP(C6accel_Handle hC6accel,int n,int n_min,int offset, int n_max)
{
    FILE  *refFd,*refFd1;
    float *w;
    float *inX;
    unsigned char *brev;
    int    i;

       /*Test C6accel_DSPF_sp_fftSPxSP() */

    /*Generate the 16 bit fixed version of sine */
    inX = (float *)pSrcBuf_16bpp;
    if ((refFd1 = fopen(ref_fftSPxSP_xinput,"rb")) == NULL){
       printf("Failed to open refFd on %s\n",ref_fftSPxSP_xinput);
       return (0);
       }

    fread(inX,  2*n, sizeof(float), refFd1);
    fclose(refFd1);

    /* Read in the twiddle table, use pWorkingBuf_16bpp for the table */
    w = (float*)pWorkingBuf_16bpp;

    if ((refFd1 = fopen(ref_fftSPxSP_twiddle,"rb")) == NULL){
       printf("Failed to open refFd on %s\n",ref_fftSPxSP_twiddle);
       return (0);
       }
    fread(w,  2*n, sizeof(float), refFd1);
    fclose(refFd1);

    /* Read in the twiddle table, use pWorkingBuf_16bpp for the table */
    brev = (unsigned char*)pWorkingBuf2_16bpp;

    if ((refFd1 = fopen(ref_fftSPxSP_brev,"rb")) == NULL){
       printf("Failed to open refFd on %s\n",ref_fftSPxSP_brev);
       return (0);
       }
    fread(brev,  64, sizeof(unsigned char), refFd1);
    fclose(refFd1);


    if ((refFd = fopen(ref_fftSPxSP_rslt,"rb")) == NULL){
       printf("Failed to open refFd on %s\n",ref_fftSPxSP_rslt);
       return (0);
       }

    /* Clear output arrays*/
    memset (pOutBuf_16bpp,0xA5,n*sizeof(float));
    memset (pRefBuf_16bpp,0xA5,n*sizeof(float));

    LOG_STRING("C6accel_DSPF_sp_fftSPxSP(),");
    START_BENCHMARK();

    /* Call the DSPF_sp_fftSPxSP function in C6Accel*/
    C6accel_DSPF_sp_fftSPxSP (hC6accel,n,inX,w,(float*)pOutBuf_16bpp,brev,n_min, offset, n_max);

    END_AND_WRITE_BENCHMARK();

    /* read in reference from test file*/
    fread(pRefBuf_16bpp,  2*n, sizeof(float), refFd);
    LOG_STRING("\n");

    /* Validate output between native DSP/ref and C6Accel on 16 bit float level */
    if (comparefloatArrays((float*)pOutBuf_16bpp, (float*)pRefBuf_16bpp,n) != 1)
         {printf("C6accel_DSPF_sp_fftSPxSP() failed\n");
          return 0;
         }

     fclose(refFd);
    printf("Test for Floating point DSP FFT function successful\n");
    return 1;
}

int c6accel_test_DSPF_sp_ifftSPxSP(C6accel_Handle hC6accel,int n,int n_min,int offset, int n_max)
{
    FILE  *refFd,*refFd1;
    float *w;
    float *inX;
    unsigned char *brev;
    int    i,status;

    /*Test C6accel_DSPF_sp_ifftSPxSP() */
     Memory_cacheWbInv(pSrcBuf_16bpp,2*n*sizeof(float));
     Memory_cacheWbInv(pWorkingBuf_16bpp,2*n*sizeof(float));
    /*Generate the 16 bit fixed version of sine */
    inX = (float *)pSrcBuf_16bpp;
    if ((refFd1 = fopen(ref_ifftSPxSP_xinput,"rb")) == NULL){
       printf("Failed to open refFd on %s\n",ref_ifftSPxSP_xinput);
       return (0);
       }

    fread(inX,  2*n, sizeof(float), refFd1);
    fclose(refFd1);

    /* Read in the twiddle table, use pWorkingBuf_16bpp for the table */
    w = (float*)pWorkingBuf_16bpp;

    if ((refFd1 = fopen(ref_ifftSPxSP_twiddle,"rb")) == NULL){
       printf("Failed to open refFd on %s\n",ref_ifftSPxSP_twiddle);
       return (0);
       }
    fread(w,  2*n, sizeof(float), refFd1);
    fclose(refFd1);

    /* Read in the twiddle table, use pWorkingBuf_16bpp for the table */
    brev = (unsigned char*)pWorkingBuf2_16bpp;

    if ((refFd1 = fopen(ref_ifftSPxSP_brev,"rb")) == NULL){
       printf("Failed to open refFd on %s\n",ref_ifftSPxSP_brev);
       return (0);
       }
    fread(brev,  64, sizeof(unsigned char), refFd1);
    fclose(refFd1);


    if ((refFd = fopen(ref_ifftSPxSP_rslt,"rb")) == NULL){
       printf("Failed to open refFd on %s\n",ref_ifftSPxSP_rslt);
       return (0);
       }

    /* Clear output arrays*/
    memset (pOutBuf_16bpp,0xA5,2*n*sizeof(float));
    memset (pRefBuf_16bpp,0xA5,2*n*sizeof(float));

    LOG_STRING("C6accel_DSPF_sp_ifftSPxSP(),");
    START_BENCHMARK();

    /* Call the DSPF_sp_ifftSPxSP function in C6Accel*/
    status= C6accel_DSPF_sp_ifftSPxSP (hC6accel,n,inX,w,(float*)pOutBuf_16bpp,brev,n_min, offset, n_max);
    
    END_AND_WRITE_BENCHMARK();

    /* read in reference from test file*/
    fread(pRefBuf_16bpp,  2*n, sizeof(float), refFd);
    LOG_STRING("\n");

    /* Validate output between native DSP/ref and C6Accel on 16 bit float level */
    if (comparefloatArrays((float*)pOutBuf_16bpp, (float*)pRefBuf_16bpp,n) != 1)
         {printf("C6accel_DSPF_sp_ifftSPxSP() failed\n");
          return 0;
         }

     fclose(refFd);
    printf("Test for Floating point DSP Inverse FFT function successful\n");
    return 1;
}

/*
 * Test for BIQUAD filter generation kernels in c6accel
 */
int c6accel_test_DSPF_BIQUAD(C6accel_Handle hC6accel,unsigned int n)
{
    FILE *refFd;
    float *inX ;
    float *hb, *ha;
    float *delay;
    int i,status;
    float y2;
    /* Create a CMem heap array that can be translated for DSP */
    short *pdelay_Cmem;
    pdelay_Cmem = Memory_alloc(2*sizeof(float),&testfxnsMemParams);

    /*  Generate the 16 bit input  */
    inX = (float *)pSrcBuf_16bpp;
    ha = (float *)pWorkingBuf2_16bpp;
    hb =(float*)pWorkingBuf_16bpp;
    delay =(float*)pdelay_Cmem;

     /*Read inputs */
    if ((refFd = fopen(ref_biquad_xinput,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_biquad_xinput);
         }
    fread(inX, n, sizeof(float), refFd);
    fclose(refFd);

    if ((refFd = fopen(ref_biquad_ha,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_biquad_ha);
         }
    fread(ha, 3, sizeof(float), refFd);
    fclose(refFd);

    if ((refFd = fopen(ref_biquad_hb,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_biquad_hb);
         }
    fread(hb, 3, sizeof(float), refFd);
    fclose(refFd);

    if ((refFd = fopen(ref_biquad_delay,"rb")) == NULL) {
        printf("Failed to open refFd on %s\n",ref_biquad_delay);
         }
    fread( delay, 2, sizeof(float), refFd);
    fclose(refFd);


    if ((refFd = fopen(ref_biquad_y,"rb")) == NULL) {
       printf("Failed to open refFd on %s\n",ref_biquad_y);
       return (0);
      }

    /*  Clear output arrays */
    memset (pOutBuf_16bpp,0x00, n*sizeof(float));
    memset (pRefBuf_16bpp,0x00, n*sizeof(float));

    LOG_STRING("C6accel_DSPF_sp_biquad(),");
    START_BENCHMARK();

    /* Call BIQUAD kernel in C6Accel */
   status = C6accel_DSPF_sp_biquad(hC6accel,inX,hb,ha,delay,(float *)pOutBuf_16bpp,n);
   
   END_AND_WRITE_BENCHMARK();

    fread(pRefBuf_16bpp, 1,n*sizeof(float), refFd);

    LOG_STRING("\n");

   /* Validate output between native DSP/ref and C6Accel on float level*/
    if (comparefloatArrays((float *)pOutBuf_16bpp, (float *)pRefBuf_16bpp,n) != 1){
       printf("C6accel_DSPF_sp_biquad() failed\n");
       return 0;
     }

     fclose(refFd);

     printf("Test for Floating point DSP BIQUAD Filter successful\n");
     return (1);
}
#endif

/* Generates a psuedo-random float between 0.0 and 0.999... */
float randfloat(){
    return (rand()/((float)(RAND_MAX)+1));
}

float randflt(float min, float max){
    if (min>max){
        return randfloat()*(min-max)+max;
    }
    else
    {
        return randfloat()*(max-min)+min;
    }
}

/* Generates a psuedo-random integer between 0 and 32767*/
int randint()
{
    return rand();
}

/* Generates a psuedo-random integer between 0 and max */
int randuint(int max)
{
    return (int)(max*rand()/(RAND_MAX+1.0));
}

/* Generates a psuedo-random integer between min and max */
int randInt(int min, int max)
{
    if (min>max){
        return max+(int)((min-max+1)*rand()/(RAND_MAX+1.0));
    }
    else{
        return min+(int)((max-min+1)*rand()/(RAND_MAX+1.0));
    }
}



