#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main(void)
{
    int fd;

    /*
     *0   stdin
     *1   stdout
     *2   stderr
     */

    /*close(1);*/
    /*getchar();*/

    /*write(1, "hello", 5);*/

    fd = open("test_open", O_RDONLY | O_CREAT, 0777);
    if (fd == -1)
    {
        /*errno = 35;*/
        perror("open");
        return 0;
    }
    printf("fd = %d\n", fd);

    close(fd);

    return 0;
}
