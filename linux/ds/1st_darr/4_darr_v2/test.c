#include <darr.h>

int cmp(const void *d1, const void *d2)
{
    return *(int *)d1 - *(int *)d2;
}

void ls(const void *data, void *arg)
{
    printf("%d ", *(int *)data);
}

int main(void)
{
    DARR *handle = NULL, *find = NULL;
    int n;

    handle = darr_create(sizeof(int));

    while (1)
    {
        printf("please input num : ");
        scanf("%d", &n);

        if (n == -1)
            break;
        darr_insert(&n, APPEND, handle);
    }

    darr_travel(ls, NULL, handle);
    printf("\n");

    n = 3;
    find = darr_findall(&n, cmp, handle);
    if (find != NULL)
    {
        printf("find : ");
        darr_travel(ls, find, handle);
        printf("\n");
    }

    darr_destroy(&handle);

    return 0;
}
