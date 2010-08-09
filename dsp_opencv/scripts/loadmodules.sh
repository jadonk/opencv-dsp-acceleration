#
# Default Memory Map
#
# Start Addr    Size    Description
# -------------------------------------------
# 0x80000000    80 MB   Linux
# 0x86300000    32 MB   CMEM
# 0x87300000    13 MB   CODEC SERVER


rmmod dsplinkk.ko
rmmod cmemk.ko
rmmod lpm_omap3530.ko

# Allocate 32MBB for CMEM but set it mainly to heap, just specify small pools for use by codec engine
#insmod cmemk.ko phys_start=0x86300000 phys_end=0x87300000 pools=20x4096
insmod cmemk.ko phys_start=0x85000000 phys_end=0x87000000 pools=20x4096
#insmod lpm_omap3530.ko
# insert DSP/BIOS Link driver
insmod dsplinkk.ko
insmod lpm_omap3530.ko

# make /dev/dsplink
rm -f /dev/dsplink
mknod /dev/dsplink c `awk "\\$2==\"dsplink\" {print \\$1}" /proc/devices` 0
