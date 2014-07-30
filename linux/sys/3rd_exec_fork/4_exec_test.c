#include <stdio.h>

extern char **environ;

int main(int argc, char *argv[])
{
    int i = 0;

    printf("test_start pid %d %d!\n", getpid(), getppid());
    printf("getuid = %d\n", getuid());

    while (environ[i] != NULL)
    {
        printf("%s\n", environ[i++]); 
    }

    
    /*
     *for (i = 0; i < argc; i++)
     *{
     *    printf("argv[%d] = %s\n", i, argv[i]); 
     *}
     */

    getchar();

    printf("test_end!\n");

    return 0;
}
