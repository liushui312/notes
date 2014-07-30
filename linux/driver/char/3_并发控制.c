并发(concurrency)指的是多个执行单元同时、并行被执行,而并发的执行单元对共享资源
(硬件资源和软件上的全局变量、静态变量等)的访问则很容易导致竞态(race conditions).

竞态问题
    1.对称多处理器(SMP)的多个CPU
    2.单 CPU 内进程与抢占它的进程
    3.中断(硬中断、软中断、Tasklet、底半部)与进程之间

规则: 避免资源的共享(少用全局变量等)
      在单个执行线程之外共享硬件资源或软件资源的任何时候, 必须显示地管理对该资源的访问.

  解决竞态问题的途径是保证对共享资源的互斥访问,所谓互斥访问是指一个执行单元在访问共享资源的时候,
其他的执行单元被禁止访问。
  访问共享资源的代码区域称为临界区(critical sections),临界区需要以某种互斥机制加以保护。

互斥机制
    1. 自旋锁   spinlock
    2. 信号量   semaphore
    3. 互斥体   mutex
    4. 完成量   completion  线程之间
    5. 中断屏蔽 
    6. 原子操作

1.自旋锁 spinlock

可在不能休眠的代码中使用, 比如中断处理函数.
自旋锁等待是不可中断的
在拥有锁时避免休眠
自旋锁必须在可能的最短时间内拥有
解决内核进程抢占, 但还受到中断和底半部(BH)的影响.
/* <linux/spinlock.h> */
    spinlock_t spin;       //定义自旋锁
    spin_lock_init(spinlock_t *lock);  //初始化自旋锁, 宏
    DEFINE_SPINLOCK(lock); //定义并初始化自旋锁

    /* 获得自旋锁 */
    spin_lock(spinlock_t *lock);   //如果能立即获得锁,它就马上返回,否则,它将自旋在那里,直到该自旋锁的保持者释放.
    spin_trylock(spinlock_t *lock);//如果能立即获得锁,它获得锁并返回真,否则立即返回假,实际上不再“在原地打转”.

    /* 释放自旋锁 */
    spin_unlock(spinlock_t *lock);     

例:
    spinlock_t lock;
    spin_lock_init(&lock);
    spin_lock (&lock) ; //获取自旋锁,保护临界区
    ... //临界区
    spin_unlock (&lock) ; //解锁

问题:
    阻塞等待锁, 效率低.
    自旋锁可能导致系统死锁.
        递归使用一个自旋锁
        如果进程获得自旋锁之后再阻塞, 所以临界区不能使用copy_from_user()、copy_to_user()和kmalloc()等

中断加自选锁
    spin_lock_irq()          = spin_lock()   + local_irq_disable()
    spin_lock_irqsave()      = spin_unlock() + local_irq_save()    //状态保存在flags
    spin_lock_bh()           = spin_lock()   + local_bh_disable()
    spin_unlock_irq()        = spin_unlock() + local_irq_enable()
    spin_unlock_irqrestore() = spin_unlock() + local_irq_restore()
    spin_unlock_bh()         = spin_unlock() + local_bh_enable()

读写自旋锁: rwlock 允许多个进程读取临界资源

    rwlock_t my_rwlock = RW_LOCK_UNLOCKED; /* 静态初始化 */

    rwlock_t my_rwlock;
    rwlock_init(&my_rwlock); /* 动态初始化 */

    DEFINE_RWLOCK(rwlock);   /* 宏 */
在对共享资源进行读取之前,应该先调用读锁定函数,完成之后应调用读解锁函数。
    /* 读锁定 */
    void read_lock(rwlock_t *lock);
    void read_lock_irqsave(rwlock_t *lock, unsigned long flags);
    void read_lock_irq(rwlock_t *lock);
    void read_lock_bh(rwlock_t *lock);
    /* 读解锁 */
    void read_unlock(rwlock_t *lock);
    void read_unlock_irqrestore(rwlock_t *lock, unsigned long flags);
    void read_unlock_irq(rwlock_t *lock);
    void read_unlock_bh(rwlock_t *lock);
在对共享资源进行写入之前,应该先调用写锁定函数,完成之后应调用写解锁函数。
    /* 写锁定 */
    void write_lock(rwlock_t *lock);
    void write_lock_irqsave(rwlock_t *lock, unsigned long flags);
    void write_lock_irq(rwlock_t *lock);
    void write_lock_bh(rwlock_t *lock);
    int write_trylock(rwlock_t *lock);
    /* 写解锁 */
    void write_unlock(rwlock_t *lock);
    void write_unlock_irqrestore(rwlock_t *lock, unsigned long flags);
    void write_unlock_irq(rwlock_t *lock);
    void write_unlock_bh(rwlock_t *lock);
例:
    rwlock_t lock; //定义 rwlock
    rwlock_init(&lock); //初始化 rwlock
    //读时获取锁
    read_lock(&lock);
    ... //临界资源
    read_unlock(&lock);
    //写时获取锁
    write_lock_irqsave(&lock, flags);
    ... //临界资源
    write_unlock_irqrestore(&lock, flags);

顺序锁 

2.信号量

/* <linux/semaphore.h> */
struct semaphore {
	spinlock_t		lock;
	unsigned int		count;
	struct list_head	wait_list;
};

