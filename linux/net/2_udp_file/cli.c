#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int sd, ret, fd;
    char buf[1024];
    struct sockaddr_in srv;
    socklen_t len = sizeof(srv);
    
    sd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sd == -1)
        perror("socket");

    srv.sin_family = AF_INET;
    srv.sin_port = htons(9999);
    srv.sin_addr.s_addr = inet_addr("2.2.2.9");

    ret = sendto(sd, argv[1], strlen(argv[1]), 0, (struct sockaddr *)&srv, len);
    if (ret == -1)
        perror("sendto");

    recvfrom(sd, &ret, 1, 0, (struct sockaddr *)&srv, &len);
    if (!ret)
    {
        printf("%s is not!\n", argv[1]);
        close(sd);
        return 0;
    }

    fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
    if (fd == -1)
        perror("open");

    while (1)
    {
        len = sizeof(struct sockaddr_in);
        ret = recvfrom(sd, buf, sizeof(buf), 0, (struct sockaddr *)&srv, &len);
        if (ret <= 0)
            break;
        printf("ret = %d\n", ret);
        write(fd, buf, ret);
        if (ret < 1024)
            break;
    }

    printf("main end!\n");

    close(fd);

    close(sd);

    return 0;
}
