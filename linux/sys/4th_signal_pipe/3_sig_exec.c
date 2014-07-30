#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <wait.h>

void sig_handler(int s)
{
    printf("sig_handler! %d\n", getpid());
}

int main(void)
{
    /*signal(SIGINT, sig_handler);*/
    signal(SIGINT, SIG_IGN);

    if (fork() == 0)
    {
        printf("child pid = %d, %d\n", getpid(), getppid());
        execl("./test", "test", NULL);
        exit(0); 
    }
    wait(NULL);
    printf("main end!\n");

    return 0;
}
