#include <stdio.h>

#define MAX     10

int isempty(int end)
{
    if (end == 0)
        return 1;
    return 0;
}

int isfull(int end)
{
    if (end == MAX)
        return 1;
    return 0;
}

int push(int data, int *stack, int *end)
{
    if (isfull(*end))
        return -1;
    stack[(*end)++] = data;
    return 0;
}

int pop(int *stack, int *end)
{
    if (isempty(*end))
        return -1;
    return stack[--(*end)];
}

int main(void)
{
    int stack[MAX];
    int end = 0;
    int i, n;

    for (i = 0; i < MAX * 2; i++)
    {
        n = rand() % 100;
        printf("%d ", n);
        push(n, stack, &end);
    }
    printf("\n");

    for (i = 0; i < MAX * 2; i++)
    {
        n = pop(stack, &end);
        printf("%d ", n);
    }
    printf("\n");

    return 0;
}
