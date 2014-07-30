#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/fb.h>
#include <sys/mman.h>

int main(void)
{
    int fd;
    struct fb_fix_screeninfo finfo;
    struct fb_var_screeninfo vinfo;
    int *m = NULL;
    int i, j;

    fd = open("/dev/fb0", O_RDWR);
    if (fd == -1)
        perror("open"); 

    ioctl(fd, FBIOGET_FSCREENINFO, &finfo);
    ioctl(fd, FBIOGET_VSCREENINFO, &vinfo);

    printf("w = %d, h = %d, bpp = %d, len = %d\n",
            vinfo.xres, vinfo.yres, vinfo.bits_per_pixel, finfo.smem_len);

    m = mmap(NULL, finfo.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (m == NULL)
        perror("mmap");

#if 0
    for (i = 0; i < vinfo.yres; i++)
    {
        for (j = 0; j < vinfo.xres; j++) 
        {
            if ((i >= 0 && i <= 10) || 
                (i >= vinfo.yres - 11 && i <= vinfo.yres - 1) || 
                (j >= 0 && j <= 10) || 
                (j >= vinfo.xres - 11 && j <= vinfo.xres - 1))
                m[j + i * vinfo.xres] = 0xff0000;
            else
                m[j + i * vinfo.xres] = 0xffffff;
        }
    }
#endif

    for (i = 100; i < 600; i++)
    {
        for (j = 200; j < 600; j++)
        {
            m[j + i * vinfo.xres] = 0xff0000;
        
        }
    }


    munmap(m, finfo.smem_len);

    close(fd);

    return 0;
}
