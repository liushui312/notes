USB 驱动程序
    1.宿主(host)系统上的驱动程序
    2.设备(device)上的驱动程序 

主机侧: USB 设备驱动 <--> USB core <--> USB 主机控制器驱动 <--> USB 主机控制器 <--> USB总线

设备侧: Gadget API   <--> UDC驱动程序 <--> Gadget 驱动程序 <--> USB总线

                  |端点|
            | 接口|端点| <-- USB驱动程序
            |     |... |
       |配置|
       |    |     |端点|
       |    | 接口|端点| <-- USB驱动程序
       |      ... |... |
USB设备| 
       |          |端点|
       |    | 接口|端点| <-- USB驱动程序
       |    |     |... |
       |配置|
            |     |端点|
        ... | 接口|端点| <-- USB驱动程序
             ...  |... |
 
设备通常具有一个或更多的配置
配置经常具有一个或更多的接口
接口通常具有一个或更多的设置
接口没有或具有一个以上的端点
                  
====================================================================================
USB端点 endpoint

    | 1.控制端点
异步|    配置设备 获取设备信息 发送命令到设备 
    | 2.中断端点 //USB_ENDPOINT_XFER_INT
         固定速率传送少量数据的设备. 如, 键盘、鼠标.
    | 3.批量端点 //USB_ENDPOINT_XFER_BULK
周期|    传输大批量数据, 需要确保数据不丢失的设备. 如, 打印机、存储设备、网络设备.
    | 4.等时端点 //USB_ENDPOINT_XFERTYPE_ISOC
         传输大批量数据, 可以应付数据丢失的设备. 如, 音频、视频.
        
struct usb_host_endpoint {
	struct usb_endpoint_descriptor	desc;    /*端点信息*/
    ...
};
/* 端点描述符 */
struct usb_endpoint_descriptor {
	__u8  bLength;  //描述符长度
	__u8  bDescriptorType; //描述符类型

	__u8  bEndpointAddress; //端点地址, 0~3 位是端点号, 第7位是方向(0-OUT,1-IN) 
    __u8  bmAttributes;i    //端点类型, bit[0:1]为00表示控制,为01表示同步,为02表示批量,为03表示中断	
    __le16 wMaxPacketSize;  //一次可以处理的最大字节数
	__u8  bInterval;        //中断端点的中断间隔,此域值的范围为 1~255
                            //轮询数据传送端点的时间间隔
                            //对于同步传送的端点,此域必须为 1
                            //对于批量传送的端点以及控制传送的端点,此域忽略
    ...

} __attribute__ ((packed));

宏
USB_DIR_OUT 和 USB_DIR_IN
USB_ENDPOINT_XFERTYPE_MASK

========================================================================================
USB接口      
    1个USB驱动程序 --> 1个USB接口 <--> 1中USB逻辑连接(一个功能)
    USB设备 --> 多个USB接口 = 多个USB驱动程序

struct usb_interface {
    /* array of alternate settings for this interface, stored in no particular order */
	struct usb_host_interface *altsetting;

	struct usb_host_interface *cur_altsetting;	/* the currently active alternate setting */
	unsigned num_altsetting;	/* number of alternate settings */

	int minor;			/* minor number this interface is bound to */
    ...
};

struct usb_host_interface {
	struct usb_interface_descriptor	desc;
    ...
};
/* 接口描述符 */
struct usb_interface_descriptor
{
    _ _u8 bLength;  //描述符长度
    _ _u8 bDescriptorType; //描述符类型
    _ _u8 bInterfaceNumber; // 接口的编号
    _ _u8 bAlternateSetting; //备用的接口描述符编号
    _ _u8 bNumEndpoints; //该接口使用的端点数,不包括端点 0
    _ _u8 bInterfaceClass;//接口类型
    _ _u8 bInterfaceSubClass; //接口子类型
     _ _u8 bInterfaceProtocol; //接口所遵循的协议
     _ _u8 iInterface; //描述该接口的字符串索引值
} _ _attribute_ _ ((packed));

宏
struct usb_device *interface_to_usbdev(struct usb_interface *);

==============================================================================================
USB配置    
    一个USB设备 --> 多个配置

