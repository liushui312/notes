adb remount
adb push bash /system/bin/
adb shell chmod 755 /system/bin/bash
adb shell mkdir /bin
adb shell ln -s /system/bin/bash /bin/sh
adb shell ln -s /system/bin/bash /bin/bash
