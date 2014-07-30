#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
    int fd;

    umask(0);

    fd = open("test_open", O_WRONLY | O_CREAT | O_TRUNC, 0777);
    if (fd == -1)
        perror("open");

    /*ftell();  lseek(fd, 0, SEEK_CUR);*/
    printf("lseek = %d\n", lseek(fd, 0, SEEK_CUR));

    /*printf("lseek = %d\n", lseek(fd, 1024 * 1024, SEEK_SET));*/

    /*ftruncate(fd, 1024 * 1024 * 10);*/

    /*write(fd, "hell", 4);*/


    close(fd);

    return 0;
}
