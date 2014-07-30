#include <queue.h>

QUEUE *queue_create(int size, int max)
{
    QUEUE *handle = NULL;
    
    handle = (QUEUE *)malloc(sizeof(QUEUE));
    ERRP(NULL == handle, goto ERR1, 0);
    
    handle->size = size;
    handle->max = max;
    handle->front = 0;
    handle->end = 0;

    handle->data = (void *)malloc(sizeof(handle->size) * max);
    ERRP(NULL == handle->data, goto ERR2, 0);

    handle->save = (void *)malloc(sizeof(handle->size));
    ERRP(NULL == handle->data, goto ERR3, 0);
    
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

    free(val->data);
    free(val->save);
    free(val);

    *handle = NULL;
}

void queue_travel(QUEUE *handle, queue_op_t *op)
{    
    int i, j, len;

    if(handle->end >= handle->front)
        len = handle->end - handle->front;
    else 
        len = handle->max - (handle->front - handle->end);

    for (i = 0, j = handle->front; i < len; i++, j++) 
    {
        j %= handle->max;
        op(handle->data + j * handle->size);
    }
}

int queue_isempty(QUEUE *handle)
{
    if (handle->front == handle->end)
        return 1;
    return 0;
}

int queue_isfull(QUEUE *handle)
{
    if ((handle->end + 1) % handle->max == handle->front)
        return 1;
    return 0;
}

int queue_en(QUEUE *handle, void *data)
{
    if (queue_isfull(handle))
        return -1;
    handle->end %= handle->max;
    memcpy(handle->data + handle->end * handle->size, data, handle->size);
    return 0;
}

void *queue_de(QUEUE *handle)
{
    if (queue_isempty(handle))
        return NULL;
    handle->front %= handle->max;
    memcpy(handle->save, handle->data + handle->front * handle->size, handle->size);
    return handle->save;
}


