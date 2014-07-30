#include <llist.h>


LLIST *llist_create(int size)
{
   LLIST *handle = NULL;

   handle = (LLIST *)malloc(sizeof(LLIST *));
   ERRP(NULL == handle, goto ERR1, 0);

   &handle->head = (struct node_t)malloc(sizeof(struct node_t));
   ERRP(NULL == &handle->head, goto ERR1, 0);

   handle->num = 0;
   handle->size = size;
   handle->head.data = NULL;
   handle->head.next = NULL;
   handle->head.prev = NULL;

   return handle;
}

int llist_append(void *data, LLIST *handle)
{
    
}

void llist_travel(llist_op_t *op, LLIST *handle)
{

}

void llist_destroy(LLIST **handle)
{

}

int llist_prepend(void *data, LLIST *handle)
{

}

int llist_insert(void *data, int index, LLIST *handle)
{

}

int llist_ind_del(int index, LLIST *handle)
{

}

void *llist_ind(int index, LLIST *handle)
{

}

int llist_num(LLIST *handle)
{

}

int llist_size(LLIST *handle)
{

}

int llist_delete(void *key, llist_cmp_t *cmp, LLIST *handle)
{

}

void *llist_find(void *key, llist_cmp_t *cmp, LLIST *handle)
{

}

void llist_sort(llist_cmp_t *cmp, LLIST *handle)
{

}

int llist_store(const char *path, LLIST *handle)
{

}

LLIST *llist_load(const char *path)
{

}

