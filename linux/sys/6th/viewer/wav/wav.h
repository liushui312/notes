#ifndef _PLAY_WAV_H
#define _PLAY_WAV_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <signal.h>

#include <linux/soundcard.h>

//#define AUDIO_PATH "/dev/audio"
#define AUDIO_PATH "/dev/dsp"
#define MAGIC "RIFF"

typedef struct {
	char 		riff_flag[4];   /* RIFF flag                         */
	unsigned int 	file_long;  	/* then wav file long                */
	char 		wav_flag[4];    /* wave flag                         */
	char		fmt_flag[4];    /* fmt flag                          */
	unsigned int	pass_byte;   	/* ususal be 16 indicate head 44 byte*/
	short 		format;      	/* 01 is pcm , or not surport        */
	short		channels;    	/* the number of sound channel       */
	int		rate;        	/* the frequency of get sample       */
	unsigned int 	tran_rate;   	/* translate rate                    */
	short 		date_block;  	/* date block adjust                 */
	short 		bits;        	/* the bit number of numeric         */
	char 		data_flag[4];  	/* data flag                         */
	unsigned int 	sound_long;  	/* wav data long                     */
}wav_head_t;

enum mode {
	SEQUENCE,
	GLOB_RECYCLE,
	SINGLE_RECYCLE,
	RANDOM
};


#endif /* _PLAY_WAV_H */


