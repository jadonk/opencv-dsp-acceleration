
========================================================================================================================================================
								OpenCV DSP Acceleration
                                                                      August 2010
========================================================================================================================================================


OpenCV DSP Acceleration is a Google Summer of Code 2010 Project for Beagleboard.org. It aims to accelerate OpenCV algorithms in OMAP3 platforms using the on-chip C64x+ DSP. It uses OpenCV 2.1 as a reference and is dependent on the same. The first release of this project demonstrates the acceleration of few of the OpenCV algorithms like cvSoble(), cvIntegral(), cvCvtColor() and cvDFT(). This work provides a separate library 'libopencvdsp.so' of these algorithms. Users have the choice to select only ARM based API or DSP based API. All DSP based API are prefixed with 'DSP_'. All the code was tested on Beagleboard Rev C4.

This work was initiated by Pramod Poudel(ktmpole@gmail.com, ppoudel@patriots.uttyler.edu) under the mentorship of Leonardo Estevez(leonardo@ti.com), Katie Roberts-Hoffman(katierh@ti.com) and Luis Gustavo Lira(lira.lg@pucp.edu.pe).

Please note that,

1) The repository of this work is maintained at http://code.google.com/p/opencv-dsp-acceleration/source/browse/#svn/trunk. You can do a 'svn checkout http://opencv-dsp-acceleration.googlecode.com/svn/trunk/ opencv-dsp-acceleration' to obtain a copy of this work or can download the tarball from http://code.google.com/p/opencv-dsp-acceleration/downloads/list.

2) All the documentation of this project is maintained at http://code.google.com/p/opencv-dsp-acceleration/w/list.

3) If you have any comments regarding this work, you can drop an e-mail to the author at ktmpole@gmail.com.

4) If you have any issues you can look into the issue tracker at http://code.google.com/p/opencv-dsp-acceleration/issues/list

5) If you find that you have come across a bug that has not been resolved, please feel free to drop the issue at http://code.google.com/p/opencv-dsp-acceleration/issues/list.


