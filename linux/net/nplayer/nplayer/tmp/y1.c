/*
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <poll.h>
#include <signal.h>

struct play_t
{
	char fname[256];
	off_t flen;		
	off_t ldlen;
	int   pos;
	int  fi;
	int  fo;
};

static pthread_t tid_play;
static pthread_t tid_load;
static volatile int playing = 0;
static volatile int loading = 0;
static volatile	int _pause = 0;

static struct play_t play;
//struct play_t play;

static pthread_mutex_t  mutex = PTHREAD_MUTEX_INITIALIZER;

static void lock(void)
{
	pthread_mutex_lock(&mutex);
}

static void unlock(void)
{
	pthread_mutex_unlock(&mutex);
}

static void do_wait(int no)
{
	while( wait(NULL) != -1);
	playing = 0;
}

static char *getval(int fd, char *key, char *val)
{
	int ch;
	char buff[1024];
	int len = strlen(key);
	char *ret;

	while(1)
	{
		read(fd, buff, len);
		if(strchr(buff, key[0]) == NULL )
		{
			continue;
		}
	}

	return ret;
}

void play_cmd(char *cmd)
{
	lock();

	if(play.fi != -1)
	{
		write(play.fi, cmd, strlen(cmd));
		if(strcmp(cmd, "pause\n") == 0)
		{
			_pause = !_pause;
		}
	}

	unlock();
}

/*
 * 
 */
static int buffering(struct play_t *this)
{
	struct stat st;
	off_t buff_len = this->flen*2/100;
	off_t pos = this->pos/100.0 * this->flen;

	stat(this->fname, &st);
	this->ldlen = st.st_size;

	if((this->ldlen - pos < buff_len) && (this->ldlen < this->flen))
	{
		if(this->fi != -1)
		{
	//		sleep(1);
			if(!_pause) play_cmd("pause\n");
		}

		while((this->ldlen - pos < buff_len) && (this->ldlen < this->flen))
		{
			stat(this->fname, &st);
			this->ldlen = st.st_size;
//			printf("\rbuffering...%d%%", (this->ldlen - pos)*100/buff_len);
//			fflush(NULL);

			usleep(100 * 1000);

			if(playing == 0)
				break;
		}

		if(this->fi != -1)
		{
			if(!_pause) play_cmd("pause\n");
		}
	}
/*
	printf("\r[%d %d %d %d]", this->pos,
		(int)((double)this->ldlen/this->flen * 100), 
		this->ldlen/1024, this->flen/1024);
*/
//	fflush(NULL);

	return 0;
}

/*
 * 播放线程
 */
static void *thr_play(void *arg)
{
	struct play_t *this = arg;
	int fi[2];
	int fo[2];
	char *p,buff[1024];

	this->pos = 0;
	this->ldlen = 0;
	this->fi = -1;
	this->fo = -1;

	buffering(this);

	pipe(fi);
	pipe(fo);

	signal(SIGCHLD, do_wait);

	if(fork() == 0)
	{
		dup2(fi[0], 0);
		dup2(fo[1], 1);

		close(fi[0]);
		close(fi[1]);
		close(fo[0]);
		close(fo[1]);

		close(2);

		execlp("mplayer", "mplayer", "-slave", "-quiet", 
			this->fname, NULL);

		exit(-1);
	}

	close(fi[0]);
	close(fo[1]);
	this->fi = fi[1];
	this->fo = fo[0];

	while(playing)
	{
		strcpy(buff, "get_percent_pos\n");

		lock();
		if(!_pause)
		{
			write(fi[1], buff, strlen(buff));
			memset(buff, 0, sizeof(buff));
			read(fo[0], buff, sizeof(buff));
			if(!strncmp(buff, "ANS_PERCENT_POSITION=", 
				strlen("ANS_PERCENT_POSITION=") ) )
			{
				p = strchr(buff, '=');
				this->pos = atoi(p+1);
			}
		}
		unlock();

		if(this->pos == 100)
			break;

		buffering(this);
		usleep(100 * 1000);
	}

	close(fi[1]);
	close(fo[0]);

	remove(play.fname);
	return NULL;
}

void trim(char *s)
{
	char *p = s + strlen(s) - 1;
	while(p>=s && *p == ' ')
	{
		*p = 0;
		p++;
	}

	p = s;
	while(*p == ' ') p++;
	strcpy(s, p);
}

//int main(int argc, char *argv[])
void play_start(char *tmp_file, off_t size)
{
	strcpy(play.fname, tmp_file);
	play.flen = size;
	playing = 1;
	_pause = 0;
	pthread_create(&tid_play, NULL, thr_play, &play);

	//pthread_join(tid_play, NULL);
}

void play_stop(void)
{
	write(play.fi, "stop\n", 5);
	pthread_join(tid_play, NULL);
	playing = 0;
}

static void *thr_load(void *arg)
{
	char *file = arg;
	char *tmp_file = ".tmp";
	struct stat st;
	int fd, fd_tmp;
	char buff[4*1024];
	int len;

	file = strchr((char *)arg, '\n');
	if(file) *file = 0;

	file = arg;
	trim(file);
	fd = open(file, O_RDONLY);
	free(file);

	fd_tmp = open(tmp_file, O_WRONLY|O_CREAT|O_TRUNC, 0644);

	fstat(fd, &st);
	play_start(tmp_file, st.st_size);

	while( loading && (len = read(fd, buff, sizeof(buff))>0) )
	{
		write(fd_tmp, buff, len);
	}

	close(fd);
	close(fd_tmp);
	loading = 0;
	return NULL;
}

void loadfile(char *file)
{
	char *p;

	printf("%d\n", __LINE__);
	if(loading)
	{
		loading = 0;
		pthread_join(tid_load, NULL);
	}

	printf("%d\n", __LINE__);
	if(playing)
	{
		play_stop();
	}

	printf("%d\n", __LINE__);
	p = malloc(strlen(file)+1);
	pthread_create(&tid_load, NULL, thr_load, p);
	printf("%d\n", __LINE__);
}

int main(int argc, char **argv)
{
	char cmd[100];
	play_start("tmp", 1000000);
	while(1)
	{
		printf("$>");
		fgets(cmd, sizeof(cmd), stdin);

		if(!strncmp(cmd, "load", 4))
		{
			loadfile(cmd + 4);
		}
		else if(!strcmp(cmd, "stop\n"))
		{
			play_stop();
			return 0;
		}
		else play_cmd(cmd);
	}
}
