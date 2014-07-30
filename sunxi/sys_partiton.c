lichee/tools/pack/out/sys_partition.fex

MBR
 16384 Kb

1. boot-resource 分区 (mmcblk0p2/nanda)  64 Mb

  bootloader.fex

2. env 分区 (mmcblk0p5/nandb)   32 Mb

  env.fex

3. boot 分区(mmcblk0p6/nandc)  32 Mb

  boot.fex
  linux 内核, kernel + ramdisk 

4. system 分区 (mmcblk0p7/nandd) 1536 Mb
 system.fex

 android 内核文件

5. data 分区(mmcblk0p8/nande)   2 Gb
 
6. misc 分区(mmcblk0p9/nandf)   32 Mb 

7. recovery 分区(mmcblk0p10/nandg) 64 Mb
 recovery.fex

 保存和boot分区中原始的内容，当恢复出厂设置时，覆盖boot分区

8. cache 分区(mmcblk0p11/nandh)  1080 Mb

9. databk 分区(nandi) 256 Mb

10. UDISK 分区(nandj)

  diskfs.fex

