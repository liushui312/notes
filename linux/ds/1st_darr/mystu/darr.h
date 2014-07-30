#ifndef __DARR_H__
#define __DARR_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct darr_t DARR;

typedef void (darr_op_t)(const void *, void *);
typedef void (darr_destroy_t)(const void *);
typedef int (darr_cmp_t)(const void *, const void *);
typedef int (darr_file_t)(FILE *fp, void *);

struct darr_t {
    void *data; 
    int num;
    int size;
    DARR *find;
    darr_destroy_t *destroy;
    darr_file_t *store;
    darr_file_t *load;
};

#define DEBUG0(...)
#define DEBUG1(...)     fprintf(stderr, _VA_ARGS__);

#define ERRP(con, ret, flag, ...)   do              \
            {                                       \
                if (con)                            \
                {                                   \
                    DEBUG##flag(__VA_ARGS__)        \
                    ret;                            \
                }                                   \
            } while (0)

#define APPEND      -1
#define PREPEND     -2

#define MAGIC       "DARR"

DARR *darr_create(int size, darr_destroy_t *destroy, darr_file_t *store, darr_file_t *load);
int darr_append(void *data, DARR *handle);
void darr_travel(darr_op_t *op, void *arg, DARR *handle);
void darr_destroy(DARR **handle);
int darr_prepend(void *data, DARR *handle);
int darr_insert(void *data, int index, DARR *handle);
int darr_ind_del(int index, DARR *handle);
void *darr_ind(int index, DARR *handle);
int darr_num(DARR *handle);
int darr_size(DARR *handle);
int darr_delete(void *key, darr_cmp_t *cmp, DARR *handle);
void *darr_find(void *key, darr_cmp_t *cmp, DARR *handle);
DARR *darr_findall(void *key, darr_cmp_t *cmp, DARR *handle);
void darr_sort(darr_cmp_t *cmp, DARR *handle);
int darr_store(const char *path, DARR *handle);
DARR *darr_load(const char *path, darr_destroy_t *destroy, darr_file_t *store, darr_file_t *load);

#endif /* __DARR_H__ */
