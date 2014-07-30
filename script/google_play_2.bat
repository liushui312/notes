@echo offset 

echo %date% %time%

set var=0
rem ************循环开始了

:continue
set /a var+=1
echo 第%var%次循环

adb devices 

adb shell sendevent /dev/input/event5 3 57 1    
adb shell sendevent /dev/input/event5 3 48 10  
adb shell sendevent /dev/input/event5 3 53 152  
adb shell sendevent /dev/input/event5 3 54 927 
adb shell sendevent /dev/input/event5 3 50 1  
adb shell sendevent /dev/input/event5 0 2 0   
adb shell sendevent /dev/input/event5 0 0 0  
adb shell sendevent /dev/input/event5 0 2 0   
adb shell sendevent /dev/input/event5 0 0 0 

ping 127.0.0.1 -n 3 > nul

adb shell sendevent /dev/input/event5 3 57 1    
adb shell sendevent /dev/input/event5 3 48 10  
adb shell sendevent /dev/input/event5 3 53 521  
adb shell sendevent /dev/input/event5 3 54 1000 
adb shell sendevent /dev/input/event5 3 50 1  
adb shell sendevent /dev/input/event5 0 2 0   
adb shell sendevent /dev/input/event5 0 0 0  
adb shell sendevent /dev/input/event5 0 2 0   
adb shell sendevent /dev/input/event5 0 0 0 

ping 127.0.0.1 -n 1 > nul

if %var% lss 1000 goto continue

rem ************循环结束了

echo 循环执行完毕
pause