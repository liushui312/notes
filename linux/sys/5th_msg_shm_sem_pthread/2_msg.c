#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msgbuf {
    long type;
    char data[1024];
};

int main(int argc, char *argv[])
{
    key_t key;
    int mid;
    struct msgbuf buf;

    key = ftok("/kyo", 1);

    mid = msgget(key, IPC_CREAT); // | IPC_EXCL);
    if (mid == -1)
        perror("msgget");

    if (!strcmp(argv[1], "d"))
    {
        msgctl(mid, IPC_RMID, NULL);
        return 0;
    }
    if (argc > 2)
    {
        buf.type = atoi(argv[1]);
        strcpy(buf.data, argv[2]);

        msgsnd(mid, &buf, strlen(buf.data) + sizeof(buf.type) + 1, 0);
    }
    else
    {
        msgrcv(mid, &buf, sizeof(buf), atoi(argv[1]), 0);
        printf("buf = %s\n", buf.data);
    }

    return 0;
}
