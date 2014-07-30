#include <stdio.h>

struct node_t {
    struct node_t *next;
};

struct A {
    int a;
    char name[128];
    struct node_t sel;
};

struct B {
    int a;
    int b;
    struct node_t sel;
};

struct C {
    int a;
    char b;
    int c;
    struct node_t sel;
};

#define container_of(ptr, type, member) 				\
	((type *)(((void *)ptr) - ((unsigned long)(&(((type *)0)->member)))))

void add_node(struct node_t *new, struct node_t *head)
{
    struct node_t *tail = NULL;

    for (tail = head; tail->next != NULL; tail = tail->next)
        ;
    
    new->next = tail->next;
    tail->next = new;
}

void test(struct node_t *head)
{
    /*struct A *p = container_of(head->next, struct A, sel);*/
    /*printf("a = %d, name = %s\n", p->a, p->name);*/
    struct B *p = container_of(head->next->next, struct B, sel);
    printf("a = %d, b = %d\n", p->a, p->b);
}

int main(void)
{
    struct A a = {11, "hello"};
    struct B b = {22, 33};
    struct C c = {1, 2, 3};
    struct node_t head = {.next = NULL};

    add_node(&a.sel, &head);
    add_node(&b.sel, &head);
    add_node(&c.sel, &head);

    test(&head);

    return 0;
}
