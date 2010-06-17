/*
 *  Do not modify this file; it is automatically 
 *  generated and any modifications will be overwritten.
 *
 * @(#) xdc-u14
 */

#define __nested__
#define __config__

#include <xdc/std.h>

/*
 * ======== GENERATED SECTIONS ========
 *     
 *     MODULE INCLUDES
 *     
 *     <module-name> INTERNALS
 *     <module-name> INHERITS
 *     <module-name> VTABLE
 *     <module-name> PATCH TABLE
 *     <module-name> DECLARATIONS
 *     <module-name> OBJECT OFFSETS
 *     <module-name> TEMPLATES
 *     <module-name> INITIALIZERS
 *     <module-name> FUNCTION STUBS
 *     <module-name> PROXY BODY
 *     <module-name> OBJECT DESCRIPTOR
 *     <module-name> SYSTEM FUNCTIONS
 *     <module-name> PRAGMAS
 *     
 *     INITIALIZATION ENTRY POINT
 *     PROGRAM GLOBALS
 *     CLINK DIRECTIVES
 */


/*
 * ======== MODULE INCLUDES ========
 */



/*
 * ======== xdc.cfg.Program TEMPLATE ========
 */

/*
 *  ======== __ASM__ ========
 *  Define absolute path to program's assembly directory
 */
xdc__META(__ASM__, "@(#)__ASM__ = /media/disk/OE/work/ti/dsp_opencv/beagle/server/opencv_unitserver_evm3530/package/cfg/opencv_unitserver_evm3530_x64P");

/*
 *  ======== __ISA__ ========
 *  Define the ISA of this executable.  This symbol is used by platform
 *  specific "exec" commands that support more than one ISA; e.g., gdb
 */
xdc__META(__ISA__, "@(#)__ISA__ = 64P");

/*
 *  ======== __PLAT__ ========
 *  Define the name of the platform that can run this executable.  This
 *  symbol is used by platform independent "exec" commands
 */
xdc__META(__PLAT__, "@(#)__PLAT__ = ti.platforms.evm3530");

/*
 *  ======== __TARG__ ========
 *  Define the name of the target used to build this executable.
 */
xdc__META(__TARG__, "@(#)__TARG__ = ti.targets.C64P");

/*
 *  ======== __TRDR__ ========
 *  Define the name of the class that can read/parse this executable.
 */
xdc__META(__TRDR__, "@(#)__TRDR__ = ti.targets.omf.cof.Coff");


/*
 * ======== ti.sdo.ce.ipc.Settings TEMPLATE ========
 */


/*
 * ======== ti.sdo.ce.osal.Global TEMPLATE ========
 */



/*
 *  ======== Global Configuration ========
 *  Do not modify the following; it is automatically generated from the template
 *  Server.xdt in the ti.sdo.ce package and will be overwritten next time the
 *  executable is configured
 */

Bool Memory_skipVirtualAddressTranslation = FALSE;


