/*!
 *  ======== C6ACCEL========
 *  C6ACCEL codec specification
 *
 *  This file specifies information necessary to integrate with the Codec
 *  Engine.
 *
 *  By inheriting ti.sdo.ce.universal.IUNIVERSAL, C6ACCEL declares that it "is
 *  a" universal algorithm.  This allows the codec engine to automatically
 *  supply simple stubs and skeletons for transparent execution of DSP
 *  codecs by the GPP.
 *
 *  In addition to declaring the type of the C6ACCEL algorithm, we
 *  declare the external symbol required by xDAIS that identifies the
 *  algorithms implementation functions.
 */
metaonly module C6ACCEL inherits ti.sdo.ce.universal.IUNIVERSAL
{
    readonly config ti.c6accel.C6ACCEL.Module alg =
        ti.c6accel.C6ACCEL;
    
    override readonly config String ialgFxns = "C6ACCEL_TI_IC6ACCEL";

}
