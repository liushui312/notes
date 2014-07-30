#include <chess.h>

#define GETS(string, ret, cmd, ...)      do             \
            {                                           \
                int r;                                  \
                printf(string);                         \
                r = scanf(cmd, __VA_ARGS__);            \
                while (getchar() != '\n')               \
                    continue;                           \
                if (r == 0)                             \
                    ret;                                \
            } while (0)

int srv_recv(void *data)
{
    struct chess_t *chess = (struct chess_t *)data;
    struct chess_data_t *chess_data = (struct chess_data_t *)(chess->packet->data);
    struct sockaddr_in cli;
    socklen_t len;
    int ret, packet_len, i;

    while (1)
    {
        packet_len = sizeof(chess->packet->type);

        ret = recvfrom(chess->fd, chess->packet, PACKET_MAX, 0, (struct sockaddr *)&cli, &len); 
        if (ret <= 0)
            break;
        DEBUG("srv recv ret = %d, type = %d, %s\n", ret, chess->packet->type, inet_ntoa(cli.sin_addr));
        switch (chess->packet->type)
        {
            case FIND:              //接收查询服务器包
                chess->packet->type = FIND_ACK;
                memcpy(chess->packet->data, &chess->msg, sizeof(struct srv_msg_t));
                packet_len += sizeof(struct srv_msg_t);
                ret = sendto(chess->fd, chess->packet, packet_len, 0, (struct sockaddr *)&cli, sizeof(struct sockaddr_in));
                DEBUG("sendto find ack ret = %d\n", ret);
                break;
            case CONN:              //接收用户连接包
                if (chess->msg.look_num != chess->msg.look_max)
                {
                    chess->packet->type = CONN_ACK;
                    chess->cli_list[chess->msg.look_num].sin_family = AF_INET;
                    chess->cli_list[chess->msg.look_num++] = cli;
                    if (chess->msg.status)
                    {
                        memcpy(chess->packet->data, &chess->cli, sizeof(chess->cli));
                        packet_len += sizeof(chess->cli);
                    }
                }
                else
                    chess->packet->type = CONN_ERR;

                ret = sendto(chess->fd, chess->packet, packet_len, 0, (struct sockaddr *)&cli, sizeof(struct sockaddr_in));
                DEBUG("sendto conn ack ret = %d\n", ret);
                break;
            case START:             //接收用户开始游戏包
                if (chess->msg.status)
                    chess->packet->type = START_ERR;
                else
                {
                    for (i = 0; i < chess->msg.look_num; i++)
                    {
                        if (chess->cli_list[i].sin_addr.s_addr == cli.sin_addr.s_addr) 
                        {
                            chess->cli = cli.sin_addr.s_addr;
                            break;
                        }
                    }
                    if (!chess->cli)
                        chess->packet->type = START_ERR;
                    else
                    {
                        chess->msg.status = 1;  
                        chess->packet->type = START_ACK;
                        memcpy(chess->packet->data, &chess->flag, sizeof(chess->flag));
                        packet_len += sizeof(chess->flag);
                        if (chess->flag)
                        {
                            SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_ENABLE); 
                            SDL_EventState(SDL_KEYDOWN, SDL_ENABLE);
                        }
                    }
                }
                ret = sendto(chess->fd, chess->packet, packet_len, 0, (struct sockaddr *)&cli, sizeof(struct sockaddr_in));
                DEBUG("sendto start ack ret = %d\n", ret);
                break;
            case DATA:              //接收用户下棋操作包
                if (chess->cli != cli.sin_addr.s_addr) 
                    break;
                memcpy(chess, chess_data, sizeof(struct chess_data_t)); 
                chess->p_flag = 1;
                chess_handle(chess);
                chess->p_flag = 0;
                SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_ENABLE); 
                SDL_EventState(SDL_KEYDOWN, SDL_ENABLE);
                break;
            default:
                break;
        }

    }

    return 0;
}


int get_local_int_ip(int fd, int eth_id)
{
    struct ifreq i;
    struct sockaddr_in *s = NULL;
    int ret;

    snprintf(i.ifr_ifrn.ifrn_name, sizeof(i.ifr_ifrn.ifrn_name), "eth%d", eth_id);
    s = (struct sockaddr_in *)&(i.ifr_ifru.ifru_addr);

    ret = ioctl(fd, SIOCGIFADDR, &i);
    if (ret == -1)
    {
        perror("ioctl");
        return -1;
    }
    return s->sin_addr.s_addr;
}

