#include "fb.h"

struct arg{
	int argc;
	char **argv;
};

int opt_delay = 0;
int opt_stretch = 1;

static void do_resize(unsigned char *old_buf,int old_x,int old_y, 
		unsigned char *new_buf, int new_x,int new_y)
{
	unsigned char *old_buf_cur;
	unsigned char *new_buf_cur;
	int new_count;
	int old_count;
	int x;
	int y;

	new_buf_cur = new_buf;

	for(y = 0;y < new_y;y++)
	{
		old_buf_cur = old_buf + y * old_y / new_y * old_x * 3;

		new_count = 0;
		for(x = 0;x < new_x;x++)
		{
			old_count = x * old_x / new_x * 3;

			new_buf_cur[new_count + 0] = old_buf_cur[old_count + 0];
			new_buf_cur[new_count + 1] = old_buf_cur[old_count + 1];
			new_buf_cur[new_count + 2] = old_buf_cur[old_count + 2];
			new_count += 3;
		}
		new_buf_cur += new_x * 3;
	}
}

static void picture_resize(struct picture old_pic,unsigned char **rgb_buf,
			int *free_flag,int *x_pic,int *y_pic,
			int x_scr,int y_scr,int mode)
{
	 if(old_pic.x > x_scr || old_pic.y > y_scr)
	 {
		 if(mode)
		 {
			if(old_pic.x > x_scr)
				*x_pic = x_scr;
			else
				*x_pic = old_pic.x;

			if(old_pic.y > y_scr)
				*y_pic = y_scr;
			else
				*y_pic = old_pic.y;
		 }
		 else
		 {
			if((old_pic.y * x_scr / old_pic.x) <= y_scr)
			{
				*x_pic = x_scr;
				*y_pic = old_pic.y * x_scr / old_pic.x;
			}
			else
			{
				*x_pic = old_pic.x * y_scr / old_pic.y;
				*y_pic = y_scr;
			}
		 }
		
		 *rgb_buf = malloc(*x_pic * *y_pic * 3);
		
		 *free_flag = 1;

		 do_resize(old_pic.buf,old_pic.x,old_pic.y,
				 *rgb_buf,*x_pic,*y_pic);
	 }
}


