#include <queue.h>

#define MAX     10

void op(const void *data)
{
    printf("%2d", *(int *)data);
}

int main(void)
{
    QUEUE *handle = NULL;
    int i, n;
    int *val = NULL;

    handle = queue_create(sizeof(int), MAX);

    for (i = 0; i < MAX * 2; i++)
    {
        n = rand() % 100;
        printf("%d ", n);
        queue_en(handle, &n);
    }
    printf("\n");

    queue_travel(handle, op);
    printf("\n");

    for (i = 0; i < MAX * 2; i++)
    {
        val = queue_de(handle);
        if (val != NULL)
            printf("%d ", *(int *)val);
        else
            printf("-1 ");
    }
    printf("\n");

    queue_destroy(&handle);

    return 0;
}
