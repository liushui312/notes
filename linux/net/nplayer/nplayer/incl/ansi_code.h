#ifndef _ANSI_CODE_H
#define _ANSI_CODE_H

enum text_attribute {
   NORMAL,
	BOLD,
	UNDERLINE = 4,
	BLINKON,  
	REVERSE_VIDEO_ON = 7,
	INVISIBLE,
};

enum color {
	BLACK = 30,
	RED,
	GREEN,
	YELLOW,
	BLUE,
	MAGENTA,
	CYAN,
	WHITE,
};

int get_win_size( int * x, int * y );
int move_xy( int x, int y );
int move_up( int line );
int move_down( int line );
int move_left( int col );
int move_right( int col );
int save_position( void );
int restore_position( void );
int clear_screen( void );
int clear_to_line_end( void );
int set_text_attribute( int attr );
int set_fgcolor( int color );
int set_bgcolor( int color );
int set_color( int fg, int bg );
int putc_xy( int x, int y, char ch );
int rectangle( int start_x, int start_y, int x_len, int y_len );
int bar( int start_x, int start_y, int x_len, int y_len, char ch );
int line_v( int start_x, int start_y, int len, int c );
int line_h( int start_x, int start_y, int len, int c );
#endif
