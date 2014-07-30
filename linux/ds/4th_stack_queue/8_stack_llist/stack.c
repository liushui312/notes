#include <stack.h>

STACK *stack_create(int size, int max)
{
    STACK *handle = NULL;

    handle = (STACK *)malloc(sizeof(STACK));
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

void stack_destroy(STACK **handle)
{
    STACK *val = *handle;
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

void stack_travel(stack_op_t *op, STACK *handle)
{
    struct node_t *tail = NULL;

    for (tail = handle->head.next; tail != &handle->head; tail = tail->next)
    {
        op(tail->data);
    }
}

int stack_isempty(STACK *handle)
{
    if (handle->num == 0)
        return 1;
    return 0;
}

int stack_isfull(STACK *handle)
{
    if (handle->num == handle->max)
        return 1;
    return 0;
}

int stack_push(void *data, STACK *handle)
{
    struct node_t *new = NULL;

    if (stack_isfull(handle))
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

void *stack_pop(STACK *handle)
{
    struct node_t *tail = handle->head.prev;

    if (stack_isempty(handle))
        return NULL;

    handle->num--;
    memcpy(handle->save, tail->data, handle->size);
    tail->next->prev = tail->prev;
    tail->prev->next = tail->next;
    free(tail->data);
    free(tail);

    return handle->save;
}

