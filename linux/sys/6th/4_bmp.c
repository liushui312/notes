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

#define RGB16(c)    ((((c >> 19) & 0x1F) << 11) | \
                    (((c >> 10) & 0x3F) << 5) | \
                    ((c >> 3) & 0x1F))

#define RGB32(c)    ((((c >> 11) & 0x1F) << 19) | \
                    (((c >> 5) & 0x3F) << 10) | \
                    ((c & 0x1F) << 3))

int main(void)
{
    int fd, img;
    struct fb_fix_screeninfo finfo;
    struct fb_var_screeninfo vinfo;
    int *m = NULL;
    int i, j, offset;

    fd = open("/dev/fb0", O_RDWR);
    if (fd == -1)
        perror("open"); 

    ioctl(fd, FBIOGET_FSCREENINFO, &finfo);
    ioctl(fd, FBIOGET_VSCREENINFO, &vinfo);

    printf("w = %d, h = %d, bpp = %d, len = %d\n",
            vinfo.xres, vinfo.yres, vinfo.bits_per_pixel, finfo.smem_len);

    img = open("bmp/3.bmp", O_RDONLY);
    if (img == -1)
        perror("img");

    lseek(img, 10, SEEK_SET);
    read(img, &offset, 4);
    lseek(img, offset, SEEK_SET);

    m = mmap(NULL, finfo.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (m == NULL)
        perror("mmap");

    for (i = vinfo.yres - 1; i >= 0; i--)
    {
        for (j = 0; j < vinfo.xres; j++)
        {
            read(img, &offset, 3);
            m[j + i * vinfo.xres] = offset;
        }
    }

    munmap(m, finfo.smem_len);

    close(fd);

    return 0;
}
