#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/udp.h>
#include <netinet/ip.h>

int main(void)
{
    int sd, ret;
    struct sockaddr_in srv;
    char buf[1024];
    socklen_t len = sizeof(srv);
    struct iphdr *ip = (struct iphdr *)buf;
    struct udphdr *udp = (struct udphdr *)(buf + 20);
    
    sd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
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
        printf("========  udp ==========\n");
        printf("source = %d, dest = %d, len = %d, check = %d\n", 
                ntohs(udp->source), ntohs(udp->dest), 
                ntohs(udp->len), ntohs(udp->check));
        printf("ret = %d, %s : %s\n", ret, inet_ntoa(srv.sin_addr), buf + 28);
    }

    close(sd);

    return 0;
}