/* List of all packages and their versions and paths, for diagnostics */
String Global_buildInfo[] = {
    "    package ti.targets.rts6000 (/media/disk/OE/work/ti/ti-xdctools-tree/packages/ti/targets/rts6000/) [1, 0, 0, 0]",
    "    package beagle.algos.opencv (/media/disk/OE/work/ti/dsp_opencv/beagle/algos/opencv/) [1, 0, 0]",
    "    package ti.sdo.fc.global (/media/disk/OE/work/ti/ti-framework-components-tree/packages/ti/sdo/fc/global/) [1, 0, 0]",
    "    package ti.sdo.utils.trace (/media/disk/OE/work/ti/ti-framework-components-tree/packages/ti/sdo/utils/trace/) [1, 0, 0]",
    "    package ti.xdais (/media/disk/OE/work/ti/ti-xdais-tree/packages/ti/xdais/) [1, 2.0, 1]",
    "    package ti.rtdx (/media/disk/OE/work/ti/ti-dspbios-tree/packages/ti/rtdx/) [2, 0, 0, 5]",
    "    package ti.psl (/media/disk/OE/work/ti/ti-dspbios-tree/packages/ti/psl/) [5, 0, 0, 0]",
    "    package ti.pmi (/media/disk/OE/work/ti/ti-dspbios-tree/packages/ti/pmi/) [1, 0, 0, 0]",
    "    package ti.pscl (/media/disk/OE/work/ti/ti-dspbios-tree/packages/ti/pscl/) [1, 0, 0, 0]",
    "    package ti.bios (/media/disk/OE/work/ti/ti-dspbios-tree/packages/ti/bios/) [5, 2, 5, 25]",
    "    package ti.sdo.fc.dskt2 (/media/disk/OE/work/ti/ti-framework-components-tree/packages/ti/sdo/fc/dskt2/) [1, 0, 4]",
    "    package ti.sdo.fc.dman3 (/media/disk/OE/work/ti/ti-framework-components-tree/packages/ti/sdo/fc/dman3/) [1, 0, 4]",
    "    package ti.sdo.ce.global (/media/disk/OE/work/ti/ti-codec-engine-tree/packages/ti/sdo/ce/global/) [1, 0, 0]",
    "    package ti.xdais.dm (/media/disk/OE/work/ti/ti-xdais-tree/packages/ti/xdais/dm/) [1, 0, 5]",
    "    package ti.sdo.ce.node (/media/disk/OE/work/ti/ti-codec-engine-tree/packages/ti/sdo/ce/node/) [1, 0, 0]",
    "    package ti.sdo.ce.ipc.dsplink.dsp (/media/disk/OE/work/ti/ti-codec-engine-tree/packages/ti/sdo/ce/ipc/dsplink/dsp/) [2, 0, 1]",
    "    package ti.sdo.ce.utils.xdm (/media/disk/OE/work/ti/ti-codec-engine-tree/packages/ti/sdo/ce/utils/xdm/) [1, 0, 2]",
    "    package ti.sdo.fc.memutils (/media/disk/OE/work/ti/ti-framework-components-tree/packages/ti/sdo/fc/memutils/) [1, 0, 0]",
    "    package ti.sdo.fc.acpy3 (/media/disk/OE/work/ti/ti-framework-components-tree/packages/ti/sdo/fc/acpy3/) [1, 0, 4]",
    "    package dsplink.dsp (/media/disk/OE/work/ti/ti-dsplink-tree/packages/dsplink/dsp/) [1, 4, 0]",
    "    package ti.bios.utils (/media/disk/OE/work/ti/ti-biosutils-tree/packages/ti/bios/utils/) [2, 0, 2, 02]",
    "    package ti.catalog.c6000 (/media/disk/OE/work/ti/ti-xdctools-tree/packages/ti/catalog/c6000/) [1, 0, 0, 0]",
    "    package ti.catalog (/media/disk/OE/work/ti/ti-xdctools-tree/packages/ti/catalog/) [1, 0, 0]",
    "    package ti.catalog.arm (/media/disk/OE/work/ti/ti-xdctools-tree/packages/ti/catalog/arm/) [1, 0, 1, 0]",
    "    package ti.platforms.evm3530 (/media/disk/OE/work/ti/ti-xdctools-tree/packages/ti/platforms/evm3530/) [1, 0, 0]",
    "    package ti.sdo.ce.osal (/media/disk/OE/work/ti/ti-codec-engine-tree/packages/ti/sdo/ce/osal/) [2, 0, 2]",
    "    package ti.sdo.ce.osal.bios (/media/disk/OE/work/ti/ti-codec-engine-tree/packages/ti/sdo/ce/osal/bios/) [2, 0, 1]",
    "    package ti.sdo.ce.ipc (/media/disk/OE/work/ti/ti-codec-engine-tree/packages/ti/sdo/ce/ipc/) [2, 0, 1]",
    "    package ti.sdo.ce.ipc.bios (/media/disk/OE/work/ti/ti-codec-engine-tree/packages/ti/sdo/ce/ipc/bios/) [2, 0, 1]",
    "    package ti.sdo.ce.alg (/media/disk/OE/work/ti/ti-codec-engine-tree/packages/ti/sdo/ce/alg/) [1, 0, 1]",
    "    package ti.sdo.ce (/media/disk/OE/work/ti/ti-codec-engine-tree/packages/ti/sdo/ce/) [1, 0, 6]",
    "    package ti.sdo.ce.bioslog (/media/disk/OE/work/ti/ti-codec-engine-tree/packages/ti/sdo/ce/bioslog/) [1, 0, 1]",
    "    package ti.sdo.ce.universal (/media/disk/OE/work/ti/ti-codec-engine-tree/packages/ti/sdo/ce/universal/) [1, 0, 0]",
    "    package beagle.algos.opencv.ce (/media/disk/OE/work/ti/dsp_opencv/beagle/algos/opencv/ce/) [1, 0, 0]",
    "    package beagle.server.opencv_unitserver_evm3530 (/media/disk/OE/work/ti/dsp_opencv/beagle/server/opencv_unitserver_evm3530/) [1, 0, 0]",
    NULL
};

