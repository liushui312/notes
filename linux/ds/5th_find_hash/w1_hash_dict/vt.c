#include <vt.h>

void gotoxy(int x, int y)
{
    KINFO("\033[%d;%dH", x, y);
}

void clean(int flag)
{
    switch (flag)
    {
        case SCREEN:
            KINFO("\033[2J");
            break;
        case LINE:
            KINFO("\033[2K");
            break;
        case B_LINE:
            KINFO("\033[K");
            break;
        default:
            break;
    }
}

void move_left(int step)
{
    KINFO("\033[%dD", step);
}

void move_right(int step)
{
    KINFO("\033[%dC", step);
}

void move_up(int step)
{
    KINFO("\033[%dA", step);
}

void move_down(int step)
{
    KINFO("\033[%dB", step);
}

void putch(int bcolor, int fcolor, int ch)
{
    KINFO("\033[%d;%dm%c\033[0m", bcolor, fcolor, ch);
}

void putstr(int bcolor, int fcolor, char *s)
{
    KINFO("\033[%d;%dm%s\033[0m", bcolor, fcolor, s);
}

void save_cur(void)
{
    KINFO("\033[s");
}

void load_cur(void)
{
    KINFO("\033[u");

}

void hide_cur(void)
{
    KINFO("\033[?25l");
}

void show_cur(void)
{
    KINFO("\033[?25h");
}

