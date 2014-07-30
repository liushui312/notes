#include <stdio.h>
#include <unistd.h>

int main(void)
{
    char ch[8];
    int ret;

    system("stty -echo -icanon");

    while (1)
    {
        ret = read(STDIN_FILENO, ch, 8);
        if (ret == 1)
        {
            if (ch[0] == 27)
                break;
            fprintf(stderr, "%d|", ch[0]); 
        }
        else if (ret == 3)
            fprintf(stderr, "%d %d %d|", ch[0], ch[1], ch[2]); 
        else if (ret == 4)
            fprintf(stderr, "%d %d %d %d|", ch[0], ch[1], ch[2], ch[3]); 
    }

    system("stty echo icanon");

    return 0;
}
