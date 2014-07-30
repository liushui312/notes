make otapackage 所生成的全升级包update.zip分析

|----boot.img
|----bootloader.fex
|----env.fex
|----system/
|----recovery/
 `|----recovery-from-boot.p   //boot.img和recovery.img的补丁(patch)
 `|----etc/                  
   `|----install-recovery.sh  //更新脚本
|---META-INF/
 `|CERT.RSA     //程序块文件，存储了用于签名JAR文件的公共签名
 `|CERT.SF      //这是JAR文件的签名文件，其中前缀CERT代表签名者。     
 `|MANIFEST.MF  //定义了与包的组成结构相关的数据
 `|----com/
   `|----google/
     `|----android/
       `|----update-binary    //相当于一个脚本解释器, 从updater重命名得到
       `|----updater-script   //具体描述了更新过程
   `|----android/
     `|----metadata //描述设备信息及环境变量的元数据。
                    //包括一些编译选项，签名公钥，时间戳以及设备型号等
     `|----otacert

还可以在包中添加userdata目录, 这部分内容在更新后会存放在系统的/data目录下

update.zip包的签名：
    update.zip更新包在制作完成后需要对其签名，否则在升级时会出现认证失败的错
误提示。而且签名要使用和目标板一致的加密公钥。加密公钥及加密需要的三个文件在
Android源码编译后生成的具体路径为：
    out/host/linux-x86/framework/signapk.jar 
    build/target/product/security/testkey.x509.pem         
    build/target/product/security/testkey.pk8 


升级时，对update.zip包检查时大致会分三步：
    1. 检验SF文件与RSA文件是否匹配
    2. 检验MANIFEST.MF与签名文件中的digest是否一致
    3. 检验包中的文件与MANIFEST中所描述的是否一致

-----------------------------------------------------------------------
更新recovery

cat /sdcard/recovery.img > /dev/block/by-name/recovery

flash_image recovery /sdcard/recovery.img

----------------------------------------------------------------------
更新系统 update.zip

1. 进入recovery模式, 选择更新包更新
2. update(升级) 应用
    2.1 选择本地更新包更新
    2.2 在线升级，update.zip包一般被下载到系统的/cache 分区下

  无论将升级包放在什么位置，在使用update.zip更新时都会重启并进入Recovery模式，
然后启动recovery服务（/sbin/recovery）来安装我们的update.zip包

android 的三种启动模式

1. 进入bootloader模式

2. 进入Recovery模式

3. 正常启动

系统进入Recovery模式后会装载Recovery分区
-----------------------------------------------------------------------


