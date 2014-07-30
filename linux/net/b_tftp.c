#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

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
    strcpy(buf + 2 + l, "octet");

    ret = sendto(sd, buf, 11 + l, 0, (struct sockaddr *)&srv, len);
    if (ret == -1)
        perror("sendto");

    fd = open("./tftp_test", O_RDWR | O_CREAT | O_TRUNC, 0777);
    if (fd == -1)
        perror("open");

    while (1)
    {
        l = recvfrom(sd, buf, sizeof(buf), 0, (struct sockaddr *)&srv, &len);

        write(fd, buf + 4, l - 4);

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
