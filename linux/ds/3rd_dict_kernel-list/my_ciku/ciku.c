#include <stdio.h>
#include <unistd.h>
#include <vt.h>
#include <string.h>
#include <stdlib.h>

#define ESC  27
#define BACKSPACE  127 
#define ENTER  10


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

struct word_t {
    char *word;
    char *trans;
    struct word_t *next;
};


int get_input(void);
struct word_t *word_load(const char * path);
struct word_t *find_word(char *word, struct word_t *word_bank);
void printf_word(struct word_t *word);
void free_word_t(struct word_t * handle);

int get_input(void)
{
    char ch[6];
    int ret, key;

    ret = read(0, ch, 6);
    if (ret == 1)
        key = ch[0];
    else
        key = -1;

    return key;
}


struct word_t *word_load(const char * path)
{
    FILE *fp = NULL;
    char buf[128], *ret;
    struct word_t handle = {.next = NULL};
    struct word_t *tail = NULL, *new = NULL;
    int len;

    fp = fopen(path, "r");
    ERRP(NULL == fp, goto ERR1, 0);

    while(1) 
    {
        new = (struct word_t *)malloc(sizeof(struct word_t));
        ERRP(NULL == new, goto ERR2, 0);
        new->next = NULL;

        ret = fgets(buf, sizeof(buf), fp);
        ERRP(ret == NULL, goto ERR3, 0);
        len = strlen(buf);
        if (buf[len - 1] == '\n')
            buf[len - 1] = '\0';
        new->word = (char *)malloc(sizeof(char) * len);
        ERRP(NULL == new->word, goto ERR3, 0);
        memcpy(new->word, buf, len);

        ret = fgets(buf, sizeof(buf), fp);
        ERRP(ret == NULL, goto ERR3, 0);
        len = strlen(buf);
        if (buf[len - 1] == '\n')
            buf[len - 1] = '\0';
        new->trans = (char *)malloc(sizeof(char) * len);
        ERRP(NULL == new->trans, goto ERR2, 0);
        memcpy(new->trans, buf, len);

        for(tail = &handle; tail->next != NULL; tail = tail->next)
            ;
        tail->next = new;

    }

ERR3:
    fclose(fp);
    return handle.next;

ERR2:
    free_word_t(handle.next);
    fclose(fp);
ERR1:
    return NULL;
}

struct word_t *find_word(char *word, struct word_t *word_bank, int memfind)
{
    struct word_t *tail = NULL;

    for(tail = word_bank; tail->next != NULL; tail = tail->next)
    {
        if (flag == 0)
        {
            if(!(strcmp(word, tail->word)))
                return tail;
        }
        else if (flag == 1 )
        {
            struct word_t *find = NULL;
            

            if(!(strncmp(word, tail->word, strlen(word)))) 
                
        }
    }
    return NULL;
}


void free_word_t(struct word_t * handle)
{
    struct word_t *tail = NULL, *hold = NULL;
    for(tail = handle; tail->next != NULL; tail = tail->next)
    {
        hold = tail;
        free(tail->word);
        free(tail->trans);
        free(tail);
        tail = hold;
    }
}

int main(void)
{
    struct word_t *word_bank, *ret_word = NULL; 

    int quit = 0, key, count = 0;
    char word[26];

    
    word_bank = word_load("./ciku.dict");
        
    system("stty -icanon -echo");
    clean(CLS_SCREEN);
    goto_xy(1, 1);

    printf("please search key : ");
    fflush(stdout);

    while(!quit)
    {
        key = get_input();
        switch(key)
        {
            case -1:
                continue;
            case ESC:
                goto_xy(1,1);
                clean(CLS_SCREEN);
                fflush(stdout);
                quit = 1;
                break;
            case BACKSPACE:
                if (count <= 0)
                    continue;
                count--;
                move_left(1);
                putchar(' ');
                move_left(1);
                fflush(stdout);
                break;
            case ENTER:
                word[count] = '\0';
                ret_word = find_word(word, word_bank, 0);              
                if(ret_word != NULL)
                {
                    move_left(count); 
                    save_cur();
                    clean(CLS_LINE);
                    goto_xy(2,4);
                    clean(CLS_LINEALL);
                    printf("en: %s\n", ret_word->word);
                    goto_xy(3,4);
                    clean(CLS_LINEALL);
                    printf("cn: %s\n", ret_word->trans);
                    load_cur();
                    fflush(stdout);
                    count = 0;
                }
                break;
            default:
                word[count++] = key;
                putchar(key);
                fflush(stdout);
                break;
        }

        word[i] = '\0';
        
        ret_word = find_word(word, word_bank, 1);              

        
        
    }

    system("stty icanon echo");

    return 0;
}
