#include <stdio.h>

void gotoxy(int x, int y)
{
    printf("\033[%d;%dH", x, y); 
}

void move_up(int step)
{
    printf("\033[%dA", step);
}

void move_right(int step)
{
    printf("\033[%dC", step);
}

void move_down(int step)
{
    printf("\033[%dB", step);
}

void move_left(int step)
{
    printf("\033[%dD", step);
}

void putch(int fcolor, int bcolor, int c)
{
    printf("\033[%d;%dm%c\033[0m", fcolor, bcolor, c);
}

void putstr(int fcolor, int bcolor, char *s)
{
    printf("\033[%d;%dm%s\033[0m", fcolor, bcolor, s);
}

void save_cur(void)
{
    printf("\033[s");
}

void hide_cur(void)
{
    printf("\033[?25l");
}

void load_cur(void)
{
    printf("\033[u");
}

void show_cur(void)
{
    printf("\033[?25h");
}

void clean(int flag)
{
    switch (flag)
    {
        case 0:
            printf("\033[2J");
            break;
        case 1:
            printf("\033[2K");
            break;
        case 2:
            printf("\033[K");
            break;
        default:
            break;
    }
}

void draw_pixel(int x, int y, int ch, int color)
{
    gotoxy(x, y);
    putch(color, 10, ch);
}

void draw_vline(int x, int y, int len, int ch, int color)
{
    int i;

    for (i = 0; i < len; i++)
    {
        draw_pixel(x + i, y, ch, color);
    }
}

void draw_hline(int x, int y, int len, int ch, int color)
{
    int i;

    for (i = 0; i < len; i++)
    {
        draw_pixel(x, y + i, ch, color);
    }
}

void draw_rect(int x, int y, int w, int h, int color, int ch)
{
    int i, j;

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

    for (i = 0; i < h; i++)
    {
        gotoxy(x + i, y);
        for (j = 0; j < w; j++) 
        {
            if (i == 0 || i == h - 1 || j == 0 || j == w - 1)
                putch(color, 10, ch);
            else
                putch(color, 10, ' ');
        }
    }
}
