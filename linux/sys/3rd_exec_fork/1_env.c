#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern char **environ;

/*int main(void)*/
/*int main(int argc, char *argv[], char *env[])*/
int main(int argc, char *argv[])
{
    int i = 0;

    printf("home = %s\n", getenv("HOME"));
    printf("home = %s\n", getenv("aa"));

    setenv("bb", "/kyo", 1);
    /*putenv("bb=/kyo");*/
    printf("bb = %s\n", getenv("bb"));

    /*
     *while (environ[i] != NULL)
     *{
     *    printf("env[%d] = %s\n", i, environ[i]);
     *    i++;
     *}
     */

    return 0;
}
