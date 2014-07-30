#include <stdio.h>                                                        

声明的类型：
  size_t
  FILE
  fpos_t

声明的宏：
  EOF          文件的结尾
  NULL
  _IOFBF
  _IOLBF
  _IONBF
  BUFSIZ
  FOPEN_MAX       最多可打开的文件数
  FILENAME_MAX    文件名的最大长度
  L_tmpnam
  SEEK_CUR
  SEEK_END
  SEEK_SET



声明的函数：
1.文件操作
FILE *fopen(const char *path, const char *mode);
FILE *fdopen(int fd, const char *mode);
//打开文件，并使该文件与流 stream (stdin ...)相关联
FILE *freopen(const char *path, const char *mode, FILE *stream);

mode:  
  r       只读
  r+      可读可写
  w       只写, 文件不存在会创建,文件存在会清空
  w+      比w功能多一个可读功能
  a       追加,文件不存会创建
  a+      比a功能多一个可读功能
注意：
  1.二进制文件，加b，如rb r+b
  2.带+号同时可读可写，交替时，必须调用 fflush()或文件定位函数
  如fseek()、fsetpos()、rewind()等

返回: 成功为 FILE 指针,失败为 NULL

int fclose(FILE *fp);
//对输出流(写打开),用于将已写到缓冲区但尚未写出的全部数据都写到文件中;
//对输入流,其结果未定义。如果写过程中发生错误则返回 EOF,正常则返回 0。
int fflush(FILE *stream);

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);

nmemb: 几个数据块
size: 每个数据块的大小

返回，成功读取或写入几个数据块

//删除文件 filename
int remove(const char *filename);
返回:成功为 0,失败为非 0 值
int rename(const char *oldfname, const char *newfname);
返回:成功为 0,失败为非 0 值

2.格式化输出
int fprintf(FILE *stream, const char *format,...);
返回:成功为实际写出的字符数,出错返回负值

int printf(const char *format, ...);

int sprintf(char *buf, const char *format, ...);
返回:实际写到字符数组的字符数,不包括'\0'

int snprintf(char *buf, size_t num, const char *format, ...);
---------------------------------------------------------------------------
#include <string.h>

strcmp
strchr
strstr
snprintf
strcasecmp
strdup
功能: 将字符串拷贝到新建的位置处
strsep
char *strpbrk(const char *s, const char *accept);
---------------------------------------------------------------------------
#include <signal.h>

信号是程序执行过程中发生的异常事件。
  同步信号:程序自身的某些动作产生的，如除零
  异步信号:程序外部产生

处理信号：
  1.默认处理，终止程序
  2.信号忽略
  3.信号处理，把控制权转移给一个指定的函数

函数raise  报告一个同步信号
函数signal 指定一种信号的处理方法

struct sigaction
  对于内核头文件而言，struct sigaction 结构体定义在kernel/include/asm/signal.h,此头文件又被kernel/include/linux/signal.h包含。
  对于用户空间的头文件而言，struct sigaction定义在 /usr/include/bits/sigaction.h,此头文件又被/usr/include/signal.h包含，所以应用程序中如果用到此结构，只要#include <signal.h>即可。注意内核中的定义和应用程序中的定义是不一样的，内核空间的sigaction结构只支持函数类型为 __sighandler_t的信号处理函数，不能处理信号传递的额外信息。
  sa_mask: __sigset_t
  sa_flags: int
  sa_handler: __sighandler_t

int sigaction(int signum, const struct sigaction *act,
                     struct sigaction *oldact);

信号设置
int sigemptyset(sigset_t *set);
int sigfillset(sigset_t *set);
int sigaddset(sigset_t *set, int signum);
int sigdelset(sigset_t *set, int signum);
//tests whether signum is a member of set.
int sigismember(const sigset_t *set, int signum); 

typedef struct {
unsigned long sig[_NSIG_WORDS]；
} sigset_t
信号集用来描述信号的集合，linux所支持的所有信号可以全部或部分的出现在信号集中，主要与信号阻塞相关函数配合使用。


--------------------------------------------------------------------------
#include <unistd.h>

int getopt(int argc, char * const argv[], const char* optstring)；

#include <getopt.h>
支持长选项
int getopt_long(int argc, char * const argv[], const char *optstring, 
        const struct option *longopts, int *longindex);

int getopt_long_only(int argc, char * const argv[],const char *optstring,
        const struct option *longopts, int *longindex);

2.
参数argc和argv是由main()传递的参数个数和内容，argc计算参数的个数是以空格为分隔符的；
参数optstring则表示欲处理的选项字符串，optstring选项字符串可能有以下三种格式的字符：
1.单个字符，表示选项
2.单个字符后接一个冒号：表示该选项后必须跟一个参数。参数紧跟在选项后或者以空格隔开。该参数的指针赋给optarg。
3 单个字符后跟两个冒号：表示该选项后必须跟一个参数。参数必须紧跟在选项后不能以空格隔开。该参数的指针赋给optarg。
举例：optstring="a:b::cd"，表示选项a还跟有参数，可能以空格隔开，选项b后还跟有参数，
                           直接接在选项后面，选项c,d均无参数。
补充一点，该函数判断是选项或是参数，依据是字符（串）是否是以-开头，如-ab中，-a为选项,b则为参数


3.影响的全局变量有四个
extern char *optarg; //选项的参数指针
extern int optind,   //下一次调用getopt的时，从optind存储的位置处重新开始检查选项。 
extern int opterr,   //当opterr=0时，getopt不向stderr输出错误信息。
extern int optopt;   //当命令行选项字符不包括在optstring中或者选项缺少必要的参数时，该选项存储在optopt中

struct option {
    const char *name;
    int         has_arg; //no_argument、optional_argument, 或required_argument,  <getopt.h>
    int        *flag;
    int         val;
};

4. 各种返回值的含义：
    返回值          含 义
      '?'          无效选项
      ':'          缺少选项参数
      -1           选项解析结束
 
 成功找到选项
  对getopt()
      返回选项字符

  对getopt_long和getopt_long_only
      返回val变量（flag is NULL）

-------------------------------------------------------------------------
#include <stdlib.h>
char *getenv(const char *name);
char *secure_getenv(const char *name);

double drand48(void);
double erand48(unsigned short xsubi[3]);
long int lrand48(void);
long int nrand48(unsigned short xsubi[3]);
long int mrand48(void);
long int jrand48(unsigned short xsubi[3]);
void srand48(long int seedval);
unsigned short *seed48(unsigned short seed16v[3]);
void lcong48(unsigned short param[7]);

------------------------------------------------------------------------
#include <time.h>
int time(time_t*);
char *ctime(const time_t *timep);

------------------------------------------------------------------------
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
int stat(const char *path, struct stat *buf);
int fstat(int fd, struct stat *buf);
int lstat(const char *path, struct stat *buf);

struct stat {
    dev_t     st_dev;     /* ID of device containing file */
    ino_t     st_ino;     /* inode number */
    mode_t    st_mode;    /* protection */
    nlink_t   st_nlink;   /* number of hard links */
    uid_t     st_uid;     /* user ID of owner */
    gid_t     st_gid;     /* group ID of owner */
    dev_t     st_rdev;    /* device ID (if special file) */
    off_t     st_size;    /* total size, in bytes */
    blksize_t st_blksize; /* blocksize for file system I/O */
    blkcnt_t  st_blocks;  /* number of 512B blocks allocated */
    time_t    st_atime;   /* time of last access */
    time_t    st_mtime;   /* time of last modification */
    time_t    st_ctime;   /* time of last status change */
};
------------------------------------------------------------------------

