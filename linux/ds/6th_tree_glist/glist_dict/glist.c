#include <stdlib.h>

struct node_t {
    char en;
    char *cn;
    struct node_t *next;
    struct node_t *sublist;
};

void insert(char *en, char *cn, struct node_t **p)
{
    struct node_t *new = NULL;
    char *val = (char *)malloc(strlen(cn) + 1);

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
    {
        (*p)->cn = val;
    }
    else
    {
        while (*en != '\0')
        {
            new = (struct node_t *)malloc(sizeof(struct node_t));
            new->en = *en;
            new->cn = NULL;
            new->next = NULL;
            new->sublist = NULL;
            *p = new;
            p = &new->sublist;
            en++;
        }
        new->cn = val;
    }
}

char *find(char *en, struct node_t *head)
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
    
    if (head == NULL)
        return NULL;
    return head->cn;
}

void travel(struct node_t *head)
{
    static int level = 0;
    int i;

    while (head != NULL)    
    {
        for (i = 0; i < level; i++)
        {
            printf("\t");  
        }
        printf("%c%s\n", head->en, head->cn);
        level++;
        travel(head->sublist);
        level--;
        head = head->next; 
    }
}

void destroy(struct node_t *head)
{
    struct node_t *save = NULL;

    while (head != NULL)    
    {
        save = head->next;
        destroy(head->sublist);
        free(head->cn);
        free(head);
        head = save;
    }
}

int main(void)
{
    struct node_t *head = NULL;

    insert("hello", "你好", &head);
    insert("world", "世界", &head);
    insert("worft", "界", &head);
    insert("hell", "地狱", &head);

    travel(head);

    printf("find = %s\n", find("hell", head));

    destroy(head);
    
    return 0;
}
