#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>


int main(void)
{
    int fd;
    char buf[10000] = {0};
    FILE *fp = NULL; 

    mkfifo("./tmp", 0777);

    fd = open("./tmp", O_RDWR);
    if (fd == -1)
        perror("open");

    if (fork() == 0)
    {
        dup2(fd, 1);
        close(fd);
        execlp("cat", "cat", "/etc/passwd", NULL);
        perror("exec");
        exit(0);
    }

    read(fd, buf, sizeof(buf));
    fp = fopen("fbuf", "w+");
    fwrite(buf, 1, sizeof(buf), fp);
    close(fd);
    execlp("grep", "grep", "root", "fbuf", "-a",  NULL);
    perror("grep");

    return 0;
}
