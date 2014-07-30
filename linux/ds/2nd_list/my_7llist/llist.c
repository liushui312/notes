#include <llist.h>  



LLIST *llist_create(int size, llist_destroy_t *destroy, 
                    llist_file_t *store, llist_file_t *load)
{

   LLIST *handle = NULL;

   handle = (LLIST *)malloc(sizeof(LLIST *));
   ERRP(NULL == handle, goto ERR1, 0);

   handle->num = 0;
   handle->size = size;
   handle->head.data = NULL;
   handle->head.next = &handle->head;
   handle->head.prev = &handle->head;
   handle->find = NULL;
   handle->destroy = destroy;
   handle->store = store;
   handle->load = load;

   return handle;
ERR1:
    return NULL;
}

void llist_travel(llist_op_t *op, void *arg, LLIST *handle)
{

    struct node_t *tail = NULL;

    if (handle->find != NULL && handle->find == arg)	
    {
        for(tail = handle->find->head.next;
                tail != &handle->find->head;
                tail = tail->next)
            op((void *)(*(char **)tail->data), arg); 
    }
    else 
    {
        
        for(tail = handle->head.next;
                tail != &handle->head;
                tail = tail->next)
            op(tail->data, arg); 
        
    }

}

void llist_destroy(LLIST **handle)
{
    
    LLIST *val = *handle;
    struct node_t *tail = NULL, *tmp = NULL;

    if(val->destroy != NULL)
    {
        for(tail = &val->head;
            tail->next != &val->head;
            tail = tail->next)
            val->destroy(tail->data);
    }

    if(val->find != NULL)
        llist_destroy(&val->find);

    tail = val->head.next;
    while(tail != &val->head)    
    {
        tmp = tail->next;
        free(tail->data);
        free(tail);
        tail = tmp;
    }
     *handle = NULL;

}

int llist_insert(void *data, int index, LLIST *handle)
{
    struct node_t *new = NULL;
    struct node_t *tail = NULL;
    int count = 0;

    new = (struct node_t *)malloc(sizeof(struct node_t));
    ERRP(NULL == new, goto ERR1,0);
    new->data = (void *)malloc(handle->size);
    ERRP(NULL == new->data, goto ERR2,0);
    memcpy(new->data, data, handle->size);
    
    if (index <= PREPEND)
        tail = handle->head.prev;
    else if (index == APPEND || index > handle->num)
        tail = &handle->head;
    else
    {
        for(tail = &handle->head;
                count != index; 
                tail = tail->next)
            count++;
    }

    new->next = tail->next;
    new->prev = tail;
    tail->next->prev = new;
    tail->next = new;

    handle->num++;
    
    return 0;
ERR2:
    free(new);
ERR1:
    return -1;
    
}


int llist_num(LLIST *handle)
{
    return handle->num;
}

int llist_size(LLIST *handle)
{
    return handle->size;
}

int llist_delete(void *key, llist_cmp_t *cmp, LLIST *handle)
{
    struct node_t *tail = NULL;
    int count = 0;
    for(tail = handle->head.next; tail != &handle->head; tail = tail->next)
    {
        if (!cmp(tail->data, key))  
        {
            tail->prev->next = tail->next;
            tail->next->prev = tail->prev;
            free(tail->data);
            free(tail);
            count++;
        }
    }
    
    handle->num--;
    return count;
}


void *llist_find(void *key, llist_cmp_t *cmp, LLIST *handle)
{
    struct node_t *tail = NULL;
    for (tail = handle->head.next; tail != &handle->head; tail = tail->next)
    {
        if(!cmp(tail->data, key)) 
            return tail->data; 
    }

    return NULL;
}