int get_local_ip(int fd, int eth_id, char *ip)
{
    struct in_addr addr;
    int t = get_local_int_ip(fd, eth_id);
    if (t == -1)
        return -1;

    addr.s_addr = t;

    strcpy(ip, inet_ntoa(addr));

    return 0;
}

int create_chess(struct chess_t *chess)
{
    int ret;
    struct sockaddr_in srv;
    
    chess->fd = socket(AF_INET, SOCK_DGRAM, 0);
    ERRP(chess->fd == -1, goto ERR1, 2, "socket");

    if(get_local_ip(chess->fd, chess->eth_id, chess->msg.ip))
    {
        printf("get_local_ip!\n");
        goto ERR2;
    }

    srv.sin_family = AF_INET;
    srv.sin_port = htons(PORT);
    /*srv.sin_addr.s_addr = inet_addr(chess->msg.ip); */
    srv.sin_addr.s_addr = INADDR_ANY;

    ret = bind(chess->fd, (struct sockaddr *)&srv, sizeof(struct sockaddr_in));
    ERRP(ret == -1, goto ERR2, 2, "bind");

    chess->packet = (struct packet_t *)malloc(PACKET_MAX);
    ERRP(chess->packet == NULL, goto ERR2, 2, "malloc packet");
    chess->net_flag = 1;
    chess->who = 1;

MAX:
    GETS("请输入服务噐连接最大人数：", goto MAX, "%d", &chess->msg.look_max);
    if (!(chess->msg.look_max > 0 && chess->msg.look_max <= 20))
        goto MAX;

    chess->cli_list = (struct sockaddr_in *)malloc(chess->msg.look_max * sizeof(struct sockaddr_in));
    ERRP(NULL == chess->cli_list, goto ERR3, 2, "malloc cli_list");

COLOR:
    printf("0. 白色!\n");
    printf("1. 黑色!\n");
    GETS("请选择颜色：", goto COLOR, "%d", &chess->flag);
    if (!(chess->flag >= 0 && chess->flag <= 1))
        goto COLOR;

    SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_DISABLE); 
    SDL_EventState(SDL_KEYDOWN, SDL_DISABLE);

    chess->conn = SDL_CreateThread(srv_recv, chess);
    ERRP(NULL == chess->conn, goto ERR4, 2, "SDL_CreateThread");

    return 0;
ERR4:
    free(chess->cli_list);
ERR3:
    free(chess->packet);
ERR2:
    close(chess->fd);
ERR1:
    return EXIT;
}

int add_node(struct srv_msg_t *msg, struct node_t *head)
{
    struct node_t *new = NULL;

    new = (struct node_t *)malloc(sizeof(struct node_t));
    ERRP(NULL == new, goto ERR1, 0);

    memcpy(&new->msg, msg, sizeof(struct srv_msg_t));
    
    new->next = head;
    new->prev = head->prev;
    head->prev->next =new;
    head->prev = new;

    return 0;
ERR1:
    return -1;
}

void destroy_list(struct node_t *head)
{
    struct node_t *tail = head->next, *save = NULL;

    while (tail != head)
    {
        save = tail->next; 
        free(tail);
        tail = save;
    }
}

int find_srv_list(struct chess_t *chess)
{
    struct sockaddr_in srv, cli;
    struct srv_msg_t *msg = (struct srv_msg_t *)(chess->packet->data);
    int ret;
    socklen_t len = sizeof(srv);
    fd_set rds;
    struct timeval t = {3, 0};    //设置等待服务器列表时间


    srv.sin_family = AF_INET;
    srv.sin_port = htons(PORT);
    srv.sin_addr.s_addr = INADDR_BROADCAST;
    /*srv.sin_addr.s_addr = inet_addr("2.2.2.255");*/

    chess->packet->type = FIND;
    ret = sendto(chess->fd, chess->packet, sizeof(struct packet_t), 0, (struct sockaddr *)&srv, sizeof(srv));
    DEBUG("send broadcast ret = %d\n", ret);
    /*perror("sendto");*/

    chess->srv_list.next = &chess->srv_list;
    chess->srv_list.prev = &chess->srv_list;

    while (1)
    {
        FD_ZERO(&rds);
        FD_SET(chess->fd, &rds);

        ret = select(chess->fd + 1, &rds, NULL, NULL, &t);
        if (ret > 0)
        {
            ret = recvfrom(chess->fd, chess->packet, PACKET_MAX, 0, (struct sockaddr *)&cli, &len);
            DEBUG("recv srv list ret = %d, type = %d, %s\n", ret, chess->packet->type, inet_ntoa(cli.sin_addr));
            if (ret <= 0)
                return -1;

            if (chess->packet->type == FIND_ACK && add_node(msg, &chess->srv_list))
                continue;
        }
        else if (ret == 0)   //超时退出
            break;
        else
            perror("select");
    }

    return 0;
}

