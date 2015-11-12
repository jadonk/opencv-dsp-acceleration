# Introduction #

This page describes all the APIs introduced in this project.

This work is related to OpenCV 2.1. Thus, it is assumed that users are familair with OpenCV data types, structures for Matrices and Images and OpenCV APIs.
For information on OpenCV data types and structures you can refer to http://opencv.willowgarage.com/documentation/index.html.
Also, [this](http://oreilly.com/catalog/9780596516130) book on OpenCV can be helpful.

All introduced APIs name starts with **`DSP_`** followed by OpenCV API name. This is the general convention used and is expected same for further development work. Input arguments to API are same as that of OpenCV.

Three of the APIs, namely **DSP\_cvStartDSP( void )**, **DSP\_cvEndDSP(void)** and **DSP\_cvSyncDSP( void )** are completely new API. These APIs are not related to original OpenCV and are used to setup DSP(make it ready for operation), to shut down DSP (after all DSP related processing is complete) and to synchronize ARM and DSP processor respectively.

#### Note:- The memroy allocated for this API should be continious in physical address space. A patch file is provided inside patch folder in the base directory. This patch should be applied to OpenCV 2.1 alloc.c file in opencv/modules/core/src and the library libopencv\_core.so should be re-compiled. The procedure to patch and build OpenCV is described in [this](http://code.google.com/p/opencv-dsp-acceleration/wiki/Instruction_For_Building_Examples) link. This newly built library will now allocate memory from contigious region. cvAlloc() can be used to request continious memory block if needed in your application. ####

This work introduces following API's.

# Introduced APIs #

### _`void DSP_cvStartDSP( void )`_ ###

This API should be used before any other API call. It performs all functions necessary to setup DSP before it could be used. This API also creates DSP side engine that is used later by other APIs. This is a mandatory function call and should be done before any algorithm specific API is used.

### _`void DSP_cvEndDSP(void)`_ ###

This API should be the last function call to DSP. This is a mandatory function call that should be performed at the end when DSP function calls will no longer be made. This API close the DSP side engine and shut down the DSP after releasing buffers that were internally used to set up DSP calls.

### _`void DSP_cvSobel( const CvArr* srcarr, CvArr* dstarr, int dx, int dy, int aperture_size )`_ ###

This API call is similar to **cvSobel()** API of OpenCV. Same restriction applies to this API as that of cvSobel(). Other added restrictions are as follows

  1. dx and dy should be equal to 1.(It can currently perform 1st order sobel derivative. Support for other order will be added later.)
  1. aperture\_size should be equal to 3.(It can currently perform 3x3 sobel filter operation. Support for other size of filter will be added later.)

### _`void DSP_cvDFT( const CvArr* srcarr, CvArr* dstarr, int flags, int nonzero_rows )`_ ###

**Note**: To use this API you need to have **cxcore.lib** compiled for c64x+. The library file must be saved to folder **dsp\_opencv/beagle/c6accel\_1\_00\_00\_04\_async/dsp/libs**. While building the project, you should remove the comment for CXCORE in Makefile at **dsp\_opencv/beagle/c6accel\_1\_00\_00\_04\_async/dsp/alg/pjt**

This API call is similar to **cvDFT()** API of OpenCV. Same restriction applies to this API as that of cvDFT(). However, following added restriction applies to it currently.

  1. The size of DFT should not be power of 2.


### _`void DSP_cvIntegral( const CvArr* image, CvArr* sumImage, CvArr* sumSqImage, CvArr* tiltedSumImage )`_ ###

**Note**: To use this API you need to install VLIB. You can request VLIB from http://www.ti.com/vlibrequest. The library file must be saved to folder **dsp\_opencv/beagle/c6accel\_1\_00\_00\_04\_async/dsp/libs**. While building the project, you should remove the comment for VLIB in Makefile at **dsp\_opencv/beagle/c6accel\_1\_00\_00\_04\_async/dsp/alg/pjt**

This API call is similar to **cvIntegral()** API of OpenCV. Same restriction applies to this API as that of cvIntegral(). Other added restrictions are as follows.

  1. The input image is single channel unsigned 8-bit image.
  1. The output image is single channel 32-bit image with same number of columns and rows.
  1. Currently this API does not implement square sum and tilted sum.

### _`void DSP_cvCvtColor( const CvArr* srcarr, CvArr* dstarr, int code )`_ ###

This API call is similar to **cvCvtColor()** API of OpenCV. Same restriction applies to this API as that of cvSobel(). Other added restriction is as follows.

  1. This API only performs CV\_RGB2GRAY. Thus, code should be CV\_RGB2GRAY

### _`void DSP_cvCvtColor_cvSobel( const CvArr* srcarr, CvArr* dstarr, int code, const void* srcarr_s, void* dstarr_s, int dx, int dy, int aperture_size )`_ ###

This API call performs DSP\_cvCvtColor() and DSP\_cvSobel() in a single function call. While this API is beyond OpenCV standard, it is added to demonstrate how the communication overhead between ARM and DSP can be reduced. Same restriction applies to this call as that ot DSP\_cvCvtColor() and DSP\_cvSobel().

### _`void DSP_cvSyncDSP( void )`_ ###

This API call is used to synchronize the operation between ARM and DSP. As all the APIs above are asynchronous call to DSP, this API should be called before the output data from other API call is referenced to maintain data coherency.