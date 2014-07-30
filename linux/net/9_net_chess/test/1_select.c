#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>

int main(void)
{
    int fd;
    fd_set rds;
    char buf[1024];
    int ret;
    struct timeval t = {3, 0};

    mkfifo("kyo", 0666);

    fd = open("kyo", O_RDWR);
    if (fd == -1)
        perror("open");


    while (1)
    {
        FD_ZERO(&rds);
        FD_SET(0, &rds);
        FD_SET(fd, &rds);
        ret = select(fd + 1, &rds, NULL, NULL, &t);
        printf("ret = %d, t.tv_sec = %ld, t.tv_usec = %ld\n", ret, t.tv_sec, t.tv_usec);
        if (ret > 0)
        {
            if (FD_ISSET(fd, &rds))
            {
                ret = read(fd, buf, sizeof(buf));
                buf[ret] = '\0';
                printf("fd buf = %s\n", buf);
            }
            else
            {
                ret = read(0, buf, sizeof(buf));
                buf[ret] = '\0';
                printf("stdin buf = %s\n", buf);
            }
        }
        else if (ret == 0)
        {
            printf("timeout!\n");
            break;
        }
        else
            perror("select");
    }


    close(fd);

    unlink("kyo");

    return 0;
}
