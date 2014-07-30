简介：
    应用 Valgrind 发现 Linux 程序的内存问题
    https://www.ibm.com/developerworks/cn/linux/l-cn-valgrind/

安装

    ./autogen.sh
    ./configure --host=arm-linux CC=arm-none-linux-gnueabi-gcc --perfix=/data/valgrind
    make 
    sudo make install

    host_os变量里加入匹配arm条件就行了，如，原有的armv7*)改成armv7*|arm)

使用
    
    valgrind [options]... program [args]

  选项
    //core 的参数，它对所有的工具都适用
    -q      run silently; only print error msgs       
    --trace-children=no|yes   Valgrind-ise child processes (follow execve)? [no]

    //具体某个工具如 memcheck 的参数
    --tool=<name>   指定工具，Valgrind 默认的工具就是 memcheck

    --leak-check=full

  1. 准备好程序: 为了定位到源代码行，建议在编译时加上-g参数，编译优化选项请选择O0

  2. valgrind [options]... program [args]

  3. 分析 valgrind 的输出信息
