#ifndef __STACK_H__
#define __STACK_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

struct node_t{
   void *data; 
   struct node_t *next;
   struct node_t *prev;
};

typedef struct stack_t{
    int max;
    int size;
    int end;
    struct node_t head;
}STACK;

typedef void (stack_op_t)(void const *data);

STACK *stack_create(int size, int max);
void stack_destroy(STACK **handle);
void stack_travel(STACK *handle, stack_op_t *op);
int stack_isempty(STACK *handle);
int stack_isfull(STACK *handle);
int stack_push(STACK *handle, void *data);
void *stack_pop(STACK *handle);


#endif /* __STACK_H__ */
