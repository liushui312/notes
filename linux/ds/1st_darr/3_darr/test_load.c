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

    handle = darr_load("./db");

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
