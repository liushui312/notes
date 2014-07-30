#include <queue.h>

QUEUE *queue_create(int size, int max)
{
    QUEUE *handle = NULL;

    handle = (QUEUE *)malloc(sizeof(QUEUE));
    ERRP(NULL == handle, goto ERR1, 0);

    handle->size = size;
    handle->max = max;
    handle->end = 0;
    handle->front = 0;

    handle->data = (void *)malloc(size * max);
    ERRP(NULL == handle->data, goto ERR2, 0);

    handle->save = (void *)malloc(size);
    ERRP(NULL == handle->save, goto ERR3, 0);

    return handle;
ERR3:
    free(handle->data);
ERR2:
    free(handle);
ERR1:
    return NULL;
}

void queue_destroy(QUEUE **handle)
{
    QUEUE *val = *handle;

    free(val->save);
    free(val->data);
    free(val);
    *handle = NULL;
}

int queue_len(QUEUE *handle)
{
    if (handle->end > handle->front)
        return handle->end - handle->front;
    else
        return handle->max - (handle->front - handle->end);
}

void queue_travel(queue_op_t *op, QUEUE *handle)
{
    int i, j, len;

    len = queue_len(handle);

    for (i = 0, j = handle->front; i < len; i++, j++)
    {
        j %= handle->max;
        op(handle->data + j * handle->size); 
    }
}

int queue_isempty(QUEUE *handle)
{
    if (handle->end == handle->front)
        return 1;
    return 0;
}

int queue_isfull(QUEUE *handle)
{
    if ((handle->end + 1) % handle->max == handle->front)
        return 1;
    return 0;
}

int queue_en(void *data, QUEUE *handle)
{
    if (queue_isfull(handle))
        return -1;

    memcpy(handle->data + handle->end * handle->size, data, handle->size);
    handle->end++;
    handle->end %= handle->max;

    return 0;
}

void *queue_de(QUEUE *handle)
{
    if (queue_isempty(handle))
        return NULL;

    memcpy(handle->save, handle->data + handle->front * handle->size, handle->size);
    handle->front++;
    handle->front %= handle->max;

    return handle->save;
}

