#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iconv.h>
#include <sdl.h>
#include <unistd.h>

struct node_t {
    long long t; 
    SDL_Surface *txt;
    SDL_Surface *f_txt;
    /*char *txt;*/
    struct node_t *next;
    struct node_t *prev;
};


struct lrc_t {
    struct node_t head;
    TTF_Font *font;
    SDL_Color fcolor;
    SDL_Color bcolor;
    pid_t play_id;
    struct sdl_t sdl;
};

int gbk_to_utf8(char *gbk, int gbk_len, char *utf8, int len)
{
    int ret;
    iconv_t id;

    bzero(utf8, len);

    id = iconv_open("UTF-8", "GB2312");
    ERRP(id == (iconv_t)-1, goto ERR1, 0);

    ret = iconv(id, &gbk, &gbk_len, &utf8, &len);
    ERRP(ret == -1, goto ERR2, 0);

    iconv_close(id);

    return 0;
ERR2:
    iconv_close(id);
ERR1:
    return -1;
}

void destroy_lrc(struct node_t *head)
{
    struct node_t *tail = head->next, *save = NULL;

    while (tail != head)
    {
        save = tail->next; 
        SDL_FreeSurface(tail->txt);
        SDL_FreeSurface(tail->f_txt);
        free(tail);
        tail = save;
    }
}

int load_lrc(const char *lrc_path, struct node_t *head, TTF_Font *font)
{
    FILE *fp = NULL;
    char buf[8192], utf8[8192 * 3], *p = NULL, *e = NULL, *s = NULL, *m = NULL;
    int buf_len, lrc_len, ret;
    struct node_t *new = NULL;

    fp = fopen(lrc_path, "r");
    ERRP(NULL == fp, goto ERR1, 1, "fopen");

    while (fgets(buf, sizeof(buf), fp) != NULL)
    {
        buf_len = strlen(buf);

        if (buf_len <= 1) 
            continue;

        if (buf[buf_len - 1] == '\n')
            buf[buf_len - 1] = '\0';

        if ((p = strchr(buf, '[')) == NULL || 
            (e = strchr(buf, ']')) == NULL ||
            (lrc_len = strlen(e + 1)) < 1 ||
            (s = strchr(p, ':')) == NULL ||
            (m = strchr(s, '.')) == NULL)
            continue;

        new = (struct node_t *)malloc(sizeof(struct node_t));
        ERRP(NULL == new, goto ERR2, 0);

        new->t = (atoi(p + 1) * 60 + atoi(s + 1)) * 1000000 + (atoi(m + 1) * 10000);

        ret = gbk_to_utf8(e + 1, lrc_len, utf8, sizeof(utf8));
        if (ret == -1)
            p = e + 1; 
        else
            p = utf8;

        printf("p = %s\n", p);

        new->txt = TTF_RenderUTF8_Blended(font, p, COLOR(0xffff00)); 
        new->f_txt = TTF_RenderUTF8_Blended(font, p, COLOR(0xff0000)); 

        new->next = head;
        new->prev = head->prev;
        head->prev->next = new;
        head->prev = new;
    }

    fclose(fp);

    return 0;
ERR2:
    destroy_lrc(head);
    fclose(fp);
ERR1:
    return -1;
}

int lrc_init(const char *lrc_path, struct lrc_t *lrc)
{
    int ret; 

    bzero(lrc, sizeof(struct lrc_t));

    lrc->sdl.width = 480;
    lrc->sdl.height = 272;
    lrc->sdl.bpp = 32;
    lrc->sdl.ttf = 1;

    ret = sdl_init(&lrc->sdl);
    ERRP(ret != 0, goto ERR1, 2, "sdl_init failed!\n");

    lrc->head.next = &lrc->head;
    lrc->head.prev = &lrc->head;

    lrc->font = TTF_OpenFont("./res/simfang.ttf", 40);
    ERRP(NULL == lrc->font, goto ERR2, 2, "TTF_Open failed!\n");
    /*TTF_SetFontStyle(lrc->font, TTF_STYLE_BOLD);*/

    ret = load_lrc(lrc_path, &lrc->head, lrc->font);
    ERRP(-1 == ret, goto ERR3, 2, "load_lrc failed!\n");

    return 0;
ERR3:
    TTF_CloseFont(lrc->font);
ERR2:
    sdl_exit(&lrc->sdl);
ERR1:
    return -1;
}

void lrc_exit(struct lrc_t *lrc)
{
    destroy_lrc(&lrc->head);    
    TTF_CloseFont(lrc->font);
    sdl_exit(&lrc->sdl);
    system("stty echo icanon");
    /*printf("\033[2J\033[1;1H");*/
}

