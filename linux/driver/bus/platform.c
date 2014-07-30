
在嵌入式SoC系统中集成的独立的外设控制器、挂接在SoC内存空间的外设等不依附于
PCI、USB、I2C、SPI等总线. 如, LCD、RTC、WDT等都被当作平台设备来处理(本身是字符型设备).

Linux 发明了一种虚拟的总线, 称为platform 总线
----------------------------------------------------------------------------------------------------
platform 总线下驱动的开发步骤是:
    1. 设备注册: 
        1）初始化 resource 结构体数组
        2）初始化 platform_device 结构变量
        3）向系统注册设备 platform_device_register()  module_init()
    2. 驱动注册: 
        1）初始化 platrorm_driver 结构变量
        2）向系统注册设备 platform_driver_register()  module_init()
        3）实现 xxx_probe() xxx_remove() 等函数

注意: 先设备注册再驱动注册. 

xxx_init() -> probe()

在内核初始化时:
do_basic_setup() -> driver_init() -> platform_bus_init() -> device_register()
                                                            bus_register()
设备注册的时候: 
platform_device_register() -> device_initialize()
                              platform_device_add()  
驱动注册时:
platform_driver_register() -> driver_register() -> bus_add_driver() -> driver_attach() -> bus_for_each_dev()
-> __driver_attach() -> driver_probe_device() -> drv->bus->match() -> platform_match() 
-> strncmp(pdev->name, drv->name, BUS_ID_SIZE), 如果相符就调用platform_drv_probe()->driver->probe(), 
如果probe成功则绑定该设备到该驱动. 

-------------------------------------------------------------------------------------------------------------
1. 数据结构
    platform_device
    platform_driver
    resource

struct platform_device {
	const char	* name;        //平台设备名
	int		id;                //设备id

	struct device	dev;       //父类
	u32		num_resources;     //资源数, = ARRAY_SIZE(arr), struct resource arr[]; 
	struct resource	* resource;//资源指针, = arr
};

eg:
    static struct platform_device sunxi_codec_pcm_device = {
        .name = "sunxi-pcm-codec-audio",
        .id = -1,
    };

struct resource {
	resource_size_t start;//资源起始值
	resource_size_t end;  //资源结束值
	const char *name;     //资源名
	unsigned long flags;  //资源类型, 如IORESOURCE_MEM(内存),IORESOURCE_IRQ(中断)
    ....
};

struct platform_driver {
	int (*probe)(struct platform_device *); //探测函数
	int (*remove)(struct platform_device *);//删除函数
	struct device_driver driver;            //父类
    ....
};

eg:
    static struct platform_driver sunxi_codec_pcm_driver = {
        .probe = sunxi_codec_pcm_probe,
        .remove = __exit_p(sunxi_codec_pcm_remove),
        .driver = {               
            .name = "sunxi-pcm-codec-audio",
            .owner = THIS_MODULE,
        },
    };


2. 函数接口
#include <linux/platform_device.h>
int platform_device_register(struct platform_device *pdev);
int platform_driver_register(struct platform_driver *drv);
void platform_device_unregister(struct platform_device *pdev);
void platform_driver_unregister(struct platform_driver *drv);

/* 获取资源; type, 资源类型; num, 某种类型中的资源编号 */
struct resource *platform_get_resource(struct platform_device *dev, unsigned int type, unsigned int num);
struct resource *platform_get_resource_byname(struct platform_device *dev, unsigned int type, char *name);

int platform_get_irq(struct platform_device *dev, unsigned int num);

/* 宏, pdev->dev->driver_data = data */
void platform_set_drvdata(struct platform_device *pdev, void *data);

eg: remove函数中：
        platform_set_drvdata(pdev, NULL);
