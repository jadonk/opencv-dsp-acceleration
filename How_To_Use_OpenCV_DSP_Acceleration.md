# Introduction #

Following is the procedure to use this work in your application


# Details #

## Step 1 ##

In your application include the header file "beagle\_opencv\_API.h". The complete path to the header file may be necessary if it is not present in your default directory.

```
#include<beagle_opencv_API.h>
```

## Step 2 ##
Call **DSP\_cvStartDSP()** at the beginning of your application. This ensures that DSP engine is open before any call to DSP is made using OpenCV\_DSP APIs.

## Step 3 ##
Make call to **OpenCV\_DSP** API as needed in the application. Ensure that all input and out buffers are allocated in contigious memory. If you have not used the patch for OpenCV which makes OpenCV allocate memory in continious region, you can use Memory\_alloc() and Memory\_free() to allocate and deallocate continious memroy. For more reference to it, refer http://software-dl.ti.com/dsps/dsps_public_sw/sdo_sb/targetcontent/ce/2_25_01_06/exports/codec_engine_2_25_01_06/docs/html/struct_memory___alloc_params.html#ab8113726ac4d721815cd6041ac236e4d

## Step 4 ##
At the end of your application call API **DSP\_cvEndDSP()** to close DSP engine.

## Step 5 ##
### Compiling your work and linking to OpenCV\_DSP\_Acceleration library ###
OpenCV DSP Acceleration library is provided as shared library. While compiling and linking your application, make sure that you are linking correctly to **libopencvdsp.so**. Specially, the path to it may need to be specified correctly.