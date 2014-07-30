#!/bin/sh
adb remount
adb push busybox /data/
adb shell chmod 777 /data/busybox
mount -o remount,rw -t yaffs2 /dev/block/nandd /system

adb shell ln -s /data/busybox /system/bin/sh
adb shell rm /system/bin/ls
adb shell ln -s /data/busybox /system/bin/ls
adb shell rm /system/bin/dirname
adb shell ln -s /data/busybox /system/bin/dirname
adb shell rm /system/bin/basename
adb shell ln -s /data/busybox /system/bin/basename
adb shell rm /system/bin/rm
adb shell ln -s /data/busybox /system/bin/rm
adb shell rm /system/bin/mkdir
adb shell ln -s /data/busybox /system/bin/mkdir
adb shell rm /system/bin/mktemp
adb shell ln -s /data/busybox /system/bin/mktemp
adb shell rm /system/bin/awk
adb shell ln -s /data/busybox /system/bin/awk
adb shell rm /system/bin/grep
adb shell ln -s /data/busybox /system/bin/grep
adb shell rm /system/bin/find
adb shell ln -s /data/busybox /system/bin/find
adb shell rm /system/bin/sed
adb shell ln -s /data/busybox /system/bin/sed
adb shell rm /system/bin/cut
adb shell ln -s /data/busybox /system/bin/cut
adb shell rm /system/bin/free
adb shell ln -s /data/busybox /system/bin/free
adb shell rm /system/bin/head
adb shell ln -s /data/busybox /system/bin/head
adb shell rm /system/bin/ldd
adb shell ln -s /data/busybox /system/bin/ldd
adb shell rm /system/bin/fdformat
adb shell ln -s /data/busybox /system/bin/fdformat
adb shell rm /system/bin/touch
adb shell ln -s /data/busybox /system/bin/touch
adb shell rm /system/bin/tr
adb shell ln -s /data/busybox /system/bin/tr
adb shell rm /system/bin/tar
adb shell ln -s /data/busybox /system/bin/tar
adb shell rm /system/bin/printf
adb shell ln -s /data/busybox /system/bin/printf
adb shell rm /system/bin/expr
adb shell ln -s /data/busybox /system/bin/expr
adb shell rm /system/bin/uname
adb shell ln -s /data/busybox /system/bin/uname
adb shell rm /system/bin/zcat
adb shell ln -s /data/busybox /system/bin/zcat
adb shell rm /system/bin/depmod
adb shell ln -s /data/busybox /system/bin/depmod
adb shell rm /system/bin/which
adb shell ln -s /data/busybox /system/bin/which
adb shell rm /system/bin/tail
adb shell ln -s /data/busybox /system/bin/tail
