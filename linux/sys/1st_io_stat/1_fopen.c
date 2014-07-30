#include <stdio.h>

int main(int argc, char *argv[])
{
    FILE *fpr = NULL, *fpw = NULL;
    char buf[1024];
    int ret;

    if (argc < 3)
        return 0;

/*
 *    r       只读
 *    r+      可读可写
 *    w       只写, 文件不存在会创建,文件存在会清空
 *    w+      比w功能多一个可读功能
 *    a       追加,文件不存会创建
 *    a+      比a功能多一个可读功能
 *
 */
    fpr = fopen(argv[1], "r");
    if (NULL == fpr)
    {
        fprintf(stderr, "fopen src failed!\n"); 
        goto ERR1;
    }

    fpw = fopen(argv[2], "w");
    if(NULL == fpw)
    {
        fprintf(stderr, "fopen dst failed!\n");
        goto ERR2;
    }

    /*
     *fgetc
     *fgets
     *fscanf
     *fread
     */

    while (1)
    {
        ret = fread(buf, sizeof(char), sizeof(buf), fpr);
        if (ret <= 0)
            break;
        fwrite(buf, 1, ret, fpw);
    }

    fclose(fpr);
    fclose(fpw);

    return 0;

ERR2:
    fclose(fpr);
ERR1:
    return 0;
}
