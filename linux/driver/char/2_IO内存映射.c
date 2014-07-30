I/O内存映射  //arch/arm/include/asm/io.h

申请I/O内存  
struct resource * request_mem_region(resource_size_t start, resource_size_t n, const char *name)
参数：
    start,物理地址
    n，大小
    name, 名字 
        cat /proc/iomem
返回值：
    成功返回资源指针，失败返回NULL

映射:
void __iomem * ioremap(unsigned long phys_addr, size_t size)
参数：
    phys_addr, 物理地址
    size, 大小
返回值：
    映射的虚拟地址

取消映射：
void iounmap(volatile void __iomem *io_addr)

void release_mem_region(resource_size_t start, resource_size_t n)
参数：
    start, 起始物理地址
    n, 大小

io内存操作：
    unsigned int ioread8(void __iomem *addr)
    unsigned int ioread16(void __iomem *addr)
    unsigned int ioread32(void __iomem *addr)

    void iowrite8(u8 val, void __iomem *addr)
    void iowrite16(u16 val, void __iomem *addr)
    void iowrite32(u32 val, void __iomem *addr)
