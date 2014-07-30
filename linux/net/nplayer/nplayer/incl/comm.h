
#ifndef _COMM_H_
#define _COMM_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <dirent.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>
#include <sys/wait.h>
#include <poll.h>
#include <sys/ioctl.h>

#include "input.h"
#include "logprint.h"
#include "ansi_code.h"
#include "tftp.h"
#include "mainwin.h"
#include "yplayer.h"

//#define  FIFO

#define  SERV_IP		"10.1.0.127"
#define  SERV_PORT		44092

#define  ARG_LEN		64
#define  PATH_LEN		128
#define  IP_LEN			16

#define  MOVIES_DIR		"/root/net/netplayer/movies"
#define  LOGFILE		"../log/play.log"

#define LOGPRINT(...) lprintn(LOGFILE, __VA_ARGS__)

#define errstr()  strerror(errno)
/*
enum cmd_et{
	CMD_LS,
	CMD_CD,
	CMD_GET,
	CMD_EXIT
};

struct cmd_st{
	_num cmd_et cmd;
	char arg[ARG_LEN];
};

struct work_st{
	int sd;
	char path[PATH_LEN];
	char peer_ip[IP_LEN];
};
*/
#endif //_COMM_H_
