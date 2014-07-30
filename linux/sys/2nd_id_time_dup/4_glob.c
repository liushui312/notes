#include <stdio.h>
#include <glob.h>

int main(int argc, char *argv[])
{
    glob_t g;
    int i;

    /*printf("argv[1] = %s\n", argv[1]);*/
    if (glob(argv[1], 0, NULL, &g) == -1)
        perror("glob");

    for (i = 0; i < g.gl_pathc; i++)
    {
        printf("%s\n", g.gl_pathv[i]);
    }

    globfree(&g);

    return 0;
}
