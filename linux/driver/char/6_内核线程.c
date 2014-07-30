
创建内核线程：
struct task_struct *kthread_create(int (*threadfn)(void *data), 
                                    void *data, const char namefmt[], ...)
参数：
    threadfn，内核线程执行函数
    data，传给threadfn的参数
    namefmt，内核线程名
返回值：
    成功返回task_struct指针，失败返回ERR_PTR(-ENOMEM).

long IS_ERR(const void *ptr);
IS_ERR(task);

唤醒线程运行：
int wake_up_process(struct task_struct *p)
返回值
    成功返回0

判断线程是否可以退出：
int kthread_should_stop(void)
返回值：
    可以退出返回1，否则返回0

通知线程可以退出:
int kthread_stop(struct task_struct *k)
返回值：
    返回值是线程的返回值
