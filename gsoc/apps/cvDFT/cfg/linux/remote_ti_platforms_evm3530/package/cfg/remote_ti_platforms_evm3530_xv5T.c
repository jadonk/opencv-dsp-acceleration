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
xdc__META(__ASM__, "@(#)__ASM__ = /media/disk/OE/work/ti/my_fir_test/gsoc/apps/cvDFT/cfg/linux/remote_ti_platforms_evm3530/package/cfg/remote_ti_platforms_evm3530_xv5T");

/*
 *  ======== __ISA__ ========
 *  Define the ISA of this executable.  This symbol is used by platform
 *  specific "exec" commands that support more than one ISA; e.g., gdb
 */
xdc__META(__ISA__, "@(#)__ISA__ = v5T");

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
xdc__META(__TARG__, "@(#)__TARG__ = gnu.targets.arm.GCArmv5T");


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
#include <ti/sdo/ce/osal/Log.h>

Log_Handle ti_sdo_ce_dvtLog = NULL; 

Bool Memory_skipVirtualAddressTranslation = FALSE;


/* List of all packages and their versions and paths, for diagnostics */
String Global_buildInfo[] = {
    "    package gnu.targets.arm.rtsv5T (/media/disk/OE/work/ti/ti-xdctools-tree/packages/gnu/targets/arm/rtsv5T/) [1, 0, 0, 0]",
    "    package gsoc.algos.cvDFT (/media/disk/OE/work/ti/my_fir_test/gsoc/algos/cvDFT/) [1, 0, 0]",
    "    package ti.sdo.ce.global (/media/disk/OE/work/ti/ti-codec-engine-tree/packages/ti/sdo/ce/global/) [1, 0, 0]",
    "    package dsplink.gpp (/media/disk/OE/work/ti/ti-dsplink-tree/packages/dsplink/gpp/) [5, 0, 0]",
    "    package ti.sdo.linuxutils.cmem (/media/disk/OE/work/ti/ti-linuxutils-tree/packages/ti/sdo/linuxutils/cmem/) [2, 2, 0]",
    "    package ti.bios.power (/media/disk/OE/work/ti/ti-local-power-manager-tree/packages/ti/bios/power/) [1, 1, 1]",
    "    package gnu.targets (/media/disk/OE/work/ti/ti-xdctools-tree/packages/gnu/targets/) [1, 0, 1]",
    "    package gnu.targets.arm (/media/disk/OE/work/ti/ti-xdctools-tree/packages/gnu/targets/arm/) [1, 0, 0, 0]",
    "    package ti.sdo.utils.trace (/media/disk/OE/work/ti/ti-framework-components-tree/packages/ti/sdo/utils/trace/) [1, 0, 0]",
    "    package ti.xdais.dm (/media/disk/OE/work/ti/ti-xdais-tree/packages/ti/xdais/dm/) [1, 0, 5]",
    "    package ti.xdais (/media/disk/OE/work/ti/ti-xdais-tree/packages/ti/xdais/) [1, 2.0, 1]",
    "    package ti.sdo.ce.node (/media/disk/OE/work/ti/ti-codec-engine-tree/packages/ti/sdo/ce/node/) [1, 0, 0]",
    "    package ti.sdo.ce.utils.xdm (/media/disk/OE/work/ti/ti-codec-engine-tree/packages/ti/sdo/ce/utils/xdm/) [1, 0, 2]",
    "    package ti.sdo.fc.global (/media/disk/OE/work/ti/ti-framework-components-tree/packages/ti/sdo/fc/global/) [1, 0, 0]",
    "    package ti.sdo.fc.memutils (/media/disk/OE/work/ti/ti-framework-components-tree/packages/ti/sdo/fc/memutils/) [1, 0, 0]",
    "    package ti.sdo.fc.utils (/media/disk/OE/work/ti/ti-framework-components-tree/packages/ti/sdo/fc/utils/) [1, 0, 2]",
    "    package ti.sdo.fc.dman3 (/media/disk/OE/work/ti/ti-framework-components-tree/packages/ti/sdo/fc/dman3/) [1, 0, 4]",
    "    package ti.sdo.fc.acpy3 (/media/disk/OE/work/ti/ti-framework-components-tree/packages/ti/sdo/fc/acpy3/) [1, 0, 4]",
    "    package ti.catalog.arm (/media/disk/OE/work/ti/ti-xdctools-tree/packages/ti/catalog/arm/) [1, 0, 1, 0]",
    "    package ti.catalog (/media/disk/OE/work/ti/ti-xdctools-tree/packages/ti/catalog/) [1, 0, 0]",
    "    package ti.catalog.c6000 (/media/disk/OE/work/ti/ti-xdctools-tree/packages/ti/catalog/c6000/) [1, 0, 0, 0]",
    "    package ti.platforms.evm3530 (/media/disk/OE/work/ti/ti-xdctools-tree/packages/ti/platforms/evm3530/) [1, 0, 0]",
    "    package ti.sdo.ce.osal (/media/disk/OE/work/ti/ti-codec-engine-tree/packages/ti/sdo/ce/osal/) [2, 0, 2]",
    "    package ti.sdo.ce.osal.linux (/media/disk/OE/work/ti/ti-codec-engine-tree/packages/ti/sdo/ce/osal/linux/) [2, 0, 1]",
    "    package ti.sdo.ce.ipc (/media/disk/OE/work/ti/ti-codec-engine-tree/packages/ti/sdo/ce/ipc/) [2, 0, 1]",
    "    package ti.sdo.ce.ipc.dsplink (/media/disk/OE/work/ti/ti-codec-engine-tree/packages/ti/sdo/ce/ipc/dsplink/) [2, 0, 1]",
    "    package ti.sdo.ce.alg (/media/disk/OE/work/ti/ti-codec-engine-tree/packages/ti/sdo/ce/alg/) [1, 0, 1]",
    "    package ti.sdo.ce (/media/disk/OE/work/ti/ti-codec-engine-tree/packages/ti/sdo/ce/) [1, 0, 6]",
    "    package ti.sdo.ce.universal (/media/disk/OE/work/ti/ti-codec-engine-tree/packages/ti/sdo/ce/universal/) [1, 0, 0]",
    "    package gsoc.algos.cvDFT.ce (/media/disk/OE/work/ti/my_fir_test/gsoc/algos/cvDFT/ce/) [1, 0, 0]",
    "    package remote_ti_platforms_evm3530 (/media/disk/OE/work/ti/my_fir_test/gsoc/apps/cvDFT/cfg/linux/remote_ti_platforms_evm3530/) []",
    NULL
};

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
extern IALG_Fxns CVDFT_GSOC_IALG;
static String typeTab_CVDFT_GSOC_IALG[] = {
    "ti.sdo.ce.universal.IUNIVERSAL",
    NULL
};


