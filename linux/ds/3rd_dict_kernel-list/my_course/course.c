#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STU_MAX     100
#define COURSE_MAX  20

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

#define DEBUG0(...)
#define DEBUG1(...)     fprintf(stderr, _VA_ARGS__);

#define ERRP(con, ret, flag, ...)   do              \
            {                                       \
                if (con)                            \
                {                                   \
                    DEBUG##flag(__VA_ARGS__)        \
                    ret;                            \
                }                                   \
            } while (0)

struct stu_t;

struct  course_t {
    char *c_name;
    int id;
    struct stu_t *stu[STU_MAX];
    struct course_t *next;
};

struct stu_t{
    char *s_name;
    int age;
    char sex;
    struct course_t *course[COURSE_MAX];
    struct stu_t *next;
};

struct course_t course_head = {.next = NULL, .stu = NULL};
struct stu_t stu_head = {.next = NULL, .course = NULL};

struct stu_t *add_stu(char *s_name)
{
    struct stu_t *new = NULL, *tail = NULL;
    int len;
    len = strlen(s_name);
    if (len != 0)
    {
        new = (struct stu_t *)malloc(sizeof(struct stu_t));
        ERRP(NULL == new, goto ERR1, 0);
        new->s_name = (char *)malloc(len);
        ERRP(NULL == new->s_name, goto ERR2, 0);
        memcpy(new->s_name, s_name, len);
        new->age = rand() % 10 + 10;
        new->sex = rand() % 2 + 'A';

        for (tail = &stu_head; tail->next != NULL; tail = tail->next)
            ;
        tail->next = new;
    }
    else
        return NULL;
    return new;

ERR2:
    free(new);
ERR1:
    return NULL;
}

int select_course(struct stu_t *stu)
{
    struct course_t *tail = NULL;
    char ch;
    int i = 0, ret;

    for (tail = &course_head; tail->next != NULL; tail = tail->next)
    {
again:        
        printf("\t\t%s(Y/N)\n", tail->next->c_name);    
        ret = scanf("%c", &ch);
        ERRP(ret != 1, goto again, 0);  

        if (ch == 'y' || ch == 'Y')
        {
            stu->course[i] = tail;
            i++;
        }
    }

    return 0;
}

struct stu_t *find_stu(char *s_name)
{
    struct stu_t *tail = NULL;

    for(tail = &stu_head; tail->next != NULL; tail = tail->next)
    {
        if (!(strcmp(tail->s_name, s_name)))
            return tail;
    }
    return NULL;
}

void printf_stu(struct stu_t *stu)
{
    int i = 0;
    printf("name = %s, sex = %c, age = %d class: ", stu->s_name, stu->sex, stu->age);
    while(stu->course[i++] != NULL)
        printf("%s ", stu->course[i]->c_name);
    printf("\n");
}

struct course_t *find_course(char * c_name)
{
    struct course_t *tail = NULL;

    for(tail = &course_head; tail->next != NULL; tail = tail->next)
    {
        if (!(strcmp(tail->c_name, c_name)))
            return tail;
    }
    return NULL;

}

void printf_course(struct course_t *course)
{
    int i = 0;
    printf("id = %d, name = %s, stu: ", course->id, course->c_name);
    while(course->stu[i++] != NULL)
        printf("%s ", course->stu[i]->s_name);
    printf("\n");
}

int main(void)
{
    int  quit1 = 0, quit2 = 0, len, option;
    char buf[128], *s_name, *c_name;
    struct course_t *new = NULL, *tail = NULL;
    struct stu_t *stu = NULL;

    while(!quit1)
    {
        GETLINES("请输入课程名称(以end结束): ",buf);

        if (!strcmp(buf, "exit"))
            quit1 = 1;

        new = (struct course_t *)malloc(sizeof(struct course_t));
 //       ERRP(NULL == new, goto ERR1, 0);
        new->c_name = (char *)malloc(len);
   //     ERRP(NULL == new, goto ERR2, 0);
        memcpy(new->c_name, buf, len);
        new->id = rand() % COURSE_MAX;

        for (tail = &course_head; tail->next != NULL; tail = tail->next)
            ;
        tail->next = new;

    }

    while(!quit2)
    {
        printf("======> 2.查询学生    <======\n");
        printf("======> 3.查询课程    <======\n");
        printf("======> 4.遍历学生    <======\n");
        printf("======> 5.遍历课程    <======\n");
        printf("======> 6.退出        <======\n");
        printf("请输入选择 [1 - 6] \n");
        scanf("%d", &option);

        switch(option)
        {
            case 1:
                GETLINES("\t请输入学生姓名:", s_name);
                stu = add_stu(s_name);
                select_course(stu);
                break;
            case 2:
                GETLINES("\t请输入查询的学生姓名:", s_name);
                stu = find_stu(s_name);
                printf_stu(stu);
                break;
            case 3:
                GETLINES("\t请输入查询课程的名称:", c_name);
                tail = find_course(c_name);
                printf_course(tail);
                break;
            case 4:

                break;
            case 5:

                break;
            case 6:
                quit2 = 1;
                break;
            default:
                break;
        }
    }
   return 0;

/*ERR2:*/
    /*free();*/
/*ERR1:*/
    /*return -1;*/
    
}
