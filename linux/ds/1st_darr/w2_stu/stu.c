#include <darr.h>

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

struct stu_t {
    int id;
    char *name;
    char sex;
    int age;
    int en;
    int cn;
};

int cmp_id(const void *d1, const void *d2)
{
    return *(int *)d1 - ((struct stu_t *)d2)->id;
}

int cmp_en(const void *d1, const void *d2)
{
    return *(int *)d1 - ((struct stu_t *)d2)->en;
}

int cmp_cn(const void *d1, const void *d2)
{
    return *(int *)d1 - ((struct stu_t *)d2)->cn;
}

int cmp_age(const void *d1, const void *d2)
{
    return *(int *)d1 - ((struct stu_t *)d2)->age;
}

int cmp_sex(const void *d1, const void *d2)
{
    return *(char *)d1 - ((struct stu_t *)d2)->sex;
}

int cmp_name(const void *d1, const void *d2)
{
    return strcmp((char *)d1, ((struct stu_t *)d2)->name);
}

int cmp_sort_id(const void *d1, const void *d2)
{
    return ((struct stu_t *)d1)->id - ((struct stu_t *)d2)->id;
}

int cmp_sort_en(const void *d1, const void *d2)
{
    return ((struct stu_t *)d1)->en - ((struct stu_t *)d2)->en;
}

int cmp_sort_cn(const void *d1, const void *d2)
{
    return ((struct stu_t *)d1)->cn - ((struct stu_t *)d2)->cn;
}

int cmp_sort_age(const void *d1, const void *d2)
{
    return ((struct stu_t *)d1)->age - ((struct stu_t *)d2)->age;
}

int cmp_sort_sex(const void *d1, const void *d2)
{
    return ((struct stu_t *)d1)->sex - ((struct stu_t *)d2)->sex;
}

int cmp_sort_name(const void *d1, const void *d2)
{
    return strcmp(((struct stu_t *)d1)->name, ((struct stu_t *)d2)->name);
}

void destroy(const void *data)
{
    free(((struct stu_t *)data)->name);
}

int store(FILE *fp, void *data)
{
    struct stu_t *s = (struct stu_t *)data;
    int len;

    fwrite(&s->id, sizeof(s->id), 1, fp);
    len = strlen(s->name);
    fwrite(&len, sizeof(len), 1, fp);
    fwrite(s->name, sizeof(char), len, fp);
    fwrite(&s->sex, sizeof(s->sex), 1, fp);
    fwrite(&s->age, sizeof(s->age), 1, fp);
    fwrite(&s->en, sizeof(s->en), 1, fp);
    fwrite(&s->cn, sizeof(s->cn), 1, fp);
    
    return 0;
}

int load(FILE *fp, void *data)
{
    struct stu_t *s = (struct stu_t *)data;
    int len;

    fread(&s->id, sizeof(s->id), 1, fp);
    fread(&len, sizeof(len), 1, fp);
    /*printf("len = %d\n", len);*/
    s->name = (char *)malloc(len + 1);
    s->name[len] = '\0';
    fread(s->name, sizeof(char), len, fp);
    fread(&s->sex, sizeof(s->sex), 1, fp);
    fread(&s->age, sizeof(s->age), 1, fp);
    fread(&s->en, sizeof(s->en), 1, fp);
    fread(&s->cn, sizeof(s->cn), 1, fp);

    return 0;
}

int add_stu(DARR *handle)
{
    char buf[1024];
    struct stu_t s;
    int ret;

    GETLINES("\t请输入学生姓名 : ", buf);

    s.name = (char *)malloc(strlen(buf) + 1);
    ERRP(NULL == s.name, goto ERR1, 1, "malloc s.name!\n");
    strcpy(s.name, buf);

    s.id = rand() % 100;
    s.sex = rand() % 2 + 'A';
    s.age = rand() % 10 + 10;
    s.en = rand() % 50 + 50;
    s.cn = rand() % 70 + 30;

    ret = darr_insert(&s, APPEND, handle);
    ERRP(-1 == ret, goto ERR2, 1, "add stu failed!\n");
    return 0;
ERR2:
    free(s.name);
ERR1:
    return -1;
}

void show_stu(const void *data, void *arg)
{
    struct stu_t *s = (struct stu_t *)data;

    printf("\tid = %d, name = %s, sex = %c, "
             "age = %d, en = %d, cn = %d\n",
              s->id, s->name, s->sex, s->age,
              s->en, s->cn);
}

