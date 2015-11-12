# Introduction #

This page describes the complete procedure to add new algorithms to this library. This library will be called by name **OpenCV\_DSP**.
Also, a brief description on internal functions and necessary structures is provided.
It is important to note that OpenCV\_DSP is implemented on **C6accel** library. **C6accel** library was extended to provide support for **OpenCV\_DSP** and new algorithm support was added to it. On documentation about **C6accel** you can see `<PROJECT_BASE>`/dsp\_opencv/beagle/c6accel\_1\_00\_00\_04\_async. c6accel is not yet released by TI. However, you can visit [http://software-dl.ti.com/dsps/dsps\_registered\_sw/sdo\_sb/targetcontent/index.html ](.md) to track its release sometime later in 2010 september.

# Details #

Name of any OpenCV API, that is to be ported to DSP will begin with **`DSP_`**.
All files, whether it is header file or source file, are stored in folder **beagle/library/opencv**.

---

### Data Types ###
OpenCV\_DSP uses 2 new data types. It is tried best to make these data types consistent with OpenCV. This new types are **DSP\_Mat** and **DSP\_Flags**.

#### _`DSP_Mat`_ ####

The **DSP\_Mat** is defined as follows,
```
  typedef struct DSP_Mat {
    int flags; // includes several bit-fields:Depth and Number of Channels
    int rows, cols;  // the number of rows and columns
    size_t step;  // a distance between successive rows in bytes; includes the gap if any
    unsigned char * data; // pointer to the data
    unsigned char * datastart; // helper fields used in locateROI and adjustROI
    unsigned char * dataend;
  } DSP_Mat;
```
Least significant 3 bits of the **flags** stores information about depth of image/matrix while 6-bits after that stores information about number of channels.

#### _`DSP_Flags`_ ####

The **DSP\_Flags** is defined as follows,
```
  typedef struct DSP_Flags {
    int flag1;
    int flag2;
    int flag3;
    int flag4;
    int flag5;
  } DSP_Flags;
```
This structure stores all the flags passed to OpenCV\_DSP APIs. For example, in the case of `DSP_cvSobel( const CvArr* srcarr, CvArr* dstarr, int dx, int dy, int aperture_size )`,
flag1=dx, flag2=dy flag3=aperture\_size, flag4=-1 and flag5=-1. The conversion of these flags to DSP\_Flags is done using helper macro **FLAGS3\_OpenCV2DSP(dx,dy,aperture\_size)**

---

### API Prototype Defination ###

API prototypes are defined in file **beagle\_opencv\_API.h**  The input arguments to the API are expected to be same as that of the Original OpenCV 2.1. This assures consistency with OpenCV and easy migration for users to DSP.

---

### API Defination and Implementation ###

The function defination is implemented in file **beagle\_opencv\_API.c**.
After the function defination, body of the function starts with declaring necessary internal variables. This is followed by conversion of **`CvArr`** to **`cv::Mat`**. This structure and class are native to OpenCV 2.1 and is implemented in similar way in OpenCV\_DSP. Thus, its description is avoided here.

#### _Error Checking_ ####

Error checking is done to ensure input and ouput matrices are as desired and flags are valid.

#### _Converting OpenCV Data Type to OpenCV\_DSP Data Type_ ####

Internal function **DSP\_Mat mat\_OpenCV2DSP(cv::Mat src)** converts OpenCV **Mat** type to **DSP\_Mat**. Input argument to this function is cv::Mat type. The function returns DSP\_Mat type.

#### _Converting input arguments(flags) other than images or matrices to DSP\_Flags_ ####

This conversion is done using **DSP\_Flags flags\_OpenCV2DSP(int a, int b, int c, int d, int e)**. Macro `FLAGS`**`NUM`**`_OpenCV2DSP()` is provided for this. In this macro call **NUM** should be replaced with a number not greater than 5 depending on the number of flags to be passed.

#### _Calling Rapper function that uses C6accel library To start DSP Process_ ####

Name of all rapper function start with OpenCV\_DSP API name appened with **Process**. For example, rapper function for DSP\_cvSobel() is named DSP\_cvSobelProcess(). All rapper function are declared **int** type which returns error codes, either **OPENCVDSP\_OK** or **OPENCVDSP\_ERR**. These error codes are defined in file opencvDsp.h and can be extended as per user need. It is always wise to perform error checking when the function call returns.
Input arguments to these functions are pointers to DSP\_Mat types for source and destination matrices  and pointer to DSP\_Flags for necessary flags to be passed.

---

### Implementing Rapper Function That Calls C6accel library ###

Prototypes to all the rapper functions are defined in file **opencvDsp.h**. The name of the rapper function starts with OpenCV\_DSP API name followed by **Process**. The function defination is implemented in **opencvDsp.c**.

As C6accel does not provide any buffer for scratch work on the DSP side, all the required buffer for scratch work or temporary storage should be allocated in the wrapper function defination. Their size should be saved for releasing the buffer when the processing is over. As DSP works on continious memroy address space, buffer allocation and deallocation should be done using **Memory\_alloc()** and **Memory\_free()**. However, their is no necessary to allocate buffer for source and destination matrices or images. It is expected that OpenCV allocates buffer in continious memory space. If not, you can patch your OpenCV using the patch provided in **patch/** folder. This allows OpenCV to allocate memroy in continious region. If you don't want to change your OpenCV source, you then have to allocate continious buffer for source and destination data and then copy your source data to this buffer.

It is assumed that your implementation for new Algorithm will provide support for both **Synchronou** and **Asynchronous** call to DSP. Care should be take when to free the buffers in the case of asynchronous call. Buffer should be freed only after DSP is synchronized in case of **Asynchronous** call. You can refer to DSP\_cvSyncDSPProcess() for this. To implement support for Aynchronous and Asynchronous call **SUPPORT\_ASYNC** flag is used and last argument to C6accel library is provided as either **SYNC** or **ASYNC** for synchronous and asynchronous call respectively.

---

### Adding Support For Your Algorithm in C6accel Library ###

You can also refer to C6accel documentation(when it will be released [here](http://software-dl.ti.com/dsps/dsps_registered_sw/sdo_sb/targetcontent/index.html)) for adding your algorithm to C6accel library. However I have tried to simplify the procedure and described it below step-wise. The procedure is described using example of RGB\_To\_Y() used by DSP\_cvCvtColor().

#### _Step 1_ (Adding Prototype) ####

All function prototypes to C6accel library is defined in file `{C6accel_Base_Dir`}/soc/c6accelw/c6accelw.h
For example, to declare prototype for RGB\_To\_Y(), following was done.
```
int C6accel_RGB_To_Y(C6accel_Handle hC6accel,

                     unsigned char * restrict ptr_src,       /* Pointer to input image in RGB format */
                     unsigned char * restrict ptr_dst,       /* Pointer to Greyscale output image    */
	             unsigned int count			     /* Total number of pixels               */

#ifdef SUPPORT_ASYNC

                     ,E_CALL_TYPE callType);

