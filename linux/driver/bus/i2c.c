

1. 简介

  I2C只有两条线,一条串行数据线:SDA,一条是时钟线SCL。I2C是一种多主机控制总线，
同一总线上可允许多个master. i2c总线适配器（adapter）就是一条i2c总线的控制器，
在物理连接上若干i2c设备。

    ---------------------
    |   字符设备        |
    ---------------------
    |                   |
    |  i2c-driver       |                       driver 层
    ---------------------
    | i2c-core          |  (i2c-core.c i2c.h)
    |                   |                       kernel 层 (i2c总线驱动)
    | i2c_algorithm     |  (algos/) 
    ---------------------
    |                   |
    | i2c-sunxi         |  (busses/)            soc 层
    |                   |
    ---------------------
    |  i2c适配器(多个)  |
    |                   |  hardware
    |   各种设备(多个)  |
    ---------------------

2. I2C 协议


3. linux I2C 

重要结构体：
  1. struct i2c_driver          //一套驱动方法, 可用于多个client
        id_table: struct i2c_device_id
      //回调函数：          
        *attach_adapter
        *detach_client
        *probe
        *remove

  2. struct i2c_client
    从设备：
        addr        低7位为芯片地址  
        *adapter    对应的i2c_adapter
        *driver     对应的i2c_driver 

  3. struct i2c_adapter
    主设备(适配器)：
        id                  algorithm的类型，定义于i2c-id.h,
        *algo               对应的i2c-algorithm
        *algo_data          algorithm数据
        bus_lock            控制并发访问的自旋锁
        retries             重试次数
        dev_released        用于同步
        userspace_clients   对应的所有从设备的链表头

  4. struct i2c_algorithm
        *master_xfer    I2C传输函数指针,       //在soc层中实现
        *smbus_xfer     smbus传输函数指针    
        *functionality  返回适配器支持的功能   //在soc层中实现

//master_xfer用于产生i2c访问周期需要的start stop ack信号，以i2c_msg（即i2c消息）为单位发送和接收通信数据。

  5. struct i2c_msg   
        __u16 addr;     /* slave address            */  
        __u16 flags;          
        __u16 len;      /* msg length               */  
        __u8 *buf;      /* pointer to msg data          */  

i2c-core为外部提供的核心函数

    EXPORT_SYMBOL(i2c_add_adapter);    //想增加适配器时，调用此函数, soc 层
    EXPORT_SYMBOL(i2c_del_adapter);    
    EXPORT_SYMBOL(i2c_register_driver);
    EXPORT_SYMBOL(i2c_del_driver);     

    EXPORT_SYMBOL(i2c_attach_client);  //从设备被探测关联时，调用此函数
    EXPORT_SYMBOL(i2c_detach_client);  //从设备被取消关联时，调用此函数

    EXPORT_SYMBOL(i2c_transfer);       //i2c传输，实现了core与adapter的联系
    调用i2c_adapter对应的i2c_algorithm，并使用i2c_algorithm的master_xfer()函数

    EXPORT_SYMBOL(i2c_master_send);
    EXPORT_SYMBOL(i2c_master_recv);

smbus command
    EXPORT_SYMBOL(i2c_smbus_read_byte);
    EXPORT_SYMBOL(i2c_smbus_read_byte_data);
    EXPORT_SYMBOL(i2c_smbus_read_word_data);
    EXPORT_SYMBOL(i2c_smbus_read_block_data);

---------------------------------------------------------------------------------------
4. I2C总线初始化

  postcore_initcall(i2c_init)
  i2c_init
    1. bus_register(&i2c_bus_type); -> 完成sysfs总线结构, /sys/bus/i2c/ 和 /sys/class/i2c-adapter/
    2. class_compat_register("i2c-adapter")
    3. i2c_add_driver(&dummy_driver); 空的设备驱动, 

--------------------------------------------------------------------------
driver层    Documentation/i2c/writing-clients

  1.提供I2C适配器的硬件驱动，探测，初始化I2C适配器(如申请I2C的I/O地址和中断号)，
驱动CPU控制的I2C适配器从硬件上产生。
　2.提供I2C控制的algorithm, 用具体适配器的xxx_xfer()函数填充i2c_algorithm的master_xfer指针，
并把i2c_algorithm指针赋给i2c_adapter的algo指针。
　3.实现I2C设备驱动中的i2c_driver接口，用具体yyy的yyy_probe()，yyy_remove()，yyy_suspend(),
yyy_resume()函数指针和i2c_device_id设备ID表赋给i2c_driver的probe,remove,suspend,resume和id_table指针。
  4.实现I2C设备所对应类型的具体驱动，i2c_driver只是实现设备与总线的挂接。


定义一个 static struct i2c_device_id
  static struct i2c_device_id foo_idtable[] = {
  	{ "foo", my_id_for_foo },
  	{ "bar", my_id_for_bar },
  	{ }
  };
MODULE_DEVICE_TABLE(i2c, foo_idtable);

定义一个 struct i2c_driver
  static struct i2c_driver foo_driver = {
  	.driver = {
  		.name	= "foo",
  	},
  
  	.id_table	= foo_idtable,
  	.probe		= foo_probe,
  	.remove		= foo_remove,
  	/* if device autodetection is needed: */
  	.class		= I2C_CLASS_SOMETHING,
  	.detect		= foo_detect,
  	.address_list	= normal_i2c,
  
  	.shutdown	= foo_shutdown,	/* optional */
  	.suspend	= foo_suspend,	/* optional */
  	.resume		= foo_resume,	/* optional */
  	.command	= foo_command,	/* optional, deprecated */
  }


module_init：
    i2c_add_driver

module_exit：
    i2c_del_driver
