#include <llist.h>

LLIST *llist_create(int size, llist_destroy_t *destroy, llist_store_t *store)
{
    LLIST *handle = NULL;

    handle = (LLIST *)malloc(sizeof(LLIST));
    ERRP(NULL == handle, return NULL, 0);

    handle->size = size;
    handle->num = 0;
    handle->head.next = &handle->head;
    handle->head.prev = &handle->head;
    handle->find = NULL;
    handle->destroy = destroy;
    handle->store = store;

    return handle;

}

void llist_set(llist_destroy_t *destroy, llist_store_t *store, LLIST *handle)
{
    if (destroy != NULL)
        handle->destroy = destroy;
    if (store != NULL)
        handle->store = store;
}

static struct node_t *del_node(struct node_t *val, llist_destroy_t *destroy)
{
    struct node_t *save = val->next;

    val->next->prev = val->prev;
    val->prev->next = val->next;

    if (destroy != NULL)
        destroy(val->data);
    free(val->data);
    free(val);

    return save;
}

static struct node_t *init_data(void *data, int size)
{
    struct node_t *new = NULL;

    new = (struct node_t *)malloc(sizeof(struct node_t));
    ERRP(NULL == new, goto ERR1, 0);

    new->data = (void *)malloc(size);
    ERRP(NULL == new, goto ERR2, 0);

    memcpy(new->data, data, size);

    return new;
ERR2:
    free(new);
ERR1:
    return NULL;
}

int llist_insert_sort(void *data, llist_cmp_t *cmp, LLIST *handle)
{
    struct node_t *tail = NULL;
    struct node_t *new = init_data(data, handle->size);

    ERRP(NULL == new, return -1, 0);

    for (tail = &handle->head; 
            tail->next != &handle->head && cmp(tail->next->data, data) < 0;
                tail = tail->next)
        ;

    new->next = tail->next;
    new->prev = tail;
    tail->next->prev = new;
    tail->next = new;

    handle->num++;

    return 0;
}

int llist_insert_append(void *data, LLIST *handle)
{
    struct node_t *new = init_data(data, handle->size);

    ERRP(NULL == new, return -1, 0);

    new->next = &handle->head;
    new->prev = handle->head.prev;
    handle->head.prev->next = new;
    handle->head.prev = new;

    handle->num++;

    return 0;
}

int llist_insert_prepend(void *data, LLIST *handle)
{
    struct node_t *new = init_data(data, handle->size);

    ERRP(NULL == new, return -1, 0);

    new->next = handle->head.next;
    new->prev = &handle->head;
    handle->head.next->prev = new;
    handle->head.next = new;

    handle->num++;

    return 0;
}

int llist_delete(void *key, llist_cmp_t *cmp, LLIST *handle)
{
    struct node_t *tail = handle->head.next;

    while (tail != &handle->head)
    {
        if (!cmp(key, tail->data)) 
        {
            del_node(tail, handle->destroy); 
            handle->num--;
            return 0;
        }
        tail = tail->next;
    }

    return -1;
}

int llist_delete_all(void *key, llist_cmp_t *cmp, LLIST *handle)
{
    struct node_t *tail = handle->head.next;
    int n = 0;

    while (tail != &handle->head)
    {
        if (!cmp(key, tail->data)) 
        {
            tail = del_node(tail, handle->destroy); 
            handle->num--;
            n++;
        }
        else
            tail = tail->next;
    }

    return n;
}

void *llist_find(void *key, llist_cmp_t *cmp, LLIST *handle)
{
    struct node_t *tail = handle->head.next;

    while (tail != &handle->head)
    {
        if (!cmp(key, tail->data)) 
            return tail->data;
        tail = tail->next;
    }

    return NULL;
}

