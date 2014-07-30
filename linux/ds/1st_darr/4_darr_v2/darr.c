#include <darr.h>

DARR *darr_create(int size)
{
    DARR *handle = NULL;

    handle = (DARR *)malloc(sizeof(DARR));
    ERRP(NULL == handle, goto ERR1, 0);


    handle->data = NULL;
    handle->num = 0;
    handle->size = size;
    handle->find = NULL;

    return handle;
ERR1:
    return NULL;
}

void darr_travel(darr_op_t *op, void *arg, DARR *handle)
{
    int i;
    void *val = NULL;

    if (handle->find != NULL && handle->find == arg)
    {
        for (i = 0; i < handle->find->num; i++)
        {
            val = handle->find->data + handle->find->size * i;
            op((void *)(*(char **)val), NULL);
        }
    }
    else
    {
        for (i = 0; i < handle->num; i++)
        {

            op(handle->data + handle->size * i, arg);
        }
    }
}

int darr_insert(void *data, int index, DARR *handle)
{
    void *new = NULL;

    if (index <= PREPEND)
        index = 0;
    if (index == APPEND || index > handle->num)
        index = handle->num;

    new = (void *)malloc((handle->num + 1) * handle->size);
    ERRP(NULL == new, goto ERR1, 0);
    
    memcpy(new, handle->data, index * handle->size);
    memcpy(new + index * handle->size, data, handle->size);

    memcpy(new + (index + 1) * handle->size, 
            handle->data + index * handle->size,
            (handle->num - index) * handle->size);

    free(handle->data);
    handle->data = new;

    handle->num++;

    return 0;
ERR1:
    return -1;
}

int darr_ind_del(int index, DARR *handle)
{
    void *new = NULL;

    if (index < 0)
        index = 0;
    if (index > handle->num)
        index = handle->num;

    new = (void *)malloc((handle->num - 1) * handle->size);
    ERRP(NULL == new, goto ERR1, 0);
    
    memcpy(new, handle->data, index * handle->size);
    memcpy(new + index * handle->size, 
            handle->data + (index + 1) * handle->size,
            (handle->num - index - 1) * handle->size);

    free(handle->data);
    handle->data = new;

    handle->num--;

    return 0;
ERR1:
    return -1;
}

void *darr_ind(int index, DARR *handle)
{
    if (index < 0 || index >= handle->num)
        return NULL;
    return handle->data + index * handle->size;
}

int darr_num(DARR *handle)
{
    return handle->num;
}

int darr_size(DARR *handle)
{
    return handle->size;
}

int darr_delete(void *key, darr_cmp_t *cmp, DARR *handle)
{
    int i;
    int c = 0;

    for (i = 0; i < handle->num; i++)
    {
        if (!cmp(key, handle->data + i * handle->size))
        {
            darr_ind_del(i, handle); 
            c++;
            i--;
        }
    }

    return c;
}

void *darr_find(void *key, darr_cmp_t *cmp, DARR *handle)
{
    int i;
    void *val = NULL;

    for (i = 0; i < handle->num; i++)
    {
        val = handle->data + i * handle->size;
        if (!cmp(key, val))
        {
            return val;
        }
    }

    return NULL;
}

DARR *darr_findall(void *key, darr_cmp_t *cmp, DARR *handle)
{
    int i;
    void *val = NULL;

    if (handle->find != NULL)
        darr_destroy(&handle->find);

    handle->find = darr_create(sizeof(void *));
    ERRP(NULL == handle->find, goto ERR1, 0);

    for (i = 0; i < handle->num; i++)
    {
        val = handle->data + i * handle->size;
        if (!cmp(key, val))
            darr_insert(&val, APPEND, handle->find);
    }
    return handle->find;
ERR1:
    return NULL;
}

void darr_sort(darr_cmp_t *cmp, DARR *handle)
{
    int i, j;
    void *v1 = NULL, *v2 = NULL;
    void *val = NULL;

    val = (void *)malloc(handle->size);

    for (i = 0; i < handle->num - 1; i++)
    {
        v1 = handle->data + i * handle->size;
        for (j = i + 1; j < handle->num; j++)
        {
            v2 = handle->data + j * handle->size;
            if (cmp(v1, v2) > 0)
            {
                memcpy(val, v1, handle->size);
                memcpy(v1, v2, handle->size);
                memcpy(v2, val, handle->size);
            }
        }
    }
    free(val);
}

int darr_store(const char *path, DARR *handle)
{
    FILE *fp = NULL;
    int ret;

    fp = fopen(path, "w");
    ERRP(NULL == fp, goto ERR1, 0);

    ret = fwrite(MAGIC, 1, sizeof(MAGIC), fp);
    ERRP(ret != sizeof(MAGIC), goto ERR2, 0);

    ret = fwrite(&handle->num, sizeof(handle->num), 1, fp);
    ERRP(ret != 1, goto ERR2, 0);
    ret = fwrite(&handle->size, sizeof(handle->size), 1, fp);
    ERRP(ret != 1, goto ERR2, 0);
    ret = fwrite(handle->data, handle->size, handle->num, fp);
    ERRP(ret != handle->num, goto ERR2, 0);

    fclose(fp);

    return 0;
ERR2:
    fclose(fp);
ERR1:
    return -1;
}

DARR *darr_load(const char *path)
{
    FILE *fp = NULL;
    DARR *handle = NULL;
    int num, size, ret;
    char buf[128];

    fp = fopen(path, "r");
    ERRP(NULL == fp, goto ERR1, 0);

    ret = fread(buf, 1, sizeof(MAGIC), fp);
    ERRP(ret != sizeof(MAGIC) || strcmp(MAGIC, buf), goto ERR2, 0);

    ret = fread(&num, sizeof(num), 1, fp);
    ERRP(ret != 1, goto ERR2, 0);

    ret = fread(&size, sizeof(size), 1, fp);
    ERRP(ret != 1, goto ERR2, 0);

    handle = (DARR *)malloc(sizeof(DARR));
    ERRP(NULL == handle, goto ERR2, 0);
    
    handle->num = num;
    handle->size = size;
    
    handle->data = (void *)malloc(num * size);
    ERRP(NULL == handle->data, goto ERR3, 0);

    ret = fread(handle->data, handle->size, handle->num, fp);
    ERRP(ret != handle->num, goto ERR4, 0);

    fclose(fp);

    return handle;
ERR4:
    free(handle->data);
ERR3:
    free(handle);
ERR2:
    fclose(fp);
ERR1:
    return NULL;
}

void darr_destroy(DARR **handle)
{
    DARR *val = *handle;

    if (val->find != NULL)
        darr_destroy(&(val->find));
    free(val->data);
    free(val);

    *handle = NULL;
}
