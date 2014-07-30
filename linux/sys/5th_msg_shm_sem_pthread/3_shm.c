#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    key_t key;
    int sid;
    char *m = NULL;

    key = ftok("/kyo", 1);

    sid = shmget(key, 1024, IPC_CREAT); // | IPC_EXCL);
    if (sid == -1)
        perror("shmget");

    if (argc > 1 && !strcmp(argv[1], "d"))
    {
        shmctl(sid, IPC_RMID, NULL);
        return 0;
    }

    m = shmat(sid, NULL, 0);
    if (m == NULL)
        perror("shmat");

    if (fork() == 0)
    {
        strcpy(m, "hello world!");
        exit(0); 
    }
    wait(NULL);

    printf("m = %s\n", m);



    shmdt(m);

    return 0;
}
