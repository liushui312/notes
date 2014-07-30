#include <stack.h>

STACK *stack_create(int size, int max)
{
    STACK *handle = NULL;

    handle = (STACK *)malloc(sizeof(STACK));
    ERRP(NULL == handle, goto ERR1, 0);

    handle->size = size;
    handle->max = max;
    handle->end = 0;

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

void stack_destroy(STACK **handle)
{
    STACK *val = *handle;

    free(val->save);
    free(val->data);
    free(val);
    *handle = NULL;
}

void stack_travel(stack_op_t *op, STACK *handle)
{
    int i;

    for (i = 0; i < handle->end; i++)
    {
        op(handle->data + i * handle->size); 
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

int stack_push(void *data, STACK *handle)
{
    if (stack_isfull(handle))
        return -1;

    memcpy(handle->data + handle->end * handle->size, data, handle->size);
    handle->end++;

    return 0;
}

void *stack_pop(STACK *handle)
{
    if (stack_isempty(handle))
        return NULL;

    handle->end--;
    memcpy(handle->save, handle->data + handle->end * handle->size, handle->size);

    return handle->save;
}

