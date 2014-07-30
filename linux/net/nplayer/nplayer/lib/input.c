#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "input.h"

int get_key( int echo )
{
	fflush( NULL );
	struct termios term, save_term;
	tcgetattr( STDIN_FILENO, &term );
	save_term = term;

	term.c_lflag &= ~ICANON;
	if( echo == 0 )
		term.c_lflag &= ~ECHO;
	tcsetattr( STDIN_FILENO, TCSANOW, &term );
	
	char buf[5];

	char up[3] = { 27, 91, 65 };
	char down[3] = { 27, 91, 66 };
	char left[3] = { 27, 91, 68 };
	char right[3] = { 27, 91, 67 };
	char home[3] = { 27, 79, 72 };
	char end[3] = { 27, 79, 70 };
	
	int ret_val;
	int ret = read( STDIN_FILENO, buf, 3 );
	if( ret == 1 )
		ret_val = buf[0];
	else {
		if( !strncmp( up, buf, 3 ) )
			ret_val = UP;
		else if( !strncmp( down, buf, 3 ) )
			ret_val = DOWN;
		else if( !strncmp( left, buf, 3 ) )
			ret_val = LEFT;
		else if( !strncmp( right, buf, 3 ) )
			ret_val = RIGHT;		
		else if( !strncmp( home, buf, 3 ) )
			ret_val = HOME;
		else if( !strncmp( end, buf, 3 ) ) 
			ret_val = END;
		else
			ret_val = NO_PRO;
	}

	tcsetattr( STDIN_FILENO, TCSANOW, &save_term );
	return ret_val;
}

