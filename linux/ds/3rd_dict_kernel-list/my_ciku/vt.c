#include <vt.h>

void goto_xy(int x, int y)
{
	printf("\033[%d;%dH", x, y);
}

void putch(int fcolor, int bcolor, int ch)
{
	printf("\033[%d;%dm%c\033[0m", fcolor, bcolor, ch);
}

void putstr(int fcolor, int bcolor, char *s)
{
	printf("\033[%d;%dm%s\033[0m", fcolor, bcolor, s);
}

void move_left(int step)
{
	printf("\033[%dD", step);
}

void move_right(int step)
{
	printf("\033[%dC", step);
}

void move_up(int step)
{
	printf("\033[%dA", step);
}

void move_down(int step)
{
	printf("\033[%dB", step);
}

void save_cur(void)
{
	printf("\033[s");
}

void load_cur(void)
{
	printf("\033[u");
}

void hide_cur(void)
{
	printf("\033[?25l");
}

void show_cur(void)
{
	printf("\033[?25h");
}

void clean(int flag)
{
	switch (flag)
	{
	case CLS_LINEALL:	
		printf("\033[2K");
		break;
	case CLS_LINE:
		printf("\033[K");
		break;
	default:
		printf("\033[2J");
		break;
	}
}


void draw_rect(int x, int y, int w, int h, int color, int ch)
{
	int i, j;

	for (i = 0; i < h; i++)
	{
		goto_xy(x + i, y);	
		for (j = 0; j < w; j++)
		{
			putch(color, 10, ch);
		}
	}
}