struct usb_host_config {
    ...
	struct usb_config_descriptor	desc;
    ...
};

/* 配置描述符 */
struct usb_config_descriptor
{
    _ _u8 bLength; //描述符长度
    _ _u8 bDescriptorType; //描述符类型编号

    _ _le16 wTotalLength; //配置所返回的所有数据的大小
    _ _u8 bNumInterfaces; // 配置所支持的接口数
    _ _u8 bConfigurationValue; //Set_Configuration 命令需要的参数值
    _ _u8 iConfiguration; //描述该配置的字符串的索引值
    _ _u8 bmAttributes; //供电模式的选择
    _ _u8 bMaxPower; //设备从总线提取的最大电流
} _ _attribute_ _ ((packed));

==============================================================================================
USB设备
    
struct usb_device {
    ...
	struct usb_device_descriptor descriptor;

	struct device dev;
    ...
};
//USB设备描述符
struct usb_device_descriptor
{
    _ _u8 bLength; //描述符长度
    _ _u8 bDescriptorType; //描述符类型编号

    _ _le16 bcdUSB; //USB 版本号
    _ _u8 bDeviceClass; //USB 分配的设备类 code
    _ _u8 bDeviceSubClass;// USB 分配的子类 code
    _ _u8 bDeviceProtocol; //USB 分配的协议 code
    _ _u8 bMaxPacketSize0; //endpoint0 最大包大小
    _ _le16 idVendor; //厂商编号
    _ _le16 idProduct; //产品编号
    _ _le16 bcdDevice; //设备出厂编号
    _ _u8 iManufacturer; //描述厂商字符串的索引
    _ _u8 iProduct; //描述产品字符串的索引
    _ _u8 iSerialNumber; //描述设备序列号字符串的索引
    _ _u8 bNumConfigurations; //可能的配置数量
} _ _attribute_ _ ((packed));

====================================================================================
USB 主机控制器
    OHCI：非PC系统上、带有SiS和ALi芯片组的PC主板上的USB芯片
    UHCI：大多数其他PC主板                               
    EHCI：由USB 2.0规范所提出, 它兼容于OHCI和UHCI

USB 主机控制器驱动
struct usb_hcd {
    ...
    const struct hc_driver *driver; /* 硬件特定的钩子函数 */
    ...
};

创建HCD
struct usb_hcd *usb_create_hcd (const struct hc_driver *driver,
                                struct device *dev, char *bus_name);
增加和移除HCD
int usb_add_hcd(struct usb_hcd *hcd, unsigned int irqnum, unsigned long irqflags);
void usb_remove_hcd(struct usb_hcd *hcd);

====================================================================================
Linux系统实现了几类通用的USB设备驱动 (主机侧)
    音频设备类 
    通信设备类 
    HID(人机接口)设备类 
    显示设备类 
    海量存储设备类 
    电源设备类 
    打印设备类
    集线器设备类

USB 设备号
    ACM USB 调制解调器的主设备号为166(默认设备名/dev/ttyACMn)
    USB 串口的主设备号为188(默认设备名/dev/ttyUSBn) 等

USB 设备文件系统 usbfs
    usbfs 动态跟踪总线上插入和移除的设备,通过它可以查看系统中USB设备的信息,包括拓扑、
    带宽、设备描述符信息、产品ID、字符串描述符、配置描述符、接口描述符、端点描述图符等

USB 设备和 USB 接口在 sysfs 中均表示为单独的 USB 设备. 
    USB sysfs设备命名方案:
        根集线器-集线器端口号(-集线器端口号...):配置编号.接口编号

struct usb_driver {
    const char *name; /* 驱动名称 */
    int (*probe) (struct usb_interface *intf, const struct usb_device_id *id); /*探测函数*/
    void (*disconnect) (struct usb_interface *intf); /*断开函数*/
    const struct usb_device_id *id_table; /* usb_device_id 表指针, 支持的 USB 设备列表*/
    ...
};

USB设备驱动程序
开始: probe()        设备被插入时被调用, 初始化软硬件资源, 如, 注册相应的字符、tty 等设备
结束: disconnect()   设备被拔出时被调用, 释放软硬件资源, 如, 注销相应的字符、tty 等设备

