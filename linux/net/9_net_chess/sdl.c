#include <sdl.h>

int sdl_init(struct sdl_t *sdl)
{
    int flags = SDL_INIT_VIDEO;
    int ret;

    if (sdl->mixer)
        flags |= SDL_INIT_AUDIO;

    ret = SDL_Init(flags);
    ERRP(-1 == ret, goto ERR1, 0);

    if (sdl->fullscreen)
        flags = SDL_SWSURFACE | SDL_FULLSCREEN;
    else
        flags = SDL_SWSURFACE;

    sdl->screen = SDL_SetVideoMode(sdl->width, sdl->height, sdl->bpp, flags);
    ERRP(NULL == sdl->screen, goto ERR2, 0);

    if (sdl->ttf)
        ERRP(-1 == TTF_Init(), goto ERR3, 0);

    if (sdl->mixer == MIXER_MUSIC)
    {
        ret = Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 512);
        ERRP(-1 == ret, goto ERR4, 0);
    }
    else if (sdl->mixer == MIXER_CHUNK)
    {
        ret = Mix_OpenAudio(22050, AUDIO_S16, 1, 256);
        ERRP(-1 == ret, goto ERR4, 0);
    }

    return TRUE;
ERR4:
    return ERR_MIXER;
ERR3:
    return ERR_TTF;  
ERR2:
    SDL_Quit();
    return ERR_VIDEO;
ERR1:
    return ERR_INIT;
}

void sdl_exit(struct sdl_t *sdl)
{
    if (sdl->ttf)
        TTF_Quit();
    if (sdl->mixer)
        Mix_CloseAudio();
    SDL_FreeSurface(sdl->screen);
    SDL_Quit();
}

void draw_pixel(SDL_Surface *dst, Uint32 x, Uint32 y, Uint32 size, Uint32 color)
{
    SDL_Rect offset;   
    
    offset.x = x;
    offset.y = y;
    offset.w = size;
    offset.h = size;

    SDL_FillRect(dst, &offset, SDL_MapRGB(dst->format, (char)(color >> 16), (char)(color >> 8), (char)color));
}

void draw_rect(SDL_Surface *dst, Uint32 x, Uint32 y, Uint32 w, Uint32 h, Uint32 color)
{
    SDL_Rect offset;   
    
    offset.x = x;
    offset.y = y;
    offset.w = w;
    offset.h = h;

    SDL_FillRect(dst, &offset, SDL_MapRGB(dst->format, (char)(color >> 16), (char)(color >> 8), (char)color));
}

void draw_circle(SDL_Surface *dst, Uint32 x, Uint32 y, Uint32 r, Uint32 color)
{
    int i, j;

    for (i = x - r; i <= x + r; i++)
    {
        for (j = y - r; j <= y + r; j++) 
        {
            if ((i - x) * (i - x) + (j - y) * (j - y) <= r * r)
                draw_pixel(dst, i, j, 1, color);
        }
    }
}

void draw_box(SDL_Surface *dst, Uint32 x, Uint32 y, Uint32 w, Uint32 h, Uint32 border, Uint32 color, Uint32 border_color)
{
    int i, j;

    for (i = y; i < y + h; i++)
    {
        for (j = x; j < x + w; j++) 
        {
            if ((i >= y && i <= y + border) ||
                    (j >= x && j <= x + border) ||
                    (i >= y + h - 1 - border && i <= y + h - 1) ||
                    (j >= x + w - 1 - border && j <= x + w - 1))
                draw_pixel(dst, j, i, 1, border_color);
            else
                draw_pixel(dst, j, i, 1, color);
        }
    }
}

SDL_Surface *pic_load(const char *path)
{
    SDL_Surface *img = NULL, *op = NULL;

    op = IMG_Load(path);
    if (op != NULL)
    {
        img = SDL_DisplayFormat(op); 
        SDL_FreeSurface(op);
    }

    return img;
}

