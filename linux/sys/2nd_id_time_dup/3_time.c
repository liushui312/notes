#include <stdio.h>
#include <sys/types.h>
#include <utime.h>
#include <time.h>

int main(void)
{
    time_t t;
    char *p = NULL;
    char buf[1024];
    struct tm *m = NULL;
    struct utimbuf u = {100000, 1265997576};

    utime("homework", &u);

    t = time(NULL);

    p = ctime(&t);
    printf("p = %p\n", p);
    p = ctime_r(&t, buf);
    printf("p = %p, buf = %p\n", p, buf);
    /*m = gmtime(&t);*/
    m = localtime(&t);
    /*printf("%s\n", asctime(m));*/
    printf("%d-%d-%d %d:%d:%d\n", m->tm_year + 1900,
        m->tm_mon + 1, m->tm_mday, m->tm_hour, m->tm_min, m->tm_sec);

    strftime(buf, sizeof(buf), "%F %R %b %B", m);
    printf("buf = %s\n", buf);

    /*printf("ctime = %s\n", ctime(&t));*/

    return 0;
}
