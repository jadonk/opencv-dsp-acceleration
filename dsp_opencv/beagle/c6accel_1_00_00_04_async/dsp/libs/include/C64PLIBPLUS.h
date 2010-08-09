/*******************************************************************************
**+--------------------------------------------------------------------------+**
 *                                                                            **
 * Copyright (c) 2010, Texas Instruments Incorporated                         **
 * All rights reserved.                                                       **
 *                                                                            **
**+--------------------------------------------------------------------------+**
 *           ======== C64PLIBPLUS.h ========                                  **
 *           Header file containing C64LIBPLUS function definitions           **
 *                                                                            **
 *                                                                            **
*******************************************************************************/


#ifndef _C64PLIBPLUS_H_
#define _C64PLIBPLUS_H_

void IMG_add_16s
(
short * restrict imgR1, /* Image 1 read pointer */
short * restrict imgR2, /* Image 2 read pointer */
short * restrict imgW, /* Output image pointer */
int count /* Number of samples in image */
);

void IMG_addS_16s
(
    short * restrict imgR, /* Read pointer for the input image */
    short * restrict imgW, /* Write pointer for the output image */
    short constData, /* Constant data */
    int count /* Number of samples in the image */
);

void IMG_addS_8
(
    char * restrict imgR, /* Read pointer for the input image */
    char * restrict imgW, /* Write pointer for the output image */
    char constData, /* Constant data */
    int count /* Number of samples in the image */
);

void IMG_and_16
(
unsigned short * restrict imgR1, /* Image 1 read pointer */
unsigned short * restrict imgR2, /* Image 2 read pointer */
short * restrict imgW, /* Output image pointer */
int count /* Number of samples in image */
);

void IMG_and_8
(
unsigned char * restrict imgR1, /* Image 1 read pointer */
unsigned char * restrict imgR2, /* Image 2 read pointer */
char * restrict imgW, /* Output image pointer */
int count /* Number of samples in image */
);

void IMG_andS_16
(
unsigned short * restrict imgR, /* Read pointer for the input image */
unsigned short * restrict imgW, /* Write pointer for the output image */
short constData, /* Constant data */
int count /* Number of samples in the image */
);

void IMG_andS_8
(
unsigned char * restrict imgR, /* Read pointer for the input image */
unsigned char * restrict imgW, /* Write pointer for the output image */
char constData, /* Constant data */
int count /* Number of samples in the image */
);

void IMG_mul_16s
(
short * restrict imgR1, /* Image 1 read pointer */
short * restrict imgR2, /* Image 2 read pointer */
int * restrict imgW, /* Output image pointer */
int count /* Number of samples in image */
);

void IMG_mul_8
(
char * restrict imgR1, /* Image 1 read pointer */
char * restrict imgR2, /* Image 2 read pointer */
short * restrict imgW, /* Output image pointer */
int count /* Number of samples in image */
);

void IMG_mulS_16s
(
    short * restrict imgR, /* Read pointer for the input image */
    int * restrict imgW, /* Write pointer for the output image */
    short constData, /* Constant data */
    int count /* Number of samples in the image */
);

void IMG_mulS_8
(
 unsigned char * restrict imgR, /* Read pointer for the input image */
 short * restrict imgW, /* Write pointer for the output image */
 char constData, /* Constant data */
 int count /* Number of samples in the image */
);

void IMG_not_16
(
unsigned short * restrict imgR, /* Image read pointer */
unsigned short * restrict imgW, /* Image write pointer */
int count /* Number of samples in image */
);

void IMG_not_8
(
unsigned char * restrict imgR, /* Image read pointer */
unsigned char * restrict imgW, /* Image write pointer */
int count /* Number of samples in image */
);

void IMG_or_16
(
unsigned short * restrict imgR1, /* Image 1 read pointer */
unsigned short * restrict imgR2, /* Image 2 read pointer */
short * restrict imgW, /* Output image pointer */
int count /* Number of samples in image */
);

void IMG_or_8
(
unsigned char * restrict imgR1, /* Image 1 read pointer */
unsigned char * restrict imgR2, /* Image 2 read pointer */
char * restrict imgW, /* Output image pointer */
int count /* Number of samples in image */
);

void IMG_orS_16
(
unsigned short * restrict imgR, /* Read pointer for the input image */
unsigned short * restrict imgW, /* Write pointer for the output image */
short constData, /* Constant data */
int count /* Number of samples in the image */
);

void IMG_orS_8
(
unsigned char * restrict imgR, /* Read pointer for the input image */
unsigned char * restrict imgW, /* Write pointer for the output image */
char constData, /* Constant data */
int count /* Number of samples in the image */
);

void IMG_subS_16s
(
    short * restrict imgR, /* Read pointer for the input image */
    short * restrict imgW, /* Write pointer for the output image */
    short constData, /* Constant data */
    int count /* Number of samples in the image */
);

void IMG_subS_8
(
char * restrict imgR, /* Read pointer for the input image */
char * restrict imgW, /* Write pointer for the output image */
char constData, /* Constant data */
int count /* Number of samples in the image */
);

void IMG_yc_demux_le8_c
(
    int n,                               /* Number of luma pixels    */
    const unsigned char *restrict yc,    /* Interbeaved luma/chroma  */
    unsigned char       *restrict y,     /* Luma plane (8-bit)       */
    unsigned char       *restrict cr,    /* Cr chroma plane (8-bit)  */
    unsigned char       *restrict cb     /* Cb chroma plane (8-bit)  */
);