/* tables of engine algorithms */
extern IALG_Fxns UNIVERSAL_STUBS;

/* algorithm-specific stub/skeleton config data objects */

static Engine_AlgDesc engineAlgs0[] = {
    {
        "cvdft",       /* name */
        {2548591481U},   /* uuid */
        &UNIVERSAL_STUBS,             /* fxns */
        NULL,        /* idma3Fxns */
        typeTab_CVDFT_GSOC_IALG,              /* typeTab */
        FALSE,          /* isLocal */
        0,        /* groupId */
        1,      /* protocol */
        NULL,         /* iresFxns */
        NULL                /* stub/skel config params  */
    },
    {NULL},
};
static Engine_AlgDesc engineAlgs1[] = {
    {NULL},
};

/* table of all engines available in this application */
static Engine_Desc engineTab[] = {
    {"cvDFTEngine",      /* engine name */
      engineAlgs0,    /* alg table */
      "cvDFT_unitserver_evm3530.x64P",         /* (optional) server name */
      NULL,        /* (optional) Link config ID */
      1 /* number of algs in alg table */
    },
    {"local",      /* engine name */
      engineAlgs1,    /* alg table */
      NULL,         /* (optional) server name */
      NULL,        /* (optional) Link config ID */
      0 /* number of algs in alg table */
    },
    {NULL, NULL, NULL, 0}       /* NULL-terminate the engine table */
};

Engine_Config Engine_config = {
    engineTab,              /* table of all engines */
    "local"   /* local RMS engine name */
};

Bool ti_sdo_ce_Engine_noCommName = FALSE;

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


/*
 *  ======== CERuntime_init ========
 */
