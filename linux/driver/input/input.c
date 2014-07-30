http://blog.csdn.net/ielife/article/details/7798952

linux输入子系统
    1. 输入子系统事件处理层（EventHandler）

        gsensor 周期性汇报即可  -> 轮询  -> input-polldev.c
        TP      需立即响应      -> 中断

    2. 输入子系统核心层（InputCore）

    3. 输入子系统设备驱动层
      主要实现对硬件设备的读写访问，中断设置，并把硬件产生的事件转换为核心层定义的规范提交给事件处理层。

include/linux/input.h
重要结构体
    1）struct input_event
         time:  struct timeval
         type:  __u16 
         code:  __u16 
         value: __s32

    2）struct input_dev      // input device

        node: struct list_head   // input_dev list 


    3）struct input_handler  // interfaces of input devices
        id_table:  struct input_device_id
        node:      struct list_head     //handler list, eg. evdev_handler, mousedev_handler
        h_node:    struct list_head
       //回调函数
        *connect            //called when attaching a handler to an input device
        *disconnect
        *filter
        *event
        *start

    4）struct input_handle  // links input device with an input handler
        input_dev:
        input_handler:

        d_node: struct list_head
        h_node: struct list_head

----------------------------------------------------------------------------------------
input-core 为外部提供的核心函数 (input.c)

    EXPORT_SYMBOL(input_allocate_device); // 使用宏 input_add_device
    EXPORT_SYMBOL(input_free_device);

    EXPORT_SYMBOL(input_register_device);
    EXPORT_SYMBOL(input_unregister_device);

    EXPORT_SYMBOL(input_register_handler);
    EXPORT_SYMBOL(input_unregister_handler);

    EXPORT_SYMBOL(input_handler_for_each_handle);

    EXPORT_SYMBOL(input_event);         //报告事件，使用以下宏函数
      void input_report_key(struct input_dev *dev, unsigned int code, int value);
      void input_report_abs(struct input_dev *dev, unsigned int code, int value)
      void input_report_rel(struct input_dev *dev, unsigned int code, int value)
      void input_sync(struct input_dev *dev)

    EXPORT_SYMBOL(input_set_abs_params);

----------------------------------------------------------------------------------------
input-polldev.h    轮询方式报告事件
input-polldev.c

struct input_polled_dev {
    void *private;

    void (*open)(struct input_polled_dev *dev);
    void (*close)(struct input_polled_dev *dev);
    void (*poll)(struct input_polled_dev *dev);
    unsigned int poll_interval;     /* msec */
    unsigned int poll_interval_max; /* msec */
    unsigned int poll_interval_min; /* msec */

    struct input_dev *input;

    /* private: */
    struct delayed_work work;
};

struct input_polled_dev *input_allocate_polled_device(void);
void input_free_polled_device(struct input_polled_dev *dev);
int input_register_polled_device(struct input_polled_dev *dev);
void input_unregister_polled_device(struct input_polled_dev *dev);

-----------------------------------------------------------------------------------------
调试
    getevent sendevent 是android系统下的一个工具，可以模拟多种按键和触屏操作
  产生的是raw event，raw event经过event hub处理产生最终的gesture事件。
  
    源码：system/core/toolbox/ sendevent.c getevent.c
  
  getevent
    查看所有/dev/input/event* 及对应驱动名字
    监控当前的事件(鼠标事件,按键事件,拖动滑动等)
    
    type 键码 最后一个根据type不同而不同

  sendevent

-----------------------------------------------------------------------------------------------------------
触摸平驱动
linux-2.6.28/drivers/input/touchscreen/s3c-ts.c
linux-2.6.28/arch/arm/plat-s3c/dev-ts.c
linux-2.6.28/arch/arm/mach-s3c6410/mach-smdk6410.c

驱动配置
 Device Drivers  ---> 
    Input device support  --->
        [*]   Touchscreens  --->
             < >   S3C touchscreen driver
                
tslib移植
# ./autogen-clean.sh
# ./autogen.sh 
# ./configure --host=arm-linux --prefix=/nfsroot
# make
    ts_test.c:(.text+0x1d4): undefined reference to `rpl_malloc'
修改config.h
    加入：#define malloc rpl_malloc
# make
# make install
# vim /etc/ts.conf
    打开module_raw input，去掉前边的空格，不然段错误

配置环境变量：
export TSLIB_TSDEVICE=/dev/input/event1
export TSLIB_CALIBFILE=/etc/pointercal
export TSLIB_CONFFILE=/etc/ts.conf
export TSLIB_PLUGINDIR=/lib/ts
export TSLIB_CONSOLEDEVICE=none
export TSLIB_FBDEVICE=/dev/fb0
export LD_LIBRARY_PATH=/lib

# ts_calibrate
    生成校准参数/etc/pointercal
