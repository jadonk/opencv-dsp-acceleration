## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: linker.cmd

linker.cmd: \
  package/cfg/c6accelw_xv5T.ov5T \
  package/cfg/c6accelw_xv5T.xdl
	$(SED) 's"^\"\(package/cfg/c6accelw_xv5Tcfg.cmd\)\"$""\"/usr/devel/a0272049/dvsdk_3_00_02_44/c6accel/soc/c6accelw/c6accelw_config/\1\""' package/cfg/c6accelw_xv5T.xdl > $@
