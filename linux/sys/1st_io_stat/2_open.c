#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    int fpr, fpw;
    char buf[1024];
    int ret;

    if (argc < 3)
        return 0;

/*
 *    r       O_RDONLY 
 *    r+      O_RDWR
 *    w       O_WRONLY | O_CREAT | O_TRUNC
 *    w+      O_RDWR | O_CREAT | O_TRUNC
 *    a       O_WRONLY | O_CREAT | O_APPEND
 *    a+      O_RDWR | O_CREAT | O_APPEND 
 */
    fpr = open(argv[1], O_RDONLY);
    if (-1 == fpr)
    {
        fprintf(stderr, "open src failed!\n"); 
        goto ERR1;
    }

    fpw = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0777);
    if(-1 == fpw)
    {
        fprintf(stderr, "open dst failed!\n");
        goto ERR2;
    }

    while (1)
    {
        ret = read(fpr, buf, sizeof(buf));
        if (ret <= 0)
            break;
        write(fpw, buf, ret);
    }

    close(fpr);
    close(fpw);

    return 0;

ERR2:
    close(fpr);
ERR1:
    return 0;
}
