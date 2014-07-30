#!/bin/bash

adb remount 
adb push su /system/xbin/
adb shell chmod 6755 /system/xbin/su
adb shell sync
adb shell reboot
