#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>


typedef void (action_t)(void *);

struct node_t {
    int id;
    int time;
    int flag;
    int count;
    action_t *action;
    void *arg;
    struct node_t *next;
    struct node_t *prev;
};

struct timer_t {
    struct node_t head;
    int num;
};

struct timer_t timer;

void init_timer(void);
int add_timer(int time, action_t *handler, void *arg,  int flag);
int del_timer(int id);
void exit_timer(void);
void sig_handler(int s);

void init_timer(void)
{
    timer.num = 0;
    timer.head.next = &timer.head;
    timer.head.prev = &timer.head;

    signal(SIGALRM, sig_handler);
}

void sig_handler(int s)
{
    struct node_t *tail = NULL;

    for (tail = timer.head.next; tail != &timer.head; tail = tail->next)
    {
         tail->count++;
         if (tail->count % tail->time == 0)
            tail->action(tail->arg); 

         if (tail->count > tail->time && tail->flag == 1)
             del_timer(tail->id);
    }
    
    alarm(1);
}

int add_timer(int time, action_t *handler, void *arg, int flag)
{
    struct node_t *new = NULL, *tail = NULL;
    
    new = (struct node_t *)malloc(sizeof(struct node_t));

    new->arg = arg; 

    timer.num++;
    new->time = time;
    new->action = handler;
    new->id = timer.num;
    new->flag = flag;
    
    new->next = &timer.head;
    new->prev = timer.head.prev;
    timer.head.prev->next = new; 
    timer.head.prev = new;

    alarm(1);
    return new->id;
}

int del_timer(int id)
{
    struct node_t *tail = NULL;

    for (tail = &timer.head; tail->next != &timer.head; tail = tail->next)
    {
         if (tail->id == id)
         {
            tail->prev->next = tail->next; 
            tail->next->prev = tail->prev;
            free(tail);
            return 0;
         }
    }

    return -1;
}

void exit_timer(void)
{
    struct node_t *tail = NULL, *save = NULL;

    tail = timer.head.next;
    while (tail != &timer.head)
    {
        save = tail->next;
        free(tail);
        tail = save;
    }
    timer.head.next = &timer.head;
    timer.head.prev = &timer.head;
}

/*==========test===================*/

struct stu_t {
    int id;
    char name[20];
};

void sig_hander1(void *arg)
{
    struct stu_t *s;
    s = (struct stu_t *)arg;
    printf("timer1 = 3! === id = %d, name = %s ===== \n", s->id, s->name);
}

void sig_hander2(void *arg)
{
    int *n; 
    n = (int *)arg;
    printf("timer2 = 5! ==== %d ====\n", *n);
}

void sig_hander3(void *arg)
{
    printf("timer3 = 8!\n");
}

void sig_hander4(void *arg)
{
    printf("timer1 = 1!\n");
}


int main(void)
{
    struct stu_t s = {.id = 12, .name = "liu"};
    int n = 333;

    init_timer();
    add_timer(3, sig_hander1, &s, 0);
    add_timer(5, sig_hander2, &n, 0);
    add_timer(8, sig_hander3, NULL, 1);
    add_timer(1, sig_hander4, NULL, 0);

    if (getchar() == '\n')
        del_timer(1);
    getchar();
    
    exit_timer();
    return 0;
}





