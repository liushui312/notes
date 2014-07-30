ioctl
系统调用
int ioctl(int d, int request, ...); 可变参数只有一个, 习惯上用 char *argp 定义 
ioctl方法原型
int ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg);

选择ioctl命令号:
/* include/asm/ioctl.h 和 Documentation/ioctl-number.txt */
1）type      幻数     8位 _IOC_TYPEBITS 
2）number    序数     8位 _IOC_NRBITS
3）directoin 传递方向 8位 _IOC_NONE  _IOC_READ _IOC_WRITE (从应用程序方向看)   
4）size      用户数据大小 _IOC_SIZEBITS

/* asm/ioctl.h 构造命令编号的宏, size = sizeof(datatype) */
_IO(type,nr) //无参数的命令编号            
_IOR(type,nr,datatype)
_IOW(type,nr,datatype)
_IOWR(type,nr,datatype)


------------------------------------------------------------------------------------------------------------
阻塞型I/O
阻塞操作是指在执行设备操作时若不能获得资源则挂起进程, 直到满足可操作的条件后再进行操作.
被挂起的进程进入休眠状态,被从调度器的运行队列移走,直到等待的条件被满足.

对设备驱动来说, 让进程休眠的规则:
    1.永远不要在原子上下文中进入休眠
    2.被唤醒时, 永远无法知道休眠时发生了什么事, 必须再次检查它所等待的条件是否为真.

阻塞进程的唤醒:
等待队列, 一个进程链表, 其中包含了等待某个特定事件的所有进程.

等待队列头
  定义和初始化
    1. DECLARE_WAIT_QUEUE_HEAD(name);
    2. wait_queue_head_t my_queue;
       init_waitqueue_head(&my_queue);

简单休眠的方式:
/* queue 不是指针, condition 布尔表达式 */
  wait_event(queue, condition);
  wait_event_interruptible(queue, condition);//可被信号中断, 返回整数值
  wait_event_timeout(queue, condition, timeout);
  wait_event_interruptible_timeout(queue, condition, timeout);

唤醒队列:
/* wake_up_interruptible()只能唤醒处于 TASK_INTERRUPTIBLE 的进程 */
  void wake_up(wait_queue_head_t *queue);
  void wake_up_interruptible(wait_queue_head_t *queue);




------------------------------------------------------------------------------------------------------------
非阻塞型I/O
非阻塞操作的进程在不能进行设备操作时并不挂起, 它或者放弃, 或者不停地查询, 直至可以进行操作为止.




