#!/bin/bash

function _signal() {
    adb pull /sdcard/logcat.txt .
    echo "********* do logcat successfull **********"
    exit 
}

trap _signal INT

adb devices || \
{
    echo "*********** do logcat failed *************"
    exit 1 
}
adb shell rm /sdcard/logcat.txt
adb logcat -c 
adb logcat -v time -f /sdcard/logcat.txt
