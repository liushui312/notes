摘自 http://linux.chinaitlab.com/soft/878567.html

linux kernel的内存管理子系统非常复杂，为了深入了解内存管理系统，我打算分多篇文章来分析linux内存管理。本文就谈谈kernel如何收集物理内存的地址空间和大小等信息。
 
嵌入式arm处理器与我们平时接触到的intel处理器有点不一样，intel处理器可以通过主板或者BIOS代码来自动检测物理内存的大小。但arm处理器下的嵌入式系统没有这么幸运了，它必须由bootloader手工（或硬编码）的方式来获知kernel板上物理内存的开始地址和大小。
 
实际上，除了物理内存信息外，命令行参数，视频卡信息和randisk信息，都是通过bootloader一一告知kernel的。Bootloader和 kernel之间必须有某种约定，才能方便让两者之间传递信息。实事上，arm-kernel约定bootloader按如下的要求来存放这些信息：
 
1.在bootloader跳到kernel执行时，r2寄存器值为存放这些信息的首地址。
2.上述各子信息必须按一定的格式来组合起来。
 
在arm-kernel里面，每个子信息项称为atag.  Struct tag的定义如下：
 
1. struct tag {   
2.     struct tag_header hdr;   
3.     union {   
4.         struct tag_core     core;   
5.         struct tag_mem32    mem;   
6.         struct tag_videotext    videotext;   
7.         struct tag_ramdisk  ramdisk;   
8.         struct tag_initrd   initrd;   
9.         struct tag_serialnr serialnr;   
10.        struct tag_revision revision;   
11.        struct tag_videolfb videolfb;   
12.        struct tag_cmdline  cmdline;   
13.        struct tag_acorn    acorn;   
14.        struct tag_memclk   memclk;   
15.     } u;   
16. };  

 
struct tag_header hdr类似于头部标识的作用，它用于标识后面的union的类型，以及此种结构占用内存的大小。 后面union表示，每个tag可以是上面几种信息其中的一种。

  
| hdr  | Core  |  hdr | mem32  |  hdr | Cmdline  | ... | hdr(.size=0)  | 
|-->core tag<--|-->mem32 tag<--|-->cmdline tag<--|     |--> end tag <--|


上图就是各种tag组合成tags的一个实例。Hdr里面记录后的是哪种tag，整个tag的长度是多少，这样很方跳到下一个tag去。Hdr的类型是struct tag_header，定义如下：
 
1. struct tag_header {   
2.     __u32 size;   
3.     __u32 tag;   
4. };  

为了方便分析，假定kernel已经知道了tags的地址，那么它调用parse_tags函数来对各个tags进行分析，代码如下：

1. [arch/arm/kernel/setup.c]   
2. static void __init parse_tags(const struct tag *t)   
3. {   
4.     for (; t->hdr.size; t = tag_next(t))   
5.         if (!parse_tag(t))   
6.             printk(KERN_WARNING   
7.                 "Ignoring unrecognised tag 0x%08x\n",   
8.                 t->hdr.tag);   
9. };  

 
Tags 假定，在排好各个tag之后，后面要跟一个hdr.size为0的空tag。所以在for中，利用t->hdr.size 为0作为结束条件。而t = tag_next(t) 就是利用t->hdr.size的大小跳到下一个tag的。tag_next定义如下：
 
#define tag_next(t) ((struct tag *)((__u32 *)(t) + (t)->hdr.size))
 
而parse_tag函数就对t所指向的tag进行分析，代码如下：

1. static int __init parse_tag(const struct tag *tag)   
2. {   
3.     extern struct tagtable __tagtable_begin, __tagtable_end;   
4.     struct tagtable *t;   
5.    
6.     for (t = &__tagtable_begin; t < &__tagtable_end; t++)   
7.         if (tag->hdr.tag == t->tag) {   
8.             t->parse(tag);   
9.             break;   
10.         }   
11.    
12.     return t < &__tagtable_end;   
13. }  

这里又使用了kernel惯用技巧，就是把各种atag关联的处理函数结构定义到一个特殊的section里面，然后在链接时，__tagtable_begin就是该section的开始地址，而__tagtable_end则是它的结束地址。从parse_tag函数可以使用，编译后该section就是一个struct tagtable数组，类型tagtable定义如下：
 
