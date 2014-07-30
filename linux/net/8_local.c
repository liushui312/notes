#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <linux/if.h>

int main(void)
{
    int sd;
    struct ifreq r;
    struct sockaddr_in *srv = (struct sockaddr_in *)&(r.ifr_ifru.ifru_addr);

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd == -1)
        perror("socket");

    strcpy(r.ifr_ifrn.ifrn_name, "eth1");

    printf("ioctl == %d\n", ioctl(sd, SIOCGIFADDR, &r));
    
    printf("ip = %s\n", inet_ntoa(srv->sin_addr));

    close(sd);

    return 0;
}
