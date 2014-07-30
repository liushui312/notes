#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

int test(int fd, int key, int type)
{
    struct input_event event;

    if (type == 3)
    {
        test(fd, key, 1); 
        type = 0;
    }

    event.type = EV_KEY;
    event.code = key;
    event.value = type;

    write(fd, &event, sizeof(event));

    memset(&event, 0, sizeof(event));
    write(fd, &event, sizeof(event));
}

int main(void)
{
    int fd;
    struct input_event event;

    fd = open("/dev/input/event4", O_RDWR);
    if (fd == -1)
        perror("open");

    test(fd, 56, 1);
    test(fd, 60, 1);
    test(fd, 56, 0);
    test(fd, 60, 0);

    exit(0);

    while (1)
    {
        read(fd, &event, sizeof(struct input_event));
        if (event.type == EV_KEY)
            printf("type = %d, code = %d, value = %d\n", event.type, event.code, event.value);
    }

    close(fd);


    return 0;
}
