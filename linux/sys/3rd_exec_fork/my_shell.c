#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int str_token(char *str, char *delim, char *save[], int save_max)
{
    int count = 0;

    if ((save[count] = strtok(str, delim)) == NULL)
        return -1;
    while (save[count++] != NULL)
    {
        if (count >= save_max)
        {
            save[count] = NULL;
            break;
        }
        save[count] = strtok(NULL, delim);
    }

    return count;
}
        
void my_system(const char *command)
{
    char *arg[50] = {"sh"};
    char *c = NULL;
    int ret, i = 1;

    c = malloc(strlen(command) + 1);
    strcpy(c, command);

    ret = str_token(c, " ", arg + 1, 50);
    if (ret == -1)
    {
        printf("Unkown command!\n");
        return;
    }
    
    printf("%s\n", arg[0]);
    while(arg[i] != NULL)
        printf("%s\n", arg[i++]);
    
    if (fork() == 0)
    {
        execvp(arg[0], arg);
        perror("execvp");
        exit(0); 
    }

    wait(NULL);

    free(c);
}

int main(void)
{
    char buf[128];
    int len;
    
    while (1)
    {
        printf("[liu]# "); 
        fgets(buf, sizeof(buf), stdin);
        len = strlen(buf);
        if (buf[len - 1] == '\n')
            buf[len -1] = '\0';   

        if (!strcmp(buf, "exit"))
            break;
        
        my_system(buf);
    }
    return 0;
}