void IMG_ycbcr422pl16_to_rgb565_c
(
    const short             coeff[5],   /* Matrix coefficients.             */
    const short             *y_data,    /* Luminence data        (Y')       */
    const short             *cb_data,   /* Blue color-difference (B'-Y')    */
    const short             *cr_data,   /* Red color-difference  (R'-Y')    */
    unsigned short *restrict rgb_data,  /* RGB 5:6:5 packed pixel output.   */
    unsigned                num_pixels  /* # of luma pixels to process.     */
);

void IMG_yuv420pl16_to_rgb565(const short *coeff,int Height, Int Width, short *pbuf_y,
                               short *pbuf_cb,short *pbuf_cr,unsigned short *pbuf_rgb);


void IMG_yuv420pl_to_rgb565(const short *coeff,int Height, Int Width, unsigned char *pbuf_y,
                               unsigned char *pbuf_cb,unsigned char *pbuf_cr,unsigned short *pbuf_rgb);

void yc_demux_le16_c
(
    int n,                               /* Number of luma pixels    */
    const unsigned char *restrict yc,    /* Interbeaved luma/chroma  */
    short         *restrict y,           /* Luma plane (8-bit)       */
    short         *restrict cr,          /* Cr chroma plane (8-bit)  */
    short         *restrict cb           /* Cb chroma plane (8-bit)  */
);

void ycbcr422pl16_to_rgb32b_c
(
    const short         coeff[5],   /* Matrix coefficients.             */
    const short         *y_data,    /* Luminence data        (Y')       */
    const short         *cb_data,   /* Blue color-difference (B'-Y')    */
    const short         *cr_data,   /* Red color-difference  (R'-Y')    */
    unsigned *restrict   rgb_data,  /* RGB 8:8:8 packed pixel output.   */
    unsigned            num_pixels, /* # of luma pixels to process.     */
    unsigned char       fill        /* upper byte fill value            */
);

void ycbcr422pl_to_rgb24pl_c
(
    const short             coeff[5],   /* Matrix coefficients.             */
    const unsigned char     *y_data,    /* Luminence data        (Y')       */
    const unsigned char     *cb_data,   /* Blue color-difference (B'-Y')    */
    const unsigned char     *cr_data,   /* Red color-difference  (R'-Y')    */
    unsigned char *restrict red_data,   /* Red pixel data                   */
    unsigned char *restrict grn_data,   /* Green pixel data                 */
    unsigned char *restrict blu_data,   /* Blue pixel data                  */
    unsigned                num_pixels  /* # of luma pixels to process.     */
);

void ycbcr422pl_to_rgb32b_c
(
    const short         coeff[5],   /* Matrix coefficients.             */
    const unsigned char *y_data,    /* Luminence data        (Y')       */
    const unsigned char *cb_data,   /* Blue color-difference (B'-Y')    */
    const unsigned char *cr_data,   /* Red color-difference  (R'-Y')    */
    unsigned            *rgb_data,  /* RGB 8:8:8 packed pixel output.   */
    unsigned            num_pixels, /* # of luma pixels to process.     */
    unsigned char       fill        /* upper byte fill value            */
);

void ycbcr422pl_to_rgb565_c
(
    const short             coeff[5],   /* Matrix coefficients.             */
    const unsigned char     *y_data,    /* Luminence data        (Y')       */
    const unsigned char     *cb_data,   /* Blue color-difference (B'-Y')    */
    const unsigned char     *cr_data,   /* Red color-difference  (R'-Y')    */
    unsigned short *restrict rgb_data,  /* RGB 5:6:5 packed pixel output.   */
    unsigned                num_pixels  /* # of luma pixels to process.     */
);

void ycbcr422sp_to_ycbcr420pl 
(
    const unsigned char * y_src,    
    const unsigned char * cbcr_src, 
    unsigned char * restrict y_dst,
    unsigned char * restrict cb_dst,
    unsigned char * restrict cr_dst,
    unsigned int num_lines, 
    unsigned int width,
    unsigned int src_pitch, 
    unsigned int dst_y_pitch,
    unsigned int dst_cbcr_pitch
);

void yuvtobt835_c
(
    const unsigned char *restrict buf_y,
    const unsigned char *restrict buf_u,
    const unsigned char *restrict buf_v,
    short npix,
    short nlin, 
    unsigned short *restrict buf_o
);

void ycbcr422pl_to_ycbcr422sp 
(
    const unsigned char * y_dst,    
    const unsigned char * cbcr_dst, 
    unsigned char * restrict y_src,
    unsigned char * restrict cb_src,
    unsigned char * restrict cr_src,
    unsigned int num_lines, 
    unsigned int width,
    unsigned int dst_pitch, 
    unsigned int src_y_pitch,
    unsigned int src_cbcr_pitch
);

void ycbcr422sp_to_ycbcr420pl 
(
    const unsigned char * y_src,    
    const unsigned char * cbcr_src, 
    unsigned char * restrict y_dst,
    unsigned char * restrict cb_dst,
    unsigned char * restrict cr_dst,
    unsigned int num_lines, 
    unsigned int width,
    unsigned int src_pitch, 
    unsigned int dst_y_pitch,
    unsigned int dst_cbcr_pitch
);

void ycbcr422sp_to_ycbcr422ile 
(
    const unsigned char * y_src,    
    const unsigned char * cbcr_src, 
    unsigned char * restrict ycbcr_dst,
    unsigned int num_lines, 
    unsigned int width,
    unsigned int src_pitch, 
    unsigned int dst_ycbcr_pitch
);

#endif
/*=============================================================================*/
/*                        End of File C64PLIBPLUS.h                            */
/*                              Version : 0.0.1                                */
/*=============================================================================*/



