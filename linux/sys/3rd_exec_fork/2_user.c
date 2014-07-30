#include <stdio.h>

int main(void)
{
    printf("getpid = %d, getppid = %d\n", getpid(), getppid());
    printf("getuid = %d\n", getuid());
    printf("geteuid = %d\n", geteuid());

    setuid(1000);
    printf("getuid = %d\n", getuid());
    /*seteuid();*/

    printf("setuid = %d\n", setuid(0));
    printf("getuid = %d\n", getuid());

    fopen("/etc/shadow", "r");
    perror("fopen");

    return 0;
}
