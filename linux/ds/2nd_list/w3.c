#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GETLINES(string, buf)   do                  \
            {                                       \
                printf(string);                     \
                fgets(buf, sizeof(buf), stdin);     \
                if (buf[strlen(buf) - 1] == '\n')   \
                    buf[strlen(buf) - 1] = '\0';    \
                else                                \
                    while (getchar() != '\n')       \
                        continue;                   \
            } while (0)

struct node_t {
    char name[128];
    struct node_t *next;
    struct node_t *prev;
};

int main(void)
{
    int n, m, i, c = 0;
    struct node_t *head = NULL, *tail = NULL, *new = NULL;

    printf("please input m & n : ");
    scanf("%d%d", &m, &n);

    while (getchar() != '\n')
        continue;

    for (i = 0; i < m; i++)
    {
        new = (struct node_t *)malloc(sizeof(struct node_t));
        GETLINES("please input name : ", new->name);
        if (head == NULL)
        {
            new->next = new; 
            new->prev = new;
            head = new;
        }
        else
        {
            new->next = head;
            new->prev = head->prev;
            head->prev->next = new;
            head->prev = new;
        }
    }

    for (tail = head; tail->next != head; tail = tail->next)
    {
        printf("%s ", tail->name); 
    }
    printf("%s\n", tail->name);

    tail = head;
    while (tail->next != tail)
    {
        new = tail->next;
        c++;
        if (c == n)
        {
            printf("%s\n", tail->name);
            tail->next->prev = tail->prev; 
            tail->prev->next = tail->next;
            free(tail);
            c = 0;
        }
        tail = new;
    }
    printf("%s\n", tail->name);
    free(tail);

    return 0;
}
