#include <stdio.h>
#include <stdlib.h>

struct node_t {
    int data;
    struct node_t *next;
};

//有头有序单向不循环链表

int main(void)
{
    struct node_t head = {.next = NULL};
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

        for (tail = &head; tail->next != NULL && tail->next->data < n; tail = tail->next)
            ;

        new->next = tail->next;
        tail->next = new;
    }

    for (tail = head.next; tail != NULL; tail = tail->next)
    {
        printf("%d ", tail->data); 
    }
    printf("\n");

    tail = head.next;
    while (tail != NULL)
    {
        new = tail->next; 
        free(tail);
        tail = new;
    }
    head.next = NULL;

    return 0;
}
