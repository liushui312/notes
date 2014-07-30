内存管理

0～4G 虚拟地址空间
    0~3G  用户空间
    3G~4G 内核空间

物理内存： 
    ZONE_DMA    3G~(3G+16M)         可用于DMA的内存|
    ZONE_NORMAL (3G+16M)~(3G+896M)  常规内存       | 低端内存，直接映射
    ZONE_HIGHMEM (3G+896M)~4G       高端内存，动态映射

kmalloc
    低端内存分配，物理地址连续
    --> 内存区段的列表
void *kmalloc(size_t size, int flags);
    flags参数    
             GFP_KERNEL     get_free_pages(), 可能睡眠.
             GFP_ATOMIC     原子分配, 用于中断处理函数或进程上下文之外
             GFP_USER       用于用户空间
             GFP_HIGHUSER   高端内存
            |GFP_NOIO       不允许I/O初始化
类GFP_KERNEL|GFP_NOFS       不允许文件系统调用

默认:常规区段   __GFP_DMA      只有DMA区段会被搜索      
     和DMA区段  __GFP_HIGHMEM  三个区段都会被搜索, 注意kmalloc不能分配高端内存 
                __GFP_COLD

    size参数
        内核只能分配一些预定义的、固定大小的字节数组
        最小内存块
        最大内存块

void kfree(const void *objp)

vmalloc
    高端内存分配, 不能保证物理地址连续, 分配大块内存
void *vmalloc(unsigned long size)
void vfree(const void *addr)

页分配和释放
struct page * alloc_pages(gfp_t gfp_mask, unsigned int order);
参数:
    gfp_mask，同kmalloc 的 flags
    order，分配2^order个页
返回值:
    页描述符指针

void __free_pages(struct page *page, unsigned int order);

void *page_address(struct page *); //取页的虚拟地址

unsigned long virt_to_phys(void *x);//取物理地址
void *phys_to_virt(unsigned long x);//取虚拟地址
注意：只用于低端内存

分配和释放单个页：
alloc_page/__free_page

unsigned long __get_free_pages(gfp_t gfp_mask, unsigned int order)
返回值:页的虚拟地址

void free_pages(unsigned long addr, unsigned int order)

alloc_pages/__free_pages
alloc_page/__free_page
__get_free_pages/free_pages
__get_free_page/free_page

伙伴系统算法
    减少外部碎片，分配的最小单位是页

SLAB分配器(后备高速缓存)

创建cache
struct kmem_cache * kmem_cache_create (const char *name, size_t size, size_t align, 
                                        unsigned long flags, void (*ctor)(void *))
参数:
    name，cache名
    size，cache中对象的大小
    align，对齐，一般为0，标准对齐
    flags，一般为0或SLAB_HWCACHE_ALIGN(Cache line对齐)
    ctor，构造函数，分配slab对其中每一个对象执行该函数

删除cache
void kmem_cache_destroy(struct kmem_cache *cachep)

分配对象
void *kmem_cache_alloc(struct kmem_cache *cachep, gfp_t flags)
参数：
    cachep，从哪个cache分配
    flags，同kmalloc
返回值：
    成功返回内存指针，失败返回NULL

释放对象
void kmem_cache_free(struct kmem_cache *cachep, void *objp)
参数：
    cachep，cache指针
    objp，对象指针

unsigned int kmem_cache_size(struct kmem_cache *cachep) //获得cache大小
char *kmem_cache_name(struct kmem_cache *cachep)        //获得cache名


