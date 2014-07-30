
#include "../incl/comm.h"
#include <sys/times.h>

#define TITLE  "网络视频点播系统(v1.0, 陈映宇)"
#define TAIL "<上,下:移动> <左:上级目录> <右:子目录> <回车:播放> <ESC:退出>"

#define BUFF_LOW	60	
#define BUFF_UP		120

struct pl_list_t pl_list;
struct pl_stat_t pl_stat;
static int buff_percent = -1;
static int load_speed = -1;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//将int转为K/M为单位的字符串
char *itoh(unsigned int i, char *h, int n)
{
	if(i < 1024)
		snprintf(h, n, "%d", i);
	else if(i<1024*1024)
		snprintf(h, n, "%.2fK", i/1024.0);
	else if(i<1024*1024*1024)
		snprintf(h, n, "%.2fM", i/(1024.0*1024));
	else 
		snprintf(h, n, "%.2fG", i/(1024.0*1024*1024));
	
	return h;
}

static void lock(void)
{
	pthread_mutex_lock(&mutex);
}

static void unlock(void)
{
	pthread_mutex_unlock(&mutex);
}

static void *thrd_win(void *arg)
{
	time_t t;
	struct tm *tm;
	char buf[64];
	char *p;
	float buff_pos= -1;
	float len;
	struct  tms tms;

	//计算下载速度用
	clock_t  clk;
	int  pos;		

	times(&tms);
	clk = tms.tms_stime;
	pos = 0;
//	HZ = sysconf(_SC_CLK_TCK);

	while(1)
	{
		t = time(NULL);
		tm = localtime(&t);

		p = get_val("get_time_pos", buf);
		if(p) pl_stat.time_pos = atof(p);

		p = get_val("get_time_length", buf);
		if(p) pl_stat.time_length = atof(p);
		else pl_stat.time_length = 0;

		//计算下载速度
		if(pl_stat.loaded < pl_stat.length)
		{
			move_xy(30, MW_H - 2);
			times(&tms);
			len = (float)(tms.tms_stime - clk) / sysconf(_SC_CLK_TCK);//时间隔
			
			load_speed = (pl_stat.loaded - pos) / len;  //下载速度
			itoh(len, buf, sizeof(buf));

			clk = tms.tms_stime;
			pos = pl_stat.loaded;
		}
		else load_speed = -1;

		//缓冲长度不够时，暂停播放
		if( (pl_stat.time_length > 0.1) && pl_stat.loaded < pl_stat.length)
		{
			len = pl_stat.time_length * pl_stat.loaded /pl_stat.length;
			len -= pl_stat.time_pos;
			
			//暂停
			if(buff_pos < 0 && len < BUFF_LOW)
			{
				buff_pos = pl_stat.time_pos;
				LOGPRINT("pause\n");
				write_cmd("pause");
			}

			//继续
			if(buff_pos >= 0 && len > BUFF_UP)
			{
				buff_pos = -1;
				LOGPRINT("resume\n");
				buff_percent = -1;
			}

			if( (buff_pos >= 0) )//&& (pl_stat.time_pos - buff_pos> 1) )
			{
				sprintf(buf, "seek %.1f 2", buff_pos);
				LOGPRINT("wrcmd:%s\n", buf);
				write_cmd(buf);
				write_cmd("pause");
				buff_percent = 100.0*len/BUFF_UP;
			}
		}

		lock();
		move_xy(MW_W -10, 2);	
//		printf("\033[7m");
		printf("%02d:%02d:%02d", tm->tm_hour, tm->tm_min, tm->tm_sec);
		printf("\033[0m");
		mw_draw_stat();
	//	move_xy(10, MW_H -2);
		//printf("%d %.1f %.1f  ", buff_percent, len, buff_pos);
		fflush(NULL);
		unlock();

		usleep(100 * 1000);
	}
}

void mw_init(void)
{
	pthread_t tid;

	clear_screen();

	memset(&pl_list, 0, sizeof(pl_list));
	memset(&pl_stat, 0, sizeof(pl_stat));

	pthread_create(&tid, NULL, thrd_win, NULL);
}

void pl_clear(void)
{
	memset(&pl_list, 0, sizeof(pl_list));
}

void pl_add(struct pl_item_t *item)
{
	if(pl_list.cnt < MAX_ITEM-1)
	{
		pl_list.items[pl_list.cnt++] = *item;
	}
}

char pl_ftype(void)
{
	if(pl_list.idx < 0)
		return 0;
	
	return pl_list.items[pl_list.idx].type;
}

int pl_fsize(void)
{
	if(pl_list.idx < 0)
		return 0;
	return pl_list.items[pl_list.idx].size;
}

char *pl_fname(void)
{
	if(pl_list.idx < 0)
		return NULL;
	
	return pl_list.items[pl_list.idx].fname;
}

void pl_set_idx(int idx)
{
	if(idx >= pl_list.cnt)
		idx = pl_list.cnt-1;
	
	pl_list.idx = idx;
}

void pl_idx_inc(void)
{
	if( ++pl_list.idx == pl_list.cnt)
	{
		pl_list.idx = 0;
	}
}

