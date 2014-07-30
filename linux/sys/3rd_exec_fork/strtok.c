#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int str_tok(char *str, char *delim, char *save[])
{
    int count = 0;

    save[count] = strtok(str, delim);
    if (NULL == save[count])
        return -1;

    while (save[count] != NULL)
    {
         count++;
         save[count] = strtok(NULL, delim);
    }

    return count;
}

int main(void)
{
    char str[] = "..f hello,world the. hell.sldfk,d  klsf ,";
    char delim[] = ", .";
    char *save[20];
    int ret, i = 0;

    ret = str_tok(str, delim, save);
    printf("count = %d\n", ret);

    while (save[i] != NULL)
    {
        printf("%s\n", save[i++]);
    }

   /* for (i = 0; i < ret; i++)*/
        /*free(save[i]);*/

    return 0;
}
