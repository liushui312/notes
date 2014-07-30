#ifndef __VT_H__
#define __VT_H__

#include <stdio.h>

#define KINFO(...)      printf(__VA_ARGS__)

#define C_NONE      10

#define SCREEN      0x1
#define LINE        0x2
#define B_LINE      0x3

void gotoxy(int x, int y);
void clean(int flag);
void move_left(int step);
void move_right(int step);
void move_up(int step);
void move_down(int step);
void putch(int bcolor, int fcolor, int ch);
void putstr(int bcolor, int fcolor, char *s);
void save_cur(void); 
void load_cur(void); 
void hide_cur(void);
void show_cur(void);

#endif /* __VT_H__ */
