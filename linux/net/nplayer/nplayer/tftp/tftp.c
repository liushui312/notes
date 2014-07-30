#include "../incl/comm.h"

#define LPRINT(...) lprintn(LOGFILE, __VA_ARGS__)
//#define LPRINT(...) printf(__VA_ARGS__)

#define MODE "octet"

#define MAX_ERR_CNT  5

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
			LPRINT("send:%s\n", errstr());
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

int tftp_get(char *ip, char *fsrc, char *fdest, int *loaded_len)
{
	TFTP_REQ req;
	TFTP_ACK ack;
	TFTP_DAT dat;
	TFTP_ERR *err;
	char *p;
	int sd;
	int  len;
	struct sockaddr_in peer;
	socklen_t peer_len;
	u_int16_t blkno;
	FILE  *fp;
	int err_cnt = 0;

	if(loaded_len != NULL)
	{
		*loaded_len = 0;
	}

	sd = socket(PF_INET, SOCK_DGRAM, 0);
	if(sd < 0)
	{
		LPRINT("socke:%s\n", errstr());
		return -1;
	}

	peer.sin_family = AF_INET;
	peer.sin_port = htons(TFTP_PORT);
	inet_pton(AF_INET, ip, &peer.sin_addr);//, sizeof(peer.sin_addr));

	//发送读请求包
	memset(&req, 0, sizeof(req));

	req.opcode = htons(TFTP_READ);
	p = req.arg;

	memcpy(p, fsrc, strlen(fsrc)+1);
	p += strlen(fsrc)+1;

	memcpy(p, MODE, strlen(MODE)+1);
	p += strlen(MODE)+1;

	len = sizeof(req.opcode) + p - req.arg;

	len = _sendto(sd, &req, len, &peer);
	if(len == -1)
	{
		LPRINT("timeout\n");
		close(sd);
		return -1;
	}

	//接收数据
	LOGPRINT("open:%s\n", fdest);
	fp = fopen(fdest, "w");
	if(fp == NULL)
	{
		LPRINT("fopen:%s\n", errstr());
		close(sd);
		return -1;
	}

	blkno = 1;
	while(1)
	{
		/*
		if(blkno > 3 * 2048)
		{
			usleep(1 * 1000);
		}
		*/

		peer_len = sizeof(peer);
		recvfrom(sd, &dat, len, 0, 
				(struct sockaddr*)&peer, &peer_len);

		dat.opcode = ntohs(dat.opcode);

		if(dat.opcode == TFTP_ERROR)
		{
			err = (TFTP_ERR*)&dat;
			err->err_no = ntohs(err->err_no);
			LPRINT("Error[%d:%s]\n", err->err_no, err->errstr);
			close(sd);
			fclose(fp);
			return  -err->err_no;
		}
		else if(dat.opcode == TFTP_FILEDATA)
		{
		/*
			if(ntohs(dat.blkno) != blkno)
			{
				char ip[16];
				LPRINT("blockno err[%d %d]\n", blkno, ntohs(dat.blkno));
				LPRINT("peer[%s:%d]\n", inet_ntop(AF_INET, &peer.sin_addr, ip, 16),
					ntohs(peer.sin_port));
				close(sd);
				fclose(fp);
				return -1;
			}
			else
		*/	
			if(ntohs(dat.blkno) == blkno)
			{
				fwrite(dat.dat, 1, len - 4, fp);
				if(loaded_len != NULL)
				{
					*loaded_len += len-4;
				}
				blkno++;

				err_cnt = 0;
			}
			else
			{
				char ip[16];

				err_cnt++;

				LPRINT("blockno err[%d][%d %d]\n", err_cnt, 
					blkno, ntohs(dat.blkno));
				LPRINT("peer[%s:%d]\n", 
					inet_ntop(AF_INET, &peer.sin_addr, ip, 16),
					ntohs(peer.sin_port));

				if(err_cnt > MAX_ERR_CNT)
				{
					close(sd);
					fclose(fp);
					return -1;
				}
			}

			ack.opcode = htons(TFTP_DATACK);

			ack.opcode = htons(TFTP_DATACK);
			ack.blkno = dat.blkno;

			//最后一个包
			if(len < sizeof(TFTP_DAT))
			{
				sendto(sd, &ack, sizeof(ack), 0,
					(struct sockaddr*)&peer, sizeof(peer));
				break;
			}

			//
			len = _sendto(sd, &ack, sizeof(ack),  &peer);
			if(len == -1)
			{
				LPRINT("timeout\n");
				close(sd);
				fclose(fp);
				return -1;
			}
		}
		else
		{
			LPRINT("未知包[%d]\n", dat.opcode);
			close(sd);
			fclose(fp);
			return -1;
		}
	}

	close(sd);
	fclose(fp);

	return 0;
}

#if 0
int main(int argc, char *argv[])
{
	int ret;
	char *fdest;

	if(argc < 3)
	{
		printf("Usage:%s <ip> <file>\n", argv[0]);
		return -1;
	}

	fdest = argv[2];
	if( strrchr(fdest, '/') != NULL)
	{
		fdest = strrchr(fdest, '/') + 1;
	}

	puts(fdest);

	ret = tftp_get("octet", argv[1],  argv[2], fdest);
	//ret = tftp_get("netascii", "127.0.0.1", argv[1], fdest);
	printf("tftp_get[%d]\n", ret);

	return 0;
}
#endif

