#!/system/bin/sh

#
# Copyright 2016 Nitrogen Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# init.qcom.post_boot.sh
# Here it is possible to write any shell a code.
# For example, to make verification of files if there is a file that to delete it.
#

# Disble mpdecision
stop mpdecision;
echo -n 0 > /sys/kernel/msm_mpdecision/conf/boost_enabled

# Enable all cores
echo -n 1 > /sys/devices/system/cpu/cpu1/online
echo -n 1 > /sys/devices/system/cpu/cpu2/online
echo -n 1 > /sys/devices/system/cpu/cpu3/online

# Set cpu governor
echo -n "ondemand" > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor
echo -n "ondemand" > /sys/devices/system/cpu/cpu1/cpufreq/scaling_governor
echo -n "ondemand" > /sys/devices/system/cpu/cpu2/cpufreq/scaling_governor
echo -n "ondemand" > /sys/devices/system/cpu/cpu3/cpufreq/scaling_governor

# Set cpu min freq
echo -n 300000 > /sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq
echo -n 300000 > /sys/devices/system/cpu/cpu1/cpufreq/scaling_min_freq
echo -n 300000 > /sys/devices/system/cpu/cpu2/cpufreq/scaling_min_freq
echo -n 300000 > /sys/devices/system/cpu/cpu3/cpufreq/scaling_min_freq

# Set cpu max freq
echo -n 2265000 > /sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq
echo -n 2265000 > /sys/devices/system/cpu/cpu1/cpufreq/scaling_max_freq
echo -n 2265000 > /sys/devices/system/cpu/cpu2/cpufreq/scaling_max_freq
echo -n 2265000 > /sys/devices/system/cpu/cpu3/cpufreq/scaling_max_freq

# Set read-ahead buffer size
echo -n 3072 > /sys/devices/virtual/bdi/179:0/read_ahead_kb

# Run other scripts
if [ -f /system/etc/nitrogen.sh ]; then
	# Alee 2 mins after boot
	sleep 120
	shell /system/etc/nitrogen.sh
fi;