#else 

);

#endif
```

#### _Step 2_ (Adding Structure to Store parameters to Be Passed to DSP) ####

All structures to hold parameters are defined in file **iC6Accel\_ti.h** in folder `{C6accel_Base_Dir`}/soc/packages/ti/c6accel and folder `{C6accel_Base_Dir`}/dsp/alg/include. To maintain consistency, you can add structure in file inside any of these folder and copy the same to other.

For our example, the structure was defined as follows
```
typedef struct IMG_RGB_To_Y_Params{
				  unsigned int src_InArrID1;              /* Input RGB image         */
				  unsigned int dst_OutArrID1;             /* Output greyscale image  */
				  unsigned int count;			  /* Total Pixels	     */
				  }IMG_RGB_To_Y_Params;
```

Due to address translation issue between virtual address space in ARM and Physical address space in DSP, pointers are not directly passed to DSP. Pointers should either by address translated or stored in structure provided by Codec-Engine which are internally address translated. Due to this restriction buffer ID is created and passed to DSP while pointer to this buffer is stored in structure that is address translated by Codec-Engine framework.

#### _Step 3_ (Adding ID for New Algorithm) ####

Algorithm ID is defined in file **iC6Accel\_ti.h**. As described in step 2, consistency should be made in files in 2 different folders.
As all functions are implemented in DSP, it is necessary to tell DSP, which function call is to be made for your algorithm. To do this, all algorithms are given ID based on C6accel convention. The convention used is
```
  Leftmost 2 digits for vendor ID. For example, 01 is for TI and 02 is for other.

  2 digits after it for selecting function category like DSPLIB or IMGLIB.

  Remaining 4 digits for function type.
```

For our example, the algorithm ID is as below.
```
#define IMG_RGB_TO_Y_FXN_ID  		  0x02020001

In this ID, most significant two digits ,ie 0x02, represent vendor ID as OTHER. Two digits after that, ie, 02, again, represent IMGLIB and finally 0001 represents unique function ID in group OTHER and category IMGLIB.
```

#### _Step 4_ ( Adding Function ID) ####

Function ID should be added in file `C6Accel.h` defined in folder `{C6accel_Base_Dir`}/dsp/alg/include. The function ID is the last 4 digits for algorithm ID. In our example, function ID is defined as
```
#define RGB_TO_Y_FXN_ID			    0x00000001 
```

#### _Step 5_ (Adding function Defination in ARM side Library) ####

Calls to the DSP algorithms are done using wrapper function that uses UNIVERSAL\_Process(), a Codec-Engine frame work implementation for non-VISA type codec. This function defination is done in file **c6accelw.c** provided inside folder `{C6accel_Base_Dir`}/soc/c6accelw .

Input and output buffers are passed using **`XDM1_BufDesc`** structure. All parameters and function ID are passed using **`IC6Accel_InArgs`**. Continious buffer should be allocated for `IC6Accel_InArgs`. The size of this buffer is calculated, in our example, as follows
```
InArg_Buf_size=  sizeof(Fxn_struct)+

                     sizeof(IMG_RGB_To_Y_Params)+

                     sizeof(CInArgs->size)+

                     sizeof(CInArgs->Num_fxns);
```

Number of input and output buffer to be passed should be defined. For our example, it is done as follows
```
inBufDesc.numBufs = 1;

outBufDesc.numBufs = 1;
```

Proper cache maintainence should be done for input and output buffer. For example,
```
CACHE_WB_INV_INPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_src,0,3*count*sizeof(char));

CACHE_INV_OUTPUT_BUFFERS_AND_SETUP_FOR_C6ACCEL(ptr_dst,0,count*sizeof(short));

Note: Here ,0, in first and second statement refers to INBUF0 and OUTBUF0 respectively.These are input and output buffer IDs. 
```

Number of algorithm functions to call should be defined. This is important for chaining function calls that avoid communication overhead between different OpenCV\_DSP function calls.
```
CInArgs->Num_fxns=1;
```

Specify algorithm ID that uniquely defines algorithm you want to call and the offset of its parameters from starting address of input arguments.
```
CInArgs->fxn[0].FxnID= IMG_RGB_TO_Y_FXN_ID;
    CInArgs->fxn[0].Param_ptr_offset=sizeof(CInArgs->size)+sizeof(CInArgs->Num_fxns)+sizeof(Fxn_struct);
```

Parameter to be passed to DSP should be saved in the contigious buffer which will be passed as input argument **CInArgs**. For, example,
```
fp0 = (IMG_RGB_To_Y_Params *)((XDAS_Int8*)CInArgs + CInArgs->fxn[0].Param_ptr_offset);



/* Fill in the fields in the parameter structure */

fp0->src_InArrID1= INBUF0;

fp0->dst_OutArrID1= OUTBUF0;

fp0->count= count;

```

If ASYNC call to DSP is to be made, all of these context should be saved before calling UNIVERSAL\_processAsync(). For example,
```

memcpy(&(c6accelAsyncParams.inBufs),&inBufDesc, sizeof (XDM1_BufDesc));
//memcpy(&(c6accelAsyncParams.inOutBufs),&inOutBufs, sizeof (XDM1_BufDesc));
memcpy(&(c6accelAsyncParams.outBufs), &outBufDesc,sizeof(XDM1_BufDesc));
memcpy(&(c6accelAsyncParams.inArgs), CInArgs,sizeof(UNIVERSAL_InArgs));
memcpy(&(c6accelAsyncParams.outArgs),&uniOutArgs,sizeof(UNIVERSAL_OutArgs));
c6accelAsyncParams.pBuf = pAlloc;
c6accelAsyncParams.pBufSize = InArg_Buf_size;

status = UNIVERSAL_processAsync(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
```

If SYNC call to DSP is to be made, you can directly call UNIVERSAL\_process(). For example,
```
status = UNIVERSAL_process(hC6accel->hUni, &inBufDesc, &outBufDesc, NULL,(UNIVERSAL_InArgs *)CInArgs, &uniOutArgs);
```

#### _Step 6_ (Adding DSP side algorithm) ####

Before implementing your algorithm, you should be aware that C6accel does not provide any scratch memory on DSP side, thus it is your responsibility to allocate continious scratch memory in ARM side and pass that to the DSP for use. To see how it is done, please refer to DSP\_cvIntegralProcess() implementation.

There are two ways to implement your algorithm. Either you can create a seperate file, **filename.c**, in folder `{C6accel_Base_Dir`}/dsp/alg/src  and then add function call to your algorithm in appropritate case statement in file **`C6Accel.c`** which is provided the same folder. For our example, a file **`RGB_To_Y.c`** was saved in the folder mentioned above. The case statement was then added in file **`C6Accel.c`** as shown below,
```
case (RGB_TO_Y_FXN_ID): {
			 /* Unmarshal Parameters */   

                         IMG_RGB_To_Y_Params *C6ACCEL_TI_IMG_RGB_To_Y_paramPtr;

                         C6ACCEL_TI_IMG_RGB_To_Y_paramPtr = pFnArray;

                         /* Parameter check */

                         if(((C6ACCEL_TI_IMG_RGB_To_Y_paramPtr->src_InArrID1)>INBUF15)|

                            ((C6ACCEL_TI_IMG_RGB_To_Y_paramPtr->dst_OutArrID1)>INBUF15)){

                            return(IUNIVERSAL_EPARAMFAIL);

                          }

                         else

                         /* Call underlying kernel */

                            IMG_RGB_To_Y((unsigned char *)inBufs->descs[C6ACCEL_TI_IMG_RGB_To_Y_paramPtr->src_InArrID1].buf,

                                         (unsigned char *)outBufs->descs[C6ACCEL_TI_IMG_RGB_To_Y_paramPtr->dst_OutArrID1].buf,

                                         C6ACCEL_TI_IMG_RGB_To_Y_paramPtr->count);

                          
			}
```

This is the method I prefer. The other method is to implement your algorithm inside the case statement, ie, after else in the case statement above.

---


### Note:- This completes the procedure to add new algorithms. After this is done, compile and test your algorithm. If you have any queries or suggestions, please feel free to leave a comment ###