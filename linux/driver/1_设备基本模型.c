Linux 设备模型

设备模型的几个主要任务:
  电源管理和系统关机：设备模型使得操作系统能够以正确的顺序遍历关闭系统硬件
  和用户空间通讯：sysfs虚拟文件系统的实现和设备模型密切相关，并向用户空间展现了系统的结构
  热拔插设备：
  设备类型：维护设备驱动的体系化数据机构
  对象的生命周期：设备模型的创建需要一系列的机制来处理对象的生命周期、对象之间的关系以及这些对象在用户空间的表示


device --> device_driver --> bus_type
                             class   

  系统中的任一设备在设备模型中都由一个 device 对象描述, 该对象用于
描述设备相关的信息,设备之间的层次关系,以及设备与总线、驱动的关系.

struct device {
	const char		*init_name;  //设备初始名
	struct bus_type	*bus;		 //设备所属总线
	struct device_driver *driver;//设备对应的驱动
	void		*driver_data;    //私有数据
	struct class		*class;  //设备所属分类
	dev_t			devt;	     //设备编号
	void	(*release)(struct device *dev);//remove设备调用
    ....
};

设备注册和注销:
int device_register(struct device *dev);//在/sys/devices下创建一个对应的目录
void device_unregister(struct device *dev); 
设置私有数据：
void dev_set_drvdata(struct device *dev, void *data);
获取私有数据：
void *dev_get_drvdata(const struct device *dev);

device_create() 
device_destroy()
get_device()
put_device()


=========================================================
系统中的每个驱动程序由一个 device_driver 对象描述

struct device_driver {
	const char		*name;
	struct bus_type		*bus;
	struct module		*owner;
	const char 		*mod_name;	/* used for built-in modules */

	int (*probe) (struct device *dev);//指向设备探测函数
	int (*remove) (struct device *dev);//指向设备移除函数
	void (*shutdown) (struct device *dev);//关闭设备
	int (*suspend) (struct device *dev, pm_message_t state); //挂起设备
	int (*resume) (struct device *dev);                      //恢复设备
	struct attribute_group **groups;
	struct pm_ops *pm;

	struct driver_private *p;
};

驱动注册和注销：
int driver_register(struct device_driver *drv);
void driver_unregister(struct device_driver *drv);

struct driver_private {
	struct kobject kobj;
	struct klist klist_devices;//设备链表
	struct klist_node knode_bus;
	struct module_kobject *mkobj;
	struct device_driver *driver;
};

==========================================================================
系统中总线由 struct bus_type 描述
每个bus_type对象都对应/sys/bus目录下的一个子目录,如PCI总线类型对应于/sys/bus/pci

struct bus_type {
	const char		*name;
	struct bus_attribute	*bus_attrs;//总线属性
	struct device_attribute	*dev_attrs;//设备属性
	struct driver_attribute	*drv_attrs;//驱动属性

	int (*match)(struct device *dev, struct device_driver *drv);
	int (*uevent)(struct device *dev, struct kobj_uevent_env *env);
	int (*probe)(struct device *dev);
	int (*remove)(struct device *dev);
	void (*shutdown)(struct device *dev);
	int (*suspend)(struct device *dev, pm_message_t state);
	int (*suspend_late)(struct device *dev, pm_message_t state);
	int (*resume_early)(struct device *dev);
	int (*resume)(struct device *dev);
	struct pm_ext_ops *pm;

	struct bus_type_private *p;
};

struct bus_type_private {
	struct klist klist_devices;//总线上的设备链表
	struct klist klist_drivers;//总线上的驱动链表
    ....
};

总线的注册和注销：
int bus_register(struct bus_type *bus);
void bus_unregister(struct bus_type *bus);

============================================================================
系统中的设备类由 struct class 描述,表示某一类设备。所有的 class 对象都属于
class_subsys 子系统,对应于sysfs文件系统中的/sys/class 目录。

struct class {
	const char		*name;
	struct module		*owner;

	struct class_attribute		*class_attrs;
	struct device_attribute		*dev_attrs;
	struct kobject			*dev_kobj;

