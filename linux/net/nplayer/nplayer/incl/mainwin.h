
#ifndef _MAINWIN_H_
#define _MAINWIN_H_

#define MW_W		80
#define MW_H		23

#define LIST_TOP	5
#define LIST_W 		68
#define LIST_H		13
#define BAR_TOP		(LIST_TOP + LIST_H + 2)
#define BAR_LEN		50

#include <unistd.h>

#define MAX_ITEM	1024

struct pl_item_t{
	char  type;
	int   size;
	char fname[128];
};

struct pl_list_t{
	int idx;
	int cnt;
	struct pl_item_t items[MAX_ITEM];
};

struct pl_stat_t{
	char name[256];		//曲名
	int   length;		//文件总长度
	int   loaded;		//已下载的长度
	float time_length;
	float time_pos;
};

extern struct pl_stat_t pl_stat;
extern struct pl_list_t pl_list;

int  pl_fsize(void);
char pl_ftype(void);
char *pl_fname(void);
void pl_clear(void);
void pl_idx_inc(void);
void pl_idx_dec(void);
void pl_set_idx(int idx);
int  pl_load(char *fname);
int  pl_idx(void);

void mw_init(void);
void mw_refresh(void);
void mw_draw_stat(void);
void mw_draw_list(void);

void mw_putmsg(char *msg);

#endif //_MAINWIN_H_