LLIST *llist_find_all(void *key, llist_cmp_t *cmp, LLIST *handle)
{
    struct node_t *tail = handle->head.next;

    if (handle->find != NULL)
        llist_destroy(&handle->find);

    handle->find = llist_create(sizeof(void *), NULL, NULL);
    ERRP(NULL == handle->find, return NULL, 0);

    while (tail != &handle->head)
    {
        if (!cmp(key, tail->data)) 
            llist_insert_append(&tail->data, handle->find);
        tail = tail->next;
    }

    return handle->find;
}

void llist_travel(llist_op_t *op, void *arg, LLIST *handle)
{
    struct node_t *tail = NULL;

    for (tail = handle->head.next; tail != &handle->head; tail = tail->next)
    {
        op(tail->data, arg);
    }
}

struct data_t {
    char *cn;
    char *en;
};

void llist_find_travel(llist_op_t *op, void *arg, LLIST *handle)
{
    struct node_t *tail = NULL;

    for (tail = handle->head.next; tail != &handle->head; tail = tail->next)
    {
        op((void *)(*(char **)(tail->data)), arg);
    }
}

int llist_sort(llist_cmp_t *cmp, LLIST *handle)
{
    struct node_t *tail = NULL, *val = NULL;
    void *t = NULL;

    for (tail = handle->head.next; tail->next != &handle->head; tail = tail->next)
    {
        for (val = tail->next; val != &handle->head; val = val->next) 
        {
            if (cmp(tail->data, val->data) > 0)
            {
                t = tail->data;
                tail->data = val->data;
                val->data = t;
            }
        }
    }
}

int llist_store(const char *path, LLIST *handle)
{
    FILE *fp = NULL;
    struct node_t *tail = NULL;

    fp = fopen(path, "w");
    ERRP(NULL == fp, return -1, 0);

    fwrite(MAGIC, sizeof(char), sizeof(MAGIC), fp);
    fwrite(&handle->size, sizeof(int), 1, fp);
    fwrite(&handle->num, sizeof(int), 1, fp);

    for (tail = handle->head.next; tail != &handle->head; tail = tail->next)
    {
        if (handle->store != NULL)
            handle->store(tail->data, fp);
        else
            fwrite(tail->data, handle->size, 1, fp);
    }

    fclose(fp);

    return 0;
}

LLIST *llist_load(const char *path, llist_load_t *load)
{
    FILE *fp = NULL;
    char buf[128];
    int num, size, ret, i;
    LLIST *handle = NULL;
    void *data = NULL;

    fp = fopen(path, "r");
    ERRP(NULL == fp, goto ERR1, 0);
    
    ret = fread(buf, sizeof(char), sizeof(MAGIC), fp);
    ERRP(ret != sizeof(MAGIC) || strncmp(MAGIC, buf, sizeof(MAGIC)), goto ERR2, 0);

    ret = fread(&size, sizeof(int), 1, fp);
    ERRP(ret != 1, goto ERR2, 0);

    ret = fread(&num, sizeof(int), 1, fp);
    ERRP(ret != 1, goto ERR2, 0);

    handle = llist_create(size, NULL, NULL); 
    ERRP(NULL == handle, goto ERR2, 0);

    data = (void *)malloc(size);
    ERRP(NULL == data, goto ERR3, 0);

    for (i = 0; i < num; i++)
    {
        if (load != NULL)
        {
            ret = load(data, fp);
            ERRP(ret == -1, goto ERR4, 0);
        }
        else
        {
            ret = fread(data, size, 1, fp); 
            ERRP(ret != 1, goto ERR4, 0);
        }
        ret = llist_insert_append(data, handle);
        ERRP(ret == -1, goto ERR4, 0);
    }

    free(data);

    fclose(fp);

    return handle;
ERR4:
    free(data);
    llist_destroy(&handle);
ERR3:
    free(handle); 
ERR2:
    fclose(fp);
ERR1:
    return NULL;
}

void llist_destroy(LLIST **handle)
{
    struct node_t *tail = NULL;
    LLIST *val = *handle;

    tail = val->head.next;

    while (tail != &val->head) 
    {
        tail = del_node(tail, val->destroy);
    }
    free(val);
    *handle = NULL;
}

