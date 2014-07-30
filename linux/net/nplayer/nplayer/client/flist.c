#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include "../incl/tftp.h"
#include "../incl/logprint.h"

//#define LPRINT(...) lprintn("tftp.log", __VA_ARGS__)
#define LPRINT(...) printf(__VA_ARGS__)
#define errstr()	strerror(errno)

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
	if(strcmp(fdest, ".") == 0) fdest = "ls.txt";

	puts(fdest);

	ret = tftp_get("octet", argv[1],  argv[2], fdest);
	//ret = tftp_get("netascii", "127.0.0.1", argv[1], fdest);
	printf("tftp_get[%d]\n", ret);

	return 0;
}
