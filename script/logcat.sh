#!/system/bin/sh

date=`date +%F_%H-%M-%S`
logcat -v time -f  /storage/external_storage/sdcard1/log/logcat_${date}.txt
