#include <stdio.h>
#include <stdlib.h>

struct node_t {
    int data;
    struct node_t *next;
    struct node_t *prev;
};

int main(void)
{
    struct node_t *head = NULL;
    struct node_t *new = NULL, *tail = NULL;
    struct node_t **val = NULL;
    int n;

    while (1)
    {
        printf("please input num : ");
        scanf("%d", &n);

        if (n == -1)
            break;

        new = (struct node_t *)malloc(sizeof(struct node_t));
        new->data = n;

        if (head == NULL)
        {
            head = new;
            new->next = new;
            new->prev = new;
        }
        else 
        {
            if (head->data > n)
            {
                new->next = head;
                new->prev = head->prev;
                head->prev->next = new;
                head->prev = new;
                head = new; 
            }
            else
            {
                for (tail = head; tail->next != head && tail->next->data < n; tail = tail->next)
                    ;

                new->next = tail->next;
                new->prev = tail;
                tail->next->prev = new;
                tail->next = new;
            }
        }
    }

    for (tail = head; tail->next != head; tail = tail->next)
    {
        printf("%d ", tail->data); 
    }
    printf("%d\n", tail->data);

    tail = head;
    while (tail->next != head)
    {
        new = tail->next; 
        free(tail);
        tail = new;
    }
    free(tail);

    return 0;
}
