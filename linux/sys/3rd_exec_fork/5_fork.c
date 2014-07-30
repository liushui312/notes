#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    pid_t id;
    int a = 333;

    printf("main pid = %d, %d\n", getpid(), getppid());

    id = fork();
    if (id == 0)
    {
        a = 666;
        printf("a = %d, &a = %p\n", a, &a);
    
        exit(0);
    }

    sleep(1);

    printf("a = %d, %p\n", a, &a);

    printf("id = %d, %d\n", id, getppid());

    getchar();

    return 0;
}
