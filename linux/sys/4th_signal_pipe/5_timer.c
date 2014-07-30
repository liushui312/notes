#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>

void sig_handler(int s)
{
    printf("sig_handler!\n");
    /*alarm(1);*/
}
int my_sleep(int s)
{
    alarm(s);
    pause();
    return alarm(0);
}

int main(void)
{
    struct itimerval it;
    signal(SIGALRM, sig_handler);
    /*
     *printf("alarm = %d\n", alarm(5));
     *sleep(2);
     *printf("alarm = %d\n", alarm(1));
     */

    it.it_interval.tv_sec = 1;
    it.it_interval.tv_usec = 0;
    it.it_value.tv_sec = 0;
    it.it_value.tv_usec = 100;

    setitimer(ITIMER_REAL, &it, NULL);

    /*pause();*/
    getchar();

    return 0;
}
