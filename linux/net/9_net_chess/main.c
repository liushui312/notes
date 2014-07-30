#include <chess.h>

void print_board(char board[][COL])
{
    int i, j;

    for (i = 0; i < ROW; i++)
    {
        for (j = 0; j < COL; j++) 
        {
            printf("%c", board[i][j]); 
        }
        printf("\n");
    }
}

int srv_send_data(struct chess_t *chess)
{
    int i, ret;
    int packet_len = sizeof(struct packet_t) + sizeof(struct chess_data_t);

    for (i = 0; i < chess->msg.look_num; i++)
    {
        if (chess->p_flag && chess->cli == chess->cli_list[i].sin_addr.s_addr)
            continue;
            
        chess->packet->type = DATA;
        memcpy(chess->packet->data, chess, sizeof(struct chess_data_t));
        chess->cli_list[i].sin_family = AF_INET;

        ret = sendto(chess->fd, chess->packet, packet_len, 0, (struct sockaddr *)&(chess->cli_list[i]), sizeof(struct sockaddr_in));
        printf("sendto data ret = %d, %s ", ret, inet_ntoa(chess->cli_list[i].sin_addr));
        printf("x = %d, y = %d, flag = %d\n", chess->x, chess->y, chess->flag);
        print_board(chess->board);
    }

    return ret;
}

int cli_send_data(struct chess_t *chess)
{
    int ret;
    struct sockaddr_in srv;
    int packet_len = sizeof(struct packet_t) + sizeof(struct chess_data_t);

    chess->packet->type = DATA;
    memcpy(chess->packet->data, chess, sizeof(struct chess_data_t));
    srv.sin_family = AF_INET;
    srv.sin_port = htons(PORT);
    srv.sin_addr.s_addr = inet_addr(chess->msg.ip);

    ret = sendto(chess->fd, chess->packet, packet_len, 0, (struct sockaddr *)&srv, sizeof(struct sockaddr_in));
    printf("sendto data ret = %d, %s ", ret, chess->msg.ip);
    printf("x = %d, y = %d, flag = %d\n", chess->x, chess->y, chess->flag);
    print_board(chess->board);

    return ret;
}

int chess_handle(struct chess_t *chess)
{
    int ret;

    if (chess->net_flag && (chess->who == 1 || !chess->p_flag))
    {
        if (chess->who == 1)
            srv_send_data(chess);
        else if (chess->who == 2)
            cli_send_data(chess);
            
        SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_DISABLE);
        SDL_EventState(SDL_KEYDOWN, SDL_DISABLE);
    }

    ret = press_chess(chess->board, chess->y, chess->x, &chess->flag);

    switch (ret)
    {
        case SUCCESS:
            show_board(chess);
            break;
        case WIN:
            show_board(chess);
            if (chess->flag)
                printf("黑方赢！\n");
            else
                printf("白方赢！\n");
            return EXIT;
        case H:
            show_board(chess);
            printf("和棋!\n");
            return EXIT;
        case NONE:
        default:
            break;
    }

    return TRUE;
}

int chess_init(struct chess_t *chess)
{
    int ret;

    memset(chess, 0, sizeof(struct chess_t));

    memset(chess->board, '+', sizeof(chess->board));
    chess->x = COL / 2;
    chess->y = ROW / 2;
    chess->flag = 1;

    chess->sdl.width = WIDTH;
    chess->sdl.height = HEIGHT;
    chess->sdl.bpp = BPP;

    ret = sdl_init(&chess->sdl);
    ERRP(ret != TRUE, goto ERR1, 0);

    show_board(chess);

    return TRUE;
ERR1:
    return FALSE;
}

void chess_exit(struct chess_t *chess)
{
    SDL_KillThread(chess->conn);
    free(chess->packet);
    free(chess->cli_list);
    close(chess->fd);
    sdl_exit(&chess->sdl);
}

int main(int argc, char *argv[])
{
    struct chess_t chess;
    SDL_Event event;
    int ret;

    ret = chess_init(&chess);
    ERRP(ret != TRUE, goto ERR1, 1, "sdl_init failed!\n");

    if (argc > 1)
        chess.eth_id = atoi(argv[1]);

    ret = chess_menu(&chess);
    ERRP(ret == EXIT, goto ERR2, 0);

    while (1)
    {
        if (SDL_WaitEvent(&event)) 
        {
            if (event.type == SDL_QUIT)
                break;
            if (event.type == SDL_KEYDOWN && 
                key_handle(&chess, event.key.keysym.sym) == EXIT)
                break;
            if (event.type == SDL_MOUSEBUTTONDOWN && 
                event.button.button == SDL_BUTTON_LEFT &&
                !pos_op(event.button.x, event.button.y, &chess) &&
                chess_handle(&chess) == EXIT)
                break;
        }
    }

ERR2:
    chess_exit(&chess);
ERR1:
    return 0;
}
