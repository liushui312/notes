#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    if (fork() == 0)
    {
        printf("child id = %d, %d\n", getpid(), getppid());
        if (fork() == 0)
        {
            printf("cchild id = %d, %d\n", getpid(), getppid());
            getchar();
            printf("cchild id = %d, %d\n", getpid(), getppid());
            exit(0);
        }
        sleep(1);
        exit(0);
    }
    wait(NULL);

    printf("parent id = %d, %d\n", getpid(), getppid());

    return 0;
}
