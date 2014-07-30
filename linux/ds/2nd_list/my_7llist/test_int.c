#include <llist.h>

void printf_s(const void *data, void* arg)
{
    printf("%d ", *(int *)data);
}

int cmp(const void *d1, const void *d2)
{
    int ret;
    ret = *(int *)d1 - *(int *)d2;
    return ret;
}

int main(void)
{
    int n, ret;
    void *ret1;
    LLIST *handle = NULL;
    LLIST *find = NULL;
    
    handle = llist_create(sizeof(int), NULL, NULL, NULL);

    while (1)
    {
        printf("please input num : ");
        scanf("%d", &n);

        if (n == -1)
            break;

        llist_insert(&n, APPEND, handle);
    }
    
    llist_travel(printf_s, NULL, handle);
    printf("\n");

    printf("insert num : ");
    scanf("%d", &n);
    llist_insert(&n, 3, handle);
    llist_travel(printf_s, NULL, handle);
    printf("\n");

    printf("delete num : ");
    scanf("%d", &n);
    llist_delete(&n, cmp, handle);
    llist_travel(printf_s, NULL, handle);
    printf("\n");

	printf("find num : ");
	scanf("%d", &n);
	ret1 = llist_find(&n, cmp, handle);
    if(ret1 != NULL)
        printf("find = %d\n", *(int*)ret1);
    printf("\n");

    printf("findall num : ");
    scanf("%d", &n);
    find = llist_findall(&n, cmp, handle);
    llist_travel(printf_s, find, handle);
    printf("\n");

    llist_sort(cmp, handle);
    printf("sort:");
    llist_travel(printf_s, NULL, handle);
	printf("\n");

    ret = llist_store("./db", handle);
    if(ret == 0)
        printf("存储成功!\n");

    llist_destroy(&handle);

    return 0;
}