/*
 * ======== ti.sdo.ce.bioslog.LogServer TEMPLATE ========
 */


    
#include <ti/sdo/ce/bioslog/LogServer.h>
#include <ti/sdo/ce/osal/Global.h>

Int LogServer_stackSize = 2048;

/*
 *  ======== LogServer_initGenerated() ========
 */
Void LogServer_initGenerated()
{
    LogServer_init();
    
    /* this causes activation of TRC bits */
    Global_setSpecialTrace("ti.bios");
}


/*
 * ======== ti.sdo.ce.Engine TEMPLATE ========
 */

/*
 *  ======== Engine Configuration ========
 *  Do not modify following; it is automatically generated from the template
 *  Engine.xdt in the ti.sdo.ce package and will be overwritten next time the
 *  executable is configured
 */
#include <ti/sdo/ce/Engine.h>
#include <ti/xdais/ialg.h>

#include <ti/xdais/idma3.h>

#include <ti/xdais/ires.h>

/* type tables for algorithms */
extern IALG_Fxns OPENCV_BEAGLE_IALG;
static String typeTab_OPENCV_BEAGLE_IALG[] = {
    "ti.sdo.ce.universal.IUNIVERSAL",
    NULL
};


/* tables of engine algorithms */
extern IALG_Fxns OPENCV_BEAGLE_IALG;

/* algorithm-specific stub/skeleton config data objects */

static Engine_AlgDesc engineAlgs0[] = {
    {
        "opencv",       /* name */
        {83001710U},   /* uuid */
        &OPENCV_BEAGLE_IALG,             /* fxns */
        NULL,        /* idma3Fxns */
        typeTab_OPENCV_BEAGLE_IALG,              /* typeTab */
        TRUE,          /* isLocal */
        0,        /* groupId */
        1,      /* protocol */
        NULL,         /* iresFxns */
        NULL                /* stub/skel config params  */
    },
    {NULL},
};

/* table of all engines available in this application */
static Engine_Desc engineTab[] = {
    {"local",      /* engine name */
      engineAlgs0,    /* alg table */
      NULL,         /* (optional) server name */
      NULL,        /* (optional) Link config ID */
      1 /* number of algs in alg table */
    },
    {NULL, NULL, NULL, 0}       /* NULL-terminate the engine table */
};

Engine_Config Engine_config = {
    engineTab,              /* table of all engines */
    "local"   /* local RMS engine name */
};

Bool ti_sdo_ce_Engine_noCommName = FALSE;

/*
 * ======== ti.sdo.ce.Server TEMPLATE ========
 */

/*
 *  ======== Server Configuration ========
 *  Do not modify following; it is automatically generated from the template
 *  Server.xdt in the ti.sdo.ce package and will be overwritten next time the
 *  executable is configured
 */
