#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>


int main(int argc, char *argv[])
{
	char cmd[1024];
	char ch;
	int ret;
	FILE *fp;

	if(argc != 2)
	{
		printf("Usage:%s filename\n", argv[0]);
		return 0;
	}

	sprintf(cmd,"mplayer -slave -quiet %s", argv[1]);
	fp = popen(cmd, "w");
	if(fp == NULL)
	{
		perror("popen");
		return -1;
	}

	sleep(1);
	system("clear");
	fread(cmd, 1, sizeof(cmd), fp);
	printf(cmd);

	while(1)
	{
		fgets(cmd, sizeof(cmd), stdin);
		fprintf(stdout, "cmd:%s", cmd);
		fprintf(fp, cmd);
	}
}
