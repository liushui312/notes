#include <stack.h>


STACK *stack_create(int size, int max)
{
    STACK *handle = NULL;
    
    handle = (STACK *)malloc(sizeof(STACK));
    ERRP(NULL == handle, goto ERR1, 0);
    handle->size = size;
    handle->max = max;
    handle->end = 0;
    handle->head.next = &handle->head;
    handle->head.prev = &handle->head;

    return handle;

ERR1:
    return NULL;
}

void stack_destroy(STACK **handle)
{
    struct node_t *tail = NULL, *save = NULL;
    STACK *val = *handle; 

    tail = val->head.next;
    while (tail != &val->head)
    {
        save = tail->next;
        free(tail);
        tail = save;
    }

    free(val);
    *handle = NULL;
}

void stack_travel(STACK *handle, stack_op_t *op)
{
    struct node_t *tail = NULL;

    if(stack_isempty(handle))
        return;
    for (tail = &handle->head; tail->next != &handle->head; tail = tail->next)
    {
        op(tail->next->data);
    }
    
}

int stack_isempty(STACK *handle)
{
    if (handle->end == 0)
        return 1;
    return 0;        
}

int stack_isfull(STACK *handle)
{
    if (handle->end == handle->max)
        return 1;
    return 0;
}

int stack_push(STACK *handle, void *data)
{
    struct node_t *new = NULL;
    
    if (!stack_isfull(handle))
    {
        new = (struct node_t *)malloc(sizeof(struct node_t));
        ERRP(NULL == new, goto ERR1, 0);
        new->data = (void *)malloc(handle->size);
        ERRP(NULL == new->data, goto ERR2, 0);
        memcpy(new->data, data, handle->size);

        handle->end++;

        new->next = &handle->head;
        new->prev = handle->head.prev;
        handle->head.prev->next = new;
        handle->head.prev = new;
    }
    else 
        return -1;

    return 0;
ERR2:
    free(new);
ERR1:
    return -1;
}

void *stack_pop(STACK *handle)
{
    void *data = NULL;
    struct node_t *save = NULL;

    if (!stack_isempty(handle))
    {
        data = (void *)malloc(handle->size);
        ERRP(NULL == data, goto ERR1, 0);
        memcpy(data, handle->head.prev->data, handle->size);

        save = handle->head.prev;
        handle->head.prev->prev->next = &handle->head;
        handle->head.prev = handle->head.prev->prev;
        free(save);
    }
    else 
        return NULL;

    return data;
ERR1:
    return NULL;
}


