#include <llist.h>

int cmp(const void *d1, const void *d2)
{
    return *(int *)d1 - *(int *)d2;
}

void ls(const void *data, void *arg)
{
    printf("%d ", *(int *)data);
}

int main(int argc, char *argv[])
{
    LLIST *handle = NULL, *find = NULL;
    int n;

    if (argc > 1)
        handle = llist_load("./db");
    else
    {
        handle = llist_create(sizeof(int));

        while (1)
        {
            printf("please input num : ");
            scanf("%d", &n);

            if (n == -1)
                break;
            /*llist_append(&n, handle);*/
            /*llist_prepend(&n, handle);*/
            /*llist_insert(&n, PREPEND, handle);*/
            llist_sort_insert(&n, cmp, handle);
        }

        llist_travel(ls, NULL, handle);
        printf("\n");

        n = 9999;
        llist_insert(&n, 3, handle);

        if (!llist_store("./db", handle))
            printf("存储成功!\n");
    }

    llist_travel(ls, NULL, handle);
    printf("\n");

    llist_sort(cmp, handle);

    printf("sort : ");
    llist_travel(ls, NULL, handle);
    printf("\n");

    while (1)
    {
        printf("please input key : ");
        scanf("%d", &n);
        if (n == -1)
            break;

        find = llist_findall(&n, cmp, handle);
        if (find != NULL)
        {
            llist_travel_find(ls, NULL, find);
            printf("\n");
        }
/*
 *        printf("delete = %d\n", llist_delete(&n, cmp, handle));
 *
 *        llist_travel(ls, NULL, handle);
 *        printf("\n");
 */
    }

    llist_destroy(&handle);

    return 0;
}
