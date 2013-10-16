#!/bin/bash

CPULOAD_HS_TMP_FILE=/tmp/CPULOAD_HS
CPULOAD_IDLE_TMP_FILE=/tmp/CPULOAD_IDLE

function get_idle() {
	cat /proc/stat | grep 'cpu ' | awk '{print $5}'
}

function get_hs() {
	echo $(($(date +%s%N) / 10000000))
}

function count_cpus() {
	cat /proc/cpuinfo | grep processor | wc -l
}

cpu_count=$(count_cpus)

if [ ! -f "$CPULOAD_HS_TMP_FILE" ]; then
	tmp_hs=$(get_hs)
	tmp_idle=$(get_idle)
	echo $tmp_hs > $CPULOAD_HS_TMP_FILE
	echo $tmp_idle > $CPULOAD_IDLE_TMP_FILE
	sleep .5
else
	tmp_hs=$(cat $CPULOAD_HS_TMP_FILE)
	tmp_idle=$(cat $CPULOAD_IDLE_TMP_FILE)
fi

hs=$(get_hs)
idle=$(get_idle)

echo $hs > $CPULOAD_HS_TMP_FILE
echo $idle > $CPULOAD_IDLE_TMP_FILE

diff_hs=$(($hs - $tmp_hs))
diff_idle=$(($idle - $tmp_idle))

idle_per_cpu=$(($diff_idle / $cpu_count))

if [ $diff_hs == 0 -o $idle_per_cpu == 0 ]; then
	real_idle=100
else
	real_idle=$(($idle_per_cpu * 100 / $diff_hs))
fi

load=$((100 - $real_idle))

echo $load
