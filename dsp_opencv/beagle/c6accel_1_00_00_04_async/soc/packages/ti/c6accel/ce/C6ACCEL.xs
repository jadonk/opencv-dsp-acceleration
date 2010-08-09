/*
 *  ======== C6ACCEL.xs ========
 *  This file implements the functions specified in the ti.sdo.ce.ICodec
 *  interface.  These functions enable the configuration tool to validate
 *  user supplied configuration parameters (e.g., stack size for the
 *  thread running this codec).
 */

var verbose = false;   /* set to true to enable verbose output during config */


/*
 *  ======== getStackSize ========
 */
function getStackSize(prog)
{
    if (verbose) {
  print("getting stack size for " + this.$name
      + " built for the target " + prog.build.target.$name
      + ", running on platform " +  prog.platformName);
    }

    return (250);
}
