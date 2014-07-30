#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void my_system(const char *command)
{
    char *c = NULL, *p = NULL;
    char *s[100];
    int i = 0, j;

    c = malloc(strlen(command) + 1);
    strcpy(c, command);

    p = c;
    while ((s[i++] = strtok(p, " ")) != NULL)
        p = NULL;

    /*
     *for (j = 0; j < i; j++)
     *{
     *    printf("s[%d] = %s\n", j, s[j]); 
     *}
     */

    if (fork() == 0)
    {
        execvp(s[0], s);
        /*execl("/bin/sh", "sh", "-c", command, NULL); */
        perror("exec");
    }
    wait(NULL);

    free(c);
}

int main(void)
{
    char buf[1024];
    char *path = NULL;
    char *user = NULL;

    while (1)
    {
        user = getenv("USER");
        path = getenv("PWD");
        printf("[%s@%s]# ", user, path);
        fgets(buf, sizeof(buf), stdin);
        if (buf[strlen(buf) - 1] == '\n')
            buf[strlen(buf) - 1] = '\0';
        if (!strncmp(buf, "exit", 4))
            break;
        if (!strncmp(buf, "cd ", 3))
        {
            chdir(buf + 3);
            setenv("PWD", buf + 3, 1);
        }
        else
            my_system(buf);
    }

    return 0;
}
