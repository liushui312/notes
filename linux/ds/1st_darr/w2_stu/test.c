#include <stdio.h>

#define S   "hello"

int main(void)
{
    char buf[1024];
    /*printf("helllosdjfk"S);*/
                /*"lasdjflkasdf\n");*/
    fgets(buf, 10, stdin);

    printf("buf = %s\n", buf);

    scanf("%s", buf);

    return 0;
}
