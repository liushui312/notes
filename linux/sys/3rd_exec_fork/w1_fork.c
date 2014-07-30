#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int sum(int num)
{
    int i, bit, n1, n2, t, j;

    for (i = num, bit = 0; i > 0; i /= 10, bit++)
        ;
    for (i = 0; i < bit / 2; i++)
    {
        for (j = 0, t = 1; j < i; j++, t *= 10)
            ;
        n1 = num / t % 10;

        for (j = 0, t = 1; j < bit - i - 1; j++, t *= 10)
            ;
        n2 = num / t % 10;

        if (n1 != n2)
            return 0;
    }

    return 1;
}

int test(int s, int e)
{
    int c = 0;

    while (s < e)
    {
        if (sum(s++))
            c++;
    }

    return c;
}

int main(void)
{
    int i;
    int c = 0, status;

    for (i = 0; i < 10; i++)
    {
        if (fork() == 0)
            exit(test(1000 + i * 500, 1000 + (i + 1) * 500)); 
    }

    for (i = 0; i < 10; i++)
    {
        wait(&status);
        c += WEXITSTATUS(status);
    }

    printf("c = %d\n", c);

    return 0;
}
