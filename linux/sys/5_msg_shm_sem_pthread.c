进程间通讯  ipcs
    消息队列        数据通信, 点对多通信, 离线通信
        msgget msgctl msgsnd msgrcv
    共享内存        数据通信, 映射同块物理内存，直接访问内存，通信效率最高, 大批量数据通信
        shmget shmctl shmat shmdt
    信号量      进程并发处理， 信号灯， 锁机制
                君子协议锁
        semget semctl semop

SYSV: BSD
    msgget msgctl msgsnd msgrcv
    shmget shmctl shmat shmdt
    semget semctl semop
POSIX： UNIX linux
    mq_open mq_close mq_unlink mq_send mq_receive
    shm_open shm_close shm_unlink mmap munmap
    sem_open sem_close sem_unlink sem_wait sem_post

线程
    创建进程会独立内存空间，创建线程不会独立内存空间, 线程依赖进程空间
    线程间通讯方法：直接访问内存
    线程的作用： 同时执行多段代码
        线程比进程效率，进程比线程稳定
    一个程序至少有一个进程，一个进程至少一个线程, 一个线程对应一个栈（函数）
    线程是进程封装库

#include <pthread.h>
    gcc ... -lpthread

    进程        线程
    pid_t       pthread_t 
    getpid      pthread_self/pthread_equal
    fork        pthread_create
    wait        pthread_join
    kill        pthread_cancel
    exit        pthread_exit
    atexit      pthread_cleanup_push/pthread_cleanup_pop

    sem         pthread_mutex
    