int show_image(char *filename)
{
	int x_pic = 0;
	int y_pic = 0;
	int x_scr = 0;
	int y_scr = 0;
	
	int x_pic_offs = 0;
	int y_pic_offs = 0;
	int x_scr_offs = 0;
	int y_scr_offs = 0;

	int ch = 0;
	int ret = 0;
	int mode = 0;
	int retransform = 1;

	unsigned char *rgb_buf = NULL;
	int free_flag = 0;

	struct picture old_pic;

	int (*load)(char *filename,unsigned char *rgb_buf,
			int x_pic,int y_pic);
	
	if(is_bmp(filename)&&(bmp_getsize(filename,&old_pic.x,&old_pic.y) == 0))
	{
		load = bmp_load;
		goto start;
	}

	if(is_jpeg(filename)&&(jpeg_getsize(filename,&old_pic.x,&old_pic.y) == 0))
	{
		load = jpeg_load;
		goto start;
	}

	fprintf(stderr,"%s: format unknown.\n",filename);
	return 1;
start:
	if((old_pic.buf = malloc(old_pic.x * old_pic.y * 3)) == NULL)
	{
		fprintf(stderr,"%s: can't get memory.\n",filename);
		ret = 1;
		goto error;
	}

	if((load(filename,old_pic.buf,old_pic.x,old_pic.y) != 0))	
	{
		fprintf(stderr,"%s: can't load image.\n",filename);
		ret = 1;
		goto error;
	}

	if(screen_getsize(&x_scr,&y_scr) != 0)
	{
		fprintf(stderr,"%s: can't get screen size.\n",filename);
		ret = 1;
		goto error;
	}
	
	while(1)
	{
		if(retransform == 1)
		{
			if(free_flag)
				free(rgb_buf);
			free_flag = 0;

			x_pic = old_pic.x;
			y_pic = old_pic.y;
			rgb_buf = old_pic.buf;

			if(opt_stretch == 1)
				picture_resize(old_pic,&rgb_buf,&free_flag,
						&x_pic,&y_pic,
						x_scr,y_scr,mode);

			printf("\033[2J");
			fflush(stdout);

			x_pic_offs = 0;
			y_pic_offs = 0;
			retransform = 0;
		}
	
		if(x_pic < x_scr)	
			x_scr_offs = (x_scr - x_pic) / 2;
		else
			x_scr_offs = 0;

		if(y_pic < y_scr)	
			y_scr_offs = (y_scr - y_pic) / 2;
		else
			y_scr_offs = 0;
		
		fb_display(rgb_buf,x_pic,y_pic,x_pic_offs,y_pic_offs,
				x_scr_offs,y_scr_offs);
		
		if(opt_delay != 0)
		{
			struct timeval tv;
			fd_set rd;
			
			tv.tv_sec = opt_delay / 10;
			tv.tv_usec = opt_delay % 10 * 100000;

			FD_ZERO(&rd);
			FD_SET(0,&rd);

			if(select(1,&rd,NULL,NULL,&tv) <= 0)
			{
				ret = 1;
				goto end;
			}
		}

again:
		ch = getchar();
		switch(ch)
		{
			case -1:
			case 'q':
				ret = 0;
				goto end;
			case '\n':
			case ' ':
			case '>':
			case '.':
				ret = 1;
				goto end;
			case '<':
			case ',':
				ret = -1;
				goto end;
			case 'A':	//up
				if(y_pic_offs == 0)			
					break;
				y_pic_offs -= y_pic / 20;
				if(y_pic_offs < 0)
					y_pic_offs = 0;
				break;
			case 'B':	//down
				if(y_scr_offs != 0)
					break;
				if(y_pic_offs >= y_pic - y_scr)
					break;
				y_pic_offs += y_pic / 20;
				if(y_pic_offs >= y_pic - y_scr)
					y_pic_offs = y_pic - y_scr;
				break;
			case 'C':	//right
				if(x_scr_offs != 0)
					break;
				if(x_pic_offs >= x_pic - x_scr)
					break;
				x_pic_offs += x_pic / 20;
				if(x_pic_offs >= x_pic - x_scr)
					x_pic_offs = x_pic - x_scr;
				break;
			case 'D':	//left
				if(x_pic_offs == 0)
					break;
				x_pic_offs -= x_pic / 20;
				if(x_pic_offs < 0)
					x_pic_offs = 0;
				break;
			case 'f':
				opt_stretch = !opt_stretch;
				retransform = 1;
				break;
			case 'i':
				mode = !mode;
				retransform = 1;
				break;
			default:
				goto again;
		}
	}
end:
	printf("\033[2J");
	fflush(stdout);

error:
	free(old_pic.buf);
	return ret;
}

void *viewer_thread(void *arg)
{
	int i;
	int ret;
	struct arg *p = arg;
	
	i = optind;
	while(1)
	{
		ret = show_image(p->argv[i]);
		if(ret == 0)
			break;
		i += ret;
		if(i < optind || i >= p->argc)
			i = optind;
	}

	raise(2);

	return NULL;
}


void setup_console(int flag)
{
	struct termios new;
	static struct termios old;
	
	if(flag)
	{
		printf("\033[?25l");
		fflush(stdout);

		tcgetattr(1,&old);
		new = old;

		new.c_lflag &= ~ECHO;
		new.c_lflag &= ~ICANON;

		tcsetattr(1,TCSANOW,&new);
	}
	else
	{
		printf("\033[?25h");
		fflush(stdout);

		tcsetattr(1,TCSANOW,&old);
	}
}

void handler(int num)
{
	setup_console(0);
	printf("\033[H\033[2J");
	fflush(stdout);
	exit(0);
}

int main(int argc,char *argv[])
{
	int opt;
	pthread_t viewer_tid;
	pthread_t wav_tid;

	struct arg arg = {argc,argv};

	if(argc < 2)
	{
		fprintf(stderr,"Usage: %s [options] image1 image2...\n",
				argv[0]);
		exit(-1);
	}

	while((opt = getopt(argc,argv,"s:f")) != -1)
	{
		switch(opt)
		{
			case 's':
				opt_delay = atoi(optarg);
				break;	
			case 'f':
				opt_stretch = 0;
				break;
			case '?':
				exit(-1);
		}
	}

	if(argv[optind] == NULL)
	{
		fprintf(stderr,"input images...\n");
		exit(-1);
	}

	signal(SIGINT,handler);

	setup_console(1);

	pthread_create(&viewer_tid,NULL,viewer_thread,&arg);
	
	pthread_create(&wav_tid,NULL,wav_thread,"wav/default.lst");

	pthread_join(wav_tid,NULL);	
	
	setup_console(0);

	return 0;
}






