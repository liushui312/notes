#ifndef __KYO_SDL_H__
#define __KYO_SDL_H__

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>

#define TRUE    0
#define FALSE   -1

#define ERR_INIT    -1
#define ERR_VIDEO   -2
#define ERR_TTF     -3
#define ERR_MIXER   -4

struct sdl_t {
    Uint32 width;
    Uint32 height;
    Uint32 bpp;
    Uint32 fullscreen:1;
    Uint32 ttf:1;
    Uint32 mixer:2;
    SDL_Surface *screen;
};

#define MIXER_DISABLE   0
#define MIXER_MUSIC     1
#define MIXER_CHUNK     2

#define DEBUG0(...)
#define DEBUG1(...)     fprintf(stderr, __VA_ARGS__);
#define DEBUG2(...)     perror( __VA_ARGS__);

#define ERRP(con, ret, flag, ...)       do          \
            {                                       \
                if (con)                            \
                {                                   \
                    DEBUG##flag(__VA_ARGS__)        \
                    ret;                            \
                }                                   \
            } while (0)

extern int sdl_init(struct sdl_t *sdl);
extern void sdl_exit(struct sdl_t *sdl);
extern void draw_pixel(SDL_Surface *dst, Uint32 x, Uint32 y, Uint32 size, Uint32 color);
extern void draw_rect(SDL_Surface *dst, Uint32 x, Uint32 y, Uint32 w, Uint32 h, Uint32 color);
extern void draw_circle(SDL_Surface *dst, Uint32 x, Uint32 y, Uint32 r, Uint32 color);
extern void draw_box(SDL_Surface *dst, Uint32 x, Uint32 y, Uint32 w, Uint32 h, Uint32 border, Uint32 color, Uint32 border_color);
extern SDL_Surface *pic_load(const char *path);

#endif /* __KYO_SDL_H__ */
