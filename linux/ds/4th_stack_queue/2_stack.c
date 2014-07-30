#include <stdio.h>

void sum(int num)
{
    if (num == 0)
        return;
    sum(num / 8);
    putchar(num % 8 + '0');
}

#define MAX     100

int stack[MAX];
int end = 0;

int isempty(void)
{
    if (end == 0)
        return 1;
    return 0;
}

int isfull(void)
{
    if (end == MAX)
        return 1;
    return 0;
}

int push(int data)
{
    if (isfull())
        return -1;
    stack[end++] = data;
    return 0;
}

int pop(void)
{
    if (isempty())
        return -1;
    return stack[--end];
}

int main(void)
{
    int n;
    /*十进制转八进制*/

    printf("please input num : ");
    scanf("%d", &n);

    printf("%d ===> %o ", n, n);
    /*sum(n);*/

    while (n)
    {
        push(n % 8);
        n /= 8;
    }

    while (!isempty())
    {
        n = pop(); 
        putchar(n + '0');
    }
    printf("\n");

    return 0;
}
