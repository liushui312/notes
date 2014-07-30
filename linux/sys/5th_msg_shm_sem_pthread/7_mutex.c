#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX     100

int fd;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/*pthread_mutex_init();*/
/*pthread_mutex_destroy();*/

void *do_work(void *data)
{
    int count = 0, i;
    char buf[1024];
    char *s = (char *)data;

    while (count < MAX)
    {
        pthread_mutex_lock(&mutex);
        snprintf(buf, sizeof(buf), "%s pid = %d, count = %d\n", s, getpid(), count++);
        for (i = 0; i < strlen(buf); i++)
        {
            write(fd, &buf[i], 1);
            usleep(1);
        }
        pthread_mutex_unlock(&mutex);
    }
}

int main(int argc, char *argv[])
{
    char *m = NULL;
    pthread_t id;

    fd = open("test_open", O_RDWR | O_CREAT | O_TRUNC, 0777);
    if (fd == -1)
        perror("open");

    pthread_create(&id, NULL, do_work, "clild");

    do_work("parent");

    pthread_join(id, NULL);

    return 0;
}
