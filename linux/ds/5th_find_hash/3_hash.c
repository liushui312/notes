#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node_t {
    int data;
    struct node_t *next;
};

#define MAX     10

int a[MAX];

void rand_a(void)
{
    int i;

    for (i = 0; i < MAX; i++)
    {
        a[i] = rand() % 1000;
    }
}

void show(void)
{
    int i;

    for (i = 0; i < MAX; i++)
    {
        printf("%d ", a[i]); 
    }
    printf("\n");
}

int hash(int data)
{
    return data % MAX;
}

int main(int argc, char *argv[])
{
    int n, i, index;
    struct node_t head[MAX];
    struct node_t *new = NULL, *tail = NULL;

    rand_a();
    show();

    memset(head, 0, sizeof(head));

    for (i = 0; i < MAX; i++)
    {
        index = hash(a[i]);
        new = (struct node_t *)malloc(sizeof(struct node_t));
        new->data = a[i];
        new->next = NULL;
        for (tail = &head[index]; tail->next != NULL; tail = tail->next)
            ;
        tail->next = new;
    }

    for (i = 0; i < MAX; i++)
    {
        printf("head[%d] : ", i);
        for (tail = head[i].next; tail != NULL; tail = tail->next)
        {
            printf("%d ", tail->data); 
        }
        printf("\n");
    }

    printf("please input key : ");
    scanf("%d", &n);

    index = hash(n);

    for (tail = head[index].next; tail != NULL; tail = tail->next)
    {
        if (tail->data == n)
            printf("find = %d\n", n);
    }

    return 0;
}
