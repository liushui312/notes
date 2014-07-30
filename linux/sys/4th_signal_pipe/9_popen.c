#include <stdio.h>

int main(void)
{
    FILE *fpr = NULL, *fpw = NULL;
    char buf[1024];
    int ret;

    fpr = popen("cat /etc/passwd", "r");
    fpw = popen("grep root", "w");

    while ((ret = fread(buf, 1, sizeof(buf) - 1, fpr)) > 0)
    {
        fwrite(buf, 1, ret, fpw);
        /*buf[ret] = '\0';*/
        /*printf("ret = %d, buf = %s\n", ret, buf);*/
    }


    return 0;
}