struct srv_msg_t *select_srv(struct node_t *head)
{
    struct node_t *tail = NULL;
    int i, n;

SRV_LIST:
    printf("服务噐列表：\n");
    for (tail = head->next, i = 1; tail != head; tail = tail->next)
    {
        printf("\t %d. %s %d/%d ", i++, tail->msg.ip, tail->msg.look_num, tail->msg.look_max); 
        if (tail->msg.status)
            printf("在战\n");
        else
            printf("待战\n");
    }
    printf("\t %d. 返回!\n", i);
    GETS("\t请输入 : ", goto SRV_LIST, "%d", &n);
    if (n < 0 || n > i)
        goto SRV_LIST;
    else if (n == i)
        return NULL;

    for (tail = head->next; --n && tail != head; tail = tail->next)
        ;

    return &tail->msg;
}

int join_srv(struct chess_t *chess)
{
    int ret;
    struct sockaddr_in srv;
    socklen_t len = sizeof(srv);
    fd_set rds;
    struct timeval t = {5, 0};

    chess->packet->type = CONN;
    srv.sin_family = AF_INET;
    srv.sin_port = htons(PORT);
    srv.sin_addr.s_addr = inet_addr(chess->msg.ip);

    ret = sendto(chess->fd, chess->packet, sizeof(chess->packet->type), 0, (struct sockaddr *)&srv, len);
    DEBUG("sendto conn join srv ret = %d\n", ret);

    FD_ZERO(&rds);
    FD_SET(chess->fd, &rds);

    ret = select(chess->fd + 1, &rds, NULL, NULL, &t);
    if (ret > 0)
    {
        ret = recvfrom(chess->fd, chess->packet, PACKET_MAX, 0, (struct sockaddr *)&srv, &len);
        printf("ret = %d %s\n", ret, inet_ntoa(srv.sin_addr));
        if (ret <= 0)
            return -1;
        if (chess->packet->type == CONN_ACK)
        {
            printf("chess->packet->data = %d\n", *(int *)(chess->packet->data));
            if (chess->msg.status)
                memcpy(&chess->cli, chess->packet->data, sizeof(chess->cli));
            return 0;
        }
    }

    return -1;
}

int start_game(struct chess_t *chess)
{
    int ret;
    struct sockaddr_in srv;
    socklen_t len = sizeof(srv);
    fd_set rds;
    char ch;
    struct timeval t = {5, 0};

SEL:
    GETS("是否开始游戏(Y/N) : ", goto SEL, "%c", &ch);
    if (!(ch == 'Y' || ch == 'y'))      //如果选择不开始游戏， 作返回主菜单
        return -1;


    chess->packet->type = START;
    srv.sin_family = AF_INET;
    srv.sin_port = htons(PORT);
    srv.sin_addr.s_addr = inet_addr(chess->msg.ip);

    ret = sendto(chess->fd, chess->packet, sizeof(chess->packet->type), 0, (struct sockaddr *)&srv, len);
    DEBUG("sendto start game srv ret = %d\n", ret);

    FD_ZERO(&rds);
    FD_SET(chess->fd, &rds);

    ret = select(chess->fd + 1, &rds, NULL, NULL, &t);
    if (ret > 0)
    {
        ret = recvfrom(chess->fd, chess->packet, PACKET_MAX, 0, (struct sockaddr *)&srv, &len);
        if (ret <= 0)
            return -1;
        if (chess->packet->type == START_ACK)
        {
            memcpy(&chess->flag, chess->packet->data, sizeof(chess->flag)); 
            if (!chess->flag)
            {
                SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_ENABLE);
                SDL_EventState(SDL_KEYDOWN, SDL_ENABLE);
            }
            return 0;
        }
    }

    return -1;
}

