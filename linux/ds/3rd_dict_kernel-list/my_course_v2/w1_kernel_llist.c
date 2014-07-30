#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEBUG0(...)
#define DEBUG1(...)     fprintf(stderr, __VA_ARGS__);

#define ERRP(con, ret, flag, ...)   do              \
            {                                       \
                if (con)                            \
                {                                   \
                    DEBUG##flag(__VA_ARGS__)        \
                    ret;                            \
                }                                   \
            } while (0)

#define GETS(string, op, ...)   do          \
            {                               \
                int ret;                    \
                printf(string);             \
                ret = scanf(__VA_ARGS__);   \
                while (getchar() != '\n')   \
                    continue;               \
                if (ret == 0)               \
                    op;                     \
            } while (0)

#define GETLINES(string, buf)   do                  \
            {                                       \
                printf(string);                     \
                fgets(buf, sizeof(buf), stdin);     \
                if (buf[strlen(buf) - 1] == '\n')   \
                    buf[strlen(buf) - 1] = '\0';    \
                else                                \
                    while (getchar() != '\n')       \
                        continue;                   \
            } while (0)


#define container_of(ptr, type, member) 				\
	((type *)(((void *)ptr) - ((unsigned long)(&(((type *)0)->member)))))

struct node_t {
    struct node_t *next;
    struct node_t *prev;
};

struct stu_t {
    int id;
    int age;
    char name[128];
    struct node_t sel;
    struct node_t s_head;
};

struct course_t {
    int id;
    char name[128];
    struct node_t sel;
    struct node_t c_head;
};

struct hub_t {
    struct stu_t *s;
    struct course_t *c;
    struct node_t s_sel;
    struct node_t c_sel;
};

void add_note(struct node_t *new, struct node_t *head)
{
    new->next = head;
    new->prev = head->prev;
    head->prev->next = new;
    head->prev = new;
}

int add_course(struct node_t *head)
{
    struct course_t *new = NULL;
    int quit = 0;
    char buf[128];
    while(!quit)
    {
        GETLINES("请输入课程名称(end为结束):", buf);
        if (!(strcmp(buf, "end")))
        {
            quit = 1;
            continue;
        }
        new = (struct course_t *)malloc(sizeof(struct course_t));
        ERRP(NULL == new, goto ERR1, 0);
        new->id = rand() % 30;
        memcpy(new->name, buf, sizeof(buf));
        new->c_head.next = &new->c_head;
        new->c_head.prev = &new->c_head;
       
        add_node(&new->sel, head); 
    }

    return 0;
ERR1:
    destroy_course(head);
    return -1;
}

int add_stu(struct node_t *head, struct node_t *c_head)
{
    struct stu_t *new = NULL;
    struct node_t *tail = NULL;
    struct hub_t *new_hub = NULL;
    char buf[128], ch;

    GETLINES("请输入学生姓名:", buf);
    new = (struct stu_t *)malloc(struct stu_t);
    ERRP(NULL == new, goto ERR1, 0);
    memcpy(new->name, buf, sizeof(buf));
    new->id = rand() % 100;
    new->sex = rand() % 2 +'A';
    new->age = rand() % 10 +20;
    
    add_node(&new->sel, head);
    
    for (tail = c_head; tail->next != c_head; tail = tail->next)
    {
        printf("\t\t%s(Y/N)", container_of(tail->next, struct course_t, name));
        GETS("", 0, "%c", &ch);

        if (ch == 'y' || ch == 'Y')
        {
            new_hub = (struct hub_t *)malloc(sizeof(struct hub_t));
            ERRP(NULL == new_hub, goto ERR2, 0);
            new_hub->c = tail->next;
            new_hub->s = new;
            
            new_hub->s_sel->next = new->s_head;
            new_hub->s_sel->prev = new->s_head->prev;
            new->s_head->prev->next = new_hub->s_sel;
            new->s_head->prev = new_hub->s_sel;

            new_hub->c_sel->next = tail->next->c_head;
            new_hub->c_sel->prev = tail->next->c_head->prev;
            new->c_head->prev->next = new_hub->c_sel;
            new->c_head->prev = new_hub->c_sel;

        }

    }
}

void find_stu(struct node_t *head)
{

}

void find_course(struct node_t *head)
{

}

void travel_stu(struct node_t *head)
{
    struct node_t *tail = NULL:
    
    for(tail = head->next; tail != &head; tail = tail->next)
    {
        c = container_of(tail, struct stu_t, );
    }
}

void travel_course(struct node_t *head)
{

}

void destroy_stu(struct node_t *head)
{

}

void destroy_course(struct node_t *head)
{

}

int main(void)
{
    int n, quit = 0;
    struct node_t stu_head = {.next = &stu_head, .prev = &stu_head};
    struct node_t course_head = {.next = &course_head, .prev = &course_head};

    add_course(&course_head);

    while (!quit)
    {
START:
        printf("==== 排课查询系统 ====\n");
        printf("==== 1. 添加学生!\n ====\n");
        printf("==== 2. 查询学生!\n ====\n");
        printf("==== 3. 查询课程!\n ====\n");
        printf("==== 4. 遍历学生!\n ====\n");
        printf("==== 5. 遍历课程!\n ====\n");
        printf("==== 6. 退出系统!\n ====\n");
        GETS("请输入 [ 1 - 6 ] : ", goto START, "%d", &n);

        switch (n)
        {
            case 1:
                add_stu(&stu_head, &course_head);
                break;
            case 2:
                find_stu(&stu_head);
                break;
            case 3:
                find_course(&course_head);
                break;
            case 4:
                travel_stu(&stu_head);
                break;
            case 5:
                travel_course(&course_head);
                break;
            case 6:
                quit = 1;
                break;
            default:
                break;
        }
    }

    destroy_stu(&stu_head);
    destroy_course(&course_head);

    return 0;
}