	int (*dev_uevent)(struct device *dev, struct kobj_uevent_env *env);
	void (*class_release)(struct class *class);
	void (*dev_release)(struct device *dev);
	int (*suspend)(struct device *dev, pm_message_t state);
	int (*resume)(struct device *dev);
	struct pm_ops *pm;

	struct class_private *p;
};

class_register()
class_unregister()
class_create()  创建并注册
class_detroy()

============================================================================
属性
在bus 、device 、driver 和 class 层次上都分别定义了其属性结构体,包括
bus_attribute、driver_attribute、class_attribute、这几个结构体的定义
在本质是完全相的.

struct bus_attribute {
	struct attribute	attr;
	ssize_t (*show)(struct bus_type *bus, char *buf);
	ssize_t (*store)(struct bus_type *bus, const char *buf, size_t count);
};

struct driver_attribute {
	struct attribute attr;
	ssize_t (*show)(struct device_driver *driver, char *buf);
	ssize_t (*store)(struct device_driver *driver, const char *buf, size_t count);
};

struct class_attribute {
	struct attribute attr;
	ssize_t (*show)(struct class *class, char *buf);
	ssize_t (*store)(struct class *class, const char *buf, size_t count);
};

创建和初始化属性结构体
BUS_ATTR(_name,_mode,_show,_store)
DRIVER_ATTR(_name,_mode,_show,_store)
CLASS_ATTR(_name,_mode,_show,_store)

添加和删除 bus、driver、class 属性
bus_create_file() bus_remove_file()
driver_create_file() driver_remove_file()
class_create_file() driver_remove_file()

xxx_create_file()函数中会调用 sysfs_create_file(), 会创建 对应 的 sysfs 文件节点,
xxx_remove_file()函数中会调用 sysfs_remove_file(), 删除对应的 xxx 文件节点.

============================================================================
kobject --> kset
device --> device_driver --> bus_type
                             class   

kobject是隐藏在sysfs后的机制, sysfs中的每个目录在内核中存在一个对应的kobject

只要调用kobject_add函数, 就能在sysfs中显示kobject. kobject_add --> sysfs中创建目录
kobject->parent --> 在parent->name目录下创建目录; kobject->name --> 目录名

每个kobject 都输出一个或多个属性，属性在sysfs 中呈现为文件.
kobject经常被嵌入到其他结构.
kobject(以及包含它的结构)的存在需要创建它的模块存在. 当kobject继续被使用时不能卸载模块.
/* <linux/kobject.h> */
struct kobject {
	const char		*name;
	struct list_head	entry;  //用于挂接该kobject对象到kset链表
	struct kobject		*parent;//指向父对象的指针
	struct kset		*kset;      //所属kset的指针
	struct kobj_type	*ktype; //kobject类型
	struct sysfs_dirent	*sd;
	struct kref		kref;       //对象引用计数
	unsigned int state_initialized:1;
	unsigned int state_in_sysfs:1;
	unsigned int state_add_uevent_sent:1;
	unsigned int state_remove_uevent_sent:1;
};

初始化:
    必须先置0, memset(&kobj, 0, sizeof(struct kobject)); 
    void kobject_init(struct kobject *kobj);  //设置 kobject 的引用计数为1
    int kobject_set_name(struct koject *kobj, const char *format, ...); //sysfs入口中目录名, 检查返回值.
    需设置的成员: ktype kset parent

对引用计数的操作:
    struct kobject *kobject_get(struct kobject *kobj); //增加引用计数, 检查返回值
    void kobject_put(struct kobject *kobj);            //减少引用计数

    /* cdev结构中引用计数的实现 */
    struct kobject *cdev_get(struct cdev *p)
    {
        struct module *owner = p->owner;
        struct kobject *kobj;
        if (owner && !try_module_get(owner)) //先创建包含它的模块的引用
            return NULL;
        kobj = kobject_get(&p->kobj);
        if (!kobj)
            module_put(owner);
        return kobj;
    }
    当引用计数为 0 时,所有该对象使用的资源将被释放。

    /* 当最后一个引用计数不再存在时, 必须异步通知. */
    void my_object_release(struct kobject *kobj)
    {
        struct my_object *mine = container_of(kobj, struct my_object, kobj);
        kfree(mine);
    }
    每个 kobject 都必须有一个 release 方法, 并且 kobject 在该方法被调用前必须保持不变(处于稳定状态).

    kobject 的 ktype 成员是 kobj_type 指针, kset 也会提供 kobj_type 指针.
    struct kobj_type *get_ktype(struct kobject *kobj);

