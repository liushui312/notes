#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main(int argc, char *argv[])
{
    key_t key;
    int mid;
    char buf[1024];

    key = ftok("/home", 1);

    mid = msgget(key, IPC_CREAT);
    if (mid == -1) {
        perror("msgget");
    }

    if (argc > 1) {
        if (!strcmp(argv[1], "d")) 
            msgctl(mid, IPC_RMID, NULL);
        msgsnd(mid, argv[1], strlen(argv[1]) + 1, 0);
    }

    return 0;
}
