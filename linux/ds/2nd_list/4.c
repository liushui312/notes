#include <llist.h> 

LLIST *llist_create(int size)
{
    LLIST *handle = NULL;

    handle = (LLIST *)malloc(sizeof(LLIST));
    ERRP(NULL == handle, goto ERR1, 0);

    handle->head.next = &handle->head;
    handle->head.prev = &handle->head;
    handle->num = 0;
    handle->size = size;

    handle->find = NULL;

    return handle;
ERR1:
    return NULL;
}

static int _insert(void *data, int index, llist_cmp_t *cmp, LLIST *handle)
{
    struct node_t *new = NULL, *tail = NULL;

    if (index < SORT || index == PREPEND)
        tail = &handle->head;
    else if (index = APPEND || index >= handle->num)
        tail = handle->head.prev;
    else if (index == SORT && cmp != NULL)
    {
        for (tail = &handle->head && cmp(data, tail->next->data), tail = tail->next) 
            ;

    }
    
}
