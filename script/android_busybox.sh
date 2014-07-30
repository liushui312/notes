#!/bin/sh

CUR_FILE_PATH=`dirname $0`
adb remount
adb shell mount -o remount,rw /dev/block/nandd /system

echo ""
echo "********* install busybox **********"
adb push $CUR_FILE_PATH/busybox /data/ || \
{
    echo "******* install failed *********"
    exit 2
}
adb shell chmod 777 /data/busybox

cmd=(ls dirname basename rm mkdir mktemp cut awk sed grep find free head ldd fdformat touch tr tar printf expr uname zcat depmod which tail hwclock seq wc diff usleep)

for i in ${cmd[@]} 
do 
    echo "adb shell rm /system/bin/$i"
    adb shell rm /system/bin/$i
    echo "adb shell ln -s /data/busybox /system/bin/$i"
    adb shell ln -s /data/busybox /system/bin/$i
done
