#ifndef __COURSE_H__
#define __COURSE_H__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>



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

struct node_t{
   struct node_t *next;
   struct node_t *prev;
}

struct  course_t {
    int id;
    int sex;
    int age;
    char name[128];
    struct node_t sel;
    struct node_t s_head;
};

struct stu_t{
    int id;
    char name[128];
    struct node_t sel;
    struct node_t c_head;
};

struct link_t{
    struct course_t *c;
    struct stu_t *s;
    struct node_t s_sel;
    struct node_t c_sel;
};



struct course_t *add_stu(char *s_name);
struct stu_t *add_stu(char *s_name);
int select_course(struct stu_t *stu);
struct stu_t *find_stu(char *s_name);
void printf_stu(struct stu_t *stu);
struct course_t *find_course(char * c_name);
void printf_course(struct course_t *course);


#endif /*__COURSE_H__*/

