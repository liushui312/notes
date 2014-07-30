#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>

int main(void)
{
    int fd, ret;
    fd_set rds;
    char buf[1024];
    struct timeval t = {6, 0};

    fd = open("./test_pipe", O_RDWR);
    if (fd == -1)
        perror("open");

    while (1)
    {
        FD_ZERO(&rds);
        FD_SET(0, &rds);
        FD_SET(fd, &rds);

        ret = select(fd + 1, &rds, NULL, NULL, NULL);
        if (ret < 0)
        {
            perror("select");
            break;
        }
        else if (ret > 0)
        {
            if (FD_ISSET(fd, &rds))
            {
                ret = read(fd, buf, sizeof(buf));
                buf[ret] = '\0';
                printf("pipe = %s\n", buf);
            }

            if (FD_ISSET(0, &rds))
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
