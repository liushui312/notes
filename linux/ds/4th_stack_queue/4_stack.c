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

int main(void)
{
    STACK *handle = NULL;
    char buf[1024], *ch = NULL;
    int i, len;

    handle = stack_create(sizeof(char), 1024);

    GETLINES("please input string : ", buf);

    len = strlen(buf);

    for (i = 0; i < len; i++)
    {
        if (buf[i] == '(' || buf[i] == '[' || buf[i] == '{') 
            stack_push(&buf[i], handle);
        if (buf[i] == ')' || buf[i] == ']' || buf[i] == '}')
        {
            ch = stack_pop(handle);
            if (ch == NULL)
                goto ERR1;
            if ((buf[i] == ')' && *ch != '(') ||
                (buf[i] == ']' && *ch != '[') ||
                (buf[i] == '}' && *ch != '{'))
                goto ERR2;
        }
    }

    if (!stack_isempty(handle))
        goto ERR3;

EXIT:
    stack_destroy(&handle);
    return 0;
ERR1:
    fprintf(stderr, "({[ is not!\n");
    goto EXIT;
ERR2:
    fprintf(stderr, "({[ )}]!\n");
    goto EXIT;
ERR3:
    fprintf(stderr, ")}] is not!\n");
    goto EXIT;
}
