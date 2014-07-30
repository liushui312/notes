
#include <stdio.h>

static char *tab[]= {"┌ ", "┐","└ ", "┘", "— ", "︳"};

void move_xy(int x, int y)
{
	printf("\033[%d;%dH", y, x);
}

void hline(int x, int y, int len)
{
	int i;

	for(i=0; i<len/2; i++)
	{
		move_xy(x + 2*i, y);
		printf(tab[4]);
	}
}

void vline(int x, int y, int len)
{
	int i;

	for(i=0; i<len; i++)
	{
		move_xy(x, y + i);
		printf(tab[5]);
	}
}

void box(int x, int y, int w, int h)
{
	move_xy(x+1, y);
	printf(tab[0]);

	move_xy(x + w - 1, y);
	printf(tab[1]);

	move_xy(x+1, y + h-1);
	printf(tab[2]);

	move_xy(x + w - 1, y + h-1);
	printf(tab[3]);

	hline(x+3, y, w-4);

	vline(x, y+1, h-2);
	vline(x+w-2, y+1, h-2);

	hline(x+3, y+h-1, w-4);
}

void getwinsz(int *w, int *h)
{
}

int main(void)
{
	system("clear");
	box(5, 5, 60, 20);
	puts("");
}


