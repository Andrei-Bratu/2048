#include <curses.h>

int main()
{
	initscr();
	int ch;
	keypad (stdscr, TRUE);
	ch = getch();
	printw ("%d\n", ch);
	getch();
	refresh();
	endwin();
	return 0;
}