int cli_recv(void *data)
{
    struct chess_t *chess = (struct chess_t *)data;
    struct chess_data_t *chess_data = (struct chess_data_t *)(chess->packet->data);
    struct sockaddr_in cli;
    socklen_t len;
    int ret, packet_len;

    while (1)
    {
        packet_len = sizeof(chess->packet->type);

        ret = recvfrom(chess->fd, chess->packet, PACKET_MAX, 0, (struct sockaddr *)&cli, &len); 
        if (ret <= 0)
            break;
        DEBUG("cli recv ret = %d, type = %d, %s\n", ret, chess->packet->type, inet_ntoa(cli.sin_addr));
        if (chess->packet->type == DATA)
        {
            memcpy(chess, chess_data, sizeof(struct chess_data_t)); 
            chess->p_flag = 1;
            printf("chess_handle DATA recv x = %d, y = %d, flag = %d\n", chess->x, chess->y, chess->flag);
            print_board(chess->board);
            chess_handle(chess);
            chess->p_flag = 0;
            if (chess->who)
            {
                SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_ENABLE); 
                SDL_EventState(SDL_KEYDOWN, SDL_ENABLE);
            }
        }
    }

    return 0;
}

int join_chess(struct chess_t *chess)
{
    int ret, val = 1, flag = 0;
    struct srv_msg_t *msg = NULL;

    chess->fd = socket(AF_INET, SOCK_DGRAM, 0);
    ERRP(chess->fd == -1, goto ERR1, 2, "socket");

    ret = setsockopt(chess->fd, SOL_SOCKET, SO_BROADCAST, &val, sizeof(val));
    ERRP(ret == -1, goto ERR2, 2, "setsockopt");

    chess->packet = (struct packet_t *)malloc(PACKET_MAX);
    ERRP(chess->packet == NULL, goto ERR2, 2, "malloc join packet");

    ret = find_srv_list(chess);
    ERRP(ret == -1, goto ERR3, 0);

    while (1)
    {
        msg = select_srv(&chess->srv_list);
        if (msg == NULL)
        {
            flag = 1;  //选择了返回主菜单
            goto ERR4;
        }

        memcpy(&chess->msg, msg, sizeof(struct srv_msg_t));

        ERRP(join_srv(chess), continue, 0);      //加入选中服务器

        break;
    }

    SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_DISABLE); 
    SDL_EventState(SDL_KEYDOWN, SDL_DISABLE);

    if (!chess->msg.status)
    {
        ERRP(start_game(chess), flag = 1; goto ERR4 , 0);
        chess->cli = get_local_int_ip(chess->fd, chess->eth_id);
        chess->who = 2;
    }
    else
        flag = 2;


    chess->conn = SDL_CreateThread(cli_recv, chess);
    ERRP(NULL == chess->conn, flag = 1; goto ERR4, 2, "SDL_CreateThread");

    printf("2 = flag = %d\n", flag);

ERR4:
    destroy_list(&chess->srv_list);
    if (flag == 0)
    {
        chess->net_flag = 1;
        if (chess->flag)
            printf("本机为白方!\n");
        else
            printf("本机为黑方!\n");
        return 0;
    }
    else if (flag == 2)
    {
        chess->net_flag = 1;
        printf("本机为观战!\n");
        return 0;
    }
ERR3:
    free(chess->packet);
    chess->packet = NULL;
ERR2:
    close(chess->fd);
ERR1:
    if (flag == 1)
        return RET;
    return EXIT;
}

int chess_menu(struct chess_t *chess)
{
    int n, ret;

    while (1)
    {
NET_MENU:
        printf("======= 1. 创建游戏 =======!\n");
        printf("======= 2. 加入游戏 =======!\n");
        printf("======= 3. 单机游戏 =======!\n");
        printf("======= 4. 退出游戏 =======!\n");
        GETS("请输入 [ 1 - 4 ] : ", goto NET_MENU, "%d", &n);

        switch (n)
        {
            case 1:
                return create_chess(chess);
            case 2:
                ret = join_chess(chess);
                if (ret == RET)
                    break;
                else
                    return ret;
            case 3:
                return SINGLE;
            case 4:
                return EXIT;
            default:
                break;
        }
    }
}
