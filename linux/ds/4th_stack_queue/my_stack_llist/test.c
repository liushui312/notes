#include <stack.h>

#define MAX     10


void op(void const *data)
{
    printf("%2d ", *(int *)data);
}
int main(void)
{
    STACK *handle = NULL;
    int i, n, ret;
    int *val = NULL;

    handle = stack_create(sizeof(int), MAX);
    ERRP(NULL == handle, goto ERR1, 1, "create failed!\n");
    
    for (i = 0; i < MAX; i++)
    {
        n = rand() % 100;
        printf("%d ", n);
        ret = stack_push(handle, &n);
        ERRP(ret == -1, goto ERR2, 1, "push failed!\n");
    }
    printf("\n");

    stack_travel(handle, op);
    printf("\n");

    for (i = 0; i < MAX; i++)
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
ERR2:
    stack_destroy(&handle);
ERR1:
    return -1;
}
