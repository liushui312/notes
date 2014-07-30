#include <queue.h>

QUEUE *queue_create(int size, int max)
{
    QUEUE *handle = NULL;

    handle = (QUEUE *)malloc(sizeof(QUEUE));
    ERRP(NULL == handle, goto ERR1, 0);

    handle->size = size;
    handle->max = max;
    handle->num = 0;

    handle->head.next = &handle->head;
    handle->head.prev = &handle->head;

    handle->save = (void *)malloc(size);
    ERRP(NULL == handle->save, goto ERR2, 0);

    return handle;
ERR2:
    free(handle);
ERR1:
    return NULL;
}

void queue_destroy(QUEUE **handle)
{
    QUEUE *val = *handle;
    struct node_t *tail = val->head.next, *save = NULL;

    while (tail != &val->head)
    {
        save = tail->next; 
        free(tail->data);
        free(tail);
        tail = save;
    }

    free(val->save);
    free(val);
    *handle = NULL;
}

int queue_len(QUEUE *handle)
{
    return handle->num;
}

void queue_travel(queue_op_t *op, QUEUE *handle)
{
    struct node_t *tail = NULL;

    for (tail = handle->head.next; tail != &handle->head; tail = tail->next)
    {
        op(tail->data);
    }
}

int queue_isempty(QUEUE *handle)
{
    if (handle->num == 0)
        return 1;
    return 0;
}

int queue_isfull(QUEUE *handle)
{
    if (handle->num == handle->max)
        return 1;
    return 0;
}

int queue_en(void *data, QUEUE *handle)
{
    struct node_t *new = NULL;

    if (queue_isfull(handle))
        return -1;

    new = (struct node_t *)malloc(sizeof(struct node_t));
    new->data = (void *)malloc(handle->size);

    memcpy(new->data, data, handle->size);

    new->next = &handle->head;
    new->prev = handle->head.prev;
    handle->head.prev->next = new;
    handle->head.prev = new;

    handle->num++;

    return 0;
}

void *queue_de(QUEUE *handle)
{
    struct node_t *tail = handle->head.next;

    if (queue_isempty(handle))
        return NULL;

    memcpy(handle->save, tail->data, handle->size);
    tail->next->prev = tail->prev;
    tail->prev->next = tail->next;
    free(tail->data);
    free(tail);
    handle->num--;

    return handle->save;
}

