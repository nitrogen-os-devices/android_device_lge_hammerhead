/*
 * This file contains device specific hooks.
 * Always enclose hooks to #if MR_DEVICE_HOOKS >= ver
 * with corresponding hook version!
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdint.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <log.h>

#if MR_DEVICE_HOOKS >= 1
int mrom_hook_after_android_mounts(const char *busybox_path, const char *base_path, int type)
{
    return 0;
}
#endif /* MR_DEVICE_HOOKS >= 1 */


#if MR_DEVICE_HOOKS >= 2
// Screen gets cleared immediatelly after closing the framebuffer on this device,
// give user a while to read the message box until it dissapears.
void mrom_hook_before_fb_close(void)
{
    usleep(800000);
}
#endif /* MR_DEVICE_HOOKS >= 2 */


#if MR_DEVICE_HOOKS >= 3
static time_t gettime(void)
{
    struct timespec ts;
    int ret;

    ret = clock_gettime(CLOCK_MONOTONIC, &ts);
    if (ret < 0) {
        ERROR("clock_gettime(CLOCK_MONOTONIC) failed: %s\n", strerror(errno));
        return 0;
    }

    return ts.tv_sec;
}

static int wait_for_file(const char *filename, int timeout)
{
    struct stat info;
    time_t timeout_time = gettime() + timeout;
    int ret = -1;
    int waited = 0;

    while (gettime() < timeout_time && ((ret = stat(filename, &info)) < 0))
    {
        waited = 1;
        usleep(30000);
    }

    if(ret >= 0)
        return waited;
    else
        return ret;
}

static void wait_for_mmc(void)
{
    // boot
    const char *filename = "/sys/devices/msm_sdcc.1/mmc_host/mmc1/mmc1:0001/block/mmcblk0/mmcblk0p19/uevent";
    int ret;

    INFO("Waiting for file %s\n", filename);
    ret = wait_for_file(filename, 5);
    if(ret > 0)
    {
        // had to wait, make sure the init is complete
        usleep(100000);
    }
    else if(ret < 0)
    {
        ERROR("Timeout while waiting for %s!\n", filename);
    }
}

static int read_file(const char *path, char *dest, int dest_size)
{
    int res = 0;
    int fd = open(path, O_RDONLY | O_CLOEXEC);
    if(fd < 0)
        return res;

    res = read(fd, dest, dest_size);
    if(res >= dest_size)
        --res;
    if(res >= 0)
        dest[res] = 0;
    close(fd);
    return res >= 0;
}

static int write_file(const char *path, const char *what)
{
    int res = 0;

    int fd = open(path, O_WRONLY | O_CLOEXEC);
    if(fd < 0)
        return res;

    res = write(fd, what, strlen(what)) >= 0;
    close(fd);
    return res;
}

