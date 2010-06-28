/*
 *  ======== package.xs ========
 *
 */
function getLibs(prog)
{
    var lib = "lib/libopencvdsp.so";
    print("    will link with " + this.$name + ":" + lib);
    return (lib);
}

function close()
{
    /* Provide dependency check on CE universal */
    try {
        t = xdc.loadPackage("ti.sdo.ce.universal");
    } catch (e) {
        print("    package ti.sdo.ce.universal not found, skipping..");
    }
}
