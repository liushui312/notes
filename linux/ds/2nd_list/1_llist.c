#include <stdio.h>
#include <stdlib.h>

struct node_t {
    int data;
    struct node_t *next;
};

/*
 *有头无序单向循环链表
 *无头无序单向循环链表
 *
 *有头有序单向循环链表    
 *无头有序单向循环链表
 *
 *有头无序双向循环链表    *
 *无头无序双向循环链表
 *
 *有头有序双向循环链表
 *无头有序双向循环链表
 *
 *无头无序单向不循环链表
 *有头无序单向不循环链表  *
 *
 *有头有序单向不循环链表
 *无头有序单向不循环链表
 *
 */
/*
 *有头无序双向不循环链表
 *无头无序双向不循环链表
 *有头有序双向不循环链表
 *无头有序双向不循环链表
 */


//有头无序单向不循环链表

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
        new->next = NULL;

        for (tail = &head; tail->next != NULL; tail = tail->next)
            ;

        tail->next = new;
    }

    for (tail = head.next; tail != NULL; tail = tail->next)
    {
        printf("%d ", tail->data); 
    }
    printf("\n");

    for (tail = head.next; tail != NULL; tail = new)
    {
        new = tail->next;
        free(tail); 
    }
    head.next = NULL;

    return 0;
}
