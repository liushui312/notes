#include <darr.h>

int cmp(const void *d1, const void *d2)
{
    return *(int *)d1 - *(int *)d2;
}

void ls(const void *data)
{
    printf("%d ", *(int *)data);
}

int main(void)
{
    DARR *handle = NULL;
    int n;
    int *val = NULL;

    handle = darr_create(sizeof(int));

    while (1)
    {
        printf("please input num : ");
        scanf("%d", &n);

        if (n == -1)
            break;
        /*darr_append(&n, handle);*/
        /*darr_prepend(&n, handle);*/
        darr_insert(&n, PREPEND, handle);
    }

    if (!darr_store("./db", handle))
        printf("存储成功!\n");

    /*n = 9999;*/
    /*darr_insert(&n, 3, handle);*/

    val = darr_ind(2, handle);
    printf("val = %d\n", *val);

    darr_travel(ls, handle);
    printf("\n");

    darr_ind_del(1, handle);

    darr_travel(ls, handle);
    printf("\n");

    darr_sort(cmp, handle);

    darr_travel(ls, handle);
    printf("\n");

    darr_destroy(&handle);

    return 0;
}
