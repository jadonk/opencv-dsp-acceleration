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
        if ( this.OPENCV.watermark == true ) {
                lib = "lib/opencv_beagle.l64P";
        }
        else {
                lib = "lib/opencv_beagle.l64P"; 
        }
        print("    will link with " + this.$name + ":" + lib);
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
