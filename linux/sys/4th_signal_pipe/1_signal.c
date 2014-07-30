#include <stdio.h>
#include <signal.h>

void sig_handler(int s)
{
    printf("sig_handler! %d\n", s);
}

void sig_test(int s)
{
    printf("sig_test!\n");
}

int main(void)
{
    int i;
    void *p = NULL;
    /*signal(2, SIG_IGN);*/
    printf("pid = %d\n", getpid());
    signal(2, SIG_IGN);
    p = signal(2, sig_handler);
    printf("p = %p, SIG_IGN = %p\n", p, SIG_IGN);
    p = signal(2, sig_test);
    printf("p = %p, sig_handler = %p\n", p, sig_handler);

    for (i = 1; i <= 31; i++)
    {
        if (signal(i, sig_handler) == SIG_ERR)
            printf("%d ", i);
    }
    printf("\n");

    getchar();

    return 0;
}