信号量必须在设备对系统其他部分可用前被初始化.
声明和初始化:
    1. struct semaphore sem;
       void sema_init(struct semaphore *sem, int val);

    2. DECLARE_MUTEX(name);       //初始化为1, 解锁状态
       DECLARE_MUTEX_LOCKED(name);//初始化为0, 锁定状态

init_MUTEX(struct semaphore *sem);       //初始化为1, 解锁状态
init_MUTEX_LOCKED(struct semaphore *sem);//初始化为0, 锁定状态

/* down 减小信号量的值, 执行成功则该进程拥有此信号量, 加锁 */
void down(struct semaphore *sem);
int down_interruptible(struct semaphore *sem); //检查返回值, 失败则返回-ERESTARTSYS 或 -EINTR
int down_trylock(struct semaphore *sem);
/* up 增加信号量的值, 执行成功则该进程不再拥有此信号量, 解锁 */
void up(struct semaphore *sem);

正确使用锁定机制的关键是, 明确指定需要保护的资源, 并确保每一个对这些资源的访问使用正确的锁定.

读写信号量: rwsem 允许多个进程读取临界资源
/* <linux/rwsem.h> */
初始化:
    struct rw_semaphore sem;
    void init_rwsem(struct rw_semaphore *sem);

    DECLARE_RWSEM(sem);

只读:
    void down_read(struct rw_semaphore *sem); //只读, 可并发
    int down_read_trylock(struct rw_semaphore *sem); //成功返回非零, 失败0 
    void up_read(struct rw_semaphore *sem);
只写:
    void down_write(struct rw_semaphore *sem);
    int down_write_trylock(struct rw_semaphore *sem);
    void up_write(struct rw_semaphore *sem);
    void downgrade_write(struct rw_semaphore *sem); //允许其他读取者的访问

3.互斥体:
DEFINE_MUTEX(mutex);
mutex_lock(&mutex);
//临界资源
mutex_unlock(&mutex);
注意：使用上与信号量一样。


4.完成变量: 实现一个线程告诉另一个线程某个工作已经完成
/* <linux/completion.h> */
初始化:
    struct completion com;
    void init_completion(struct completion *com);

    DECLARE_COMPLETION(com);

等待:    
void wait_for_completion(struct completion *c);//进入睡眠状态，等待通知唤醒
void wait_for_completion_interruptible(struct completion *c);//进入信号可中断睡眠状态，等待通知唤醒

唤醒:
void complete(struct completion *c);//唤醒一个
void complete_all(struct completion *c);//唤醒全部

5. 中断屏蔽将使得中断与进程之间的并发不再发生,而且,由于 Linux 内核的进程调度等操作都依赖
中断来实现,内核抢占进程之间的并发也就得以避免了,并不能解决 SMP 多 CPU 引发的竞态。

/* include/linux/irqflags.h */
local_irq_disable() //屏蔽中断
critical_section    //临界区
local_irq_enable()  //开中断

local_irq_save(flags) //除了进行禁止中断的操作以外,还保存目前 CPU 的中断位信息
local_irq_restore(flags)

local_bh_disable() //禁止中断的底半部
local_bh_enable()  //使能中断的底半部

6.原子操作: 整型 和 位

/* arch/arm/include/asm/atomic.h */
整型原子操作 
atomic_t v = ATOMIC_INIT(0);         //定义原子变量 v 并初始化为 0
atomic_read(atomic_t *v);            //返回原子变量的值, 宏
void atomic_set(atomic_t *v, int i); //设置原子变量的值为 i
void atomic_add(int i, atomic_t *v); //原子变量增加 i
void atomic_sub(int i, atomic_t *v); //原子变量减少 i
void atomic_inc(atomic_t *v);        //原子变量增加 1
void atomic_dec(atomic_t *v);        //原子变量减少 1
/*相应操作后, 测试其是否为 0, 为 0 则返回 true,否则返回 false*/
int atomic_inc_and_test(atomic_t *v);
int atomic_dec_and_test(atomic_t *v);
int atomic_sub_and_test(int i, atomic_t *v);
/*相应操作后, 返回新的值.*/
int atomic_add_return(int i, atomic_t *v);
int atomic_sub_return(int i, atomic_t *v);
int atomic_inc_return(atomic_t *v);
int atomic_dec_return(atomic_t *v);

位原子操作
void set_bit(nr, void *addr);
void clear_bit(nr, void *addr);
void change_bit(nr, void *addr);
test_bit(nr, void *addr);
int test_and_set_bit(nr, void *addr);
int test_and_clear_bit(nr, void *addr);
int test_and_change_bit(nr, void *addr);

例:
    static atomic_t xxx_available = ATOMIC_INIT(1); /* 定义原子变量 1为可用, 0为不可用 */
    /* 判断设备是否可用 */
    static int xxx_open(struct inode *inode, struct file *filp)
    {
        if (!atomic_dec_and_test(&xxx_available)) {
            atomic_inc(&xxx_available);
            return - EBUSY; /* 已经打开, 不可用 */
        }
    ...
    }
    /* 在 release 函数中释放设备 */
    atomic_inc(&xxx_available); /* 释放设备 */


