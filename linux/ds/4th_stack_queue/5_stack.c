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
    char buf[1024], ch;
    int i, len;

    handle = stack_create(sizeof(char), 1024);

    GETLINES("please input string : ", buf);

    len = strlen(buf);

    for (i = 0; i < len; i++)
    {
        switch (buf[i])
        {
            case '#':
                stack_pop(handle);
                break;
            case '@':
                ch = '\n';
                stack_push(&ch, handle);
                break;
            default:
                stack_push(&buf[i], handle);
                break;
        }
    }

    stack_travel(ls, handle);
    printf("\n");

    stack_destroy(&handle);

    return 0;
}