定义和初始化
struct usb_driver usb_storage_driver = {
	.owner =	THIS_MODULE,
	.name =		"xxx",
	.probe =	xxx_probe,
	.disconnect =	xxx_disconnect,
	.id_table =	xxx_usb_ids,
};

注册和销毁
int usb_register(struct usb_driver *new_driver);
void usb_deregister(struct usb_driver *driver);

usb_device_id 包含USB设备的制造商ID、产品ID、产品版本、设备类、接口类等信息及其要匹配标志成员 match_flags
struct usb_device_id {
	/* which fields to match against? */
	__u16		match_flags; //usb core 根据usb_device_id给设备寻找适合她的driver,给driver寻找适合他的device
                            
	/* Used for product specific matches; range is inclusive */
	__u16		idVendor;
	__u16		idProduct;
	__u16		bcdDevice_lo;
	__u16		bcdDevice_hi;

	/* Used for device class matches */
	__u8		bDeviceClass;
	__u8		bDeviceSubClass;
	__u8		bDeviceProtocol;

	/* Used for interface class matches */
	__u8		bInterfaceClass;
	__u8		bInterfaceSubClass;
	__u8		bInterfaceProtocol;

	/* not matched against */
	kernel_ulong_t	driver_info;
};

创建 usb_device_id 结构体的实例
    USB_DEVICE(vendor, product);
    USB_DEVICE_VER(vendor, product, lo, hi);
    USB_DEVICE_INFO(class, subclass, protocol);    //match_flags = USB_DEVICE_ID_MATCH_DEV_INFO
    USB_INTERFACE_INFO(class, subclass, protocol); //match_flags = USB_DEVICE_ID_MATCH_INT_INFO

USB设备驱动
    总线上挂载设备的驱动
    本身所属设备类型的驱动两部分

write()、read()、ioctl() 中与USB设备通信不再是I/O内存和I/O 端口的访问,而贯穿始终的是 USB请求块 urb


struct urb {
	/* private: usb core and host controller only fields in the urb */
	struct kref kref;		/* reference count of the URB */
	void *hcpriv;			/* private data for host controller */
	atomic_t use_count;		/* concurrent submissions counter */
	u8 reject;			    /* submissions will fail */
	int unlinked;			/* unlink error code */

	/* public: documented fields in the urb that can be used by drivers */
	struct list_head urb_list;	  /* list head for use by the urb's current owner */
	struct list_head anchor_list; /* the URB may be anchored */
	struct usb_anchor *anchor;
	struct usb_device *dev; 	  /* (in) pointer to associated device */
	struct usb_host_endpoint *ep; /* (internal) pointer to endpoint */
	unsigned int pipe;		      /* (in) pipe information */
	int status;			          /* (return) non-ISO status */
	unsigned int transfer_flags;  /* (in) URB_SHORT_NOT_OK | ...*/
	void *transfer_buffer;		  /* (in) associated data buffer */
	dma_addr_t transfer_dma;	  /* (in) dma addr for transfer_buffer */
	int transfer_buffer_length;	  /* (in) data buffer length */
	int actual_length;		      /* (return) actual transfer length */
	unsigned char *setup_packet;  /* (in) setup packet (control only) */
	dma_addr_t setup_dma;		  /* (in) dma addr for setup_packet */
	int start_frame;		      /* (modify) start frame (ISO) */
	int number_of_packets;		  /* (in) number of ISO packets */
	int interval;			      /* (modify) transfer interval (INT/ISO) */
	int error_count;		      /* (return) number of ISO errors */
	void *context;			      /* (in) context for completion */
	usb_complete_t complete;	  /* (in) completion routine */
	struct usb_iso_packet_descriptor iso_frame_desc[0]; /* (in) ISO ONLY */
};

