#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int sd, ret;
    struct sockaddr_in srv;
    socklen_t len = sizeof(srv);
    
    sd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sd == -1)
        perror("socket");

    srv.sin_family = AF_INET;
    srv.sin_port = htons(9923);
    srv.sin_addr.s_addr = inet_addr("2.2.2.9");

    ret = sendto(sd, argv[1], strlen(argv[1]), 0, (struct sockaddr *)&srv, len);
    if (ret == -1)
        perror("sendto");

    close(sd);

    return 0;
}
