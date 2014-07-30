#include <stdio.h>

int get_input(void)
{
    int key, ret;
    char ch[8];

    ret = read(0, ch, 8);
    if (ret == 1)
        key = ch[0];
    else if (ret == 3 && ch[0] == 27 && ch[1] == 91)
        key = ch[2];
    else
        key = -1;
    return key;
    
}

int main(void)
{
    int ch;

    system("stty -icanon -echo");

    while(1)
    {
        ch = get_input();
        /*printf("%d\n", ch);*/
        if (ch == 27)
            break;
        else if (ch == 68)
            printf("\033[D");
        else if (ch == 66)
            printf("\033[B");
        else if (ch == 67)
            printf("\033[C");
        else if (ch == 65)
            printf("\033[A");
        else 
            putchar(ch);
        fflush(stdout);
    }

    system("stty icanon echo");

    return 0;
}