#include <ti/sdo/ce/node/node.h>
#include <ti/sdo/ce/rms.h>

static NODE_Attrs nodeAttrs0 = {
    3, 8346, 0
};
extern SKEL_Fxns UNIVERSAL_SKEL;


/*  table of all "codec server" nodes available in this server */
NODE_Desc RMS_nodeTab[] = {
    /*  name            uuid        skel fxns       params      protocol */
    {"opencv",      {83001710U},   &UNIVERSAL_SKEL, &nodeAttrs0, 1},
    {NULL}  /* NULL terminate RMS_nodeTab[] */
};


RMS_Config RMS_config = {
    1,          /* RMS server's priority */
    16384,         /* RMS server's stack size */
    0,        /* RMS server's stack memory seg */
};

/*
 * ======== ti.sdo.ce.CERuntime TEMPLATE ========
 */


#include <ti/sdo/ce/osal/Global.h>
#include <ti/sdo/ce/ipc/Comm.h>
#include <ti/sdo/ce/osal/Memory.h>
#include <ti/sdo/ce/osal/Queue.h>
#include <ti/sdo/ce/osal/Sem.h>
#include <ti/sdo/ce/osal/SemMP.h>
#include <ti/sdo/ce/osal/Thread.h>
#include <ti/sdo/ce/ipc/Processor.h>
#include <ti/sdo/ce/alg/Algorithm.h>
#include <ti/sdo/ce/osal/Lock.h>
#include <ti/sdo/ce/osal/LockMP.h>
#include <ti/sdo/ce/Server.h>
#include <ti/sdo/ce/Engine.h>
#include <ti/sdo/ce/rms.h>
#include <ti/sdo/ce/utils/xdm/XdmUtils.h>

#include <ti/sdo/utils/trace/gt.h>

/* This symbol is referenced by Server_init() */
Int Global_useLinkArbiter = FALSE;

/*
 *  ======== CERuntime_init ========
 */
Void CERuntime_init(Void)
{
    extern Void IPC_generatedInit();

    GT_init();



    /* allow user to over-ride via CE_TRACE. */
    GT_set(Global_getenv("CE_TRACE"));
//% print("CERuntime.xdt: Engine.hasServer() = " + Engine.hasServer() +
//%       " Server.$used = " + Server.$used);
    IPC_generatedInit();
    Global_init();

    Sem_init();
    SemMP_init();


    Memory_init();
    Queue_init();
    Comm_init();
    Thread_init();
    Processor_init();
    LockMP_init();  /* Must be called before DMAN3_init() */
    Algorithm_init();
    XdmUtils_init();
    Lock_init();

    RMS_init();
    Global_atexit((Fxn)RMS_exit);
    Engine_init();
    Server_init();

    /* init code contribution from ti.sdo.ce.bioslog */
    {
        extern Void LogServer_initGenerated();
        LogServer_initGenerated();
    }


}

/*
 *  ======== CERuntime_exit ========
 */
Void CERuntime_exit(Void)
{
    Global_exit();
}

/* for backward compatibility with xdc-m based tools */
Void ti_sdo_ce_CERuntime_init__F(Void) {
    CERuntime_init();
}

/*
 * ======== ti.sdo.ce.Settings TEMPLATE ========
 */

/*
 *  ======== ti.sdo.ce Settings Configuration ========
 *  Do not modify following; it is automatically generated from the template
 *  Settings.xdt in the ti.sdo.ce package and will be overwritten next time the
 *  executable is configured
 */
Bool VISA_checked = FALSE;

/*
 * ======== ti.sdo.fc.dskt2.DSKT2 TEMPLATE ========
 */

/* C code contribution from ti/sdo/fc/DSKT2.xdt */

#include <xdc/std.h>
#include <ti/sdo/fc/dskt2/dskt2.h>

extern Void BCACHE_wbInv();

DSKT2_CacheWBInvFxn DSKT2_cacheWBInvFxn = (DSKT2_CacheWBInvFxn)BCACHE_wbInv;

