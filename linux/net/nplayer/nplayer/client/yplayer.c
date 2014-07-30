/*
 *
 */
#include "comm.h"

//static  char fname[256];

static	int  fin;
static	int  fout;
static  volatile int  playing=0;
static void do_wait(int no)
{
	wait(NULL);
	playing = 0;
	close(fin);
	close(fout);
}

//从fout中读一行
static char *get_line(char *buf, int len)
{
	char ch;
	char *p = buf;
	int ret;

	while(1)
	{
		ret = read(fout, &ch, 1);
		if(ret != 1)
		{
			LOGPRINT("read:%d\n", ret);
			return NULL;
		}
		if(ch == '\n')
			break;

		if(len > 0)
		{
			len--;
			*p++ = ch;
		}
	}

	*p = 0;

	return buf;
}

void write_cmd(char *cmd)
{
	write(fin, cmd, strlen(cmd));
	write(fin, "\n", 1);
}

char *get_val(char *key, char *ans)
{
	struct { char *key; char *ans; } tab[] = 
	{
		{"get_file_name", "ANS_FILENAME="},
		{"get_time_pos", "ANS_TIME_POSITION="},
		{"get_time_length", "ANS_LENGTH="},
		{"get_percent_pos", "ANS_PERCENT_POSITION="},
		{"get_audio_samples", "ANS_AUDIO_SAMPLES="},
		{NULL, NULL}
	};
	int i, j;
	char buf[64], *p;

	*ans = 0;

	if(playing == 0)
	{
		return NULL;
	}

	for(i=0; tab[i].key != NULL; i++)
	{
		if(strcmp( tab[i].key, key ) == 0)
		{
			write_cmd(tab[i].key);

			for(j=0; j<1000; j++)
			{
				if( get_line(buf, sizeof(buf)) == NULL)
					break;

				//if(strncmp(buf, "ANS", 3) == 0) puts(buf);

				if(strncmp(buf, tab[i].ans, strlen(tab[i].ans)) == 0)
				{
					p = strchr(buf, '=');
					strcpy(ans, p+1);
					return ans;
				}
			}

			return NULL;
		}
	}

	return NULL;
}

void play_stop(void)
{
	if(playing)
	{
		write_cmd("stop");
		while(playing)
		{
			usleep(10*1000);
		}
	}
}

void play(char *fname)
{
	int fi[2];
	int fo[2];
	struct stat st;
	int i;
	i = 5;
	while( stat(fname, &st) == -1) //文件不存在
	{
		sleep(1);
		LOGPRINT("stat:%s\n", errstr());
		if(i-- == 0)
		{
			return;
		}
	}

	play_stop();

	pipe(fi);
	pipe(fo);

	signal(SIGCHLD, do_wait);
	playing = 1;

	if(fork() == 0)
	{
		dup2(fi[0], 0);
		dup2(fo[1], 1);

		close(fi[0]);
		close(fi[1]);
		close(fo[0]);
		close(fo[1]);

		close(2);

		LOGPRINT("play:%s\n", fname);
		execlp("mplayer", "mplayer", "-slave", "-quiet", 
			fname, NULL);

		exit(-1);
	}

	close(fi[0]);
	close(fo[1]);
	fin = fi[1];
	fout = fo[0];
}

#if 0
int main(int argc, char **argv)
{
	char buf[100];
	
	play(argv[1]);

	get_val("get_file_name", buf);

	printf("file:%s\n", buf);

	get_val("get_time_length", buf);

	printf("len:%s\n", buf);

printf("%d\n", __LINE__);
	while(playing)
	{
		get_val("get_time_pos", buf);
		printf("\r%s ", buf);

		get_val("get_percent_pos", buf);
		printf("%s ", buf);

		fflush(NULL);
		usleep(100*1000);
	}

	puts("end=====");
}
#endif
