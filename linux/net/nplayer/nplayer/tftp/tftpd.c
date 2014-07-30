#include "comm.h"

#define errstr()	strerror(errno)

//#define tftp_root		".."
char tftp_root[256];

struct tftpd_t{
	TFTP_REQ  req;
	int req_len;	
	struct sockaddr_in peer;
};

/* 
 * 发送并等待应答包
 * 成功返回应答包大小， 失败返回-1
 */
static int 
_sendto(int sd, void *buf, size_t len, struct sockaddr_in *peer)
{
	struct pollfd fds;
	int ret, i;

	fds.fd = sd;
	fds.events = POLLIN;

	for(i=3; i>0; i--)
	{
		ret = sendto(sd, buf, len, 0, (struct sockaddr*)peer, 
				sizeof(struct sockaddr_in));
		if(ret == -1)
		{
			LOGPRINT("send:%s\n", errstr());
			return -1;
		}

		poll(&fds, 1, 1000);
		if(fds.revents & POLLIN)
		{
			break;
		}
	}

	if(i == 0)		//timeout
	{
		return -1;
	}

	ioctl(sd, FIONREAD, &ret);

	return ret;
}

/*
 * tftp发送
 * 如果fname为目录, 将发送"ls -l fname"的内容
 */
int send_file(char *fname, struct sockaddr_in *peer)
{
	FILE *fp = NULL;
	int sd;
	TFTP_DAT dat;
	TFTP_ERR *err;
	TFTP_ACK *ack;
	char buff[512];
	int ret, len;
	u_int16_t blkno;
	u_int16_t opcode;
	struct stat st;

	LOGPRINT("send file:%s\n", fname);

	sd = socket(PF_INET, SOCK_DGRAM, 0);
	if(sd == -1)
	{
		LOGPRINT("sockdt:%s\n", errstr());
		return -1;
	}

	if( stat(fname, &st) == 0)
	{
		if( S_ISDIR(st.st_mode) )
		{
			sprintf(buff, "ls -l '%s'", fname);
			fp = popen(buff, "r");
			fgets(buff, sizeof(buff), fp);	//第一行为"总计 xxx",去掉
		}
		else
		{
			sprintf(buff, "%s", fname);
			fp = fopen(buff, "r");
		}
	}

	if(fp == NULL)
	{
		err = (TFTP_ERR*)buff;
		err->opcode = htons(TFTP_ERROR);
		err->err_no = htons(TFTP_FILE_NOT_FOUND);
		strcpy(err->errstr, errstr());
		
		sendto(sd, err, strlen(err->errstr)+1+4, 0,
			(struct sockaddr*)peer, sizeof(struct sockaddr_in));	

		close(sd);
		return 0;
	}

	blkno = 1;
	dat.blkno = 0;
	while(1)
	{
		if(dat.blkno != htons(blkno) )
		{
			dat.blkno = htons(blkno);
			dat.opcode = htons(TFTP_FILEDATA);

			len = fread(dat.dat, 1, sizeof(dat.dat), fp);
		}

		ret = _sendto(sd, &dat, len+4, peer);
		if( ret == -1)
		{
			LOGPRINT("timeout\n");
			ret = -1;
			goto quit;
		}

		recvfrom(sd, buff, ret, 0, NULL, NULL);

		opcode = ntohs( *(u_int16_t*)buff );
		switch(opcode)
		{
		case TFTP_DATACK:
			ack = (TFTP_ACK*)buff;
			ack->blkno = ntohs(ack->blkno);
			if(ack->blkno == blkno)
			{
				blkno += 1;
			}
			break;

		case TFTP_ERROR:
			err = (TFTP_ERR*)buff;
			err->err_no = ntohs(err->err_no);
			LOGPRINT("Error[%d]:%s\n", err->err_no, err->errstr);
			ret =  -err->err_no;
			goto quit;

		default:
			LOGPRINT("未知包[%d]\n", opcode);
			ret = -1;
			goto quit;
		}

		if(len < 512) 
			break;
	}

	ret = 0;
quit:
	close(sd);
	if( S_ISDIR(st.st_mode) ) pclose(fp);
	else fclose(fp);

	LOGPRINT("send block:%d\n", blkno);
	return ret;
}

void *thrd_main(void *arg)
{
	//int len = ((struct tftpd_t *)arg)->req_len;
	TFTP_REQ req = ((struct tftpd_t *)arg)->req;
	struct sockaddr_in peer =  ((struct tftpd_t *)arg)->peer;
	u_int16_t opcode = ntohs(*(u_int16_t*)&req);
	free(arg);
	char fname[256];

	switch(opcode)
	{
	case TFTP_READ:
		sprintf(fname, "%s/%s", tftp_root, req.arg);
		send_file(fname, &peer);
		break;
	case TFTP_WRITE:
		break;
	}

	return NULL;
}

int main(int argc, char *argv[])
{
	int ret;
	int sd, len;
	socklen_t sock_len;
	struct pollfd fds;
	struct sockaddr_in addr, peer;
	TFTP_REQ req;
	struct tftpd_t *arg;
	pthread_t tid;
	char cwd[256];

	if( argc < 2 )
	{
		printf("Usage:%s <server root dir>\n", argv[0]);
		return 0;
	}

	getcwd(cwd, sizeof(cwd));
	chdir(argv[1]);
	getcwd(tftp_root, sizeof(tftp_root));
	chdir(cwd);

/*
	if(argc < 2)
	{
		printf("Usage:%s port\n", argv[0]);
		return -1;
	}
*/
	sd = socket(PF_INET, SOCK_DGRAM, 0);	
	if(sd < 0)
	{
		printf("socket:%s\n", errstr());
		return -1;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons( TFTP_PORT );
	inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr);

	ret = bind(sd, (struct sockaddr*)&addr, sizeof(addr));

	fds.fd = sd;
	fds.events = POLLIN;

	while(1)
	{
		char ip[16];

		poll(&fds, 1, -1);
		ioctl(sd, FIONREAD, &len);

		sock_len = sizeof(peer);
		recvfrom(sd, &req, len, 0, (struct sockaddr*)&peer, &sock_len);

		LOGPRINT("recvfrom[%s:%d]\n", 
			inet_ntop(AF_INET, &peer.sin_addr, ip, sizeof(ip)), 
			ntohs(peer.sin_port));
			
		arg = malloc( sizeof(struct tftpd_t) );
		arg->req_len = len;
		arg->req = req;
		arg->peer = peer;
		ret = pthread_create(&tid, NULL, thrd_main, arg);
		if(ret == -1)
		{
			LOGPRINT("ptread_create:%s\n", errstr());
			free(arg);
		}
	}

	return 0;
}

