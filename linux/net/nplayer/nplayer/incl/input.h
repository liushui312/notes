#ifndef __INPUT_H_
#define __INPUT_H_

int get_key( int echo );

#define UP	0xff1
#define DOWN 	0xff2
#define LEFT	0xff3
#define RIGHT	0xff4
#define HOME	0xff5
#define END		0xff6

#define ESC		0x1b
#define ENTER  0x0a
#define BACKSPACE 0x7f
#define TAB		0x09	
#define NO_PRO	0xfff

#endif /* _INPUT_H_ */
