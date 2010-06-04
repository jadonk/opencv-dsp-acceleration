## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: linker.cmd

linker.cmd: \
  package/cfg/remote_ti_platforms_evm3530_xv5T.ov5T \
  package/cfg/remote_ti_platforms_evm3530_xv5T.xdl
	$(SED) 's"^\"\(package/cfg/remote_ti_platforms_evm3530_xv5Tcfg.cmd\)\"$""\"/media/disk/OE/work/ti/my_fir_test/gsoc/apps/cvDFT/cfg/linux/remote_ti_platforms_evm3530/\1\""' package/cfg/remote_ti_platforms_evm3530_xv5T.xdl > $@
