
#include "fb.h"


#define BMP_TORASTER_OFFSET	10
#define BMP_SIZE_OFFSET		18
#define BMP_BPP_OFFSET		28


int is_bmp(char *filename)
{
	int fd;
	char id[2] = {0};

	if((fd = open(filename, O_RDONLY)) == -1)
		return 0;

	read(fd, id, 2);

	close(fd);

	if(id[0] == 'B' && id[1] == 'M')
		return 1;

	return 0;
}

int bmp_load(char *filename,unsigned char *buffer,int x_pic,int y_pic)
{
	int fd;
	short bpp;
	int raster;
	int x;
	int y;
	int ret = -1;
	unsigned char buff[4];
	unsigned char *buffer_cur = buffer + x_pic * (y_pic - 1) * 3;

	if((fd = open(filename,O_RDONLY)) == -1)
		return -1;

	if(lseek(fd,BMP_TORASTER_OFFSET,SEEK_SET) == -1)
		goto error;

	read(fd,&raster,4);

	if(lseek(fd, BMP_BPP_OFFSET, SEEK_SET) == -1)
		goto error;

	read(fd, &bpp, 2);

	if(bpp != 24)
		goto error;

	lseek(fd, raster, SEEK_SET);

	for(y = 0; y < y_pic; y++)
	{
		for(x = 0; x < x_pic; x++)
		{
			read(fd, buff, 3);
			*buffer_cur++ = buff[2];
			*buffer_cur++ = buff[1];
			*buffer_cur++ = buff[0];
		}
		buffer_cur -= x_pic * 3 * 2;
	}

	ret = 0;

error:
	
	close(fd);

	return ret;
}

int bmp_getsize(char *filename,int *x_pic,int *y_pic)
{
	int fd;
	int ret = -1;

	if(filename == NULL || x_pic == NULL || y_pic ==  NULL)
		return -1;

	if((fd = open(filename, O_RDONLY)) == -1)
		return -1;

	if(lseek(fd, BMP_SIZE_OFFSET, SEEK_SET) == -1)
		goto error;

	if((read(fd, x_pic, 4)) != 4)
		goto error;

	if((read(fd, y_pic, 4)) != 4)
		goto error;

	ret = 0;
error:

	close(fd);

	return ret;
}


