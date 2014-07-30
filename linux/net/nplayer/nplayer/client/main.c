
#include "../incl/comm.h"

//#define  DATFILE   pl_fname() 
#define  DATFILE   "./dat"

char path[256] = ".";
static	char *ip = "127.0.0.1";
static pthread_t download_tid = 0;
static volatile int loading = 0;

static int sp = 0;
static int stack[128];

void push(int idx)
{
	stack[sp++] = idx;
}

int pop(void)
{
	return stack[--sp];
}

static void cd(char *dir)
{
	char *p;

	if( strcmp(dir, "..") == 0)
	{
		p = strrchr(path, '/');
		if(p == NULL)  return;
		*p = 0;

		tftp_get(ip,  path, "ls.txt", NULL);
		pl_load("ls.txt");
		pl_set_idx( pop() );
	}
	else
	{
		push( pl_idx() );

		strcat(path, "/");
		strcat(path, dir);

		tftp_get(ip,  path, "ls.txt", NULL);
		pl_load("ls.txt");
	}
}

void *thrd_download(void *arg)
{
	char  fname[256];
	int ret;

	pl_stat.length = pl_fsize();

	sprintf(fname, "%s/%s", path, pl_fname());

	pl_stat.loaded = 0;
	loading = 1;
	LOGPRINT("get:%s\n", fname);
	ret = tftp_get(ip, fname, DATFILE, &pl_stat.loaded);
	loading = 0;
	return NULL;
}

void download(void)
{
	LOGPRINT("loading:%d\n", loading);
	if(loading)
	{
		LOGPRINT("cancel download\n");
		pthread_cancel(download_tid);
		pthread_join(download_tid, NULL);
		LOGPRINT("cancel download\n");
		loading = 0;
	}

	pthread_create(&download_tid, NULL, thrd_download, NULL);
}

int main(int argc, char **argv)
{
	int key;
	
	if(argc < 2)
	{
		printf("Usage:%s <server IP>\n", argv[0]);
		return -1;
	}

	ip = argv[1];
	key = tftp_get(ip,  ".", "ls.txt", NULL);
	if(key == -1)
	{
		printf("网络连接失败\n");
		return -1;
	}

	printf("\033[?25l");

	mw_init();

	pl_load("ls.txt");
	remove("ls.txt");
	mw_refresh();

	system("stty -echo");

	while(1)
	{
		key = get_key(0);
		if(key == ESC)
		{
			play_stop();
			break;
		}

		switch(key)
		{
		case 'p':
		case 'P':
			write_cmd("pause");
			break;
		case UP:
			pl_idx_dec();
			break;
		case DOWN:
			pl_idx_inc();
			break;
		case ENTER:
		case RIGHT:
			if(pl_ftype() == 'd')
			{
//				push(pl_idx());
				cd(pl_fname());
//				tftp_get(ip,  path, "ls.txt", NULL);
//				pl_load("ls.txt");
//				remove("ls.txt");
			}
			else if(key == ENTER)
			{
				play_stop();
				remove(DATFILE);
				download();
				play(DATFILE);
			}
			break;
		case LEFT:
			cd("..");
//			tftp_get(ip,  path, "ls.txt", NULL);
//			pl_load("ls.txt");
//			pl_set_idx(	pop() );
//			remove("ls.txt");
			break;
		}

		mw_refresh();
	}

	printf("\033[?25h");

	remove(DATFILE);

	system("stty echo");
	system("clear");
	return 0;
}


