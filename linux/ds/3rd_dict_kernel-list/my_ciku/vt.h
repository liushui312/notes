#ifndef _VT_H_
#define _VT_H_

#include <stdio.h>

#define	CLS_SCREEN		0
#define CLS_LINEALL		1
#define CLS_LINE		2

void goto_xy(int x, int y);
void putch(int fcolor, int bcolor, int ch);
void putstr(int fcolor, int bcolor, char *s);
void move_left(int step);
void move_right(int step);
void move_up(int step);
void move_down(int step);
void save_cur(void);
void load_cur(void);
void hide_cur(void);
void show_cur(void);
void clean(int flag);
void draw_rect(int x, int y, int w, int h, int color, int ch);

#endif /* _VT_H_ */