void show_find_stu(const void *data, void *arg)
{
    struct stu_t *s = (struct stu_t *)data;

    printf("\t+++ id = %d, name = %s, sex = %c, "
             "age = %d, en = %d, cn = %d +++\n",
              s->id, s->name, s->sex, s->age,
              s->en, s->cn);
}

struct key_t {
    void *data; 
    darr_cmp_t *cmp;
} key[6] = {
    {NULL, cmp_name},
    {NULL, cmp_sex},
    {NULL, cmp_id},
    {NULL, cmp_age},
    {NULL, cmp_en},
    {NULL, cmp_cn}
};

darr_cmp_t *sort_cmp[6] = {
        cmp_sort_name,
        cmp_sort_sex,
        cmp_sort_id,
        cmp_sort_age,
        cmp_sort_en,
        cmp_sort_cn
};

int stu_find_menu(char *buf, int *n, DARR *handle)
{
    int index;

    printf("\t----> 1. name    <-----\n");
    printf("\t----> 2. sex     <-----\n");
    printf("\t----> 3. id      <-----\n");
    printf("\t----> 4. age     <-----\n");
    printf("\t----> 5. en      <-----\n");
    printf("\t----> 6. cn      <-----\n");
    printf("\t----> 7. return  <-----\n");
    GETS("please input [ 1 - 7 ] : ", return -1, "%d", &index);

    if (index >= 7 || index < 1)
        return -1;

    if (buf == NULL || n == NULL)
        return index - 1;

    if (index == 1)
        GETLINES("\t+++请输入关键词: ", buf);
    else if (index == 2)
        GETS("\t+++请输入关键词: ", return -1, "%c", (char *)n);
    else
        GETS("\t+++请输入关键词: ", return -1, "%d", n);

    key[0].data = buf;
    key[1].data = n;
    key[2].data = n;
    key[3].data = n;
    key[4].data = n;
    key[5].data = n;
    
    return index - 1;
}

int main(void)
{
    int quit = 0, n, data, index;
    DARR *handle = NULL, *find = NULL;
    char buf[1024];

    handle = darr_create(sizeof(struct stu_t), destroy, store, load);
    ERRP(NULL == handle, goto ERR1, 1, "darr_create failed!\n");

    while (!quit)
    {
START:
        printf("======== 学生成绩管理系统  ==========\n");
        printf("=== 1. 添加 ===\n");
        printf("=== 2. 查找 ===\n");
        printf("=== 3. 删除 ===\n");
        printf("=== 4. 遍历 ===\n");
        printf("=== 5. 排序 ===\n");
        printf("=== 6. 存储 ===\n");
        printf("=== 7. 加载 ===\n");
        printf("=== 8. 退出 ===\n");
        GETS("请输入 [ 1 - 8 ] : ", goto START, "%d", &n);

        switch (n)
        {
            case 1:
                add_stu(handle);
                break;
            case 2:
                index = stu_find_menu(buf, &data, handle);
                if (index == -1)
                    break;
                find = darr_findall(key[index].data, key[index].cmp, handle);
                if (find != NULL)
                    darr_travel(show_find_stu, find, handle);
                break;
            case 3:
                index = stu_find_menu(buf, &data, handle);
                if (index == -1)
                    break;
                darr_delete(key[index].data, key[index].cmp, handle);
                break;
            case 4:
                darr_travel(show_stu, NULL, handle);
                break;
            case 5:
                index = stu_find_menu(NULL, NULL, handle);
                if (index == -1)
                    break;
                darr_sort(sort_cmp[index], handle);
                break;
            case 6:
                GETLINES("please input path [./db] : ", buf);
                if (strlen(buf) == 0)
                    strcpy(buf, "./db");
                if (darr_store(buf, handle))
                    printf("存储失败!\n"); 
                else
                    printf("存储成功!\n"); 
                break;
            case 7:
                GETLINES("please input path [./db] : ", buf);
                if (strlen(buf) == 0)
                    strcpy(buf, "./db");
                
                if (handle != NULL)
                    darr_destroy(&handle);
                handle = darr_load(buf, destroy, store, load);
                if (handle == NULL)
                    printf("加载失败!\n"); 
                else
                    printf("加载成功!\n"); 
                break;
            case 8:
                quit = 1;
                break;
            default:
                break;
        }
    }

    darr_destroy(&handle);

ERR1:
    return 0;
}
