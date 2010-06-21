
/*
 *  ======== opencv_beagle_vt.c ========
 *   
 */

#include <xdc/std.h>
#include <ti/xdais/dm/iuniversal.h>
#include <ti/xdais/ialg.h>
#include "iopencv.h"
#include "opencv_beagle.h" 

extern Void OPENCV_BEAGLE_activate(IALG_Handle handle);
extern Int OPENCV_BEAGLE_alloc(const IALG_Params *, IALG_Fxns **, IALG_MemRec *);
extern Void OPENCV_BEAGLE_deactivate(IALG_Handle handle);
extern Int  OPENCV_BEAGLE_free(IALG_Handle, IALG_MemRec *);
extern Int OPENCV_BEAGLE_initObj(IALG_Handle, const IALG_MemRec *, IALG_Handle, const IALG_Params *);
extern Void OPENCV_BEAGLE_moved(IALG_Handle, const IALG_MemRec *, IALG_Handle, const IALG_Params *);
extern Int OPENCV_BEAGLE_process(IUNIVERSAL_Handle, XDM1_BufDesc *, XDM1_BufDesc *, XDM1_BufDesc, IUNIVERSAL_InArgs *, IUNIVERSAL_OutArgs *);
extern Int  OPENCV_BEAGLE_numAlloc(Void);

#define IALGFXNS                                                 \
    &OPENCV_BEAGLE_IALG,       /* module ID */                          \
    OPENCV_BEAGLE_activate,    /* activate */                           \
    OPENCV_BEAGLE_alloc,       /* algAlloc */                           \
    NULL,                      /* control (NULL => no control ops) */   \
    OPENCV_BEAGLE_deactivate,  /* deactivate */                         \
    OPENCV_BEAGLE_free,        /* free */                               \
    OPENCV_BEAGLE_initObj,     /* init */                               \
    OPENCV_BEAGLE_moved,       /* moved (NULL => not suported) */       \
    OPENCV_BEAGLE_numAlloc     /* numAlloc (NULL => IALG_DEFMEMRECS) */

/*
 *  ======== OPENCV_BEAGLE_IOPENCV ========
 *  This structure defines implementation of the DFT
 *  for the OPENCV_BEAGLE module.
 */
/* Note that the typecasting of the process and control functions is needed as the IUNIVERSAL_Fxns struct expects IUNIVERSAL_process() to return XDAS_Int32 whereas our OPENCV_BEAGLE_process() call returns Int for legacy purposes */
IUNIVERSAL_Fxns OPENCV_BEAGLE_IOPENCV = { /* module_vendor_interface */
    {IALGFXNS},                 /* IALG functions */
    (XDAS_Int32(*)(IUNIVERSAL_Handle, XDM1_BufDesc *, XDM1_BufDesc *, XDM1_BufDesc *, IUNIVERSAL_InArgs *, IUNIVERSAL_OutArgs *))OPENCV_BEAGLE_process,             /* Process function */

};

/*
 *  ======== OPENCV_BEAGLE_IALG ========
 *  This structure defines TI's implementation of the IALG interface
 *  for the OPENCV_BEAGLE module.
 */
#ifdef __TI_COMPILER_VERSION__
asm("_OPENCV_BEAGLE_IALG .set _OPENCV_BEAGLE_IOPENCV");
#else
IALG_Fxns OPENCV_BEAGLE_IALG = { IALGFXNS };
#endif
/*
 *  We could have also duplicate the structure here to allow this code to be 
 *  compiled and run non-DSP platforms at the expense of unnecessary data space
 *  consumed by the definition below.
 *
 *  IALG_Fxns OPENCV_BEAGLE_IALG = {
 *      IALGFXNS
 *  };
 */


