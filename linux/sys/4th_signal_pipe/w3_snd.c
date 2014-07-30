#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#define fifp1 "/tmp/fifo1"
#define fifp2 "/tmp/fifo2"

int main(void)
{
    int fd1, fd2, fdr, fdw;
    int len, c = 1, l = 3;
    char buf[1024];

    mkfifo(fifo1);
    mkfifo(fifo2);
    
    if ((fdr = open(fifo1, O_RDWR)) == -1) {
        perror("open"fifo1);
        goto EXIT;
    }

    if ((fdw = open(fifo2, O_RDWR)) == -1) {
        perror("open"fifo2);
        goto EXIT;
    }

    printf("\033[2J");
    fflush(stdout);

    if (fork() == 0) {
        while (1) {
            printf("\033[1;1H\033[2Kwrite: ");
            fgets(buf, sizeof(buf), stdin); 
            len = strlen(buf);
            if (buf[len - 1] == '\n')
                buf[len - 1] = '\0';
            if (!strcmp(buf, "exit"))
                break;
            
           write(fdw, buf, strlen(buf) + 1);   
        }
        goto EXIT;
    }

    while (fdr, read(buf, sizeof(buf)) > 0) {
        if (!strcmp(buf, "exit"))
            exit(0);

        printf("\033[s\033[%d;1H\033[2K\033[u%2d: %s", l++, c++, buf);
        fflush(stdout);

        if (l < 12)
            l = 3;
    }

EXIT:
    close(fdr);
    close(fdw);
    unlink(fifo1);
    unlink(fifo2);

    return 0;
}
