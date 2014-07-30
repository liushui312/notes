用户信息
    getuid
    getgid
    /etc/passwd
        getpwnam getpwuid
    /etc/shadow
        getspnam getpass crypt
    /etc/group
        getgrnam getgrgid 
        initgroups  把某个用户加入某个组

时间
    time        系统调用
    utime
    ctime
    asctime
    localtime
    gmtime
    mktime
    strftime
------------------------------------------------------------
#include <sys/times.h>
clock_t times(struct tms *buf); //获取当前进程时间，存于buf中
成功返回      ，失败返回-1，并置errno

typedef long clock_t;
struct tms {
    clock_t tms_utime;  /* user time */
    clock_t tms_stime;  /* system time */
    clock_t tms_cutime; /* user time of children */
    clock_t tms_cstime; /* system time of children */
};





