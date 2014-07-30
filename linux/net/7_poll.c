#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <poll.h>

int main(void)
{
    int fd, ret;
    char buf[1024];
    struct pollfd rds[2];

    fd = open("./test_pipe", O_RDWR);
    if (fd == -1)
        perror("open");

    rds[0].fd = 0;
    rds[0].events = POLLIN;
    rds[1].fd = fd;
    rds[1].events = POLLIN;

    while (1)
    {
        ret = poll(rds, 2, 5000);
        if (ret < 0)
        {
            perror("poll");
            break;
        }
        else if (ret > 0)
        {
            if (rds[1].revents == POLLIN)
            {
                ret = read(fd, buf, sizeof(buf));
                buf[ret] = '\0';
                printf("pipe = %s\n", buf);
            }

            if (rds[0].revents == POLLIN)
            {
                ret = read(0, buf, sizeof(buf));
                buf[ret] = '\0';
                printf("input = %s\n", buf);
            }
        }
        else
        {
            printf("timeout!\n");
            break;
        }
    }


    return 0;
}
