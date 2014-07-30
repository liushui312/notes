/sys/class/android_usb/android0/

f_mass_storage  
f_adb           
f_mtp           
f_acm           
f_audio_source  
f_ptp
f_accessory     
f_ffs           
f_rndis


//config PID and VID, select driver: mass_storage
echo 0 > /sys/class/android_usb/android0/enable
echo 18d1 > /sys/class/android_usb/android0/idVendor
echo 0001 > /sys/class/android_usb/android0/idProduct
echo mass_storage > /sys/class/android_usb/android0/functions

//insmod device inode and enable usb
echo /dev/nande > /sys/class/android_usb/android0/f_mass_storage/lun/file
echo 1 > /sys/class/android_usb/android0/enable

//disable usb driver
echo 0 > /sys/class/android_usb/android0/enable