#if 0
int show_lrc(void *data)
{
    struct lrc_t *lrc = (struct lrc_t *)data;
    struct node_t *cur = lrc->head.next, *tail = NULL, *save = NULL;
    int i, t = 6, ret;
    SDL_Rect dst = {0, 0, 0, 0};

    while (cur != &lrc->head)
    {
        draw_rect(lrc->sdl.screen, 0, 0, lrc->sdl.width, lrc->sdl.height, 0x0);

        if (t >= 0)
            save = lrc->head.next;
        else
            save = save->next;
        tail = save;

        /*printf("tail = %ld ", tail->t);*/
        /*printf("cur = %ld\n", cur->t);*/

        draw_rect(lrc->sdl.screen, 0, 5 * tail->txt->h, lrc->sdl.width, tail->txt->h, 0x333333);
        for (i = 0; i < 13; i++)
        {
            if (i < t)
                continue;

            dst.x = (lrc->sdl.width - tail->txt->w) / 2;  
            dst.y = i * tail->txt->h;
            SDL_BlitSurface(tail->txt, NULL, lrc->sdl.screen, &dst);
            if (tail->next != &lrc->head)
                tail = tail->next;
        }

        SDL_Flip(lrc->sdl.screen);
        usleep(cur->t - cur->prev->t);
        cur = cur->next;
        if (t >= 0)
            t--;
    }

    return 0;
}
#else

int show_lrc(void *data)
{
    struct lrc_t *lrc = (struct lrc_t *)data;
    struct node_t *cur = lrc->head.next;
    int i, t, ret;
    SDL_Rect offset = {0, 0, 0, 0};
    SDL_Rect src = {0, 0, 0, 0};

    while (cur != &lrc->head)
    {
        draw_rect(lrc->sdl.screen, 0, 0, lrc->sdl.width, lrc->sdl.height, 0x0);

        SDL_BlitSurface(cur->txt, NULL, lrc->sdl.screen, NULL);
        SDL_Flip(lrc->sdl.screen);

        if (cur == lrc->head.next)
            usleep(cur->t);
         
        t = (cur->next->t - cur->t) / cur->f_txt->w - 2000; 

        printf("cur->t = %d ", cur->t);
        printf("%d ", cur->next->t - cur->t);
        printf("t = %d\n", t);

        src.x = 0;
        offset.x = 0;
        offset.w = cur->f_txt->w;
        offset.h = cur->f_txt->h;
        for (i = 0; i < cur->f_txt->w; i++)
        {
            src.h = cur->f_txt->h;
            src.w = i;
            if (i > lrc->sdl.width)
            {
                src.x++;
                offset.x++;
            }
            draw_rect(lrc->sdl.screen, 0, 0, lrc->sdl.width, lrc->sdl.height, 0x0);
            SDL_BlitSurface(cur->txt, &offset, lrc->sdl.screen, NULL);
            SDL_BlitSurface(cur->f_txt, &src, lrc->sdl.screen, NULL);
            SDL_Flip(lrc->sdl.screen);
            usleep(t);
        }
        cur = cur->next; 
    }

    return 0;
}

#endif

int main(int argc, char *argv[])
{
    struct lrc_t lrc;
    char lrc_name[256], *p = NULL, buf[1024];
    int ret;
    SDL_Event event;
    SDL_Thread *lrc_id;

    if (argc < 2)
    {
        printf("please input mp3 file!\n"); 
        return 0;
    }
    p = strrchr(argv[1], '.');
    snprintf(lrc_name, p - argv[1] + 1, "%s", argv[1]);
    strcat(lrc_name, ".lrc");

    /*printf("lrc_name = %s\n", lrc_name);*/

    ret = lrc_init(lrc_name, &lrc);
    ERRP(ret == -1, return 0, 2, "lrc_init failed!\n");

    lrc_id = SDL_CreateThread(show_lrc, &lrc);

    if ((lrc.play_id = fork()) == 0)
    {
        close(1);
        close(2);
        execlp("mplayer", "mplayer", argv[1], NULL); 
        perror("execlp");
    }

    while (1)
    {
        if (SDL_WaitEvent(&event))
        {
            if (event.type == SDL_QUIT)
                break;
            if (event.type == SDL_KEYDOWN &&
                event.key.keysym.sym == SDLK_ESCAPE)
                break;
        }
    }

    SDL_KillThread(lrc_id);
    snprintf(buf, sizeof(buf), "kill -9 %d", lrc.play_id);
    system(buf);

    lrc_exit(&lrc);

    return 0;
}
