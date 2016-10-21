/*
 * This file contains device specific recovery hooks.
 * Always enclose hooks to #if MR_DEVICE_RECOVERY_HOOKS >= ver
 * with corresponding hook version!
 */

#if MR_DEVICE_RECOVERY_HOOKS >= 1
const char *mrom_hook_ubuntu_touch_get_extra_mounts(void)
{
	return "    mkdir -p \\$LXC_ROOTFS_PATH/persist\\n"
			"    mkdir -p \\$LXC_ROOTFS_PATH/firmware\\n"
			"    mount -n -t ext4 -o nosuid,nodev,barrier=1,data=ordered,nodelalloc /dev/mmcblk0p16 \\$LXC_ROOTFS_PATH/persist\\n"
			"    mount -n -t vfat -o ro,uid=1000,gid=1000,dmask=227,fmask=337 /dev/mmcblk0p1 \\$LXC_ROOTFS_PATH/firmware\\n";
}
#endif
