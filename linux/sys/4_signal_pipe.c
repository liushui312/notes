进程组  一个进程或多个进程
    getpgrp setpgrp
    进程组描述符 就是进程组组长的描述符
        组长结束不影响进程组描述符

会话    一个进程组或多个进程组
    一个会话对应一个控制终端
    组长不能做会话描述符

控制终端    输入输出

前台进程组  使用控制终端的进程组
后台进程组
守护进程

--------------------------------------------------------------------------
进程间通讯
    信号 管道 消息队列 共享内存 信号量 套接字(socket)
        怎么杀死进程
-------------------------------------------------------------------------
信号: 异步通知  软件层模拟硬件中断机制
            
  发信号 
      1.某些硬件中断
      2.内核
      3.kill命令
      4.系统调用kill函数
          
  捕捉信号
      1.系统调用signal

  信号处理: 
      默认        SIG_DFL     大部分是结束本进程，没任何操作
      忽略        SIG_IGN
      自定义操作

  注意事项:
      9 19是不能捕捉和忽略
      子进程会复制父进程信号绑定情况，执行了exec只会继承默认和忽略操作

      在进程表的表项中有一个软中断信号域，该域中每一位对应一个信号，当有
      信号发送给进程时，对应位置位。

  信号的类型 
    信号源 kill -l; 
    64个信号,其中前面31个信号为不可靠信号(非实时的，可能会出现信号的丢失)，
  后面的信号为可靠信号(实时的real_time,对信号排队,不会丢失)
    1 SIGHUP (挂起)  当运行进程的用户注销时通知该进程，使进程终止
    2 SIGINT (中断)  当用户按下ctrl+c时,通知前台进程组终止进程
    3 SIGQUIT (退出) 当用户按下ctrl+\时,通知前台进程组终止进程
    4 SIGILL (非法指令) 执行了非法指令
    5 SIGTRAP 由断点指令或其它trap指令产生. 由debugger使用
    6 SIGABRT (异常中止) 调用abort函数生成的信号
    7 SIGBUS (非法地址) 包括内存地址对齐(alignment)出错 
                        eg, 访问一个四个字长的整数, 但其地址不是4的倍数.
    8 SIGFPE (算术异常) 致命算术运算错误,包括浮点运算错误、溢出及除数为0.
    9 SIGKILL (确认杀死) 当用户通过kill -9向进程发送信号时，可靠的终止进程
    10 SIGUSR1 (用户使用)
    11 SIGSEGV (段越界) 当进程尝试访问不属于自己的内存空间导致内存错误时，
                        终止进程
    12 SIGUSR2 (用户使用)
    13 SIGPIPE 写至无读进程的管道, 或者Socket通信SOCT_STREAM的读进程
               已经终止，而再写入。
    14 SIGALRM (超时) alarm函数使用该信号，时钟定时器超时响应
    15 SIGTERM (软中断) 使用不带参数的kill命令时终止进程
    17 SIGCHLD (子进程结束) 当子进程终止时通知父进程
    18 SIGCONT (暂停进程继续) 让一个停止(stopped)的进程继续执行. 
                              本信号不能被阻塞.
    19 SIGSTOP (停止) 作业控制信号,暂停停止(stopped)进程的执行. 
                      本信号不能被阻塞, 处理或忽略.
    20 SIGTSTP (暂停/停止) 交互式停止信号, Ctrl-Z 发出这个信号
    21 SIGTTIN 当后台作业要从用户终端读数据时, 终端驱动程序产生SIGTTIN信号
    22 SIGTTOU 当后台作业要往用户终端写数据时, 终端驱动程序产生SIGTTOU信号
    23 SIGURG 有紧急数据或网络上带外数据到达socket时产生.
    24 SIGXCPU 超过CPU时间资源限制. 这个可以由getrlimit/setrlimit来读取改变
    25 SIGXFSZ 当进程企图扩大文件以至于超过文件大小资源限制。
    26 SIGVTALRM 虚拟时钟信号. 计算的是该进程占用的CPU时间.
    27 SIGPROF (梗概时间超时) setitimer(2)函数设置的梗概统计间隔计时器
                              (profiling interval timer)
    28 SIGWINCH 窗口大小改变时发出.
    29 SIGIO(异步I/O) 文件描述符准备就绪, 可以开始进行输入/输出操作.
    30 SIGPWR 电源失效/重启动
    31 SIGSYS 非法的系统调用。 

  信号机制

  有关信号的系统调用
    1.signal
      #include <signal.h>
      typedef void (*sighandler_t)(int);
      sighandler_t signal(int signum, sighandler_t handler); 

      成功时返回返回上一次绑定的操作; 错误时返回错误代码SIG_ERR;
      在执行信号处理函数的时候会屏蔽当前信号
      传递给信号处理例程的整数参数是信号值，这样可以使得一个信号处理例程
    处理多个信号。
       signal(2, SIG_IGN) //忽略信号2

    2.kill
      #include <sys/types.h>
      #include <signal.h>
      int kill(pid_t pid, int sig);

      pid == 0    给本进程组发信号
      pid == -1   给除了进程1和自身以外所有进程发信号
      pid > 0     给pid对应进程发信号
      pid < -1    给取绝对值pid对应进程组发信号

      成功返回0; 错误返回-1，并设置相应的错误代码errno
        EINVAL：指定的信号sig无效
        ESRCH：参数pid指定的进程或进程组不存在。注意，在进程表项中存在的进程
        ，可能是一个还没有被wait收回，但已经终止执行的僵死进程。
        EPERM：进程没有权力将这个信号发送到指定接收信号的进程

    3.pause //等待一个信号，进程进入睡眠，直到接收到一个信号为止
      #include <unistd.h>
      int pause(void);
      该调用总是返回-1，并设置错误代码为EINTR

    4.alarm //设置一个定时器，当定时器计时到达时，将发出SIGALRM信号给进程
      #include <unistd.h>
      unsigned int alarm(unsigned int seconds); 
    
    5.setitimer getitimer
      #include <sys/time.h>
      int getitimer(int which, struct itimerval *value);
      int setitimer(int which, const struct itimerval *value, 
                struct itimerval *ovalue); 
      which:
        ITIMER_REAL：按实际时间计时，计时到达将给进程发送 SIGALRM 信号
        99vIMER_VIRTUAL：仅当进程执行时才进行计时。发送 SIGVTALRM 信号
        99vIMER_PROF：当进程执行时和系统为该进程执行动作时都计时。
          与99vIMER_VIR-TUAL是一对，该定时器经常用来统计进程在用户态和内核态
          花费的时间。计时到达将发送 SIGPROF 信号给进程。 

      struct itimerval {
          struct timeval it_interval;   /* Interval time from second time */
          struct timeval it_value;      /* Timeout first time */
      }; 
      struct timeval {
          long tv_sec; /* 秒 */
          long tv_usec; /* 微秒，1秒 = 1000000 微秒*/
      }; 


------------------------------------------------------------------------    
管道
    管道文件 管道命令“｜”

    无名管道    内核返回文件描述符
        pipe
            fd[0]       读端
            fd[1]       写端
        只能用在有关系的进程之间

    有名管道    管道文件
        mkfifo
        必须以读写形式打开，否则会阻塞

    半双工通信机制
    管道对应内核提供缓冲区，读出来的数据会清空缓冲区, 缓冲区为空，读会阻塞
    管道只能点对点通信, 读写端必须同时存在
    管道读端全关闭，再往管道写，会产生信号SIGPIPE
    
#include <unistd.h>
int pipe(int pipefd[2]); //创建管道
pipefd[0]   读端
pipefd[1]   写端

