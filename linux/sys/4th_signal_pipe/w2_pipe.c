#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
    int fd[2];

    if (pipe(fd))
        perror("pipe");

    if (fork() == 0)
    {
        close(fd[0]);
        dup2(fd[1], 1);
        execlp("cat", "cat", "/etc/passwd", NULL);
        perror("execlp");
        exit(0);
    }
    close(fd[1]);
    dup2(fd[0], 0);
    execlp("grep", "grep", "root", NULL);
    perror("execlp");

    return 0;
}