Uns _DSKT2_ALLOW_EXTERNAL_SCRATCH = 1;

#if 8 > DSKT2_NUM_SCRATCH_GROUPS
#error too many values in DARAM_SCRATCH_SIZES array, please fix your .cfg
#endif

#if 8 > DSKT2_NUM_SCRATCH_GROUPS
#error too many values in SARAM_SCRATCH_SIZES array, please fix your .cfg
#endif

Uns _DSKT2_DARAM_SCRATCH_SIZES[ DSKT2_NUM_SCRATCH_GROUPS ] = {
    65536,
    8192,
    0,
    0,
    0,
    0,
    0,
    0
};

Uns _DSKT2_SARAM_SCRATCH_SIZES[ DSKT2_NUM_SCRATCH_GROUPS ] = {
    65536,
    8192,
    0,
    0,
    0,
    0,
    0,
    0
};


/*
 * ======== ti.sdo.fc.dman3.DMAN3 TEMPLATE ========
 */


/* C code contribution from ti/sdo/fc/dman3/DMAN3.xdt */

#include <ti/sdo/fc/dman3/dman3.h>
#include <ti/sdo/fc/acpy3/acpy3.h>

#if 20 > DMAN3_MAXGROUPS
#error DMAN3 internal assertion:  const DMAN3.MAXGROUPS larger than header const
#endif

__FAR__ Uns DMAN3_QDMA_CHANNELS[8] = {
    0,
    1,
    2,
    3,
    4,
    5,
    6,
    7
};

__FAR__ Uns DMAN3_QDMA_QUEUE_MAP[8] = {
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1
};

__FAR__ Uns DMAN3_QUEUE_TC_MAP[8] = {
    0,
    1,
    2,
    3,
    4,
    5,
    6,
    7
};

__FAR__ Uns DMAN3_QUEUE_PRIORITIES[8] = {
    3,
    7,
    0,
    0,
    0,
    0,
    0,
    0
};

extern Void  _DMAN3_initInternal();
extern Void  _DMAN3_initImpl(Void);
/* declaration of user-supplied DMAN3.scratchAllocFxn */
extern Bool DSKT2_allocScratch(IALG_Handle alg, Int mutexId,
            IALG_MemRec *memTab, Int numRecs);

/* declaration of user-supplied DMAN3.scratchFreeFxn */
extern Void DSKT2_freeScratch(Int mutexId, Void *addr, Uns size);


/*
 *  ======== DMAN3_init  ========
 */
Void DMAN3_init(Void)
{
    extern __FAR__ DMAN3_Params DMAN3_PARAMS;

    /* BIOS heap labels */
    extern __FAR__ Int L1DHEAP;
    extern __FAR__ Int DDRALGHEAP;

    DMAN3_PARAMS.heapInternal = L1DHEAP;
    DMAN3_PARAMS.heapExternal = DDRALGHEAP;
    _DMAN3_initImpl();
}


/*
 *  ======== DMAN3_CE_init  ========
 *  Supplied to support legacy Codec Engine Frameworks. To be deprecated in the future.
 */
Void DMAN3_CE_init()
{
    DMAN3_init();
    ACPY3_init();
}

/*
 *  ======== DMAN3_CE_exit  ========
 *  Supplied to support legacy Codec Engine Frameworks. To be deprecated in the future.
 */
Void DMAN3_CE_exit()
{
    DMAN3_exit();
    ACPY3_exit();
}

/*
 *  ======== DMAN3_PARAMS ========
 *  Default module configuration structure for DMAN3 implementation.
 *  It is set at design time by the system integrator to adjust the behaviour
 *  of the module to be optimal for its execution environment.
 */
