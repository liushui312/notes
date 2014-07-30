#include <llist.h>
#include <sdl.h>

#define NUM     10

#define WIDTH       480
#define HEIGHT      272

#define CX          120
#define CY          30

struct data_t {
    char *cn;
    char *en;
};

struct dict_t {
    LLIST *handle;
    char input[128];
    TTF_Font *font;
    struct sdl_t sdl;
    int index;
    int num;
};

void dict_destroy(const void *data)
{
    struct data_t *d = (struct data_t *)data;

    free(d->en);
    free(d->cn);
}

LLIST *load_dict(const char *path)
{
    FILE *fp = NULL;
    char buf[1024], *new = NULL;
    LLIST *handle = NULL;
    int flag = 1;
    struct data_t data; 

    fp = fopen(path, "r");
    ERRP(NULL == fp, goto ERR1, 0);

    handle = llist_create(sizeof(struct data_t), dict_destroy, NULL);
    ERRP(NULL == handle, goto ERR2, 0);

    while (fgets(buf, sizeof(buf), fp) != NULL)
    {
        if (buf[strlen(buf) - 1] == '\n')
            buf[strlen(buf) - 1] = '\0';

        new = (char *)malloc(strlen(buf) + 1);
        ERRP(NULL == new, goto ERR3, 0);
        strcpy(new, buf);

        if (flag)
            data.en = new; 
        else
        {
            data.cn = new;
            llist_insert_append(&data, handle);
        }
        flag = !flag;
    }

    fclose(fp);

    return handle;
ERR3:
    llist_destroy(&handle);
ERR2:
    fclose(fp);
ERR1:
    return NULL;
}

void draw_dict_bg(SDL_Surface *screen, TTF_Font *font)
{
    SDL_Surface *txt = NULL;
    SDL_Rect dst = {0, 0, 0, 0};

    draw_box(screen, 0, 0, WIDTH, CY, 1, 0xcccccc, 0x0);
    draw_box(screen, 0, CY, CX,  HEIGHT - CY, 1, 0xcccccc, 0x0);
    draw_box(screen, CX, CY, WIDTH - CX, HEIGHT - CY, 1, 0xcccccc, 0x0);
    txt = TTF_RenderUTF8_Blended(font, "Input Key : ", COLOR(0x0)); 

    dst.y = (CY - txt->h) / 2;

    SDL_BlitSurface(txt, NULL, screen, &dst);
    SDL_FreeSurface(txt);
}

int dict_init(const char *path, struct dict_t *dict)
{
    int ret;

    memset(dict, 0, sizeof(struct dict_t));

    dict->handle = load_dict(path);
    ERRP(NULL == dict->handle, goto ERR1, 0);

    dict->sdl.width = 480;
    dict->sdl.height = 272;
    dict->sdl.bpp = 32;
    dict->sdl.ttf = 1;

    ret = sdl_init(&dict->sdl);
    ERRP(ret == -1, goto ERR2, 0);

    dict->font = TTF_OpenFont("./simfang.ttf", 20);
    ERRP(NULL == dict->font, goto ERR3, 0);

    draw_dict_bg(dict->sdl.screen, dict->font);

    SDL_Flip(dict->sdl.screen);

    return 0;
ERR3:
    sdl_exit(&dict->sdl);
ERR2:
    dict_destroy(&dict->handle);
ERR1:
    return -1;
}

void dict_exit(struct dict_t *dict)
{
    llist_destroy(&dict->handle);
    TTF_CloseFont(dict->font);
    sdl_exit(&dict->sdl);
}

void show(struct data_t *content, struct dict_t *dict)
{
    SDL_Surface *txt = NULL;
    SDL_Rect dst = {0, 0, 0, 0};

    if (content == NULL)
        return;

    txt = TTF_RenderUTF8_Blended(dict->font, content->en, COLOR(0x0));
    dst.x =  CX + 10;
    dst.y =  CY + 10;
    SDL_BlitSurface(txt, NULL, dict->sdl.screen, &dst);
    SDL_FreeSurface(txt);

    txt = TTF_RenderUTF8_Blended(dict->font, content->cn, COLOR(0x0));
    dst.x =  CX + 10;
    dst.y =  CY + txt->h + 10;
    SDL_BlitSurface(txt, NULL, dict->sdl.screen, &dst);
    SDL_FreeSurface(txt);
}

void show_find(const void *data, void *arg)
{
    struct dict_t *dict = (struct dict_t *)arg;
    struct data_t *content = (struct data_t *)data; 
    SDL_Surface *txt = NULL;
    SDL_Rect dst = {3, 5, 0, 0};
    SDL_Rect src = {0, 0, 0, 0};

    if (dict->num > NUM)
        return;

    txt = TTF_RenderUTF8_Blended(dict->font, content->en, COLOR(0x0));
    dst.y += (dict->num * txt->h) + CY;

    src.w = CX;
    src.h = txt->h;
    SDL_BlitSurface(txt, &src, dict->sdl.screen, &dst);
    SDL_FreeSurface(txt);

    dict->num++;
}

int cmp(const void *s1, const void *s2)
{
    return strcmp((char *)s1, ((struct data_t *)s2)->en);
}

int cmp_len(const void *s1, const void *s2)
{
    return strncmp((char *)s1, ((struct data_t *)s2)->en, strlen((char *)s1));
}

void key_handle(int key, struct dict_t *dict)
{
    SDL_Surface *txt = NULL;
    SDL_Rect dst = {0, 0, 0, 0};
    LLIST *find = NULL;

    if (dict->index > 36)
        return;

    draw_dict_bg(dict->sdl.screen, dict->font);

    if ((key >= 'a' && key <= 'z') || 
        (key >= 'A' && key <= 'Z') || key == SDLK_SPACE)
    {
        dict->input[dict->index++] = key;
    }
    else if (key == SDLK_RETURN)
    {
        dict->input[dict->index] = '\0';

        show(llist_find(dict->input, cmp, dict->handle), dict);

        dict->index = 0; 
    }
    else if (key == SDLK_BACKSPACE && dict->index > 0)
        dict->index--;   
    else
        return;

    if (dict->index > 0)
    {
        dict->input[dict->index] = '\0';


        find = llist_find_all(dict->input, cmp_len, dict->handle);
        if (find != NULL)
        {
            llist_find_travel(show_find, dict, find); 
            dict->num = 0;
        }

        txt = TTF_RenderUTF8_Blended(dict->font, dict->input, COLOR(0x0));
        dst.x = CX;
        dst.y = (CY - txt->h) / 2;
        SDL_BlitSurface(txt, NULL, dict->sdl.screen, &dst);
        SDL_FreeSurface(txt);
    }

    SDL_Flip(dict->sdl.screen);
}

int main(void)
{
    struct dict_t dict;
    SDL_Event event;
    int ret;

    ret = dict_init("./ciku.dict", &dict);
    ERRP(-1 == ret, return 0, 1, "dict init failed!\n");

    while (1)
    {
        if (SDL_WaitEvent(&event))
        {
            if (event.type == SDL_QUIT)
                break;
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    break;
                key_handle(event.key.keysym.sym, &dict);
            }
        }
    }

    dict_exit(&dict);

    return 0;
}
