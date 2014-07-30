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
    int n;
    LLIST *handle = NULL;
    LLIST *find = NULL;
    
    handle = llist_load("./db", NULL, NULL, NULL);
    
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
	printf("find = %d\n", *(int *)llist_find(&n, cmp, handle));
	printf("\n");

    printf("findall num : ");
    scanf("%d", &n);
    find = llist_findall(&n, cmp, handle);
    llist_travel(printf_s, find, handle);
    printf("\n");

    llist_sort(cmp, handle);
    llist_travel(printf_s, NULL, handle);
	printf("\n");

    llist_destroy(&handle);

    return 0;
}
