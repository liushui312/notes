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
typedef int (llist_store_t)(const void *, FILE *fp);
typedef int (llist_load_t)(void *, FILE *fp);

struct llist_t {
    int size;
    int num;
    struct node_t head;
    LLIST *find;
    llist_destroy_t *destroy;
    llist_store_t *store;
};

#define DEBUG0(...)
#define DEBUG1(...)     fprintf(stderr, __VA_ARGS__);

#define ERRP(con, ret, flag, ...)       do          \
            {                                       \
                if (con)                            \
                {                                   \
                    DEBUG##flag(__VA_ARGS__)        \
                    ret;                            \
                }                                   \
            } while (0)

#define MAGIC       "LLIST"

extern LLIST *llist_create(int size, llist_destroy_t *destroy, llist_store_t *store);
extern void llist_set(llist_destroy_t *destroy, llist_store_t *store, LLIST *handle);
extern int llist_insert_sort(void *data, llist_cmp_t *cmp, LLIST *handle);
extern int llist_insert_append(void *data, LLIST *handle);
extern int llist_insert_prepend(void *data, LLIST *handle);
extern int llist_delete(void *key, llist_cmp_t *cmp, LLIST *handle);
extern int llist_delete_all(void *key, llist_cmp_t *cmp, LLIST *handle);
extern void *llist_find(void *key, llist_cmp_t *cmp, LLIST *handle);
extern LLIST *llist_find_all(void *key, llist_cmp_t *cmp, LLIST *handle);
extern void llist_travel(llist_op_t *op, void *arg, LLIST *handle);
extern void llist_find_travel(llist_op_t *op, void *arg, LLIST *handle);
extern int llist_sort(llist_cmp_t *cmp, LLIST *handle);
extern int llist_store(const char *path, LLIST *handle);
extern LLIST *llist_load(const char *path, llist_load_t *load);
extern void llist_destroy(LLIST **handle);

#endif /* __LLIST_H__ */