DMAN3_Params DMAN3_PARAMS = {

    /* qdmaPaRamBase: Physical base address for PARAM0 */
    (Uns *)0x1c04000,

    /* The maxPaRamEntries:  Total number of PARAM Table entries on the
     * hardware (eg, for IVA2 this is 128, for Himalaya, this is 256).
     */
    128,

    /*
     * paRamBaseIndex: (0>value>255) represents the first PARAM TABLE ENTRY
     * NUMBER that is assigned by configuration for exclusive DMAN3 allocation.
     */
    80,

    /*
     * numPaRamEntries:  number of PARAM Table entries starting at
     * DMAN3_PARAM_BASE_INDEX assigned by configuration for exclusive DMAN3
     * allocation.
     */
    48,


    /* maxQdmaChannels:  contains the total number of Physical QDMA channels
     * available on the hardware (eg, 8 for DaVinci, 4 for IVA2).
     */
    8,

    /*
     * numQdmaChannels: contains the number of Physical QDMA channels
     * that are assigned to DMAN3 via configuration.
     * qdmaChannels: array containing list of DMAN3_NUM_QDMA_CHANNELS
     * many Physical QDMA channels assigned to DMAN3 via configuration.
     */
    8,

    DMAN3_QDMA_CHANNELS,

    /*
     * tccAllocationMaskH and tccAllocationMaskL:  are 32-bit bitmasks
     * representing configuration provided list of TCCs for exclusive DMAN3
     * allocation.
     *
     * For example, for TCC's in the range 0-31 the Low Mask (tccAllocationMaskL)
     * is configured so that a '1' in bit position 'i' indicates the TCC 'i' is
     * assigned to DMAN3
     */
    0xffffffff,
    0x0,

     /*
      * heapInternal: BIOS Heap ID for dynamic allocation of DMAN3 objects
      * must be allocated in L1D Internal RAM.
      * heapExternal: BIOS Heap ID for dyn allocation of private DMAN3
      * data structures.
      *
      * A value of -1 indicates that the heap is NOT DEFINED.
      */
    -1, //L1DHEAP,
    0,  //DDRALGHEAP,

    /* numTccGroup[DMAN3_MAXGROUPS] */
    {
    32,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
    },

    /* numPaRamGroup[DMAN3_MAXGROUPS] */
    {
    48,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
    },

    /* idma3Internal */
    0,

    /* scratchAllocFxn */
    DSKT2_allocScratch,

    /* scratchFreeFxn */
    DSKT2_freeScratch,

    /* nullPaRamIndex */
    0,

    /* maxTcs */
    8,

    DMAN3_QDMA_QUEUE_MAP,

    DMAN3_QUEUE_TC_MAP,

    DMAN3_QUEUE_PRIORITIES,

    /* allowUnshared */
    1,
};


/*
 * ======== ti.sdo.fc.global.Settings TEMPLATE ========
 */

Bool ti_sdo_fc_multiProcess = TRUE;

/*
 * ======== ti.sdo.utils.trace.GT TEMPLATE ========
 */




/*
 * C code contribution from ti/sdo/utils/GT.xdt 
 */
#include <xdc/std.h>
#include <ti/sdo/utils/trace/gt.h>

extern Void GTConfig_sysPrintf(String fmt, ...); 
extern Ptr GTConfig_biosMemAlloc(Int size);
 
extern Int GTConfig_tskId(void);
extern void GTConfig_sysPrintfError(String fmt, ...);
extern void GTConfig_biosMemFree(Ptr, Int);
extern UInt32 GTConfig_getTime(void);
extern Int GTConfig_semPend(void);
extern Int GTConfig_semPost(void);
extern Int GTConfig_biosInit(void);

GT_Config _ti_sdo_utils_trace_GT_params = {
    GTConfig_sysPrintf,
    NULL,
    GTConfig_tskId,
    GTConfig_sysPrintfError,
    GTConfig_biosMemAlloc,
    GTConfig_biosMemFree,
    GTConfig_getTime,
    GTConfig_semPend,
    GTConfig_semPost,
    GTConfig_biosInit    
};

GT_Config *GT = &_ti_sdo_utils_trace_GT_params;

/*
 * ======== ti.sdo.ce.node.NODE TEMPLATE ========
 */

