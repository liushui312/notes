#include <stdio.h>

struct packet_t {
    char type;
    char data[0];
};

int main(void)
{
    char a[1024];
    char data[0];
    char b[1024];
    struct packet_t *packet = (struct packet_t *)malloc(100);

    printf("sizeof(struct packet_t) = %d\n", sizeof(struct packet_t));
    strcpy(data, "hello world!");

    printf("data = %d\n", sizeof(data));
    printf("a = %p, b = %p, data = %p\n", a, b, data);
    printf("a = %s, %s\n", a, b);


    return 0;
}
