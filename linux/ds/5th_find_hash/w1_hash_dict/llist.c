#include <llist.h>

LLIST *llist_create(int size, llist_destroy_t *destroy, llist_file_t *store, llist_file_t *load)
{
    LLIST *handle = NULL;

    handle = (LLIST *)malloc(sizeof(LLIST));
    ERRP(NULL == handle, goto ERR1, 0);

    handle->head.next = &handle->head;
    handle->head.prev = &handle->head;
    handle->num = 0;
    handle->size = size;
    handle->destroy = destroy;
    handle->store = store;
    handle->load = load;

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
    else if (index == APPEND || index >= handle->num)
        tail = handle->head.prev; 
    else if (index == SORT && cmp != NULL)
    {
        for (tail = &handle->head; 
             tail->next != &handle->head && 
                cmp(data, tail->next->data) > 0;
             tail = tail->next)
            ;
    }
    else
    {
        for (tail = &handle->head; 
             --index && tail->next != &handle->head; 
             tail = tail->next)
            ;
    }

    new = (struct node_t *)malloc(sizeof(struct node_t));
    ERRP(NULL == new, goto ERR1, 0);

    new->data = (void *)malloc(handle->size);
    ERRP(NULL == new->data, goto ERR2, 0);
    memcpy(new->data, data, handle->size);

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

    
int llist_append(void *data, LLIST *handle)
{
    return _insert(data, APPEND, NULL, handle);
}

int llist_prepend(void *data, LLIST *handle)
{
    return _insert(data, PREPEND, NULL, handle);
}

int llist_insert(void *data, int index, LLIST *handle)
{
    return _insert(data, index, NULL, handle);
}

int llist_sort_insert(void *data, llist_cmp_t *cmp, LLIST *handle)
{
    return _insert(data, SORT, cmp, handle);
}

void llist_travel(llist_op_t *op, void *arg, LLIST *handle)
{
    struct node_t *tail = NULL;

    for (tail = handle->head.next; tail != &handle->head; tail = tail->next)
    {
        op(tail->data, arg); 
    }
}

void llist_travel_find(llist_op_t *op, void *arg, LLIST *handle)
{
    struct node_t *tail = NULL;

    for (tail = handle->head.next; tail != &handle->head; tail = tail->next)
    {
        op((void *)(*(char **)(tail->data)), arg); 
    }
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
    struct node_t *tail = NULL, *save = NULL;
    int c = 0;

    for (tail = handle->head.next; tail != &handle->head; tail = save)
    {
        save = tail->next;
        if (!cmp(key, tail->data))
        {
            tail->next->prev = tail->prev; 
            tail->prev->next = tail->next;
            if (handle->destroy != NULL)
                handle->destroy(tail->data); 
            else
                free(tail->data);
            free(tail);
            handle->num--;
            c++;
        }
    }

    return c;
}

void *llist_find(void *key, llist_cmp_t *cmp, LLIST *handle)
{
    struct node_t *tail = NULL;

    for (tail = handle->head.next; tail != &handle->head; tail = tail->next)
    {
        if (!cmp(key, tail->data))
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
        if (!cmp(key, tail->data))
            llist_append(&tail->data, handle->find);
    }

    return handle->find;
ERR1:
    return NULL;
}

void llist_sort(llist_cmp_t *cmp, LLIST *handle)
{
    struct node_t *v1 = NULL, *v2 = NULL;
    void *val = NULL;

    for (v1 = handle->head.next; v1->next != &handle->head; v1 = v1->next) 
    {
        for (v2 = v1->next; v2 != &handle->head; v2 = v2->next) 
        {
            if (cmp(v1->data, v2->data) > 0)
            {
                val = v1->data;
                v1->data = v2->data;
                v2->data = val;
            }
        }
    }
}

int llist_store(const char *path, LLIST *handle)
{
    struct node_t *tail = NULL;
    FILE *fp = NULL;
    int ret;

    fp = fopen(path, "w");
    ERRP(NULL == fp, goto ERR1, 0);

    ret = fwrite(MAGIC, 1, sizeof(MAGIC), fp);
    ERRP(ret != sizeof(MAGIC), goto ERR2, 0);

    ret = fwrite(&handle->num, sizeof(handle->num), 1, fp);
    ERRP(ret != 1, goto ERR2, 0);
    ret = fwrite(&handle->size, sizeof(handle->size), 1, fp);
    ERRP(ret != 1, goto ERR2, 0);

    for (tail = handle->head.next; tail != &handle->head; tail = tail->next)
    {
        if (handle->store != NULL)
        {
            ret = handle->store(fp, tail->data);
            ERRP(ret == -1, goto ERR2, 0);
        }
        else
        {
            ret = fwrite(tail->data, handle->size, 1, fp); 
            ERRP(ret != 1, goto ERR2, 0);
        }
    }

    fclose(fp);

    return 0;
ERR2:
    fclose(fp);
ERR1:
    return -1;

}

LLIST *llist_load(const char *path, llist_destroy_t *destroy, llist_file_t *store, llist_file_t *load)
{
    FILE *fp = NULL;
    LLIST *handle = NULL;
    int num, size, ret, i;
    char buf[128];
    void *data = NULL;

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
            ERRP(ret == -1, goto ERR4, 0);
        }
        else
        {
            ret = fread(data, size, 1, fp); 
            ERRP(ret != 1, goto ERR4, 0);
        }
        ret = llist_append(data, handle);
        ERRP(ret == -1, goto ERR4, 0);
    }

    free(data);

    fclose(fp);

    return handle;
ERR4:
    free(data);
ERR3:
    llist_destroy(&handle);
ERR2:
    fclose(fp);
ERR1:
    return NULL;
}

void llist_destroy(LLIST **handle)
{
    struct node_t *tail = NULL, *save = NULL;
    LLIST *val = *handle;

    if (val->find != NULL)
        llist_destroy(&val->find);

    tail = val->head.next;
    while (tail != &val->head)
    {
        save = tail->next; 
        if (val->destroy != NULL)
            val->destroy(tail->data);
        else
            free(tail->data);
        free(tail);
        tail = save;
    }
    *handle = NULL;
}
