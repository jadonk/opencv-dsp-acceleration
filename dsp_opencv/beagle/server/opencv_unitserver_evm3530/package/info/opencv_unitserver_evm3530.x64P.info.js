{
    "DSP memory map": [
        [
            "L4PER",
            {
                "addr": 0x49000000,
                "type": "other",
                "size": 0x00100000,
            },
        ],
        [
            "DDR2",
            {
                "addr": 0x87A00000,
                "type": "main",
                "size": 0x00400000,
            },
        ],
        [
            "IRAM",
            {
                "addr": 0x107F8000,
                "type": "code",
                "size": 0x00008000,
                "gppAddr": 0x5C7F8000,
            },
        ],
        [
            "L1DSRAM",
            {
                "addr": 0x10F04000,
                "type": "code",
                "size": 0x00010000,
                "gppAddr": 0x5CF04000,
            },
        ],
        [
            "RESET_VECTOR",
            {
                "addr": 0x87F00000,
                "type": "reset",
                "size": 0x00001000,
            },
        ],
        [
            "DDRALGHEAP",
            {
                "addr": 0x86800000,
                "type": "other",
                "size": 0x01200000,
            },
        ],
        [
            "L4CORE",
            {
                "addr": 0x48000000,
                "type": "other",
                "size": 0x01000000,
            },
        ],
        [
            "DSPLINKMEM",
            {
                "addr": 0x87E00000,
                "type": "link",
                "size": 0x00100000,
            },
        ],
        [
            "CMEM",
            {
                "addr": 0,
                "type": "other",
                "size": 0,
            },
        ],
    ],
    "server algorithms": {
        "programName": "opencv_unitserver_evm3530.x64P",
        "algs": [
            {
                "mod": "beagle.algos.opencv.ce.OPENCV",
                "pkg": "beagle.algos.opencv.ce",
                "rpcProtocolVersion": 1,
                "ialgFxns": "OPENCV_BEAGLE_IALG",
                "threadAttrs": {
                    "priority": 3,
                },
                "groupId": 0,
                "serverFxns": "UNIVERSAL_SKEL",
                "stubFxns": "UNIVERSAL_STUBS",
                "name": "opencv",
                "pkgVersion": [
                    "1, 0, 0",
                ],
            },
        ],
    },
}
