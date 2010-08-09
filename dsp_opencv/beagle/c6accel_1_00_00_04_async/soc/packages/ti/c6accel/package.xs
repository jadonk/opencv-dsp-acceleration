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
   
    /*Irrespective of target the l674 library is picked when FLOAT variable is set 
      and l64P is picked when FLOAT is false. watermark controls passing of library.
    */
       var codecLibMap = new Array();
       codecLibMap.push( { FLOAT:true, watermark:false, lib:"lib/C6Accel.l674" } );
       codecLibMap.push( { FLOAT:true, watermark:true, lib:null } );
       codecLibMap.push( { FLOAT:false, watermark:false, lib:"lib/C6Accel.l64P" } );
       codecLibMap.push( { FLOAT:false, watermark:true, lib:null } );
       
       for (var i in codecLibMap) {
        if( 
            codecLibMap[i].FLOAT==this.C6ACCEL.FLOAT 
            && codecLibMap[i].watermark ==this.C6ACCEL.watermark 
          ) {
            lib = codecLibMap[i].lib;
        }
      } 
        print("    will link with " + this.$name + ":" + lib);
  
        
    return (lib);
}

/*
 *  ======== getSects ========
 */
function getSects()
{
    var template = null;

    if (Program.build.target.isa == "64P") {
        template = "ti/c6accel/link.xdt";
    }

    return (template);
}
