#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/soundcard.h>

int main(void)
{
    int fd, audio, ret;
    char buf[512];
    /*采样频率 声道数 量化位*/

    fd = open("/dev/dsp", O_RDWR);
    if (fd == -1)
    {
        perror("open"); 
        return 0;
    }

    ret = 44100;
    ioctl(fd, SOUND_PCM_WRITE_RATE, &ret);
    ret = 16;
    ioctl(fd, SOUND_PCM_WRITE_BITS, &ret);
    ret = 2;
    ioctl(fd, SOUND_PCM_WRITE_CHANNELS, &ret);

    audio = open("123.raw", O_RDONLY);
    if (audio == -1)
        perror("audio");

    while ((ret = read(audio, buf, sizeof(buf))) > 0)
    {
        write(fd, buf, ret); 
    }
    
    close(fd);

    return 0;
}
