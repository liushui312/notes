#include <stdio.h>
#include <string.h>
#include <sys/types.h>

char name[128];

char *getname(uid_t id)
{
    FILE *fp = NULL;
    char buf[1024], *p = NULL;

    fp = fopen("passwd", "r");
    if (fp == NULL)
    {
        perror("fopen");
        return 0;
    }

    while (fgets(buf, sizeof(buf), fp) != NULL)
    {
        p = strchr(buf, ':');
        *p = '\0';
        if (id == atoi(strchr(p + 1, ':') + 1))
        {
            fclose(fp);
            strcpy(name, buf);
            return name;
        }
    } 

    printf("BUFSIZ = %d\n", BUFSIZ);

    fclose(fp);

    return NULL;
}

int main(int argc, char *argv[])
{
    printf("name = %s\n", getname(atoi(argv[1]))); 
    return 0;
}
