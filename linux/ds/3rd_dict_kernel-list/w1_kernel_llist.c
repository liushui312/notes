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


#define container_of(ptr, type, member) \
        ((type *)(((char *)ptr) - ((unsigned long)(&(((type *)0)->member)))))

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

void add_node(struct node_t *new , struct node_t *head)
{
    new->next = head;
    new->prev = head->prev;
    head->prev->next = new;
    head->prev = new;
}

int add_course(struct node_t *head)
{
    char buf[1024];
    struct course_t *c = NULL;

    while (1)
    {
        GETLINES("请输入课程名称(END为结束) : ", buf);
        if (!strcasecmp(buf, "end"))
            break;

        c = (struct course_t *)malloc(sizeof(struct course_t));
        c->id = rand() % 100;
        strcpy(c->name, buf);
        c->c_head.next = &c->c_head;
        c->c_head.prev = &c->c_head;

        add_node(&c->sel, head);
    }
}

int add_stu(struct node_t *head, struct node_t *c_head)
{
    struct stu_t *s = NULL;
    struct course_t *c = NULL;
    struct hub_t *h = NULL;
    struct node_t *tail = NULL;
    int ch;

    s = (struct stu_t *)malloc(sizeof(struct stu_t));
    
    GETLINES("请输入学生姓名: ", s->name);
    s->id = rand() % 100;
    s->age = rand() % 10 + 10;
    s->s_head.next = &s->s_head;
    s->s_head.prev = &s->s_head;

    add_node(&s->sel, head);

    for (tail = c_head->next; tail != c_head; tail = tail->next)
    {
        c = container_of(tail, struct course_t, sel);
        printf("\t\t%s(Y/N) : ", c->name);
        ch = getchar();

        while (getchar() != '\n')
            continue;

        if (ch == 'Y' || ch == 'y')
        {
            h = (struct hub_t *)malloc(sizeof(struct hub_t));
            h->s = s;
            h->c = c;
            add_node(&h->s_sel, &s->s_head);
            add_node(&h->c_sel, &c->c_head);
        }
    }
}

void find_stu(struct node_t *head)
{
    struct node_t *tail = NULL, *val = NULL;
    struct stu_t *s = NULL;
    struct hub_t *h = NULL;
    char buf[1024];

    GETLINES("\t请输入查找的学生姓名 : ", buf);

    for (tail = head->next; tail != head; tail = tail->next)
    {
        s = container_of(tail, struct stu_t, sel);
        if (!strcmp(buf, s->name))
        {
            printf("\t\tid = %d, age = %d, name = %s, course: ", s->id, s->age, s->name);

            for (val = s->s_head.next; val != &s->s_head; val = val->next)
            {
                h = container_of(val, struct hub_t, s_sel);
                printf("%s ", h->c->name);
            }
            printf("\n");
            return;
        }
    }

}

void find_course(struct node_t *head)
{
    struct node_t *tail = NULL, *val = NULL;
    struct course_t *c = NULL;
    struct hub_t *h = NULL;
    char buf[1024];

    GETLINES("\t请输入查找的课程名称 : ", buf);

    for (tail = head->next; tail != head; tail = tail->next)
    {
        c = container_of(tail, struct course_t, sel);
        if (!strcmp(buf, c->name))
        {
            printf("\t\tid = %d, name = %s, stu: ", c->id, c->name);

            for (val = c->c_head.next; val != &c->c_head; val = val->next)
            {
                h = container_of(val, struct hub_t, c_sel);
                printf("%s ", h->s->name);
            }
            printf("\n");
            return;
        }
    }
}

void travel_stu(struct node_t *head)
{
    struct node_t *tail = NULL;
    struct stu_t *s = NULL;

    for (tail = head->next; tail != head; tail = tail->next)
    {
        s = container_of(tail, struct stu_t, sel);
        printf("\tid = %d, age = %d, name = %s\n", s->id, s->age, s->name);
    }
}

void travel_course(struct node_t *head)
{
    struct node_t *tail = NULL;
    struct course_t *c = NULL;

    for (tail = head->next; tail != head; tail = tail->next)
    {
        c = container_of(tail, struct course_t, sel);
        printf("\tid = %d, name = %s\n", c->id, c->name);
    }
}

void destroy_stu(struct node_t *head)
{
    struct node_t *tail = NULL, *save = NULL;
    struct stu_t *s = NULL;
    struct node_t *val = NULL, *val_save = NULL;
    struct hub_t *h = NULL;

    for (tail = head->next; tail != head; tail = save)
    {
        save = tail->next;
        s = container_of(tail, struct stu_t, sel);
        for (val = s->s_head.next; val != &s->s_head; val = val_save)
        {
            val_save = val->next;
            h = container_of(val, struct hub_t, s_sel);
            free(h);
        }
        free(s);
    }
}

void destroy_course(struct node_t *head)
{
    struct node_t *tail = NULL, *save = NULL;
    struct course_t *c = NULL;

    for (tail = head->next; tail != head; tail = save)
    {
        save = tail->next;
        c = container_of(tail, struct course_t, sel);
        free(c);
    }
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
        printf("==== 1. 添加学生! ====\n");
        printf("==== 2. 查询学生! ====\n");
        printf("==== 3. 查询课程! ====\n");
        printf("==== 4. 遍历学生! ====\n");
        printf("==== 5. 遍历课程! ====\n");
        printf("==== 6. 退出系统! ====\n");
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
