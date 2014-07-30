#include <stdio.h>
#include <stdlib.h>

struct node_t {
    int data;
    struct node_t *prev;
    struct node_t *next;
};

//有头无序双向循环链表

int main(void)
{
    struct node_t head = {.next = &head, .prev = &head};
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

        new->next = &head;
        new->prev = head.prev;
        head.prev->next = new;
        head.prev = new;
    }

    for (tail = head.next; tail != &head; tail = tail->next)
    {
        printf("%d ", tail->data); 
    }
    printf("\n");

    for (tail = head.prev; tail != &head; tail = tail->prev)
    {
        printf("%d ", tail->data); 
    }
    printf("\n");

    tail = head.next;
    while (tail != &head)
    {
        new = tail->next; 
        free(tail);
        tail = new;
    }
    head.next = &head;
    head.prev = &head;

    return 0;
}
