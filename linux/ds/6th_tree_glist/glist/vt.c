#include <stdio.h>
#include <vt.h>

void gotoxy(int x, int y)
{
    DEBUG("\033[%d;%dH", x, y);
}

void putch(int fcolor, int bcolor, int ch)
{
    DEBUG("\033[%d;%dm%c\033[0m", fcolor, bcolor, ch);
}

void putstr(int fcolor, int bcolor, char *s)
{
    DEBUG("\033[%d;%dm%s\033[0m", fcolor, bcolor, s);
}
void save_cur(void)
{
    DEBUG("\033[s");
}

void load_cur(void)
{
    DEBUG("\033[u");
}

void hide_cur(void)
{
    DEBUG("\033[?25l");
}

void show_cur(void)
{
    DEBUG("\033[?25h");
}

void move_up(int step)
{
    DEBUG("\033[%dA", step);
}

void move_down(int step)
{
    DEBUG("\033[%dB", step);
}

void move_left(int step)
{
    DEBUG("\033[%dD", step);
}

void move_right(int step)
{
    DEBUG("\033[%dC", step);
}

void clean(int flag)
{
    switch (flag)
    {
        case CLR_SCREEN:
            DEBUG("\033[2J");
            break;
        case CLR_LINE:
            DEBUG("\033[2K");
            break;
        case CLR_BACK:
            DEBUG("\033[K");
            break;
        default:
            break;
    }
}

void draw_rect(int x, int y, int w, int h, int color, int ch)
{
    int i, j;
    
    gotoxy(x, y);
    for (i = 0; i < h; i++)
    {
        gotoxy(x + i, y);
        for (j = 0; j < w; j++) 
        {
            putch(color, 10, ch); 
        }
    }
}

void draw_box(int x, int y, int w, int h, int color, int ch)
{
    int i, j;
    
    gotoxy(x, y);
    for (i = 0; i < h; i++)
    {
        gotoxy(x + i, y);
        for (j = 0; j < w; j++) 
        {
            if (i == 0 || j == 0 || i == h - 1 || j == w - 1)
                putch(color, 10, ch); 
            else
                putch(color, 10, ' '); 
        }
    }
}
