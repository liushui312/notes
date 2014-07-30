#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

void add_buf(int fd, int ch, char *buf, int *end)
{
    int e = *end;

    if (e == 8192)
    {
        write(fd, buf, e);
        e = 0;
    }
    buf[e++] = ch;

    *end = e;
}

void show_txt(char *buf, int len)
{
    int i;
    char ch;

    for (i = 0; i < len; i++)
    {
        ch  = buf[i];
        printf("%d ", ch);
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    int sd, ret, l = 0, fd, i, j;
    struct sockaddr_in srv;
    socklen_t len = sizeof(srv);
    char buf[516];
    char filebuf[8192];
    int end = 0, flag = 0;
    
    sd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sd == -1)
        perror("socket");

    srv.sin_family = AF_INET;
    srv.sin_port = htons(69);
    srv.sin_addr.s_addr = inet_addr("2.2.2.9");

    ret = htons(1);
    memcpy(buf, &ret, 2);
    strcpy(buf + 2, argv[1]);
    l = strlen(argv[1]) + 1;
    strcpy(buf + 2 + l, "netascii");

    ret = sendto(sd, buf, 11 + l, 0, (struct sockaddr *)&srv, len);
    if (ret == -1)
        perror("sendto");

    fd = open("./tftp_test", O_RDWR | O_CREAT | O_TRUNC, 0777);
    if (fd == -1)
        perror("open");

    while (1)
    {
        l = recvfrom(sd, buf, sizeof(buf), 0, (struct sockaddr *)&srv, &len);

        if (flag)
        {
            if (buf[4] != '\n')
                add_buf(fd, '\r', filebuf, &end);
            flag = !flag;
        }

        for (i = 4; i < l - 1; i++)
        {
            if (buf[i] != '\r' || buf[i + 1] != '\n')
                add_buf(fd, buf[i], filebuf, &end);
        }
        if (buf[i] != '\r')
            add_buf(fd, buf[i], filebuf, &end);
        else
            flag = !flag;

        /*printf("%d %d [%d %d]\n", buf[ret - 2], buf[ret - 1], '\r', '\n');*/
        /*buf[ret] = '\0';*/
        /*printf("ret = %d, buf = %s\n", ret, buf + 4);*/

        ret = htons(4);
        memcpy(buf, &ret, 2);
        ret = sendto(sd, buf, 4, 0, (struct sockaddr *)&srv, len);
        if (ret == -1)
            perror("sendto ack");

        if (l < 516)
            break;
    }
    write(fd, filebuf, end);

    close(sd);

    return 0;
}
