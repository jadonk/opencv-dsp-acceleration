/*!
 *  ======== OPENCV========
 *  OPENCV codec specification
 *
 *  This file specifies information necessary to integrate with the Codec
 *  Engine.
 *
 *  By inheriting ti.sdo.ce.universal.IUNIVERSAL, OPENCV declares that it "is
 *  a" universal algorithm.  This allows the codec engine to automatically
 *  supply simple stubs and skeletons for transparent execution of DSP
 *  codecs by the GPP.
 *
 *  In addition to declaring the type of the OPENCV algorithm, we
 *  declare the external symbol required by xDAIS that identifies the
 *  algorithms implementation functions.
 */
metaonly module OPENCV inherits ti.sdo.ce.universal.IUNIVERSAL
{
    readonly config beagle.algos.opencv.OPENCV.Module alg =
        beagle.algos.opencv.OPENCV;
    
    override readonly config String ialgFxns = "OPENCV_BEAGLE_IALG";

}
