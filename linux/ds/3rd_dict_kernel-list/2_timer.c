#include <stdio.h>
#include <sys/time.h>
#include <signal.h>

int i = 0;

void action(int sig)
{
    printf("heloo %d\n", i++);
}

int main(void)
{
    struct itimerval it;
    char ch;

    it.it_interval.tv_sec = 1;
    it.it_interval.tv_usec = 0;

    it.it_value.tv_sec = 0;
    it.it_value.tv_usec = 100;

    signal(SIGALRM, action);
    setitimer(ITIMER_REAL, &it, NULL);

    system("stty -icanon -echo");

    while (1)
    {
        ch = getchar();
        if (ch == 27)
            break;
    }

    system("stty icanon echo");

    return 0;
}
