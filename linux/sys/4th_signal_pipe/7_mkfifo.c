#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int fd;
    char buf[1024];

    printf("pid = %d\n", getpid());

    mkfifo("./fifo_test", 0777);

    fd = open("./fifo_test", O_RDWR);
    if (fd == -1)
        perror("open");

    if (argc > 1)
        write(fd, argv[1], strlen(argv[1]) + 1); 
    else
    {
        read(fd, buf, sizeof(buf));
        printf("buf = %s\n", buf);
    }

    close(fd);

    return 0;
}
