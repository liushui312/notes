#!/system/bin/sh

date=`date +%F_%H-%M-%S`
LOG_FILE=/storage/external_storage/sdcard1/log/info_${date}.txt

echo "########## /proc/meminfo #############" >> $LOG_FILE
cat /proc/meminfo >> $LOG_FILE

echo "########## dumpsys meminfo ###########" >> $LOG_FILE
dumpsys meminfo >> $LOG_FILE

echo "########## top #############" >> $LOG_FILE
top -t -m 20 -n 3 > $LOG_FILE

