#ifndef __LLIST_H__
#define __LLIST_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct node_t {
    void *data;
    struct node_t *next;
};

typedef struct llist_t LLIST;

typedef void (llist_op_t)(const void *, void *); 
typedef int (llist_cmp_t)(const void *, const void *);

struct llist_t {
    struct node_t head;
    int size;
    int num;
    LLIST *find;
};

#define DEBUG0(...)
#define DEBUG1(...)     fprintf(stderr, __VA_ARGS__);

#define ERRP(con, ret, flag, ...)   do          \
            {                                   \
                if (con)                        \
                {                               \
                    DEBUG##flag(_VA_ARGS__)     \
                        ret;                    \
                }                               \
            } while(0)                          \
            
#define PREEND      0
#define APPEND      -1  
#define SORT        -2

#define MAGIC       "LLIST"

LLIST *llist_create(int size);
int llist_append(void *data, LLIST *handle);
int llist_prepend(void *data, LLIST *handle);
int llist_insert(void *data, LLIST *handle);
int llist_sort_insert(void *data, llist_cmp_t *cmp, LLIST *handle);
void llist_destroy(LLIST **handle);
void llist_travel(llist_op_t *op, LLIST *handle);
void llist_travel_find(llist_op_t *op, void *args, LLIST *handle);
int llist_num(LLIST *handle);
int llist_size(LLIST *handle);
LLIST *llist_findall(void *key, llist_cmp_t *cmp, LLIST *handle);
void llist_sort(llist_cmp_t, LLIST *handle);
int llist_store(const char *path, LLIST *handle);
LLIST *llist_load(const char *path);

#endif /* __LLIST_H__ */
