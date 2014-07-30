#ifndef __KYO_QUEUE_H__
#define __KYO_QUEUE_H__

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

typedef void (queue_op_t)(const void *);

typedef struct queue_t {
    void *data;
    int size;
    int max;
    int end;
    int front;
    void *save;
}QUEUE;

QUEUE *queue_create(int size, int max);
void queue_destroy(QUEUE **handle);
void queue_travel(queue_op_t *op, QUEUE *handle);
int queue_isempty(QUEUE *handle);
int queue_isfull(QUEUE *handle);
int queue_len(QUEUE *handle);
int queue_en(void *data, QUEUE *handle);
void *queue_de(QUEUE *handle);

#endif /* __KYO_QUEUE_H__ */
