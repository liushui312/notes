#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <wait.h>

void sig_handler(int s)
{
    printf("sig_handler! %d\n", getpid());
    /*sleep(10);*/
    /*printf("sig_handlet end\n");*/
}

int main(void)
{
    signal(SIGINT, sig_handler);

    if (fork() == 0)
    {
        printf("child pid = %d, %d\n", getpid(), getppid());
        signal(SIGINT, SIG_IGN);
        getchar();
        exit(0); 
    }

    getchar();

    return 0;
}