1. struct tagtable {   
2.     __u32 tag;   
3.     int (*parse)(const struct tag *);   
4. };  

 
tag 是它所描述的tag类型，而parse则就此种类型atag的分析处理函数。结合parse_tag函数可知，tagtable数组就是定义好各种 atag的处理函数，当bootloader将atags传递到kernel里，kernel依次检查各atag的类型，然后查表（tagtable）调用它的处理函数。
 实际上，bootloader向kernel传递的atags种类较多，它们各有各的用途，本文主要分析kernel是如何收集物理内存信息的。因此，我们只关心类型为ATAG_MEM的tag。我们先看ATAG_MEM类型的tagtable定义：

1. #define __tag __used __attribute__((__section__(".taglist.init")))   
2. #define __tagtable(tag, fn) \   
3. static struct tagtable __tagtable_##fn __tag = { tag, fn }   
4.    
5. __tagtable(ATAG_MEM, parse_tag_mem32);  

 
从这种tagtable的定义可以看出，ATAG_MEM这种tag的处理函数是parse_tag_mem32，它的代码如下：

1. static int __init parse_tag_mem32(const struct tag *tag)   
2. {   
3.     return arm_add_memory(tag->u.mem.start, tag->u.mem.size);   
4. }  

 
当tag->hdr.tag 为ATAG_MEM时，tag.u的数据为mem成员有效，而mem成员是下面这种类型的：

1. struct tag_mem32 {   
2.     __u32   size;   
3.     __u32   start;  /* physical start address */   
4. };  
 
故 parse_tag_mem32函数，将物理内存的信息传递并调用arm_add_memory函数。在arm-kernel的启动阶段，它使用一个叫 meminfo的数据结构来记录系统的所有物理内存。在arm系统架构中，SDRAM是连接到arm的bank里的，即物理内存是分bank的。故 meminfo的类型，也是类似的，它的定义如下：
 
1. struct membank {   
2.     unsigned long start;   
3.     unsigned long size;   
4.     int           node;   
5. };   
6.    
7. struct meminfo {   
8.     int nr_banks;   
9.     struct membank bank[NR_BANKS];   
10. };  

 
Nr_banks记录当前meminfo已存放多少个bank，而bank[i]成员则记录第i个bank内存的开始地址和大小。
毫无疑问，arm_add_memory函数的工作就是它atags里面的物理内存信息增加到meminfo结构是，代码如下：
 
1. static int __init arm_add_memory(unsigned long start, unsigned long size)   
2. {   
3.     struct membank *bank = &meminfo.bank[meminfo.nr_banks];   
4.    
5.     if (meminfo.nr_banks >= NR_BANKS) {   
6.         printk(KERN_CRIT "NR_BANKS too low, "   
7.             "ignoring memory at %#lx\n", start);   
8.         return -EINVAL;   
9.     }   
10.    
11.     /* 使start和size均为PAGE_SIZE(即4K) 的倍数  
12.      * 其中start以4K向上取整，而size则以4K向下取整  
13.      */   
14.     size -= start & ~PAGE_MASK;   
15.     bank->start = PAGE_ALIGN(start);   
16.     bank->size  = size & PAGE_MASK;   
17.     bank->node  = PHYS_TO_NID(start);   
18.    
19.     /*  
20.      * Check whether this memory region has non-zero size or  
21.      * invalid node number.  
22.      */   
23.     if (bank->size == 0 || bank->node >= MAX_NUMNODES)   
24.         return -EINVAL;   
25.    
26.     meminfo.nr_banks++;   
27.     return 0;   
28. }  

 
当处理完所有的atags后，meminfo数据结构所描述的信息，就是个开发板上的物理内存信息。为了获得更直观的运行结果，我在arm_add_memory函数添加了如下的打印代码：
Printk(KERN_ERR “add memory bank(%d) 0x%08lx-0x%08lx\n”,
Meminfo.nr_banks, bank->start, bank->start + bank->size);
 
便可获知开发板上的物理内存情况了。就天嵌的s3c2440开板来说，它的SDRAM是接到bank6里面，故它的起始地址是0x3000000，大小是64M，它的结束地址是0x32000000。
 
尽管知道parse_tag是用于收到物理内存信息的，但仍不知道它是在何处被调用的。其实它是在启动阶段进行收集的，它的调用关系如下：
Start_kernel() -> setup_arch() -> parse_tags()
 
当执行完parse_tags函数，那么meminfo结构就收集完了物理内存信息。接下来的事情就是建立最终的内核空间的页表，以及建立临时的内存管理系统和初始化所有的物理页。

