#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void sig_handler(int s)
{
    printf("sig_handler %d\n", getpid());
}

int main(void)
{
    int fd[2];
    char buf[1024];

    if (pipe(fd))
        perror("pipe");

    signal(SIGPIPE, sig_handler);

    if (fork() == 0)
    {
        close(fd[0]);
        write(fd[1], "hello parent!", 14);
        printf("write end!\n");
        close(fd[1]);
        exit(0);
    }
    //close(fd[0]);  //读端关闭再写会产生信号
    /*close(fd[1]);*/
    /*wait(NULL);*/
    read(fd[0], buf, sizeof(buf));
    printf("buf = %s\n", buf);
    close(fd[0]);

    return 0;
}
