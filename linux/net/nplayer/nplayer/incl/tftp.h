
#ifndef _TFTP_H_
#define _TFTP_H_
 
#if 0
	#define  TFTP_PORT		69
#else
	#define  TFTP_PORT		44092
#endif

enum tftp_opcode{
	TFTP_READ = 1,
	TFTP_WRITE,
	TFTP_FILEDATA,
	TFTP_DATACK,
	TFTP_ERROR,

	TFTP_DIR
};

enum tftp_errno{
	TFTP_FILE_NOT_FOUND = 1, 
	TFTP_UNKNOWN_MODE = 4,
};

typedef struct tftp_req_t{
	u_int16_t opcode;
	char arg[512 - 2];
}TFTP_REQ;

typedef struct tftp_dat_t{
	u_int16_t opcode;
	u_int16_t blkno;
	u_int8_t  dat[512];
}TFTP_DAT;

typedef struct tftp_ack_t{
	u_int16_t opcode;
	u_int16_t blkno;
}TFTP_ACK;

typedef struct tftp_err_t{
	u_int16_t opcode;
	u_int16_t err_no;
	char errstr[512];
}TFTP_ERR;

int tftp_get(char *ip, char *fsrc, char *fdest, int *loaded_len);

#endif //_TFTP_H_
