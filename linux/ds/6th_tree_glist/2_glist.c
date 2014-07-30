#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct node_t {
    char en;
    char *cn;
    struct node_t *sublist;
    struct node_t *next;
};

int insert(char *en, char *cn, struct node_t **p)
{
    struct node_t *new = NULL;
    char *val = NULL;

    val = malloc(strlen(cn) + 1);
    strcpy(val, cn);

    while (*p != NULL)
    {
        if (*en == (*p)->en)
        {
            if (*(en + 1) == '\0')
                break;
            p = &(*p)->sublist;
            en++;
        }
        else
            p = &(*p)->next;
    }

    if (*p != NULL)
        (*p)->cn = val;
    else
    {
        while (*en != '\0')
        {
            new = (struct node_t *)malloc(sizeof(struct node_t));
            new->en = *en;
            new->cn = NULL;
            new->sublist = NULL;
            new->next = NULL;
            *p = new;
            p = &new->sublist;

            en++; 
        }

        new->cn = val;
    }

    return 0;
}

char *find(const char *en, struct node_t *head)
{
    while (head != NULL)
    {
        if (*en == head->en)
        {
            if (*(en + 1) == '\0')
                break;
            head = head->sublist;
            en++;
        }
        else
            head = head->next;
    }

    if (head != NULL)
        return head->cn;
    else
        return NULL;
}

int main(void)
{
    struct node_t *head = NULL;

    insert("hello", "你好", &head);
    insert("hell", "地狱", &head);
    insert("world", "世界", &head);
    insert("werld", "好", &head);

    printf("find = %s\n", find("hello", head));
    printf("find = %s\n", find("hell", head));
    printf("find = %s\n", find("world", head));
    printf("find = %s\n", find("werld", head));

    return 0;
}
