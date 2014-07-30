#ifndef __KYO_STACK_H__
#define __KYO_STACK_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEBUG0(...)
#define DEBUG1(...)     fprintf(stderr, __VA_ARGS__);

#define ERRP(con, ret, flag, ...)   do              \
            {                                       \
                if (con)                            \
                {                                   \
                    DEBUG##flag(__VA_ARGS__)        \
                    ret;                            \
                }                                   \
            } while (0)

typedef void (stack_op_t)(const void *);

struct node_t {
    void *data;
    struct node_t *next;
    struct node_t *prev;
};

typedef struct stack_t {
    struct node_t head;
    int size;
    int max;
    int num;
    void *save;
}STACK;

STACK *stack_create(int size, int max);
void stack_destroy(STACK **handle);
void stack_travel(stack_op_t *op, STACK *handle);
int stack_isempty(STACK *handle);
int stack_isfull(STACK *handle);
int stack_push(void *data, STACK *handle);
void *stack_pop(STACK *handle);

#endif /* __KYO_STACK_H__ */
