#include <darr.h>

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
    DARR *handle = NULL, *find = NULL;
    struct class_t c;
    char buf[1024];
    int len;

    handle = darr_create(sizeof(struct class_t), destroy, store, load);

    while (1)
    {
        printf("please input name : ");
        fgets(buf, sizeof(buf), stdin);
        len = strlen(buf);
        if (buf[len - 1] == '\n')
            buf[len - 1] = '\0';

        if (!strncmp(buf, "exit", 4))
            break;
        c.id = rand() % 100;
        c.name = (char *)malloc(strlen(buf) + 1);
        strcpy(c.name, buf);
        darr_insert(&c, APPEND, handle);
    }

    darr_store("./db", handle);

    darr_travel(ls, NULL, handle);

    while (1)
    {
        printf("please input id : ");
        scanf("%d", &len);
        if (len == -1)
            break;
        find = darr_findall(&len, cmp_id, handle);
        if (find != NULL)
        {
            printf("find : \n");
            darr_travel(ls, find, handle);
        }
    }

    darr_destroy(&handle);

    return 0;
}
