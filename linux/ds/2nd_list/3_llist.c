#include <stdio.h>
#include <stdlib.h>

struct node_t {
    int data;
    struct node_t *next;
};

//有头无序单向循环链表

int main(void)
{
    struct node_t head = {.next = &head};
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

        for (tail = &head; tail->next != &head; tail = tail->next)
            ;
        tail->next = new;
    }

    for (tail = head.next; tail != &head; tail = tail->next)
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

    return 0;
}
