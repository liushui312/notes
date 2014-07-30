#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX     100

int lock(int sid)
{
    struct sembuf op;

    op.sem_num = 0;
    op.sem_op = -1;
    op.sem_flg = 0;

    return semop(sid, &op, 1);
}

int unlock(int sid)
{
    struct sembuf op;

    op.sem_num = 0;
    op.sem_op = 1;
    op.sem_flg = 0;

    return semop(sid, &op, 1);

}

int fd;
int sid;

void *do_work(void *data)
{
    int count = 0, i;
    char buf[1024];
    char *s = (char *)data;

    while (count < MAX)
    {
        lock(sid);
        snprintf(buf, sizeof(buf), "%s pid = %d, count = %d\n", s, getpid(), count++);
        for (i = 0; i < strlen(buf); i++)
        {
            write(fd, &buf[i], 1);
            usleep(1);
        }
        unlock(sid);
    }
}

int main(int argc, char *argv[])
{
    key_t key;
    char *m = NULL;
    pthread_t id;

    key = ftok("/kyo", 1);

    sid = semget(key, 1, IPC_CREAT); // | IPC_EXCL);
    if (sid == -1)
        perror("semget");

    semctl(sid, 0, SETVAL, 1);
    printf("semctl = %d\n", semctl(sid, 0, GETVAL));

    if (argc > 1 && !strcmp(argv[1], "d"))
    {
        semctl(sid, 0, IPC_RMID);
        return 0;
    }

    fd = open("test_open", O_RDWR | O_CREAT | O_TRUNC, 0777);
    if (fd == -1)
        perror("open");

    pthread_create(&id, NULL, do_work, "clild");

    do_work("parent");

    pthread_join(id, NULL);

    return 0;
}
