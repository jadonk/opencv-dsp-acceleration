/*!
 *  ======== C6ACCEL ========
 */
 
metaonly module C6ACCEL
{
    /*!
     *  ======== FLOAT ========
     *  This config param allows the user to indicate whether to include
     *  a FLOAT or not.    
     */
    config Bool FLOAT = false;

    /*!
     *  ======== watermark ========
     *  This config param allows the user to indicate whether to include
     *  a watermark or not.    
     */
    config Bool watermark = false;

    /*!
     *  ======== Code Section ========
     */
    config String codeSection;

    /*!
     *  ======== Uninitialized Data Section ========
     */
    config String udataSection;

    /*!
     *  ======== Initialized Data Section ========
     */
    config String dataSection;   

}
