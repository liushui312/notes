
编译命令

$ cd path/to/android/root/
$ source build/envsetup.sh
$ make PRODUCT-

Android 编译系统是通过各种.mk 和 shell 脚本共同定义了一个编译框架

1. 编译中枢 (build/core/)
    包含各种mk文件，将遍历所有子项目，并生成所有target
    内部定义了一些变量子项目使用：
      LOCAL_PATH
      LOCAL_MODULE_TAGS
      LOCAL_SRC_FILES
      LOCAL_PACKAGE_NAME

    
2. 子项目
    每个项目都必须包含一个Android.mk, 该文件描述包含哪些源文件，target类型等
    对编译中枢定义的变量赋值

3. 输出路径
-----------------------------------------------------------------------------

1. 编译 uboot
  


