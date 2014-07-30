#include <stack.h>

#define MAX     10

int main(void)
{
    STACK *handle = NULL;
    int i, n;
    int *val = NULL;

    handle = stack_create(sizeof(int), MAX);

    for (i = 0; i < MAX * 2; i++)
    {
        n = rand() % 100;
        printf("%d ", n);
        stack_push(&n, handle);
    }
    printf("\n");

    for (i = 0; i < MAX * 2; i++)
    {
        val = stack_pop(handle);
        if (val != NULL)
            printf("%d ", *val);
        else
            printf("-1 ");
    }
    printf("\n");

    stack_destroy(&handle);

    return 0;
}
