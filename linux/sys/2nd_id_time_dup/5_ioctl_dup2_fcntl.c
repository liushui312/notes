#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
    int fd, new, ret;
    char buf[1024];
    struct winsize win;

    fd = open("test_open", O_RDWR | O_CREAT | O_TRUNC, 0777);
    if (fd == -1)
        perror("open");

    /*new = dup(fd);*/
    /*dup2(fd, 1);*/
    printf("new = %d, fd = %d\n", new, fd);

    /*write(1, "hello", 5);*/

    new = fcntl(0, F_GETFL);
    fcntl(0, F_SETFL, new | O_NONBLOCK);

    while (1)
    {
        ret = read(0, buf, sizeof(buf));
        if (ret > 0)
        {
            printf("ret = %d, buf = %s\n", ret, buf);
            break;
        }
        else
        {
            printf("wait!\n");
            usleep(500000);
        }
    }

    ret = ioctl(0, TIOCGWINSZ, &win);
    if (ret < 0)
        perror("ioctl");

    printf("w = %d, h = %d\n", win.ws_col, win.ws_row);

    close(fd);

    return 0;
}
