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
        if ( this.CVDFT.watermark == true ) {
                lib = "lib/cvDFT_gsoc.l64P";
        }
        else {
                lib = "lib/cvDFT_gsoc.l64P"; 
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
        template = "gsoc/algos/cvDFT/link.xdt";
    }

    return (template);
}