static void set_cpu_governor(void)
{
    size_t i;
    char buff[256];
    static const char *governors[] = { "interactive", "ondemand" };

    if(!read_file("/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor", buff, sizeof(buff)))
        return;

    if(strncmp(buff, "performance", 11) != 0)
        return;

    if(!read_file("/sys/devices/system/cpu/cpu0/cpufreq/scaling_available_governors", buff, sizeof(buff)))
        return;

    // Prevents init from remounting root as ro when the device is encrypted,
    // (Device or resource busy), because that totally makes all the fucking sense.
    // Some preempt bug, the kernel open file counter gets stuck..?
    /*write_file("/sys/module/msm_thermal/core_control/enabled", "0");
    write_file("/sys/devices/system/cpu/cpu1/online", "1");
    write_file("/sys/devices/system/cpu/cpu2/online", "1");
    write_file("/sys/devices/system/cpu/cpu3/online", "1");*/

    for(i = 0; i < sizeof(governors)/sizeof(governors[0]); ++i)
    {
        if(strstr(buff, governors[i]))
        {
            write_file("/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor", governors[i]);
            write_file("/sys/devices/system/cpu/cpu1/cpufreq/scaling_governor", governors[i]);
            write_file("/sys/devices/system/cpu/cpu2/cpufreq/scaling_governor", governors[i]);
            write_file("/sys/devices/system/cpu/cpu3/cpufreq/scaling_governor", governors[i]);
            break;
        }
    }

    write_file("/sys/devices/system/cpu/cpufreq/ondemand/up_threshold","90");
    write_file("/sys/devices/system/cpu/cpufreq/ondemand/sampling_rate","50000");
    write_file("/sys/devices/system/cpu/cpufreq/ondemand/io_is_busy","1");
    write_file("/sys/devices/system/cpu/cpufreq/ondemand/sampling_down_factor","4");
    write_file("/sys/devices/system/cpu/cpufreq/ondemand/down_differential","10");
    write_file("/sys/devices/system/cpu/cpufreq/ondemand/up_threshold_multi_core","70");
    write_file("/sys/devices/system/cpu/cpufreq/ondemand/down_differential_multi_core","3");
    write_file("/sys/devices/system/cpu/cpufreq/ondemand/optimal_freq","960000");
    write_file("/sys/devices/system/cpu/cpufreq/ondemand/sync_freq","960000");
    write_file("/sys/devices/system/cpu/cpufreq/ondemand/up_threshold_any_cpu_load","80");
    write_file("/sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq","300000");
    write_file("/sys/devices/system/cpu/cpu1/cpufreq/scaling_min_freq","300000");
    write_file("/sys/devices/system/cpu/cpu2/cpufreq/scaling_min_freq","300000");
    write_file("/sys/devices/system/cpu/cpu3/cpufreq/scaling_min_freq","300000");

    //write_file("/sys/module/msm_thermal/core_control/enabled", "1");

    write_file("/sys/module/lpm_resources/enable_low_power/l2", "2");
    write_file("/sys/module/lpm_resources/enable_low_power/pxo", "1");
    write_file("/sys/module/lpm_resources/enable_low_power/vdd_dig", "1");
    write_file("/sys/module/lpm_resources/enable_low_power/vdd_mem", "1");
    write_file("/sys/module/pm_8x60/modes/cpu0/power_collapse/suspend_enabled", "1");
    write_file("/sys/module/pm_8x60/modes/cpu1/power_collapse/suspend_enabled", "1");
    write_file("/sys/module/pm_8x60/modes/cpu2/power_collapse/suspend_enabled", "1");
    write_file("/sys/module/pm_8x60/modes/cpu3/power_collapse/suspend_enabled", "1");
    write_file("/sys/module/pm_8x60/modes/cpu0/power_collapse/idle_enabled", "1");
    write_file("/sys/module/pm_8x60/modes/cpu0/standalone_power_collapse/suspend_enabled","1");
    write_file("/sys/module/pm_8x60/modes/cpu1/standalone_power_collapse/suspend_enabled","1");
    write_file("/sys/module/pm_8x60/modes/cpu2/standalone_power_collapse/suspend_enabled","1");
    write_file("/sys/module/pm_8x60/modes/cpu3/standalone_power_collapse/suspend_enabled","1");
    write_file("/sys/module/pm_8x60/modes/cpu0/standalone_power_collapse/idle_enabled","1");
    write_file("/sys/module/pm_8x60/modes/cpu1/standalone_power_collapse/idle_enabled","1");
    write_file("/sys/module/pm_8x60/modes/cpu2/standalone_power_collapse/idle_enabled","1");
    write_file("/sys/module/pm_8x60/modes/cpu3/standalone_power_collapse/idle_enabled","1");
    write_file("/sys/module/pm_8x60/modes/cpu0/retention/idle_enabled","1");
    write_file("/sys/module/pm_8x60/modes/cpu1/retention/idle_enabled","1");
    write_file("/sys/module/pm_8x60/modes/cpu2/retention/idle_enabled","1");
    write_file("/sys/module/pm_8x60/modes/cpu3/retention/idle_enabled","1");
}

void tramp_hook_before_device_init(void)
{
    // Some hammerhead kernels are too fast and mmcblk initialization
    // occurs a bit too late, wait for it.
    wait_for_mmc();

    // hammerhead's kernel has "performance" as default
    set_cpu_governor();
}
#endif /* MR_DEVICE_HOOKS >= 3 */
