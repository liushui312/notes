#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FIFO1   "/tmp/kyo_fifo1"
#define FIFO2   "/tmp/kyo_fifo2"

int main(void)
{
    int fd1, fd2;
    int *fdr = NULL, *fdw = NULL, c = 1, l = 3;
    char buf[1024];

    printf("\033[2J");
    fflush(stdout);

    if (access(FIFO1, F_OK))
    {
        mkfifo(FIFO1, 0777);
        while (access(FIFO2, F_OK))
        {
            printf("\033[1;1H等待中....!");
            fflush(stdout);
            usleep(1000);
            continue;
        }
        fdr = &fd1;
        fdw = &fd2;
    }
    else
    {
        if (!access(FIFO2, F_OK))
        {
            printf("\033[1;1H正在聊天...!\n");
            return 0;
        }
        mkfifo(FIFO2, 0777);
        fdw = &fd1;
        fdr = &fd2;
    }


    fd1 = open(FIFO1, O_RDWR);
    if (fd1 == -1)
    {
        perror("open"FIFO1);
        goto EXIT;
    }
    fd2 = open(FIFO2, O_RDWR);
    if (fd2 == -1)
    {
        perror("open"FIFO2);
        goto EXIT;
    }

    if (fork() == 0)
    {
        while (1)
        {
            read(*fdr, buf, sizeof(buf));
            if (!strcmp(buf, "exit"))
                break;
            printf("\033[s\033[%d;1H\033[2K%2d:%s\033[u", l++, c++, buf);
            if (l > 12)
                l = 3;
            fflush(stdout);
        }
        goto EXIT;
    }

    while (1)
    {
        printf("\033[1;1H\033[2Kwrite : ");
        fflush(stdout);
        fgets(buf, sizeof(buf), stdin);
        if (buf[strlen(buf) - 1] == '\n')
            buf[strlen(buf) - 1] = '\0';
        write(*fdw, buf, strlen(buf) + 1);
        if (!strcmp(buf, "exit"))
            break;
    }

EXIT:
    close(fd1);
    close(fd2);
    unlink(FIFO1);
    unlink(FIFO2);

    printf("\033[2J");
    fflush(stdout);

    kill(0, SIGKILL);

    return 0;
}
