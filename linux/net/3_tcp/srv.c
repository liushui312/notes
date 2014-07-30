#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

void cli_handler(int cli_fd, struct sockaddr_in cli)
{
    char buf[1024];
    int ret;

    ret = read(cli_fd, buf, sizeof(buf));
    buf[ret] = '\0';
    printf("%s : %s\n", inet_ntoa(cli.sin_addr), buf);
    sleep(2);

    ret = write(cli_fd, buf, sizeof(buf));
    printf("write end ret = %d!\n", ret);

    ret = write(cli_fd, buf, sizeof(buf));
    printf("write end ret = %d!\n", ret);

    ret = write(cli_fd, buf, sizeof(buf));
    printf("write end ret = %d!\n", ret);
}

void cli_connect(int sd)
{
    int cli_fd;
    struct sockaddr_in srv;
    socklen_t len = sizeof(srv);

    while (1)
    {
        cli_fd = accept(sd, (struct sockaddr *)&srv, &len);

        cli_handler(cli_fd, srv);

        close(cli_fd);
    }
}

void sig_handler(int sig)
{
    printf("sig_handler!\n");
}

int main(void)
{
    int sd, ret, i;
    struct sockaddr_in srv;
    socklen_t len = sizeof(srv);

    signal(SIGPIPE, sig_handler);
    
    /*sd = socket(AF_INET, SOCK_STREAM, 0);*/
    sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sd == -1)
        perror("socket");

    srv.sin_family = AF_INET;
    srv.sin_port = htons(9999);
    /*srv.sin_addr.s_addr = inet_addr("2.2.2.9");*/
    srv.sin_addr.s_addr = 0;

    ret = bind(sd, (struct sockaddr *)&srv, len);
    if (ret == -1)
        perror("bind");

    ret = listen(sd, 20);
    if (ret == -1)
        perror("listen");

    for (i = 0; i < 5; i++)
    {
        if (fork() == 0)
        {
            cli_connect(sd);
            exit(0); 
        }
    }

    getchar();

    kill(0, SIGKILL);

    close(sd);

    return 0;
}