LLIST *llist_findall(void *key, llist_cmp_t *cmp, LLIST *handle)
{
    struct node_t *tail = NULL;
    
    if (handle->find != NULL)
        llist_destroy(&handle->find);

    handle->find = llist_create(sizeof(void *), NULL, NULL, NULL);
    ERRP(NULL == handle->find, goto ERR1, 0);

    for (tail = handle->head.next; tail != &handle->head; tail = tail->next)
    {
        if(!cmp(tail->data, key)) 
           llist_insert(&tail->data, APPEND, handle->find); 
    }

    return handle->find;
ERR1:
    return NULL;

}

void llist_sort(llist_cmp_t *cmp, LLIST *handle)
{
    struct node_t *t1 = NULL, *t2 = NULL;
    void *val = NULL;

    for(t1 = handle->head.next; t1 != handle->head.prev; t1 = t1->next)
    {  
        for(t2 = t1->next; t2 != &handle->head; t2 = t2->next)
        {
            
            if(cmp( t1->data, t2->data) > 0)
            {
                val = t2->data;
                t2->data = t1->data;
                t1->data = val;
            }

        }
    }
}

int llist_store(const char *path, LLIST *handle)
{
    FILE *fp = NULL;
    int ret;
    struct node_t *tail = NULL;

    fp = fopen(path, "w");
    ERRP(fp == NULL, goto ERR1, 0);

    printf("open\n");
    ret = fwrite(MAGIC, 1, sizeof(MAGIC), fp);
    ERRP(ret != sizeof(MAGIC), goto ERR2, 0);
    ret = fwrite(&handle->num, sizeof(int), 1, fp);
    ERRP(ret != 1, goto ERR2, 0);
    ret = fwrite(&handle->size, sizeof(int), 1, fp);
    ERRP(ret != 1, goto ERR2, 0);
   
    printf("1\n");
    for(tail = handle->head.next; tail != &handle->head; tail = tail->next)    
    {
        if(handle->store != NULL)
        {
            ret = handle->store(fp, tail->data);
            ERRP(ret != 0, goto ERR2, 0);
        }
        else
        {
            ret = fwrite(tail->data, handle->size, 1, fp);
            ERRP(ret != 1, goto ERR2, 0);
            
            printf("2\n");
        }
    }

    printf("333\n");
    fclose(fp);
    return 0;

ERR2:
    fclose(fp);
ERR1:
    printf("err1\n");
    return -1;
}

LLIST *llist_load(const char *path, llist_destroy_t *destroy, llist_file_t *store, llist_file_t *load)
{

    FILE *fp = NULL;
    LLIST *handle = NULL;
    int num, size, ret;
    char buf[128];
    void *data = NULL;
    int i;

    fp = fopen(path, "r");
    ERRP(NULL == fp, goto ERR1, 0);

    ret = fread(buf, 1, sizeof(MAGIC), fp);
    ERRP(ret != sizeof(MAGIC) || strcmp(MAGIC, buf), goto ERR2, 0);

    ret = fread(&num, sizeof(num), 1, fp);
    ERRP(ret != 1, goto ERR2, 0);

    ret = fread(&size, sizeof(size), 1, fp);
    ERRP(ret != 1, goto ERR2, 0);

    handle = llist_create(size, destroy, store, load);
    ERRP(NULL == handle, goto ERR2, 0);

    handle->size = size;
    
    data = (void *)malloc(size);
    ERRP(NULL == data, goto ERR3, 0);

    for (i = 0; i < num; i++)
    {
        if (handle->load != NULL)
        {
            ret = handle->load(fp, data); 
            ERRP(ret != 0, goto ERR4, 0);
        }
        else
        {
            ret = fread(data, size, 1, fp);
            ERRP(ret != 1, goto ERR4, 0);
        }
        printf("%d\n", *(int *)data);
        ret = llist_insert(data, APPEND, handle);
        ERRP(ret != 0, goto ERR4, 0);
    }
    
    
    free(data);

    fclose(fp);

    return handle;
ERR4:
    free(data);
ERR3:
    destroy(&handle);
ERR2:
    fclose(fp);
ERR1:
    return NULL;
}


