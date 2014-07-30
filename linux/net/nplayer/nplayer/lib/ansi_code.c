#include <stdio.h>
#include <asm/termios.h>
#include <stdlib.h>
#include <unistd.h>
#include "ansi_code.h"

int get_win_size( int * x, int * y )  
{
	struct winsize winsz;
	ioctl( STDIN_FILENO, TIOCGWINSZ, &winsz );
	*x = winsz.ws_col;
	*y = winsz.ws_row;
}

int move_xy( int x, int y )			 
{
	printf("\033[%d;%dH", y, x );
}

int move_up( int line )             
{
	printf("\033[%dA", line );
}

int move_down( int line )          
{
	printf("\033[%dB", line );
}

int move_left( int col )		
{
	printf("\033[%dD", col );
}

int move_right( int col )      
{
	printf("\033[%dC", col );
}

int save_position( void )	
{
	printf("\033[s");
}

int restore_position( void ) 
{
	printf("\033[u");
}

int clear_screen( void )    
{
	printf("\033[2J");
}

int clear_to_line_end( void ) 
{
	printf("\033[K");	
}

int set_text_attribute( int attr )  
{
	printf("\033[%dm", attr );	
}

int set_fgcolor( int color )       
{
	printf("\033[%dm", color );	
}

int set_bgcolor( int color )      
{
	printf("\033[%dm", color + 10 );	
}

int set_color( int fg, int bg )   
{
	printf("\033[%d;%dm", fg, bg + 10 );
}

int putc_xy( int x, int y, char ch )   
{
	save_position();            
	move_xy( x, y );		
	putchar( ch );		
	restore_position(); 
}

int rectangle( int start_x, int start_y, int x_len, int y_len )   
{
	save_position();
	putc_xy( start_x, start_y, '+' );
	line_h( start_x+1, start_y, x_len-2, '=' );
	putc_xy( start_x+x_len-1, start_y, '+' );
	line_v( start_x, start_y+1, y_len-1, '|' );
	line_v( start_x+x_len-1, start_y+1, y_len-1, '|' );
	putc_xy( start_x, start_y+y_len-1, '+' );
	line_h( start_x+1, start_y+y_len-1, x_len-2, '=' );
	putc_xy( start_x+x_len-1, start_y+y_len-1, '+' );
	restore_position();
}

int line_h( int start_x, int start_y, int len, int ch ) 
{

	int i;
	save_position();
	move_xy(start_x, start_y);
	for(i = 0; i < len; ++i)
	{
		putchar(ch);
	}
	restore_position();
}


int line_v( int start_x, int start_y, int len, int ch ) 
{
	int i;
	save_position();
	move_xy(start_x, start_y);
	for(i = 0; i < len; ++i, start_y++)
	{
		putchar(ch);
	    move_xy(start_x, start_y);
	}
	restore_position();
}

