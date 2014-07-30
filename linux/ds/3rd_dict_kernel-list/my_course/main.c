#include <course.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    int  quit1 = 0, quit2 = 0, len = 0, option;
    char buf[128], *s_name = NULL, *c_name = NULL;
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
