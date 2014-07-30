#ifndef __LLIST_H__
#define __LLIST_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct node_t {
    void *data;
    struct node_t *next;
    struct node_t *prev;
};

typedef struct llist_t LLIST;
typedef void (llist_op_t)(const void *, void *);
typedef void (llist_destroy_t)(const void *);
typedef int (llist_cmp_t)(const void *, const void *);
typedef int (llist_file_t)(FILE *fp, void *);


struct llist_t {
    struct node_t head;
    int size;
    int num;
    LLIST *find;
    llist_destroy_t *destroy;
    llist_file_t *store;
    llist_file_t *load;
};

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

#define PREPEND     0 
#define APPEND      -1
#define SORT        -2

#define MAGIC       "LLIST"

LLIST *llist_create(int size, llist_destroy_t *destroy, llist_file_t *store, llist_file_t *load);
int llist_append(void *data, LLIST *handle);
int llist_prepend(void *data, LLIST *handle);
void llist_travel(llist_op_t *op, void *arg, LLIST *handle);
int llist_sort_insert(void *data, llist_cmp_t *cmp, LLIST *handle);
void llist_travel_find(llist_op_t *op, void *arg, LLIST *handle);
void llist_destroy(LLIST **handle);
int llist_insert(void *data, int index, LLIST *handle);
int llist_num(LLIST *handle);
int llist_size(LLIST *handle);
int llist_delete(void *key, llist_cmp_t *cmp, LLIST *handle);
void *llist_find(void *key, llist_cmp_t *cmp, LLIST *handle);
LLIST *llist_findall(void *key, llist_cmp_t *cmp, LLIST *handle);
void llist_sort(llist_cmp_t *cmp, LLIST *handle);
int llist_store(const char *path, LLIST *handle);
LLIST *llist_load(const char *path, llist_destroy_t *destroy, llist_file_t *store, llist_file_t *load);

#endif /* __LLIST_H__ */