/*
 *  ======== NODE Configuration ========
 *  Do not modify following; it is automatically generated from the template
 *  NODE.xdt in the ti.sdo.ce.node package and will be overwritten next time the
 *  executable is configured
 */
#include <ti/sdo/ce/node/node.h>

NODE_Config NODE_config = {
    0,       /* allocate node instances from this MEM seg id */
    0,   /* Segment to allocate message frames */
    0, /* if >0, size NODE must use with MSGQ_alloc */
};
NODE_Config *NODE = &NODE_config;

/*
 * ======== ti.sdo.ce.ipc.dsplink.dsp.Settings TEMPLATE ========
 */


/*
 * ======== ti.sdo.ce.ipc.bios.Ipc TEMPLATE ========
 */



/* IPC_generatedInit() function, called by CERuntime_init() */

Void IPC_generatedInit()
{
    extern Void DSPLINK_init();

    /* call Link's init function */
    DSPLINK_init();
}

/* configuration for interprocessor communication */

/* size of the communication message */
UInt32 Comm_MSGSIZE = 4096;


/* MSGQ and POOL configuration on the DSP when DSPLINK is used */

#include <ti/bios/include/std.h>
#include <ti/bios/include/msgq.h>
#include <ti/bios/include/pool.h>

/*
 * MSGQ configuration
 * ==================
 */
/* number of message queues and storage for them */
#define Global_NUMMSGQUEUES 64

static MSGQ_Obj Global_msgQueues[Global_NUMMSGQUEUES];

/* transport params */
typedef struct Global_ZCPYMQT_Params_tag {
    UInt32 poolId;
} Global_ZCPYMQT_Params;

static Global_ZCPYMQT_Params Global_mqtParams = {
    0   /* pool id; index into POOL_config.allocators[] */
};

/* processor IDs defined by DSPLINK */
#define Global_MAX_PROCESSORS 2
#define Global_ID_GPP         1

/* MSGQ transport functions defined by DSPLINK */
extern Void               ZCPYMQT_init();
extern MSGQ_TransportFxns ZCPYMQT_FXNS;

/* MSGQ transport objects */
static MSGQ_TransportObj Global_transports[Global_MAX_PROCESSORS] =
{
     MSGQ_NOTRANSPORT,     /* Represents the local processor */
     {
        ZCPYMQT_init,      /* Init Function                 */
        &ZCPYMQT_FXNS,     /* Transport interface functions */
        &Global_mqtParams, /* Transport params              */
        NULL,              /* Filled in by transport        */
        Global_ID_GPP      /* remote processor Id           */
     }
};

/* MSGQ object configuration */
MSGQ_Config MSGQ_config = {
    Global_msgQueues,      /* storage for message queue handles */
    Global_transports,     /* indexed array of transports */
    Global_NUMMSGQUEUES,   /* number of message queue handles (above) */
    Global_MAX_PROCESSORS, /* number of processors */
    0,                     /* startUnitialized ??? */
    MSGQ_INVALIDMSGQ,      /* errorQueue to recieve async transport errors */
    POOL_INVALIDID         /* errorPoolId to alloc error messges from */
};

 /*
 * POOL configuration
 * ==================
 */

/* number of pools */
#define Global_NUMALLOCATORS 1

/* SMAPOOL params */
typedef struct Global_SMAPOOL_Params_tag {
    Uint16     poolId ;
    Bool       exactMatchReq ;
} Global_SMAPOOL_Params ;

static Global_SMAPOOL_Params Global_smaPoolParams =
{
    0, /* Pool ID */
    TRUE
} ;

/* DSPLINK's SMAPOOL declarations */
extern Void SMAPOOL_init();
extern POOL_Fxns SMAPOOL_FXNS;

/* pools themselves */
static POOL_Obj Global_pools[Global_NUMALLOCATORS] =
{
    {
        &SMAPOOL_init,              /* Init Function                      */
        &SMAPOOL_FXNS,              /* Pool interface functions           */
        &Global_smaPoolParams,      /* Pool params                        */
        NULL                        /* Pool object: Set within pool impl. */
    }
};