kobject_cleanup()  
kobject_add()      kobject_del()
kobject_register() kobject_unregister()

struct kobj_type
{
    void (*release)(struct kobject *);//release 函数
    struct sysfs_ops *sysfs_ops;      //属性操作
    struct attribute **default_attrs; //默认属性
};

struct attribute {
	const char		*name;
	struct module	*owner;
	mode_t			mode;
};

struct sysfs_ops {
	ssize_t	(*show)(struct kobject *, struct attribute *,char *);
	ssize_t	(*store)(struct kobject *,struct attribute *,const char *, size_t);
};

int sysfs_create_file(struct kobject *kobj, struct attribute *attr); //添加属性
int sysfs_remove_file(struct kobject *kobj, struct attribute *attr); //删除属性

二进制属性

符号链接: 驱动程序与其所管理的设备之间的关系
int sysfs_create_link(struct kobject *kobj, struct kobject *target, char *name);
void sysfs_remove_link(struct kobject *kobj, char *name);

热拔插事件

kobject 层次结构:
    1.parent 成员表示了上一层的节点, 最重要的用途是在 sysfs 分层结构中定位对象.


    2.kset 是具有相同类型的kobject的集合.
    一般, kobject->parent ---> kobject->kset->kobject
一旦设置了kset并把它添加到系统中, 将在sysfs中创建一个目录.
kset数据结构还内嵌了一个kobject对象kobj,所有属于这个kset的kobject对象的parent均指向这个内嵌的对象.
此外,kset还依赖于kobj维护引用计数, kset的引用计数实际上就是内嵌的kobject对象的引用计数.

kset 的名字为 kset->kobject->name
struct kset {
	struct list_head list;
	spinlock_t list_lock;
	struct kobject kobj;               //嵌入的 kobject
	struct kset_uevent_ops *uevent_ops;//事件操作集
};

将kobject添加到kset:
    1.kobject->kset --> kset
    2.int kobject_add(struct kobject *kobj);  //增加它的引用计数
extern int kobject_register(struct kobject *kobj) = kobject_init() + kobject_add()

void kobject_del(struct kobject *kobj);
kobject_unregister() = kobject_del() + kobject_put() 

/* 对kset中kobject调用 kobject_xxx() */
void kset_init(struct kset *kset);
int kset_add(struct kset *kset);
int kset_register(struct kset *kset);
void kset_unregister(struct kset *kset);
struct kset *kset_get(struct kset *kset);
void kset_put(struct kset *kset);


kobject被创建或删除时会产生事件(event),kobject 所属的kset将有机会过滤事件
或为用户空间添加信息。每个kset能支持一些特定的事件变量,在热插拔事件发生时,
kset的成员函数可以设置一些事件变量,这些变量将被导出到用户空间。

struct kset_uevent_ops {
	int (*filter)(struct kset *kset, struct kobject *kobj);//事件过滤
	const char *(*name)(struct kset *kset, struct kobject *kobj);
	int (*uevent)(struct kset *kset, struct kobject *kobj,
	struct kobj_uevent_env *env);//环境变量设置
};
filter()函数用于过滤掉不需要导出到用户空间的事件,
uevent()函数用于导出一些环境变量给用户的热插拔处理程序.

老接口:
    子系统, block_subsys devices_subsys 等
struct subsystem {
    struct kset kset;
    struct rw_semaphore rwsem;
};
    声明
decl_subsys(name, struct kobj_type *type, struct kset_hotplug_ops *hotplug_ops);

void subsystem_init(struct subsystem *subsys);
int subsystem_register(struct subsystem *subsys);
void subsystem_unregister(struct subsystem *subsys);
struct subsystem *subsys_get(struct subsystem *subsys)
void subsys_put(struct subsystem *subsys);


