#include <llist.h>
#include <unistd.h>
#include <vt.h>

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



struct dict_t {
    char *en;
    char *cn;
};

void destroy(const void *data)
{
    struct dict_t *dict = (struct dict_t *)data;
    free(dict->en);
    free(dict->cn);
}

LLIST *dict_init(const char *path)
{
    LLIST *handle = NULL;
    FILE *fp = NULL;
    struct dict_t dict;
    char buf[1024];
    char *val = NULL;
    int len, flag = 1;

    handle = llist_create(sizeof(struct dict_t), destroy, NULL, NULL);
    ERRP(NULL == handle, goto ERR1, 1, "llist_create!\n");

    fp = fopen(path, "r");
    ERRP(NULL == fp, goto ERR2, 1, "fopen!\n");

    while (fgets(buf, sizeof(buf), fp) != NULL)
    {
        len = strlen(buf);
        if (buf[len - 1] == '\n')
            buf[len - 1] = '\0';

        val = (char *)malloc(strlen(buf) + 1);
        strcpy(val, buf);

        if (flag)
            dict.en = val;
        else
        {
            dict.cn = val; 
            llist_append(&dict, handle);
        }
        flag = !flag;
    }

    fclose(fp);

    system("stty -echo -icanon");
    clean(SCREEN);
    gotoxy(1, 1);
    printf("please input key : ");
    fflush(stdout);

    return handle;
ERR2:
    free(handle);
ERR1:
    return NULL;
}

int cmp_en(const void *d1, const void *d2)
{
    return strcmp((char *)d1, ((struct dict_t *)d2)->en);
}

int cmp_enall(const void *d1, const void *d2)
{
    return strncmp((char *)d1, ((struct dict_t *)d2)->en, strlen((char *)d1));
}

int get_input(void)
{
    int key, ret;
    char ch[8];

    ret = read(0, ch, 8);
    if (ret == 1)
        key = ch[0];
    else if (ret == 3 && ch[0] == 27 && ch[1] == 91)
        key = ch[2];
    else
        key = -1;

    return key;
}

int c = 0;

void show_dict(const void *data, void *arg)
{
    struct dict_t *dict = (struct dict_t *)data;

    if (c > 10)
        return;
    clean(LINE);
    printf("%s\n", dict->en);

    c++;
}


int main(void)
{
    LLIST *handle = NULL, *find = NULL;
    struct dict_t *dict = NULL;
    char input[256];
    int key, i = 0, j;

    handle = dict_init("./ciku.dict");
    if (NULL == handle)
        return 0;

    while (1)
    {
        key = get_input();
        if (key == -1)
            continue;

        if (key == 27)
            break;
        if (key == 10)
        {
            move_left(i); 
            clean(B_LINE);
            input[i] = '\0';
            dict = llist_find(input, cmp_en, handle);
            if (dict != NULL)
            {
                save_cur();
                gotoxy(2, 4);
                clean(LINE);
                printf("en : %s", dict->en);
                gotoxy(3, 4);
                clean(LINE);
                printf("cn : %s", dict->cn);
                load_cur();
                fflush(stdout);
            }
            i = 0;
            continue;
        }
        if (key == 127)
        {
            if (i <= 0)
                continue;
            i--; 
            move_left(1);
            putchar(' ');
            move_left(1);
            fflush(stdout);
        }
        else
        {
            if (i > 9)
                continue;
            input[i++] = key;
            putchar(key);
        }

        input[i] = '\0';
        find = llist_findall(input, cmp_enall, handle);
        if (find != NULL)
        {
            save_cur();
            for (j = 0; j < 11; j++)
            {
                gotoxy(5 + j, 1);
                clean(LINE);
            }
            gotoxy(5, 1);
            llist_travel_find(show_dict, NULL, find); 
            load_cur();
            fflush(stdout);
            c = 0;
        }
    }
    system("stty icanon echo");
    gotoxy(15, 1);
    fflush(stdout);

    llist_destroy(&handle);

    return 0;
}
