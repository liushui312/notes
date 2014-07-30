#ifndef __KYO_NET_H__
#define __KYO_NET_H__

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define LOGIN       0x1
#define SUCCESS     0x2
#define ERR_USR     0x3
#define ERR_PWD     0x4
#define ERR_PATH    0x5
#define DATA_START  0x6
#define DATA        0x7
#define DATA_ACK    0x8
#define DATA_END    0x9

#define PACKET_MAX  1024

#define DEBUG0(...)
#define DEBUG1(...)     fprintf(stderr, __VA_ARGS__);
#define DEBUG2(...)     perror(__VA_ARGS__);

#define ERRP(con, ret, flag, ...) do        \
            {                               \
                if (con)                    \
                {                           \
                    DEBUG##flag(__VA_ARGS__)\
                    ret;                    \
                }                           \
            } while (0)

struct login_t {
    char usr;
    char passwd;
    char path;
};

struct packet_t {
    char type;
    char data[0];
};

#endif /* __KYO_NET_H__ */
