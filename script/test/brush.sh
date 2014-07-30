#!/bin/bash

eval `adb devices | awk 'BEGIN{cnt=-1}/device/{cnt++}END{print "count="cnt}'`

if [ ${count} -eq 0 ]; then 
    echo "err: No devices is connected."
    exit 1
elif [ ${count} -gt 1 ]; then
    echo "err: Multiple devices connected."
    exit 2
elif [ ${count} -eq 1 ]; then
    echo "one device is connected."
fi

echo ""
echo "******** 开始刷机，请不要拔出usb ************"
INSTALL_PATH="/home/liushui/Bin/LiveSuit"
${INSTALL_PATH}/LiveSuit.sh &
lpid=$!

sleep 5
adb reboot efex

sleep 900
kill -9 $lpid
