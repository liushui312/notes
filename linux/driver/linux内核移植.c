linux 内核编译配置
    
    make menuconfig  -> .config 


内核启动过程：

1. head.s 和 head-common.s

2. init/main.c 中的start_kernel()函数
    负责初始化内核各个子系统，最后调用reset_init()

关键函数：setup_arch() 和 reset_init()

3. reset_init()，启动一个叫做init的内核线程，继续初始化


内核编译系统：
kbuild通常不会直接去调用某个目录下的Makefile，而是让该目录作为scripts/Makefile.build的参数

scripts/Makefile.build这个文件很重要。看看它做了什么：
由于scripts/Makefile.build后面没跟目标，所以默认为第一个目标：
    PHONY := __build

builtin-target，subdir-ym 等变量，
subdir-ym 记录了需要递归调用的子目录。以后递归调用Makefile全靠它了。
builtin-target 指当前目录下的目标文件，即$(obj)/built-in.o
