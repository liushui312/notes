#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

typedef void (ktimer_t)(void *);

struct node_t {
    int t;          //定时器设置时间
    int count;      //计数器（记录时间）
    ktimer_t *handle;  //时间到了要执行的函数
    void *data;     //保存执行函数的参数
    int flag;       //记录周期执行或执行一次
    struct node_t *next;
    struct node_t *prev;
}head = {.next = &head, .prev = &head};

void sig_handler(int sig)
{
    struct node_t *tail = head.next, *save = NULL;

    while (tail != &head)
    {
        save = tail->next;
        tail->count++;
        if (tail->count == tail->t)
        {
            tail->handle(tail->data); 
            if (tail->flag)
                tail->count = 0;
            else
            {
                tail->next->prev = tail->prev;
                tail->prev->next = tail->next;
                free(tail);
            }
        }
        tail = save; 
    }

    alarm(1);
}

void exit_timer(void);

void init_timer(void)
{
    head.next = &head;
    head.prev = &head;
    signal(SIGALRM, sig_handler);
    alarm(1);
    atexit(exit_timer);
}

int add_timer(int t, ktimer_t *handle, void *data, int flag)
{
    struct node_t *new = NULL;

    new = (struct node_t *)malloc(sizeof(struct node_t));
    if (new == NULL)
        return -1;
    new->t = t;
    new->count = 0;
    new->handle = handle;
    new->data = data;
    new->flag = flag;

    new->next = &head;
    new->prev = head.prev;
    head.prev->next = new;
    head.prev = new;

    return 0;
}

void exit_timer(void)
{
    struct node_t *tail = head.next, *save = NULL; 

    printf("exit_timer!\n");

    while (tail != &head)
    {
        save = tail->next;
        free(tail);
        tail = save;
    }
    head.next = &head;
    head.prev = &head;
}

void test(void *data)
{
    printf("test %d\n", (int)data);
}

int main(void)
{
    init_timer();

    add_timer(1, test, (void *)1111, 1);
    add_timer(3, test, (void *)3333, 0);
    add_timer(5, test, (void *)55555, 1);
    add_timer(8, test, (void *)88888, 0);
    add_timer(15, test, (void *)1515, 0);

    getchar();

    /*exit_timer();*/

    return 0;
}
