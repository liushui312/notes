#ifndef __KYO_CHESS_H__
#define __KYO_CHESS_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/if.h>
#include <sys/select.h>
#include <sdl.h>

#define WIDTH       640
#define HEIGHT      480
#define BPP         32

#define ROW         9 
#define COL         11 

#define X           30
#define Y           30
#define W           500
#define H           400

#define R           20
#define SPACE       50

#define BG          0xA9750F 
#define BC          0x0
#define WC          0xffffff

#define NONE    0
#define EXIT    1
#define SUCCESS 2
#define WIN     3
#define HE      4
#define RET     5
#define SINGLE  6       //表示单机



#define PORT        9000
#define PACKET_MAX  1024

#define FIND        0x1
#define FIND_ACK    0x2
#define CONN        0x3
#define CONN_ACK    0x4
#define CONN_ERR    0x5
#define START       0x6
#define START_ACK   0x7
#define START_ERR   0x8
#define DATA        0x9
#define DATA_ACK    0x10

#define DEBUG(...)      fprintf(stderr, __VA_ARGS__)

struct chess_data_t {
    int x;
    int y;
    int flag;
    char board[ROW][COL];
};

struct packet_t {
    char type;
    char data[0];
};

struct srv_msg_t {
    char ip[16];                    //服务器IP地址
    int status;                     //服务器状态，0为待战 1为在战
    int look_num;                   //服务器连接了多少个客户端
    int look_max;                   //服务器连接客户端最大数
};

struct node_t {
    struct srv_msg_t msg;
    struct node_t *next;
    struct node_t *prev;
};

struct chess_t {
    int x;
    int y;
    int flag;
    char board[ROW][COL];

    int fd;                         //用于保存创建socket返回的套接字描述符
    int eth_id;                     //指定第几张网卡，用于取IP地址
    struct packet_t *packet;        //用于接收发送数据存储空间

    struct srv_msg_t msg;           //记录服务器信息
    struct sockaddr_in *cli_list;   //用于记录连接的所有客户机的信息
    in_addr_t cli;                  //用于记录对战客户机

    int p_flag;                     //用于区分按键/网络所产生游戏数据改变  1为网络  0为按键
    int who;                        //用于记录本机是服务器/客户端 1为服务噐 2为对战客户端
    int net_flag;                   //用于区分单机/网络版本  1为网络  0为单机
    SDL_Thread *conn;               //用于保存服务器/客户端接收网络信息线程ID
    struct node_t srv_list;         //用于保存客户端查询在线服务器列表
    struct sdl_t sdl;
};

extern int chess_menu(struct chess_t *chess);
extern int key_handle(struct chess_t *chess, Uint32 key);
extern int press_chess(char (*board)[COL], int x, int y, int *flag);
extern int chess_handle(struct chess_t *);
extern void show_board(struct chess_t *);
extern int pos_op(int x, int y, struct chess_t *chess);
extern void print_board(char board[][COL]);

#endif /* __KYO_CHESS_H__ */
