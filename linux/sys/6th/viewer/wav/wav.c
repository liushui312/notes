
#include "wav.h"

char * get_file( char ** file_list, int mode )
{
	static int line = 0;
	
	int len = 0;
	char ** p = file_list;
	for( ; *p != NULL; p ++ )
		len ++;
	len ++;

	int ret;
	switch ( mode ) {
		case SEQUENCE:
			ret = line ++;
			if( line == len )
				return NULL;
			break;
		case GLOB_RECYCLE:
			ret = line ++;
			if( len == line  ) 
				line = 0;
			break;
		case SINGLE_RECYCLE:
			ret = line;
			break;
		case RANDOM:
			ret = random() % ( len - 1 );
			break;					
		default:
			return NULL;
			break;				
	}	
	return file_list[ ret ];
}

char ** parse_list_file( char * filename )
{
	char ** list;					
	
	FILE * fp = fopen( filename, "rb" );
	if( fp == NULL ) {
		perror("fopen list");
		return NULL;
	}		
	
	char ch;
	int line_num = 0;
	while( !feof( fp ) ) {
		ch = fgetc( fp );
		if( ch == '\n' )
			line_num ++;	
	}			
	
	list = calloc( line_num+1, sizeof(char*) );
	rewind( fp );
	
	char buf[1024 * 10];
	int char_num = 0;
	line_num = 0;

	while( !feof( fp ) ) {
		ch = fgetc( fp );
		if( ch == '\n' ) {
			list[line_num] = calloc( char_num + 1, sizeof(char) );
			strncpy( list[line_num], buf, char_num );
			line_num ++;
			char_num = 0;
			continue;
		}
		buf[char_num] = ch;
		char_num ++;						
	}	
	
	return list;	
}

int set_dev_arg( wav_head_t head, int dev_fd )
{
	int arg;

	/* set numeric bits of get sample */
	arg = head.bits;
	if(ioctl( dev_fd, SOUND_PCM_WRITE_BITS, &arg ) == -1)
		return -1;
	if( head.bits != arg )
		perror("Unable to set sample bits !");


	/* set the number of channel at get sample */
	arg = head.channels;
	if(ioctl( dev_fd, SOUND_PCM_WRITE_CHANNELS, &arg ) == -1)
		return -1;
	if( arg != head.channels )
		perror("Unable to set number of channels !");


	/* set the frequence of get samples */
	arg = head.rate;
	if(ioctl(dev_fd,SOUND_PCM_WRITE_RATE,&arg) == -1)
		return -1;
	if( arg != head.rate )
		perror("Uable to set number of rate");

	return 0;
}


int dev_sync( int dev_fd )
{
	return ioctl(dev_fd,SOUND_PCM_SYNC,0);
}


int player_sound(char *filename)
{
	int sound_fd;
	int dev_fd;
	int ret = -1;
	int buf_size;
	wav_head_t sound_head;
	char *wav_buf;

	if((sound_fd = open(filename,O_RDONLY)) == -1)
	{
		fprintf(stderr,"open %s failed...\n",filename);
		return ret;
	}

	if((dev_fd = open(AUDIO_PATH,O_WRONLY)) == -1)
	{
		fprintf(stderr,"open %s failed...",AUDIO_PATH);
		close(sound_fd);
		return ret;
	}

	if((ret = read(sound_fd,&sound_head,sizeof(sound_head))) != sizeof(sound_head))
	{
		fprintf(stderr,"read %s sound head failed...\n",filename);
		goto end;
	}
#if 0
	printf("riff_flag = %s\n",sound_head.riff_flag);
	printf("file_long = %d\n",sound_head.file_long); 
	printf("wav_flag =  %s\n",sound_head.wav_flag);
	printf("fmt_flag =  %s\n",sound_head.fmt_flag);
	printf("pass_byte = %d\n",sound_head.pass_byte); 
	printf("format 	  = %d\n",sound_head.format); 
	printf("channels =  %d\n",sound_head.channels); 
	printf("rate     =  %d\n",sound_head.rate); 
	printf("tran_rate = %d\n",sound_head.tran_rate); 
	printf("date_block = %d\n",sound_head.date_block); 
	printf("bits	 =  %d\n",sound_head.bits); 
	printf("data_flag = %s\n",sound_head.data_flag);
	printf("sound_long = %d\n",sound_head.sound_long); 

	printf("sound_head = %d\n",sizeof(sound_head));	
	return 0;
#endif
	if(strncmp(sound_head.riff_flag,MAGIC,4) != 0)
	{
		fprintf(stderr,"unknown format...\n");
		goto end;
	}

	if(set_dev_arg(sound_head,dev_fd) != 0)
	{
		fprintf(stderr,"set dev arg error...\n");
		goto end;
	}

	buf_size = sound_head.bits * sound_head.channels * sound_head.rate / 8 / 10;

	if((wav_buf = malloc(buf_size)) == NULL)
	{
		fprintf(stderr,"%s:  can't get memory...\n",filename);
		goto end;
	}

	while((ret = read(sound_fd,wav_buf,buf_size)) > 0)
	{
		write(dev_fd,wav_buf,ret);
	}

	dev_sync(dev_fd);
end:
	close(sound_fd);
	close(dev_fd);

	return ret;
}

void *wav_thread(void *arg)
{	
	char **list = parse_list_file(arg);
	char *cur_filename;

	while(1)
	{
		cur_filename = get_file(list,SEQUENCE);
		if(cur_filename == NULL)
			break;
		player_sound(cur_filename);
	}

	while(1)
	{
		sleep(1);
	}
	//raise(2);

	return NULL;
}



