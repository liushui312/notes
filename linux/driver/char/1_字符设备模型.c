app --> /dev/xxx --> file   --> filp 
                                private_data
                     inode  --> dev_t

app --> open/read/write  --> swi --> sys_open (ISR) --> VFS 
--> chrdev[major] --> file_operations --> open/read/write 

--------------------------------------------------------------------------
1. 定义设备结构体:  cdev、私有数据及信号量等
2. 实现file_operations 结构体中成员函数
    open    打开设备: inode --> container_of --> 设备结构体 --> 
                      filp->private_data 
                      inode --> iminor --> minor --> 确保真正要打开的设备
            申请IO内存及映射内存 request_mem_region() ioremap()
            实现硬件初始化 
    release 释放由open 分配的、filp->private_data 中的数据
            释放IO内存及取消映射关系
    read    copy_to_user
    write   copy_from_user
    ioctl   
3. 定义 file_operations 结构体
4. 模块加载: static int __init xxx_init (void)  
    设备号的申请 alloc_chrdev_region() 或 resgister_chrdev_region()
    cdev 的注册  cdev_init() cdev_add() 
    分配设备结构体，并初始化 kmalloc
5. 模块卸载: static void __exit xxx_exit (void) 
    cdev 的注销  cdev_del() 
    设备号的释放 unregister_chrdev_region() 
    释放设备结构体  kfree

使用cdev结构体描述字符设备
struct cdev {
	struct kobject kobj;    /* 内嵌的 kobject 对象 */
	struct module *owner;   /*所属模块, THIS_MODULE*/
	const struct file_operations *ops;
	struct list_head list;
	dev_t dev;              /*设备编号，主设备号12位，次设备号20位*/
	unsigned int count;     /*设备编号数*/
};

MAJOR(dev_t dev)
MINOR(dev_t dev)
MKDEV(int major, int minor)

//用于初始化 cdev 的成员,并建立 cdev 和 file_operations 之间的连接
void cdev_init(struct cdev *, const struct file_operations *);
//用于动态申请一个 cdev 内存
struct cdev *cdev_alloc(void);
//添加设备,一个驱动可添加多个设备
int cdev_add(struct cdev *, dev_t, unsigned); 
void cdev_del(struct cdev *);
void cdev_put(struct cdev *p);
void cd_forget(struct inode *);

//在调用cdev_add()函数向系统注册字符设备之前, 应首先向系统申请设备号
int alloc_chrdev_region(dev_t *dev, unsigned baseminor, 
        unsigned count, const char *name);
参数：
    dev，用来存设备编号
    baseminor，第一个次设备号
    count，连续的次设备号数
    name，设备或驱动名
返回值：
    成功返回0，失败返回负的错误码
int register_chrdev_region(dev_t from, unsigned count, const char *name);
参数：
    from，申请的第一个设备号
    count，连续的设备号数
    name，设备名
返回值：
    成功返回0，失败返回负的错误码
//在调用cdev_del()函数从系统注销字符设备之后, 应该释放原先申请的设备号
void unregister_chrdev_region(dev_t from, unsigned count);


unsigned long copy_from_user(void *to, const void __user *from, 
        unsigned long count);
unsigned long copy_to_user(void __user *to, const void *from, 
        unsigned long count);

老接口
注册字符设备:
int register_chrdev(unsigned int major, const char *name, const struct file_operations *fops)
参数:
    major，主设备号 name, 设备名 fops, 设备对应操作
返回值：
       if major > 0 指定major
        成功返回0，失败返回错误码
       if major = 0 分配major
        成功返回分配的major，失败返回错误码
注销设备：
void unregister_chrdev(unsigned int major, const char *name)
参数:
    major，主设备号 name, 设备名

============================================================================
//文件描述符：每个打开的文件在内部用 file 结构描述
//单个文件可能被打开多次, 有多个文件描述符 file 结构
//linux/fs.h

struct file {
	const struct file_operations	*f_op;//文件对应的操作
	void			*private_data;//存放私有数据
    ....
};

============================================================================
//一个文件对应一个 inode 结构 linux/fs.h
//根据inode 结构取得设备编号: 
unsigned int iminor(struct inode *inode)
unsigned int imajor(struct inode *inode)

struct inode {
	dev_t			i_rdev;     //设备编号
	union {
		struct cdev		*i_cdev;//指向当前字符设备的指针
	};
    ....
};

============================================================================
file_operations 定义了字符设备驱动提供给虚拟文件系统的接口函数。

struct file_operations 
open:int (*) (struct inode *, struct file *);
release:int (*) (struct inode *, struct file *);
read:ssize_t (*) (struct file *, char __user *, size_t, loff_t *);
write:ssize_t (*) (struct file *, const char __user *, size_t, loff_t *);
ioctl:int (*) (struct inode *, struct file *, unsigned int, unsigned long);
mmap:int (*) (struct file *, struct vm_area_struct *);
llseek: loff_t (*) (struct file *, loff_t, int);