Void CERuntime_init(Void)
{
    extern Void IPC_generatedInit();

    GT_init();


    /* if CE_DEBUG is set, turn on tracing and DSP auto trace collection */
    if (Global_getenv("CE_DEBUG") != NULL) {
        extern Bool   Engine_alwaysCollectDspTrace;
        extern String Engine_ceDebugDspTraceMask;

        Engine_alwaysCollectDspTrace = TRUE;



        if (Global_getenv("CE_DEBUG")[0] == '1') {
            GT_set("*+67,CE-3,GT_time=0,GT_prefix=1235");
            Engine_ceDebugDspTraceMask = "*+67,GT_prefix=1235,GT_time=3";
        }
        else if (Global_getenv("CE_DEBUG")[0] == '2') {
            GT_set(
                "*+01234567,CE-3,ti.sdo.ce.osal.SemMP=67,OG=467,OM=4567,OC=67,GT_time=0,GT_prefix=1235");
            Engine_ceDebugDspTraceMask =
                "*+01234567,CR=67,ti.sdo.fc.dman3-2,ti.sdo.fc.dskt2-2,GT_prefix=1235,GT_time=3";
        } else {
            GT_set("*+01234567,CE-3,GT_time=0,GT_prefix=12345");
            Engine_ceDebugDspTraceMask = "*+01234567,GT_prefix=12345,GT_time=3";
        }
    }

    if (Global_getenv("CE_CHECK") != NULL) {
        extern Bool VISA_checked;

        /*
         * Currently just change _this_ processor's value... perhaps we should
         * enable remote processors as well?
         */
        if (Global_getenv("CE_CHECK")[0] == '1') {
            VISA_checked = TRUE;
            /* turn on all GT_7CLASS trace (errors) */
            GT_set("*+7");
        } else if (Global_getenv("CE_CHECK")[0] == '0') {
            VISA_checked = FALSE;
        } else {
            /* leave it whatever it was... maybe we should drop a warning? */
        }
    }

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

    Engine_init();
    Server_init();

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
 * ======== ti.sdo.ce.ipc.dsplink.Ipc TEMPLATE ========
 */


/* Max timeout for MSGQ_get() allowed. */
UInt32 ti_sdo_ce_ipc_dsplink_Ipc_maxTimeout = 4294967295UL;

Void IPC_generatedInit()
{
}

/* configuration for interprocessor communication */

/* size of the communication message */
UInt32 Comm_MSGSIZE = 4096;

/* number of Comm_MSGSIZE-sized messages in the system */
UInt32 Comm_NUMMSGS = 64;

/* configure number of retries Comm_locate should do before giving up */
UInt32 Comm_LOCATERETRIES = 20;


/* Arm-side DSPLINK configuration
 * ==============================
 */


#include <ti/sdo/ce/osal/Global.h>

/* Arm-side DspLink configuration tables { */


/* Arm-side DspLink memory map for server "cvDFT_unitserver_evm3530.x64P":
 *
 * table entry format (osal/Global.h), used by DSP/BIOS Link
 *     name of the memory section
 *     DSP virtual address (start of memory section)
 *     physical address (start of memory section)
 *     size in bytes of the memory region
 *     shared access memory
 *     synchronized
 */
static Global_ArmDspLinkConfigMemTableEntry armDspLinkConfigMemTable_cvDFT_unitserver_evm3530_x64P[] = {
    { "DDR2", 0x87A00000, 0x87A00000, 0x00400000, 1, 0 },
    { "DSPLINKMEM", 0x87E00000, 0x87E00000, 0x00100000, 1, 0 },
    { "RESET_VECTOR", 0x87F00000, 0x87F00000, 0x00001000, 1, 0 },
    { "L4PER", 0x49000000, 0x49000000, 0x00100000, 0, 0 },
    { "IRAM", 0x107F8000, 0x5C7F8000, 0x00008000, 1, 0 },
    { "L1DSRAM", 0x10F04000, 0x5CF04000, 0x00010000, 1, 0 },
    { "DDRALGHEAP", 0x86800000, 0x86800000, 0x01200000, 0, 0 },
    { "L4CORE", 0x48000000, 0x48000000, 0x01000000, 0, 0 },
    { "CMEM", 0x00000000, 0x00000000, 0x00000000, 0, 0 },
    { NULL, 0, 0, 0, 0, 0 }
};


/* Arm-side DspLink configuration table for server "cvDFT_unitserver_evm3530.x64P": */
static Global_ArmDspLinkConfig ti_sdo_ce_ipc_armDspLinkConfig_cvDFT_unitserver_evm3530_x64P = {
    armDspLinkConfigMemTable_cvDFT_unitserver_evm3530_x64P,
    BootNoPwr
};

/* List of server names for all DspLink configuration tables */
String ti_sdo_ce_ipc_armDspLinkConfigServerNames[] = {
    "cvDFT_unitserver_evm3530.x64P",
    NULL
};

/* List of matching-positions-by-name DspLink configuration tables for all servers */
Global_ArmDspLinkConfig *ti_sdo_ce_ipc_armDspLinkConfigs[] = {
    &ti_sdo_ce_ipc_armDspLinkConfig_cvDFT_unitserver_evm3530_x64P,
    NULL
};
/* } end of arm-side DspLink configuration tables. */

Int Global_useLinkArbiter = FALSE;

Int LAD_connect(String clientName, Int * id) { return 0; }
Int LAD_disconnect(Int id) { return 0; }
Int LAD_getDspStatus(Int cpuId, Int * statusInfo) { return 0; }
Int LAD_releaseDsp(Int id) { return 0; }
Int LAD_startupDsp(Int id, Int cpuId, Int linkId, String image) { return 0; }




/*
 * ======== ti.sdo.ce.osal.linux.Settings TEMPLATE ========
 */

UInt32 ti_sdo_ce_osal_linux_SemMP_ipcKey = 0x4c41534f;

UInt32 Memory_maxCbListSize = 100;


/*
 * ======== ti.bios.power.OMAP3530GPP TEMPLATE ========
 */


/* configuration parameters needed to avoid undefined references */
int PWRM_doImcopConfig = 0;
int PWRM_enableImcopAtPowerOn;

/*
 * ======== ti.sdo.utils.trace.GT TEMPLATE ========
 */




/*
 * C code contribution from ti/sdo/utils/GT.xdt 
 */
#include <xdc/std.h>
#include <ti/sdo/utils/trace/gt.h>

extern Void GTConfig_tracePrintf(String fmt, ...); 
extern Ptr GTConfig_libcMalloc(Int size);
 
extern Int GTConfig_threadId(void);
extern void GTConfig_error(String fmt, ...);
extern void GTConfig_libcFree(Ptr, Int);
extern UInt32 GTConfig_gettimeofday(void);
extern Int GTConfig_pthreadLock(void);
extern Int GTConfig_pthreadUnlock(void);
extern Int GTConfig_posixInit(void);

GT_Config _ti_sdo_utils_trace_GT_params = {
    GTConfig_tracePrintf,
    NULL,
    GTConfig_threadId,
    GTConfig_error,
    GTConfig_libcMalloc,
    GTConfig_libcFree,
    GTConfig_gettimeofday,
    GTConfig_pthreadLock,
    GTConfig_pthreadUnlock,
    GTConfig_posixInit    
};

GT_Config *GT = &_ti_sdo_utils_trace_GT_params;

/*
 * ======== ti.sdo.ce.alg.Settings TEMPLATE ========
 */

unsigned int ti_sdo_ce_osal_alg_ALG_useHeap = FALSE;
unsigned int ti_sdo_ce_osal_alg_ALG_useCache = FALSE;

unsigned int ti_sdo_ce_osal_alg_ALG_maxGroups = 20; 

UInt32 ti_sdo_ce_alg_ipcKey = 0x4f474c41;

int _ALG_groupUsed[20] = {
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

// No IDMA3 using algs in the system, and DMAN3 is not configured into the
// system.  Stub out these fxn calls.
Void DMAN3_CE_init()
{
}
Void DMAN3_CE_exit()
{
}

int DMAN3_grantDmaChannels(int groupId, void *algHandles, void *dmaFxns,
        int numAlgs)
{
    /* TODO:M  Should assert(false) here.  Should never be called */
    return (0);
}

int DMAN3_releaseDmaChannels(void *algHandles, void * dmaFxns, int numAlgs)
{
    /* TODO:M  Should assert(false) here.  Should never be called */
    return (0);
}

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
 * ======== ti.sdo.fc.global.Settings TEMPLATE ========
 */

Bool ti_sdo_fc_multiProcess = TRUE;

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

