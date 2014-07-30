#include <llist.h>


struct class_t {
    int id;
    char *name;
};

void printf_s(const void *data, void* arg)
{
    struct class_t *s = (struct class_t *)data;
    printf("id = %d, name = %s \n", s->id, s->name);
}

int main()
{
    LLIST *handle = NULL;
    struct class_t c;
    char buf[1024];
    int len;

    handle = llist_create(sizeof(struct class_t), NULL, NULL, NULL);

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
        llist_insert(&c, APPEND, handle);
    }

    llist_travel(printf_s, NULL, handle);

    return 0;
}