/* POOL object configuration */
POOL_Config POOL_config = {
    Global_pools,
    Global_NUMALLOCATORS
};




/*
 * ======== ti.sdo.ce.alg.Settings TEMPLATE ========
 */

unsigned int ti_sdo_ce_osal_alg_ALG_useHeap = FALSE;
unsigned int ti_sdo_ce_osal_alg_ALG_useCache = FALSE;

unsigned int ti_sdo_ce_osal_alg_ALG_maxGroups = 20; 

UInt32 ti_sdo_ce_alg_ipcKey = 0x4f474c41;

int _ALG_groupUsed[20] = {
     1,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
};

#include <ti/sdo/ce/osal/SemMP.h>
#include <ti/xdais/ialg.h>

SemMP_Handle _ALG_sems[20];

IALG_Handle _Algorithm_lockOwner[20] = {
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
};


// No IRES using algs in the system, and RMAN is not configured into the
// system.  Stub out these fxn calls.
int RMAN_activateAllResources(void *algHandle, void *resFxns, int scratchId)
{
    /* TODO:M  Should assert(false) here.  Should never be called */
    return (0);
}

int RMAN_assignResources(void *algHandle, void *resFxns, int scratchGroupId)
{
    /* TODO:M  Should assert(false) here.  Should never be called */
    return (0);
}

int RMAN_deactivateAllResources(void *algHandle, void *resFxns, int scratchId)
{
    /* TODO:M  Should assert(false) here.  Should never be called */
    return (0);
}

int RMAN_freeResources(void *algHandle, void * resFxns, int scratchGroupId)
{
    /* TODO:M  Should assert(false) here.  Should never be called */
    return (0);
}

int RMAN_init(void)
{
    return (0);
}

int RMAN_exit(void)
{
    return (0);
}


/*
 * ======== ti.sdo.ce.osal.bios.Settings TEMPLATE ========
 */


/*
 * ======== Socrates logging configuration ========
 */
#include <ti/sdo/ce/osal/Log.h>
#include <ti/bios/include/log.h>

extern far LOG_Obj ti_sdo_ce_osal_LOG_Buffer;
far Log_Handle ti_sdo_ce_dvtLog = (Log_Handle)&ti_sdo_ce_osal_LOG_Buffer;


/* Number of BIOS heaps */
Int Memory_numHeaps = 3;

/* Array containing the names of the BIOS heaps. */
String Memory_heapNames[3];

/*
 *  These arrays will contain the original base address and size of each heap.
 */
Uint32 Memory_heapBases[3];
Uint32 Memory_heapSizes[3];

/*
 *  BIOS heap segments.
 */
extern Int DDR2;
extern Int L1DSRAM;
extern Int DDRALGHEAP;

/*
 *  ======== ti_sdo_ce_osal_bios_init ========
 *  Generated init function for BIOS OSAL.
 */
Void ti_sdo_ce_osal_bios_init()
{
    Memory_heapNames[DDR2] = "DDR2";
    Memory_heapNames[L1DSRAM] = "L1DSRAM";
    Memory_heapNames[DDRALGHEAP] = "DDRALGHEAP";
}

/*
 *  Definition of the globally visible character buffer holding all trace
 *  data
 */
Char Global_traceBuffer[262144];
Int  Global_traceBufferSize = 262144;


/*
 * ======== PROGRAM GLOBALS ========
 */


/*
 * ======== CLINK DIRECTIVES ========
 */

#ifdef __ti__
#define PRAGMA(x) _Pragma(#x)
#define xdc__clink_constant(var) \
PRAGMA(DATA_SECTION(var, ".const:" #var))  \
asm("   .sect \".const:" #var "\"\n"  \
  "   .clink\n"  \
  "   .sect \"[0].const:" #var "\"\n"  \
  "   .clink\n"  \
  "   .sect \"[1].const:" #var "\"\n"  \
  "   .clink")

#endif

