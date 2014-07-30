#ifndef __QUEUE_H__
#define __QUEUE_H__

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct queue_darr{
    int max;
    int size;
    void *data;
    int front;
    int end;
    void *save;
}QUEUE; 

typedef void (queue_op_t)(const void *);

QUEUE *queue_create(int size, int max);
void queue_destroy(QUEUE **handle);
void queue_travel(QUEUE *handle, queue_op_t *op);
int queue_isempty(QUEUE *handle);
int queue_isfull(QUEUE *handle);
int queue_en(QUEUE *handle, void *data);
void *queue_de(QUEUE *handle);

#endif /*__QUEUE_H__*/
