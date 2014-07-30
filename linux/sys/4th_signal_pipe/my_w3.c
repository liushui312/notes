#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int flag = 0, fd1, fd2, f1, f2, ret = 0;
    char buf[1024];

    if (argc < 2)
    {
        printf("please input arg!\n");
        return 0;
    }
    
    
    if (atoi(argv[1]) == 1 )
        flag = 1;
    else if (atoi(argv[1]) == 2)
        flag = 2;

    mkfifo("./p1", 0777);
    mkfifo("./p2", 0777);
    
    fd1 = open("p1", O_RDWR);
    fd2 = open("p2", O_RDWR);

    printf("#%d: ", flag);
    fflush(stdout);

    if (flag == 1 )
    {
        f1 = fd1;
        f2 = fd2;
    }
    else if (flag == 2)
    {
        f1 = fd2;
        f2 = fd1;
    }

    dup2(f1, 0);

    while (1)
    {
        ret = read(f2, buf, sizeof(buf)); 
        printf("ret = %d\n", ret);
        sleep(1);
        if (ret && ret != -1)
            printf("#%d: %s\n", flag, buf);
    }

    return 0;
}
