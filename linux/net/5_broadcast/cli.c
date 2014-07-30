#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int srv_send(void)
{
    int sd, ret, val;
    struct sockaddr_in srv;
    socklen_t len = sizeof(srv);
    
    sd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sd == -1)
        perror("socket");

    val = 1;
    ret = setsockopt(sd, SOL_SOCKET, SO_BROADCAST, &val, sizeof(val));
    if (ret == -1)
        perror("setsockopt");

    srv.sin_family = AF_INET;
    srv.sin_port = htons(9999);
    srv.sin_addr.s_addr = inet_addr("2.2.2.255");

    ret = sendto(sd, argv[1], strlen(argv[1]), 0, (struct sockaddr *)&srv, len);
    if (ret == -1)
        perror("sendto");

    close(sd);

    return 0;
}
int cli_send(void)
{

	int sock_fd;
	struct sockaddr_in cli;
	char buf[1024];
	int ret, val = 1;

	sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock_fd == -1)
		perror("socket");

	ret = setsockopt(sock_fd, SOL_SOCKET, SO_BROADCAST, &val, sizeof(val));
	if (ret == -1)
		perror("setsockopt");

	cli.sin_family = AF_INET;
	cli.sin_port = htons(9999);
	cli.sin_addr.s_addr = inet_addr("2.2.2.255");

	ret = sendto(sock_fd, argv[1], strlen(argv[1]) + 1, 0, (struct sockaddr *)&cli, sizeof(cli));
	if (ret < 0)
		perror("sendto");

	close(sock_fd);

	return 0;
}

int main(void)
{
	printf("1为服务器，0为客户端：");
	return 0;
}
