#include <curses.h>

int main(void)
{
	initscr();
	box(stdscr, 0, 0);
	refresh();
	sleep(1);
	endwin();
}
