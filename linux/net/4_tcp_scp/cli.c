#include <net.h>
#include <sys/mman.h>

int main(int argc, char *argv[])
{
    int sd, ret, fd, ch;
    char ip[16], *p = NULL, *srcpath = NULL, *passwd = NULL;
    struct sockaddr_in srv;
    struct passwd *pw = NULL;
    struct login_t *login = NULL;
    struct packet_t *packet = NULL;
    int usr_len, pw_len, path_len, packet_len = sizeof(struct packet_t), login_len = sizeof(struct login_t);
    int file_len;
    char *m = NULL;
    socklen_t len = sizeof(srv);

    ERRP(argc < 3, goto ERR1, 1, "./a.out ip:srcpath dstpath!\n");

    p = strchr(argv[1], ':');
    ERRP(p == NULL, goto ERR1, 1, "./a.out ip:srcpath dstpath!\n");
    snprintf(ip, p - argv[1] + 1, "%s", argv[1]);
    srcpath = p + 1;

    pw = getpwuid(getuid());

    passwd = getpass("Password: ");

    usr_len = strlen(pw->pw_name) + 1;
    pw_len = strlen(passwd) + 1;
    path_len = strlen(srcpath) + 1;

    packet = (struct packet_t *)malloc(PACKET_MAX);
    ERRP(packet == NULL, goto ERR1, 2, "malloc packet");

    packet->type = LOGIN;
    login = (struct login_t *)packet->data;

    login->usr = login_len;
    memcpy(packet->data + login->usr, pw->pw_name, usr_len);
    login->passwd = login->usr + usr_len;
    memcpy(packet->data + login->passwd, passwd, pw_len);
    login->path = login->passwd + pw_len;
    memcpy(packet->data + login->path, srcpath, path_len);
    
    sd = socket(AF_INET, SOCK_STREAM, 0);
    ERRP(sd == -1, goto ERR2, 2, "socket");

    srv.sin_family = AF_INET;
    srv.sin_port = htons(9999);
    srv.sin_addr.s_addr = inet_addr(ip);

    ret = connect(sd, (struct sockaddr *)&srv, len);
    ERRP(ret == -1, goto ERR3, 2, "connect");

    ret = write(sd, packet, packet_len + login_len + usr_len + pw_len + path_len);
    ERRP(ret == -1, goto ERR3, 2, "send login");

    ret = read(sd, packet, PACKET_MAX);
    ERRP(ret <= 0, goto ERR3, 2, "recv login ack");

    switch (packet->type)
    {
        case ERR_USR:
            fprintf(stderr, "用户名不存在!\n");
            goto ERR3;
        case ERR_PWD:
            fprintf(stderr, "密码验证失败!\n");
            goto ERR3;
        case ERR_PATH:
            fprintf(stderr, "%s 在服务器不存在!\n", srcpath);
            goto ERR3;
        case SUCCESS:
            break;
        default:
            fprintf(stderr, "非法操作!\n");
            goto ERR3;
    }

    if (!access(argv[2], F_OK))
    {
        printf("是否覆盖(Y/N):");
        ch = getchar();
        if (!(ch == 'Y' || ch == 'y'))
            goto ERR3;
        unlink(argv[2]);
    }

    memcpy(&file_len, packet->data, sizeof(file_len));

    fd = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0777);
    ERRP(fd == -1, goto ERR3, 2, "open dstpath");
    ftruncate(fd, file_len);

    m = mmap(NULL, file_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    ERRP(m == NULL, goto ERR4, 2, "mmap");

    packet->type = DATA_START;
    write(sd, packet, packet_len);

    len = 0;
    while (1)
    {
        ret = read(sd, packet, PACKET_MAX);
        ERRP(ret <= 0, goto ERR4, 1, "数据传输失败!\n");
        if (packet->type == DATA)
        {
            memcpy(m + len, packet->data, ret - packet_len);
            len += ret - packet_len;
            /*write(fd, packet->data, ret - packet_len); */
            packet->type = DATA_ACK;
            write(sd, packet, packet_len);
        }
        if (packet->type == DATA_END || ret < PACKET_MAX)
            break;
    }

    munmap(m, file_len);
ERR4:
    close(fd);
ERR3:
    close(sd);
ERR2:
    free(packet);
ERR1:
    return 0;
}
