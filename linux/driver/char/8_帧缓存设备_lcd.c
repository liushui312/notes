Framebuffer(帧缓存)设备驱动

帧缓冲设备 --> /dev/fb*
    标准字符设备, 主设备号为29, 次设备号为0到31

驱动程序
  分配系统内存作为显存;
  实现 file_operations 结构中的接口, 为应用程序服务;
  实现 fb_ops 结构中的接口, 控制和操作 LCD 控制器;
  将显存的起始地址和长度传给 LCD 控制器的寄存器;

-----------------------------------------------------------------------------------------------------------
帧缓存的应用程序访问
    1. read/write /dev/fb  相当于读/写屏幕缓冲区
       cp  /dev/fb0 tmp    将当前屏幕的内容拷贝到一个文件中
       cp  tmp > /dev/fb0  则将图形文件tmp显示在屏幕上
    2. 映射(map)操作
       通过映射操作, 可将屏幕缓冲区的物理地址映射到用户空间的一段虚拟地址中, 之后用户就可以通过读写这段虚拟地址访问屏幕缓冲区, 在屏幕上绘图了. 而且若干个进程可以映射到同一显示缓冲区. 实际上, 使用帧缓冲设备的应用程序都是通过映射操作来显示图形的.
    3. I/O控制
       对于帧缓冲设备, 对设备文件的ioctl操作可读取/设置显示设备及屏幕参数, 如分辨率, 显示颜色数, 屏幕大小等.

    打开帧缓存设备 --> 获取屏幕参数 --> 计算显存长度 --> 映射 --> 读写操作

内核配置：
Device Drivers  --->
    Graphics support  ---> 
     <*> Support for frame buffer devices  --->
       <*>   S3C Framebuffer Support

-----------------------------------------------------------------------------------------------------------
帧缓冲的设备驱动结构

1. 应用程序与系统调用
2. 适用于所有设备的通用代码, 包括 file_operations 结构、framebuffer 接口等
3. 操作具体硬件的代码, 主要是 fs_ops 结构

/* inlcude/linux/fb.h */
数据结构
fb_info           包含当前 video card 的状态信息, 只有 fb_info 对内核可见; 
fb_var_screeninfo 描述了一种显卡显示模式的所有信息, 如宽、高、颜色深度等, 不同的显示模式对应不同的信息;
fb_fix_screeninfo 定义了显卡信息, 如 framebuffer 内存的起始地址, 地址长度等;
fb_cmap           设备无关的颜色表信息, 可以通过ioctl的 FBIOGETCMAP 和 FBIOPUTCMAP 命令读取或者设定颜色表;
fb_ops            应用程序使用 ioctl 系统调用操作底层的 LCD 硬件, fb_ops 结构中定义的方法用于支持这些操作;

struct fb_info {
	struct fb_var_screeninfo var;//可变参数
	struct fb_fix_screeninfo fix;//固定参数
    struct fb_cmap cmap;
	struct fb_ops *fbops;        //fb对应操作
	char __iomem *screen_base;	 //DMA内存虚拟地址
	unsigned long screen_size;	 //DMA内存大小
	void *par;                   //存放私有数据指针
    ....
};

struct fb_var_screeninfo {
	__u32 xres;			    //可视分辨率
	__u32 yres;
	__u32 xres_virtual;		//虚拟分辨率
	__u32 yres_virtual;
	__u32 xoffset;	        //可视分辨率与虚拟分辨率的偏移
	__u32 yoffset;		
	__u32 bits_per_pixel;   //每个像素点位数。
    //像素点中每种颜色的位数和偏移
	struct fb_bitfield red;	
	struct fb_bitfield green;
	struct fb_bitfield blue;
    //时序相关参数
	__u32 pixclock;		    //像素时钟
	__u32 left_margin;		/* time from sync to picture	*/
	__u32 right_margin;		/* time from picture to sync	*/
	__u32 upper_margin;		/* time from sync to picture	*/
	__u32 lower_margin;
	__u32 hsync_len;		/* length of horizontal sync	*/
	__u32 vsync_len;		/* length of vertical sync	*/
    ....
};

struct fb_fix_screeninfo {
	unsigned long smem_start;//DMA内存的物理地址
	__u32 smem_len;			 //DMA内存长度
	__u32 type;			    /* see FB_TYPE_*		*/
    ....
};

struct fb_ops {
	struct module *owner;//THIS_MODULE
    //检查用户设置是否有效
	int (*fb_check_var)(struct fb_var_screeninfo *var, struct fb_info *info);
    //LCD控制器的初始化
	int (*fb_set_par)(struct fb_info *info);
    //使用内核的实现
	void (*fb_fillrect) (struct fb_info *info, const struct fb_fillrect *rect);
	void (*fb_copyarea) (struct fb_info *info, const struct fb_copyarea *region);
	void (*fb_imageblit) (struct fb_info *info, const struct fb_image *image);
    ...
};

/* drivers/video/fbmem.c */
framebuffer驱动的接口
1. 提供给用户应用程序的接口
static const struct file_operations fb_fops = {
    .owner =            THIS_MODULE,
    .read =             fb_read,
    .write =            fb_write,
    .unlocked_ioctl =   fb_ioctl,
    .mmap =             fb_mmap,
    .open =             fb_open,
    .release =          fb_release,         
};    

2. 提供给底层设备驱动的接口
全局变量:
struct fb_info *registered_fb [FB_MAX];
int num_registered_fb; 
函数:
int register_framebuffer(struct fb_info *fb_info);
int unregister_framebuffer(struct fb_info *fb_info);
/* size, 私有数据大小, fb_info->par指向size内存. dev, fb所属设备，可以为NULL
   注意：实际分配内存为sizeof(struct fb_info)+size */
struct fb_info *framebuffer_alloc(size_t size, struct device *dev);
void framebuffer_release(struct fb_info *info);

/* handle 用来保存dma物理地址, 返回虚拟地址; cpu_addr 虚拟地址 */
void *dma_alloc_coherent(struct device *dev, size_t size, dma_addr_t *handle, gfp_t gfp);
void dma_free_coherent(struct device *dev, size_t size, void *cpu_addr, dma_addr_t handle);

底层驱动程序的工作基本上是填充 fb_info 结构, 然后注册它.
------------------------------------------------------------------------------------------------------
LCD驱动程序
1. 分配系统内存作显存 
    对于LCD controller 没有自己的显存, 
    对于带独立显存的显卡, 使用 request_mem_region 和 ioremap 将显卡外设内存映射到处理器虚拟地址空间.

2. 根据具体的硬件特性, 实现 fb_ops 的接口

ioctl 命令和 fb_ops 结构中的接口之间的关系如下所示:
    FBIOGET_VSCREENINFO fb_get_var
    FBIOPUT_VSCREENINFO fb_set_var
    FBIOGET_FSCREENINFO fb_get_fix
    FBIOPUTCMAP         fb_set_cmap
    FBIOGETCMAP         fb_get_cmap
    FBIOPAN_DISPLAY     fb_pan_display
只要我们实现了那些 fb_XXX 函数, 那么用户应用程序就可以使用 FBIOXXXX 宏来操作 LCD 硬件了.

platform 机制


