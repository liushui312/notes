#ifndef __KYO_SDL_H__
#define __KYO_SDL_H__

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>


#define KYO_DISABLE 0
#define KYO_MUSIC   1
#define KYO_CHUNK   2

struct sdl_t {
    Uint32 width;
    Uint32 height;
    Uint32 bpp;
    Uint8 mixer:2;
    Uint8 fullscreen:1;
    Uint8 ttf:1;
    SDL_Surface *screen;
};

#define ERR_INIT    -1
#define ERR_VIDEO   -2
#define ERR_TTF     -3
#define ERR_MIXER   -4

#define DEBUG0(...)         
#define DEBUG1(...)         perror(__VA_ARGS__);
#define DEBUG2(...)         fprintf(stderr, __VA_ARGS__);

#define ERRP(con, ret, flag, ...)    do         \
            {                                   \
                if (con)                        \
                {                               \
                    DEBUG##flag(__VA_ARGS__)    \
                    ret;                        \
                }                               \
            } while (0)


#define COLOR(a)    (SDL_Color){(char)(a >> 16), (char)(a >> 8), (char)a}

extern int sdl_init(struct sdl_t *);
extern void sdl_exit(struct sdl_t *);
extern void draw_pixel(SDL_Surface *dst, int x, int y, int size, Uint32 color);
extern void draw_rect(SDL_Surface *dst, int x, int y, int w, int h, Uint32 color);
extern void draw_box(SDL_Surface *dst, int x, int y, int w, int h, int border, Uint32 color, Uint32 border_color);
extern void draw_circle(SDL_Surface *dst, int x, int y, int r, Uint32 color);
extern SDL_Surface *pic_load(const char *);


#endif /* __KYO_SDL_H__ */
