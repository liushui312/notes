#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <vt.h>

#define ERRP(con, ret, ...)      do                     \
            {                                           \
                if (con)                                \
                {                                       \
                    fprintf(stderr, __VA_ARGS__);       \
                    ret;                                \
                }                                       \
            } while (0)

#define ESC         27
#define ENTER       10
#define BACKSPACE   127
#define ERROR       -1

#define LEN         20 
#define MAX         12

static char word[MAX][500];
static int d = 0;

struct node_t {
    char en;
    char *cn;
    struct node_t *next;
    struct node_t *sublist;
};

void glist_insert(char *en, char *cn, struct node_t **p)
{
    struct node_t *new = NULL;
    char *val = (char *)malloc(strlen(cn) + 1);

    strcpy(val, cn);

    while (*p != NULL)
    {
        if (*en == (*p)->en)
        {
            if (*(en + 1) == '\0')
                break;
            p = &(*p)->sublist;
            en++;
        }
        else
            p = &(*p)->next;
    }

    if (*p != NULL)
    {
        (*p)->cn = val;
    }
    else
    {
        while (*en != '\0')
        {
            new = (struct node_t *)malloc(sizeof(struct node_t));
            new->en = *en;
            new->cn = NULL;
            new->next = NULL;
            new->sublist = NULL;
            *p = new;
            p = &new->sublist;
            en++;
        }
        new->cn = val;
    }
}

struct node_t *glist_find(char *en, struct node_t *head)
{
    while (head != NULL)
    {
        if (*en == head->en)
        {
            if (*(en + 1) == '\0')
                break;
            head = head->sublist;
            en++;
        }
        else
            head = head->next;
    }

    return head;
}

void glist_destroy(struct node_t *head)
{
    struct node_t *save = NULL;

    while (head != NULL)    
    {
        save = head->next;
        glist_destroy(head->sublist);
        free(head->cn);
        free(head);
        head = save;
    }
}

struct node_t *dict_init(const char *path)
{
    struct node_t *handle = NULL;
    FILE *fp = NULL;
    char buf[1024], en[1024], cn[1024];
    int flag = 1;

    fp = fopen(path, "r");
    ERRP(NULL == fp, goto ERR1, "fopen failed!\n");

    while (fgets(buf, sizeof(buf), fp) != NULL)
    {
        if (buf[strlen(buf) - 1] == '\n')
            buf[strlen(buf) - 1] = '\0';

        if (flag)
            strcpy(en, buf);
        else
        {
            strcpy(cn, buf);
            glist_insert(en, cn, &handle);
        }
        flag = !flag;
    }

    system("stty -echo -icanon");
    clean(CLR_SCREEN);
    gotoxy(1, 1);
    DEBUG("please input search key : ");

    fclose(fp);

    return handle;
ERR1:
    return NULL;
}

void dict_exit(struct node_t *handle)
{
    glist_destroy(handle);
    system("stty echo icanon");
    gotoxy(MAX + 5, 1);
}

int get_input(void)
{
    char ch[8];
    int key = ERROR, ret;

    ret = read(STDIN_FILENO, ch, 8);
    if (ret == 1)
        key = ch[0];
    else if (ret == 3 && ch[0] == 27 && ch[1] == 91)
        key = ch[2];

    return key;
}

void show_find(char *key, struct node_t *handle)
{
    struct node_t *w = NULL;

    w = glist_find(key, handle); 
    if (w != NULL && w->cn != NULL)
    {
        save_cur();
        gotoxy(2, 1);
        clean(CLR_LINE);
        DEBUG("\ten : %s\n", key); 
        clean(CLR_LINE);
        DEBUG("\tcn : %s", w->cn);
        fflush(stdout);
        load_cur();
    }
}

void add_word(struct node_t *head, int i, char *buf)
{
    i++;
    while (head != NULL)
    {
        if (d >= MAX)
            return;
        buf[i - 1] = head->en;
        if (head->cn != NULL)
        {
            memcpy(word[d], buf, i); 
            d++;
            i = 0;
            return;
        }
        add_word(head->sublist, i, buf);
        head = head->next;
    }
}

void show_word(char *key, struct node_t *handle)
{
    struct node_t *w = NULL;
    char buf[500];
    int i;

    memset(word, 0, sizeof(word));

    w = glist_find(key, handle);
    if (w != NULL)
    {
        d = 1;
        save_cur();
        add_word(w->sublist, 0, buf);
        for (i = 0; i <= MAX; i++)
        {
            gotoxy(5 + i, 1); 
            clean(CLR_LINE);
        }
        gotoxy(5, 1);
        for (i = 0; i < d; i++)
        {
            printf("%s%s\n", key, word[i]); 
        }
        load_cur();
    }
}

int main(void)
{
    struct node_t *handle = NULL;
    int key, quit = 0, pos = 0;
    char buf[LEN + 1];

    handle = dict_init("./ciku.dict");
    ERRP(NULL == handle, goto ERR1, "dict init failed!\n");

    while (!quit)
    {
        key = get_input();
        
        switch (key)
        {
            case ESC:
                quit = 1; 
                break;
            case ENTER:
                buf[pos] = '\0';
                show_find(buf, handle);
                pos = 0;
                gotoxy(1, 27);
                clean(CLR_BACK);
                break;
            case BACKSPACE:
                if (pos > 0)
                {
                    move_left(1);
                    putch(0, 0, ' ');
                    move_left(1);
                    pos--;
                    buf[pos] = '\0';
                    show_word(buf, handle);
                }
                break;
            case ERROR:
                break;
            default:
                if (pos < LEN)
                {
                    putchar(key);
                    fflush(stdout);
                    buf[pos++] = key;
                    buf[pos] = '\0';
                    show_word(buf, handle);
                }
                break;
        }
        fflush(stdout);
    }

    dict_exit(handle);

ERR1:
    return 0;
}
