#include <stack.h>

#define GETLINES(string, buf)   do                  \
            {                                       \
                printf(string);                     \
                fgets(buf, sizeof(buf), stdin);     \
                if (buf[strlen(buf) - 1] == '\n')   \
                    buf[strlen(buf) - 1] = '\0';    \
                else                                \
                    while (getchar() != '\n')       \
                        continue;                   \
            } while (0)

void ls(const void *data)
{
    printf("%c", *(char *)data);
}

int main(void)
{
    STACK *handle = NULL;
    char buf[1024] = "3521+-*67/+";
    char *ch = NULL, v1, v2, val;
    int i, len;

    handle = stack_create(sizeof(char), 1024);

    /*GETLINES("please input string : ", buf);*/

    len = strlen(buf);

    for (i = len - 1; i >= 0; i--)
    {
        if (buf[i] >= '0' && buf[i] <= '9')
            stack_push(&buf[i], handle);
    }

    for (i = 0; i < len; i++)
    {
        if (buf[i] == '+' || buf[i] == '-' || buf[i] == '*' || buf[i] == '/' || buf[i] == '%')    
        {
            ch = stack_pop(handle);
            if (ch == NULL)
                goto EXIT;
            v1 = *ch - '0';
            ch = stack_pop(handle);
            if (ch == NULL)
                goto EXIT;
            v2 = *ch - '0';

            switch (buf[i])
            {
                case '+':
                    val = v1 + v2;
                    break;
                case '-':

                    val = v1 - v2;
                    break;
                case '*':
                    val = v1 * v2;
                    break;
                case '/':
                    val = v1 / v2;
                    break;
                case '%':
                    val = v1 % v2;
                    break;
                default:
                    break;
            }
            val += '0';
            printf("v1 = %d, v2 = %d, val = %c\n", v1, v2, val);
            stack_push(&val, handle);
        }
    }
    ch = stack_pop(handle);
    if (ch == NULL || !stack_isempty(handle))
        goto EXIT;

    printf("%s = %c\n", buf, *ch);

EXIT:
    stack_destroy(&handle);

    return 0;
}
