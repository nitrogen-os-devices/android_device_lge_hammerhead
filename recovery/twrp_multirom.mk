#MultiROM config. MultiROM also uses parts of TWRP config
MR_INPUT_TYPE := type_b
MR_INIT_DEVICES := device/lge/hammerhead/recovery/multirom/mr_init_devices.c
MR_DPI := xhdpi
MR_DPI_FONT := 340
MR_FSTAB := device/lge/hammerhead/recovery/twrp.fstab
MR_KEXEC_MEM_MIN := 0x20000000
MR_KEXEC_DTB := true
MR_INFOS := device/lge/hammerhead/recovery/multirom/infos
MR_DEVICE_HOOKS := device/lge/hammerhead/recovery/multirom/mr_hooks.c
MR_DEVICE_HOOKS_VER := 3
MR_DEVICE_RECOVERY_HOOKS := device/lge/hammerhead/recovery/multirom/mr_hooks_recovery.c
MR_DEVICE_RECOVERY_HOOKS_VER := 1
MR_CONTINUOUS_FB_UPDATE := true
MR_PIXEL_FORMAT := "RGB_565"
#MR_ENCRYPTION := true
#MR_ENCRYPTION_SETUP_SCRIPT := device/lge/hammerhead/recovery/multirom/mr_cp_crypto.sh

DEVICE_RESOLUTION := 1080x1920
TARGET_RECOVERY_IS_MULTIROM := true