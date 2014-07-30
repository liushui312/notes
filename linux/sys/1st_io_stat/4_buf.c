#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
    FILE *fp = NULL;
    char buf[1024];
    int fd;

#if 0
    fp = fopen("test_open", "w");
    if (NULL == fp)
    {
        perror("fopen"); 
        return 0;
    }

    setvbuf(fp, buf, _IOFBF, sizeof(buf));

    fwrite("hello world", 1, 11, fp);
    printf("buf = %s\n", buf);

    sleep(10);

    fclose(fp);
#endif
    
    fd = open("test_open", O_WRONLY | O_CREAT | O_TRUNC);
    if (fd == -1)
        perror("open");

    write(fd, "hello world", 11);

    sleep(10);


    close(fd);


    return 0;
}
