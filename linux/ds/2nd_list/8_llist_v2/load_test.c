#include <llist.h>

struct class_t {
    int id;
    char *name;
};

int cmp_id(const void *d1, const void *d2)
{
    return *(int *)d1 - ((struct class_t *)d2)->id;
}

int cmp_name(const void *d1, const void *d2)
{
    return strcmp((char *)d1, ((struct class_t *)d2)->name);
}

void ls(const void *data, void *arg)
{
    struct class_t *c = (struct class_t *)data;

    printf("id = %d, name = %s\n", c->id, c->name);
}

void destroy(const void *data)
{
    free(((struct class_t *)data)->name);
}

int store(FILE *fp, void *data)
{
    struct class_t *c = (struct class_t *)data;
    int len = strlen(c->name);

    fwrite(&len, sizeof(int), 1, fp);
    fwrite(c->name, sizeof(char), len, fp);
    fwrite(&c->id, sizeof(c->id), 1, fp);

    return 0;
}

int load(FILE *fp, void *data)
{
    struct class_t *c = (struct class_t *)data;
    int len;

    fread(&len, sizeof(int), 1, fp);
    c->name = (char *)malloc(len + 1);
    fread(c->name, sizeof(char), len, fp);
    fread(&c->id, sizeof(c->id), 1, fp);

    return 0;
}

int main(void)
{
    LLIST *handle = NULL, *find = NULL;
    int len;

    handle = llist_load("./db", destroy, store, load);
    if (handle == NULL)
    {
        printf("llist_load!\n");
        return 0;
    }

    llist_travel(ls, NULL, handle);

    while (1)
    {
        printf("please input id : ");
        scanf("%d", &len);
        if (len == -1)
            break;
        find = llist_findall(&len, cmp_id, handle);
        if (find != NULL)
        {
            printf("find : \n");
            llist_travel_find(ls, NULL, find);
        }
    }

    llist_destroy(&handle);

    return 0;
}
