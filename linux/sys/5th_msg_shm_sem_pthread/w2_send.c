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
    int fdr, fdw, c = 1, l = 3;
    char buf[1024];

    mkfifo(FIFO1, 0777);
    mkfifo(FIFO2, 0777);

    fdr = open(FIFO1, O_RDWR);
    if (fdr == -1)
    {
        perror("open"FIFO1);
        goto EXIT;
    }
    fdw = open(FIFO2, O_RDWR);
    if (fdw == -1)
    {
        perror("open"FIFO2);
        goto EXIT;
    }
    printf("\033[2J");
    fflush(stdout);

    if (fork() == 0)
    {
        while (1)
        {
            read(fdr, buf, sizeof(buf));
            if (!strcmp(buf, "exit"))
                break;
            printf("\033[s\033[%d;1H\033[2K%2d:%s\033[u", l++, c++, buf);
            if (l > 15)
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
        write(fdw, buf, strlen(buf) + 1);
        if (!strcmp(buf, "exit"))
            break;
    }

EXIT:
    close(fdr);
    close(fdw);
    unlink(FIFO1);
    unlink(FIFO2);

    kill(0, SIGKILL);

    return 0;
}
