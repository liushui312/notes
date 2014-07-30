
Alsa Util 移植

Util 会生成用于播放，录制，配置音频的应用文件，对测试很有用处。
编译过程如下： 
1. Configure

./configure --host=arm-none-linux-gnueabi --prefix=/home/liushui/github/out/alsa-lib/arm-linux/ CFLAGS="-I/home/liushui/github/out/alsa-lib/arm-linux/include" LDFLAGS="-L/home/liushui/github/out/alsa-lib/arm-linux/lib -lasound" --disable-alsamixer --disable-xmlto --with-alsa-inc-prefix=/home/liushui/github/out/alsa_lib/arm-linux/include --with-curses=ncurses

  1）--host 指定编译器，与 lib 的配置选项相同
  2）--prefix 指定编译后文件的安装路径，与 lib 的配置选项相同
  3）CFLAGS   lib 编译后生成的头文件
  4）LDFLAGS  lib 编译生成的库文件
  5）--with-alsa-inc-prefix 指定头文件目录，这个与 3） 类似，但必须指定，
     因为 Configure 程序会去该目录检查版本情况。如果不指定的话，
     则会直接去默认目录 ( 即宿主机对应的 alsalib 目录中寻找，可能会有错误 ) 
  
  2. make 
  3. sudo make install 
  4. 安装
        bin/下的可执行文件 copy 板子/system/bin/
        alsa.conf copy 板子/usr/local/share/alsa.conf 

--------------------------------------------------------------------------
util 的使用

arecord [flags] [filename]
aplay [flags] [filename [filename]] ...

设备:       -D
采样格式:   -f    [U8]
采样率:     -r    [8000hz]
通道:       -c    [1]
录音时间：  -d    [0]         Interrupt after # seconds
数据帧大小：--period-size     PCM DMA单次传送数据帧的大小
            --buffer-size

---------------------------------------------------------------------------
issue

1.执行命令时：No such file or directory
原因：动态编译的程序有INTERP段，它指明了程序在加载过程中需要使用的动态链接加
      载器，如果这个加载器未找到，那么就会提示“ No such file or directory”
解决：使用readelf -l查看可执行文件需要的动态链接加载器，在板子上创建拷贝它。

遇到 configure: error: required curses helper header not found
→执行 sudo apt-get install libncurses5-dev 安装缺少的套件   
遇到 configure: error: panelw library not found
→改执行./configure --with-curses=ncurses
遇到 /bin/bash: xmlto: command not found
→执行 sudo apt-get install xmlto 安装缺少的套件 

2.执行aplay时，未指定period-size参数则kernel panic; 
  指定则出现aplay: pcm_write:1939: write error: Input/output error

解决: 执行aplay时加-N参数
