/*
 *  ======== package.xs ========
 *
 */

/*
 *  ======== getLibs ========
 */
function getLibs(prog)
{
    var lib = null;
  
    if (prog.build.target.isa == "64P") {        
        
       lib = "lib/opencv_beagle.l64P";
        
    }
    return (lib);
}

/*
 *  ======== getSects ========
 */
function getSects()
{
    var template = null;

    if (Program.build.target.isa == "64P") {
        template = "beagle/algos/opencv/link.xdt";
    }

    return (template);
}
