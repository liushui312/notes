#include <stdio.h>
#include <stdlib.h>

struct node_t {
    int data;
    struct node_t *next;
};

//无头无序单向不循环链表

int main(void)
{
    struct node_t *head = NULL;
    struct node_t *new = NULL, *tail = NULL;
    int n;

    while (1)
    {
        printf("please input num : ");
        scanf("%d", &n);

        if (n == -1)
            break;

        new = (struct node_t *)malloc(sizeof(struct node_t));
        new->data = n;
        new->next = NULL;

        if (head == NULL)
            head = new;
        else 
        {
            for (tail = head; tail->next != NULL; tail = tail->next)
                ;
            tail->next = new;
        }
    }

    for (tail = head; tail != NULL; tail = tail->next)
    {
        printf("%d ", tail->data); 
    }
    printf("\n");

    tail = head;
    while (tail != NULL)
    {
        new = tail->next; 
        free(tail);
        tail = new;
    }
    head = NULL;

    return 0;
}
