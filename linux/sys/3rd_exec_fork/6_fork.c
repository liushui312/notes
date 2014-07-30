#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    int status;

    if (fork() == 0)
    {
        printf("child pid = %d, %d\n", getpid(), getppid()); 
        //getchar();
        for(;;) {}
        printf("child end!\n");
        exit(256);
    }

    printf("parent start!\n");
    //wait(&status);
    //printf("status = %d\n", WEXITSTATUS(status));
    //printf("exit = %d, signal = %d\n", WIFEXITED(status), WIFSIGNALED(status));

    printf("parent pid = %d, %d\n", getpid(), getppid());

    return 100;
}
