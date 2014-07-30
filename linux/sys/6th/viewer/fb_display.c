
#include "fb.h"

#define ERROR(condition,msg)	\
if(condition)			\
{				\
	fprintf(stderr,"%s: %d: %s: %s\n",__FILE__,__LINE__,msg,strerror(errno));	\
	exit(-1);		\
}


void fb_display(unsigned char *rgb_buf,
		int x_pic, int y_pic, 
		int x_pic_offs, int y_pic_offs, 
		int x_scr_offs, int y_scr_offs)
{
	struct fb_var_screeninfo var;
	
	void *image_buf = NULL;
	unsigned short *image_buf_16 = NULL;
	unsigned int *image_buf_32 = NULL;
	unsigned char *image_buf_cur = NULL;
	
	unsigned char *fb_buf = NULL;
	unsigned char *fb_buf_cur = NULL;
	
	int ret;
	int fd;
	int i;	
	
	int x_scr;
	int y_scr;
	int screen_pixel;
	int pic_pixel;
	int x_min;
	int y_min;
	int cpp;

	fd = open(FB_PATH,O_RDWR);
	ERROR(fd == -1,"open()");

	ret = ioctl(fd, FBIOGET_VSCREENINFO, &var);
	ERROR(ret != 0,"ioctl()");

	x_scr = var.xres;
	y_scr = var.yres;
	screen_pixel = var.xres * var.yres;
	pic_pixel = x_pic * y_pic;



	switch(var.bits_per_pixel)
	{
		case 16:
			cpp = 2;
			image_buf_16 = calloc(pic_pixel,2);
			for(i = 0;i < pic_pixel;i++)
			{
				unsigned char r = rgb_buf[i * 3 + 0];
				unsigned char g = rgb_buf[i * 3 + 1];
				unsigned char b = rgb_buf[i * 3 + 2];

				image_buf_16[i] = (((r >> 3) & 0x1f) << 11) | 
					(((g >> 2) & 0x3f) << 5) | 
					((b >> 3) & 0x1f);
			}
			image_buf = image_buf_16;
			break;
		case 24:
		case 32:
			cpp = 4;
			image_buf_32 = calloc(pic_pixel,4);
			for(i = 0;i < pic_pixel;i++)
				image_buf_32[i] = *(unsigned int *)(rgb_buf + i * 3) & 0xffffff;
			image_buf = image_buf_32;
			break;
		default:
			fprintf(stderr,"unsuported video mode!!!\n");
			exit(-1);
			break;
	}


	if(x_pic_offs > x_pic - x_scr)
		x_pic_offs = 0;
	if(y_pic_offs > y_pic - y_scr)
		y_pic_offs = 0;

	if(x_scr_offs + x_pic > x_scr)
		x_scr_offs = 0;
	if(y_scr_offs + y_pic > y_scr)
		y_scr_offs = 0;

	if(x_pic < x_scr)
		x_min = x_pic;
	else
		x_min = x_scr;

	if(y_pic < y_scr)
		y_min = y_pic;
	else
		y_min = y_scr;

	fb_buf = mmap(NULL,screen_pixel * cpp,PROT_WRITE | PROT_READ,MAP_SHARED,fd,0);
	ERROR(fb_buf == MAP_FAILED,"mmap()");

	fb_buf_cur = fb_buf + (y_scr_offs * x_scr + x_scr_offs) * cpp;
	
	image_buf_cur = image_buf + (y_pic_offs * x_pic + x_pic_offs) * cpp;

	i = 0;
	while(i < y_min)
	{
		memcpy(fb_buf_cur,image_buf_cur,x_min * cpp);
		fb_buf_cur += x_scr * cpp;
		image_buf_cur += x_pic * cpp;
		i++;
	}	
	
	munmap(fb_buf,screen_pixel * cpp);
	free(image_buf);
	close(fd);
}

int screen_getsize(int *x_scr,int *y_scr)
{
	struct fb_var_screeninfo var;
	int fd;

	if((fd = open(FB_PATH,O_RDWR)) == -1) 
		return -1;

	if(ioctl(fd, FBIOGET_VSCREENINFO, &var) != 0)
		return -1;

	if(x_scr == NULL || y_scr == NULL)
		return -1;

	*x_scr = var.xres;
	*y_scr = var.yres;

	close(fd);

	return 0;
}