void pl_idx_dec(void)
{
	if(pl_list.idx-- == 0)
	{
		pl_list.idx = pl_list.cnt -1;
	}
}

int  pl_idx(void)
{
	return pl_list.idx;
}

/*
 * 列表文件格式:type size fname
 * type='-':文件, ='d':目录 
 */
int pl_load(char *fname)
{
	FILE *fp;
	char buff[256];
	char *p;
	struct pl_item_t item;

	pl_clear();

	fp = fopen(fname, "r");
	if(fp == NULL)
	{
		return -1;
	}

	while( fgets(buff, sizeof(buff), fp) )
	{
		p = strchr(buff, '\n');
		if(p) *p = 0;

		memset(&item, 0, sizeof(item));
		item.type = buff[0];

		p = strchr(buff+2, ' ');
		*p = 0;
		item.size = atoi(buff+2);

		strcpy(item.fname, p+1);

		pl_add(&item);
	}

	fclose(fp);

	mw_refresh();

	return 0;
}
//进度条
void mw_draw_stat(void)
{
	int i;
	int left, top;
	char buf[32];
	//left = (MW_W - BAR_LEN) / 2 + 1;
	left = 10;
	top = BAR_TOP;
	move_xy(left-1, top);
	
	if(pl_stat.length == 0)
	{
		putchar('[');
		for(i=0; i<BAR_LEN; i++)
			putchar(' ');
		putchar(']');
	}
	else
	{
		putchar('[');

		for(i=0; i < (float)pl_stat.loaded/pl_stat.length * BAR_LEN; i++)
		{
			putchar('-');
		}
		for(; i< BAR_LEN; i++)
		{
			putchar(' ');
		}
		putchar(']');
		//printf("[%d/%d]", pl_stat.loaded/1024, pl_stat.length/1024);
		if(pl_stat.time_length > 0)
		      printf("%6.1f/%-6.1f ", pl_stat.time_pos, pl_stat.time_length);
		else  printf("%15s", "");

		move_xy(left, top+1);
		if(buff_percent >= 0)
			printf("缓冲...%3d%%", buff_percent);
		else 	
			printf("%12s", "");

		buf[0] = 0;
		move_xy(left + 15, top+1);
		if(load_speed >= 0)
		{
			itoh(load_speed, buf, sizeof(buf));
			strcat(buf, "/s");
		}
		printf("%10s", buf);

		if(pl_stat.time_length > 0)
		{
			i = (float)pl_stat.time_pos/pl_stat.time_length *BAR_LEN;
			move_xy(left + i, top);
			putchar('#');
		}
	}

	move_xy( 1, MW_H+1);
}

extern char path[];
void mw_draw_list(void)
{
	int i, n;
	int left, top;
	char buf[32];
	char *p;

	//画播放列表
	left = (MW_W - LIST_W-2)/2 + 1;
	top = LIST_TOP;

	move_xy(left, top-1);
	printf("                            ");
	move_xy(left, top-1);
	if(pl_list.cnt > 0)
	{
		printf("[%3d/%-3d]", pl_list.idx+1, pl_list.cnt);
		
		n = LIST_W - 9;

		p = path;
		if(strlen(p) > n)
		{
			p = path + strlen(p) - n;  //path长度>n时，只显示后n个字节
		}

		printf("%-*.*s", LIST_W-9, LIST_W-9, p);
	}

	rectangle(left, top, LIST_W+2, LIST_H+2);

	left++;
	top++;

	i = pl_list.idx/LIST_H * LIST_H; 
	for(n = 0; (i < pl_list.cnt) && (n < LIST_H); i++, n++)
	{
		if(pl_list.items[i].type == 'd')
			printf("\033[34m");

		if(i == pl_list.idx)
		{
			printf("\033[7m");		//反显
		}

		move_xy(left, top+n); 
		printf("%*s", LIST_W,  "");

		move_xy(left, top+n); 
		itoh(pl_list.items[i].size, buf, sizeof(buf));

		printf("%c %8s %.*s", pl_list.items[i].type, buf,
			LIST_W-12, pl_list.items[i].fname);

		printf("\033[0m");
	}

	for(; n<LIST_H;n++)
	{
		move_xy(left, top+n); 
		printf("%*s",LIST_W,  "");
	}

	if(pl_list.cnt == 0)
	{
		move_xy(left, top);
		printf("(^_^) 列表为空");
	}
}

void mw_refresh(void)
{
	lock();

	rectangle(1, 1, MW_W, MW_H);
	move_xy( (MW_W - strlen(TITLE))/2, 2);
	printf(TITLE);
	move_xy( 10, MW_H-1);
	printf(TAIL);

	line_h(2, 3, MW_W-2, '-');
	//
	mw_draw_list();
	mw_draw_stat();

	move_xy( 1, MW_H+1);

	fflush(NULL);

	unlock();
}

void mw_putmsg(char *s)
{
	lock();
	move_xy( 1, MW_H+1);
	printf("%-*.*s", MW_W, MW_W, s);
	unlock();
}




