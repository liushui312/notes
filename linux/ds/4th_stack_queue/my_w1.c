#include <stdio.h>
#include <stdio.h>

#define MAX     30

int isempty(int end)
{
    if (end == 0)
        return 1;
    return 0;
}

int isfull(int end)
{
    if (end == MAX)
        return 1;
    return 0;
}

int push(char data, char *stack, int *end)
{
    if (isfull(*end))
        return -1;
    stack[(*end)++] = data;
    return 0;
}

int pop(char *stack, int *end)
{
    if (isempty(*end))
        return -1;
    return stack[--(*end)];
}

int juge(char *buf)
{
    char stack[MAX];
    int end = 0, i = 0, j = 0, flag = 0;  
    int lef[MAX];
    char ch;

    while(buf[i++] != '\0')
    {
        if (buf[i] == '(' || buf[i] == '[' || buf[i] == '{')
        {
            lef[j++] = i;
            flag = 1;
        }
        else if (buf[i] == ')' || buf[i] == ']' || buf[i] == '}')
            {
                if (flag == 0)
                    return -1;
                else 
                    puch(i, stack, &end);
            }
    }
    
    while(!isempty(end))
    {
        ch =  pop(stack, &end)
        i = 0;
        if(ch != buf[i++])
            return -1;
    }

    return 0;

}

int main(void)
{
    char buf[1024];
    int len;

    printf("please input char:");
    fgets(buf, sizeof(buf), stdin);
    len = strlen(buf);
    if (buf[len - 1] == '\n')
        buf[len - 1] = '\0';

    if(!juge(buf))
        printf("括号匹配!\n");
    else
        printf("括号不匹配!\n");
    
    return 0;
}