//宏, 创建pipe(unsigned int), [13:7] devnum [17:14] endpoint, [6] 方向 
unsigned int usb_sndctrlpipe(struct usb_device *dev, unsigned int endpoint);/*控制OUT端点*/
unsigned int usb_rcvctrlpipe(struct usb_device *dev, unsigned int endpoint);/*控制IN端点*/
unsigned int usb_sndbulkpipe(struct usb_device *dev, unsigned int endpoint);/*批量OUT端点*/
unsigned int usb_rcvbulkpipe(struct usb_device *dev, unsigned int endpoint);/*批量IN端点*/
unsigned int usb_sndintpipe(struct usb_device *dev, unsigned int endpoint);/*中断OUT端点*/
unsigned int usb_rcvintpipe(struct usb_device *dev, unsigned int endpoint);/*中断IN端点*/
unsigned int usb_sndisocpipe(struct usb_device *dev, unsigned int endpoint);/*等时OUT端点*/
unsigned int usb_rcvisocpipe(struct usb_device *dev, unsigned int endpoint);/*等时IN端点*/

USB 设备中的每个端点都处理一个 urb 队列,在队列被清空之前,一个 urb 的典型生命周期
    被一个 USB 设备驱动创建
    初始化, 被安排给一个特定 USB 设备的特定端点
    被 USB 设备驱动提交给 USB 核心
    提交由 USB 核心指定的 USB 主机控制器驱动        |
    被 USB 主机控制器处理,进行一次到 USB 设备的传送 |由 USB 核心和主机控制器完成
    当 urb 完成, USB 主机控制器驱动通知 USB 设备驱动
        1.urb 被成功发送给设备, 并且设备返回正确的确认, urb->status 为 0 
        2.发送数据到设备或从设备接收数据时发生了错误, 
        3.urb 被从 USB 核心“去除连接”,这发生在驱动通过 usb_unlink_urb()或usb_kill_urb()
          取消 urb, 或 USB 设备被拔出的情况下.

创建和销毁urb
struct urb *usb_alloc_urb(int iso_packets, int mem_flags); //不能静态创建,  urb 被初始化为 0
    参数
      iso_packets   等时数据包的数量, 等时urb有用, 其他为0
      mem_flags     传递给kmalloc的flags
      
void usb_free_urb(struct urb *urb);

初始化urb, 被安排给一个特定 USB 设备的特定端点.
  中断urb
    void usb_fill_int_urb(struct urb *urb, struct usb_device *dev,
                          unsigned int pipe, void *transfer_buffer,
                          int buffer_length, usb_complete_t complete,
                          void *context, int interval);
    usb_sndintpipe()或usb_rcvintpipe()创建pipe

  批量urb
    void usb_fill_bulk_urb(struct urb *urb, struct usb_device *dev,
                           unsigned int pipe, void *transfer_buffer,
                           int buffer_length, usb_complete_t complete,
                           void *context);
    usb_sndbulkpipe()或usb_rcvbulkpipe()创建pipe

  控制urb
    void usb_fill_control_urb(struct urb *urb, struct usb_device *dev,
                              unsigned int pipe, unsigned char *setup_packet,
                              void *transfer_buffer, int buffer_length,
                              usb_complete_t complete, void *context);
    setup_packet 指向即将被发送到端点的设置数据包的数据.
    usb_sndctrlpipe()或usb_rcvictrlpipe()创建pipe.

  等时urb
    手动初始化urb
    
提交urb
   int usb_submit_urb(struct urb *urb, int mem_flags); 
    在提交urb到USB核心后, 直到完成函数被调用之前,不要访问 urb 中的任何成员
    flags
        GFP_ATOMIC
        GFP_NOIO
        GFP_KERNEL
========================================================================================
USB通信
1.发送控制命令
读usb 协议
struct usb_ctrlrequest {
	__u8 bRequestType;
	__u8 bRequest;
	__le16 wValue;
	__le16 wIndex;
	__le16 wLength;
} __attribute__ ((packed));


========================================================================================
不使用urb的USB传输
    1.创建一个USB批量urb, 把它发送到指定设备, 然后在返回调用者之前等待它的结束.
    int usb_bulk_msg(struct usb_device *usb_dev, unsigned int pipe,
                     void *data, int len, int *actual_length,
                     int timeout);
    成功返回0, actual_length 参数包含从该消息发送或接收的字节数, 失败返回一个负的错误值.

    2.
    int usb_control_msg(struct usb_device *dev, unsigned int pipe,
                        __u8 request, __u8 requesttype,
                        __u16 value, __u16 index,
                        void *data, __u16 size, int timeout);

