/*
 *  ======== app.c ========
 *  Test application for OPENCVTEST algorithm.
 */

#include <stdio.h>
#include <stdlib.h>
#include <xdc/std.h>
#include <ti/xdais/xdas.h>
#include <beagle/algos/opencv/iopencv.h>
#include "../beagle_opencv_API.h"







static XDAS_Int32 frameLen;
static XDAS_Int16 *inBuf0; // input buffer
static XDAS_Int16 *outBuf;
IOPENCV_Params opencvParams;
OPENCV_Operation operation = OPENCV_OPERATION_SOBEL3x3 ;

static void OPENCV_GetInput();
static void display(XDAS_Int16 [], XDAS_Int32 );
static void writeImage();
static void readImage();

/*
 *  ======== main ========
 */
int main(Int argc, String argv[])
{
    

    
    
    switch (operation) {
	   case OPENCV_OPERATION_GEN_TWIDDLE :
	    	OPENCV_GetInput();
		OPENCVTEST_main( &opencvParams, operation ); 
		printf("OPENCV Input: ");
    		display(inBuf0, frameLen/2 );    

    		printf("OPENCV Output: ");
    		display(outBuf, frameLen/2 ); 
                free(inBuf0);
  		free(outBuf);

		break;

	   case OPENCV_OPERATION_DFT :
		OPENCV_GetInput();
     		OPENCVTEST_main( &opencvParams, operation );

   		printf("OPENCV Input: ");
    		display(inBuf0, frameLen/2);    

    		printf("OPENCV Output: ");
		display(outBuf, frameLen/2); 
  
		free(inBuf0);
  		free(outBuf);

		break;  

    	   case OPENCV_OPERATION_SOBEL3x3 :
	   case OPENCV_OPERATION_SOBEL5x5 :
 	   case OPENCV_OPERATION_SOBEL7x7 :
		readImage();
		OPENCVTEST_main( &opencvParams, operation );
		writeImage();
		break;

	   default :
                return(0);
    }
    
    return (0);

}



static void OPENCV_GetInput()
{   XDAS_Int16 *inputbuffer; 
    XDAS_Int32 i;
    
    opencvParams.size = sizeof(opencvParams);
    opencvParams.width = 128;
    opencvParams.height = 0; 

    if (opencvParams.width != 0 && opencvParams.height != 0)
       frameLen = opencvParams.width * opencvParams.height;
    else if (opencvParams.width != 0)
       frameLen = opencvParams.width;
    else if (opencvParams.height !=0)
       frameLen = opencvParams.height;
    else
       printf("Wrong Data Size \n"); 
    
    /* For real and imaginary. (DFT type is XDAS_Int16 where as image is XDAS_Int8) */
    if(operation == OPENCV_OPERATION_DFT) frameLen = (frameLen / sizeof (XDAS_Int8)) * 2 * sizeof (XDAS_Int16);     
      
    opencvParams.frameLen = frameLen;
 
    inBuf0 = (XDAS_Int16 *)malloc( frameLen );
    opencvParams.data = inBuf0;
    inputbuffer = inBuf0;

    outBuf = (XDAS_Int16 *)malloc(frameLen ); 
    opencvParams.output = outBuf;

        
    for (i=0; i<frameLen/4; i++){
	*inputbuffer = (XDAS_Int16)i +1;
	inputbuffer++;
	*inputbuffer = 0;
	inputbuffer++;
	}
    inputbuffer = inBuf0;
          
}

static void display(XDAS_Int16 a[], XDAS_Int32 n)
{
    XDAS_Int16 i;
    for (i=0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
}



/*
=========================================
function    Read file into buffer
=========================================
*/
static void readImage()
{
     XDAS_Int8 *ptr8_buf; 
     FILE *image, *result;
	
     XDAS_Int32 imageLen, i, count;
     XDAS_Int8 c;

     opencvParams.size = sizeof(opencvParams);
     opencvParams.width = 1024;
     opencvParams.height = 695; 

     /* Calculating image buffer size */
     if (opencvParams.width != 0 && opencvParams.height != 0)
        frameLen = opencvParams.width * opencvParams.height;
     else if (opencvParams.width != 0)
        frameLen = opencvParams.width;
     else if (opencvParams.height !=0)
        frameLen = opencvParams.height;
     else
        printf("Wrong Data Size \n"); 
 
     opencvParams.frameLen = frameLen;

     inBuf0 = (XDAS_Int16 *)malloc( frameLen * sizeof(XDAS_Int8)); 
     opencvParams.data = inBuf0; 
     outBuf = (XDAS_Int16 *)malloc( frameLen * sizeof(XDAS_Int8));
     opencvParams.output = outBuf; 

 
     ptr8_buf = (XDAS_Int8 *)inBuf0;

     /* Open Image file to read */
     image = fopen("road_grey.pgm", "rb");
     if (!image) {
	fprintf(stderr, "Unable to open file %s \n", "road_grey.pgm");
	return;
     }
		
     /* Get file length */
     fseek(image, 0, SEEK_END);
     imageLen = (XDAS_Int32 )(ftell(image));
     fseek(image, 0, SEEK_SET);

     /* Open file to store result and copy header */
     result = fopen("result.pgm", "wb");
     if (!result) {
        fprintf(stderr, "Unable to open file %s \n", "result.pgm");
	return;
     }
	
    /* Storing Header to the output file */
     count = 0;
     do {
        count++;
        c = fgetc(image);
        fputc(c, result);
     }while(count != 16);	

     count++;

     for (i=count; i < imageLen; i++) {	
	 c = fgetc(image);
	*ptr8_buf = c;  
	 ptr8_buf++;
	 if(feof(image)) break;
     }
	
	
     fclose(image);
     fclose(result);


return;

}

/*
=========================================
function    Write image into file 
=========================================
*/
void writeImage()
{
     FILE *wr_file;
     XDAS_Int32 j;
     XDAS_Int8 *img_data = (XDAS_Int8 *)outBuf;

     if ((wr_file=fopen("result.pgm","a")) == NULL) {
        printf("\n Write file open Error. \n");
	fclose(wr_file);
     }
     
     for(j = 0; j <  frameLen ; j++ ) {
     putc(img_data[j],wr_file);  
     }
     
     fclose (wr_file);
    
     free(inBuf0);
     free(outBuf);
}
