/*!
 *  ======== CVDFT========
 *  CVDFT codec specification
 *
 *  This file specifies information necessary to integrate with the Codec
 *  Engine.
 *
 *  By inheriting ti.sdo.ce.universal.IUNIVERSAL, CVDFT declares that it "is
 *  a" universal algorithm.  This allows the codec engine to automatically
 *  supply simple stubs and skeletons for transparent execution of DSP
 *  codecs by the GPP.
 *
 *  In addition to declaring the type of the CVDFT algorithm, we
 *  declare the external symbol required by xDAIS that identifies the
 *  algorithms implementation functions.
 */
metaonly module CVDFT inherits ti.sdo.ce.universal.IUNIVERSAL
{
    readonly config gsoc.algos.cvDFT.CVDFT.Module alg =
        gsoc.algos.cvDFT.CVDFT;
    
    override readonly config String ialgFxns = "CVDFT_GSOC_IALG";

}
