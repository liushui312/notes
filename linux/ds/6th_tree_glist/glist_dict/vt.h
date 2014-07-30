#ifndef __VT_H__
#define __VT_H__

#define CLR_SCREEN      0
#define CLR_LINE        1
#define CLR_BACK        2

#define DEBUG(...)      fprintf(stderr, __VA_ARGS__)

//#define DEGUB(format, args...)      printf(format, ##args);

extern void gotoxy(int x, int y);
extern void putch(int fcolor, int bcolor, int ch);
extern void putstr(int fcolor, int bcolor, char *s);
extern void save_cur(void);
extern void load_cur(void);
extern void hide_cur(void);
extern void show_cur(void);
extern void move_up(int step);
extern void move_down(int step);
extern void move_left(int step);
extern void move_right(int step);
extern void clean(int flag);
extern void draw_rect(int x, int y, int w, int h, int color, int ch);
extern void draw_box(int x, int y, int w, int h, int color, int ch);

#endif /* __VT_H__ */
