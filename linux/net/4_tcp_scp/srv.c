#include <net.h>
#include <signal.h>
#include <shadow.h>

extern char *crypt(const char *, const char *);

void cli_handler(int cli_fd, struct sockaddr_in cli)
{
    int ret, fd;
    struct packet_t *packet = NULL;
    struct login_t *login = NULL;
    struct spwd *sp = NULL;
    char *user = NULL, *passwd = NULL, *path = NULL;
    struct stat s;
    int packet_len = sizeof(struct packet_t);

    printf("%s connenct!\n", inet_ntoa(cli.sin_addr));

    packet = (struct packet_t *)malloc(PACKET_MAX);
    ERRP(packet == NULL, goto ERR1, 2, "malloc packet");

    ret = read(cli_fd, packet, PACKET_MAX);
    ERRP(ret <= 0 || packet->type != LOGIN, goto ERR2, 1, "recv login");

    login = (struct login_t *)packet->data;
    user = packet->data + login->usr;
    passwd = packet->data + login->passwd;
    path = packet->data + login->path;

    sp = getspnam(user);
    ERRP(NULL == sp, packet->type = ERR_USR; goto ERR3, 0);

    ERRP(strcmp(crypt(passwd, sp->sp_pwdp), sp->sp_pwdp), packet->type = ERR_PWD; goto ERR3, 0);

    ret = stat(path, &s);
    ERRP(ret == -1, packet->type = ERR_PATH; goto ERR3, 0);

    fd = open(path, O_RDONLY);
    ERRP(fd == -1, packet->type = ERR_PATH; goto ERR3, 0);

    packet->type = SUCCESS;
    memcpy(packet->data, &s.st_size, sizeof(s.st_size));
    write(cli_fd, packet, packet_len + sizeof(s.st_size));

    ret = read(cli_fd, packet, PACKET_MAX);
    ERRP(ret <= 0 || packet->type != DATA_START, goto ERR2, 0);

    while ((ret = read(fd, packet->data, PACKET_MAX - packet_len)) > 0)
    {
        packet->type = DATA;
        ret = write(cli_fd, packet, ret + packet_len);
        printf("write end %d\n", ret);
        while (1)
        {
            ret = read(cli_fd, packet, PACKET_MAX);
            if (ret <= 0 || packet->type == DATA_ACK)
                break;
        }
    }
    close(fd);
    packet->type = DATA_END;

ERR3:
    write(cli_fd, packet, packet_len);
ERR2:
    free(packet);
ERR1:
    printf("%s disconnect!\n", inet_ntoa(cli.sin_addr));
    return;
}

void cli_connect(int sd)
{
    int cli_fd;
    struct sockaddr_in srv;
    socklen_t len = sizeof(srv);

    while (1)
    {
        cli_fd = accept(sd, (struct sockaddr *)&srv, &len);
        if (cli_fd == -1)
            continue;

        cli_handler(cli_fd, srv);

        close(cli_fd);
    }
}

int main(void)
{
    int sd, ret, i;
    struct sockaddr_in srv;
    socklen_t len = sizeof(srv);
    
    sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    ERRP(sd == -1, goto ERR1, 2, "socket");

    srv.sin_family = AF_INET;
    srv.sin_port = htons(9999);
    srv.sin_addr.s_addr = 0;

    ret = bind(sd, (struct sockaddr *)&srv, len);
    ERRP(ret == -1, goto ERR2, 2, "bind");

    ret = listen(sd, 20);
    ERRP(ret == -1, goto ERR2, 2, "listen");

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
ERR2:
    close(sd);
ERR1:
    return 0;
}
