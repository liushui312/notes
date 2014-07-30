#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(void)
{
    int sd, ret;
    struct sockaddr_in srv;
    char buf[1024];
    socklen_t len = sizeof(srv);
    
    /*socket(AF_INET, SOCK_DGRAM, 0);*/
    sd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sd == -1)
        perror("socket");

    srv.sin_family = AF_INET;
    srv.sin_port = htons(9999);
    /*srv.sin_addr.s_addr = inet_addr("2.2.2.9");*/
    srv.sin_addr.s_addr = 0;

    ret = bind(sd, (struct sockaddr *)&srv, len);
    if (ret == -1)
        perror("bind");

    while (1)
    {
        len = sizeof(struct sockaddr_in);
        ret = recvfrom(sd, buf, sizeof(buf), 0, (struct sockaddr *)&srv, &len);
        buf[ret] = '\0';
        printf("%s : %s\n", inet_ntoa(srv.sin_addr), buf);
    }

    close(sd);

    return 0;
}
