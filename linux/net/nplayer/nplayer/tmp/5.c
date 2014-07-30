#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>

int main(void)
{
	int fd, fp;
	int fdcmd;
	char buff[4*1024];
	int len;
	int i;

	fp = open("./yw.rmvb", O_RDONLY); 
	fd = open("./dat", O_WRONLY|O_CREAT|O_TRUNC, 0644); 

	i = 0;
	while( (len=read(fp, buff, sizeof(buff)) ) != 0)
	{
		write(fd, buff, len);			
		if(i++ > 1024) break;
	}

	if( fork() == 0 )
	{
		execlp("mplayer", "mplayer", "-slave", "-quiet", 
			"-input", "file=./cmd", "./dat", NULL);

		exit(0);
	}

	while( (len=read(fp, buff, sizeof(buff)) ) != 0)
	{
		write(fd, buff, len);			
	}

	fdcmd = open("./cmd", O_WRONLY);

	while(1)
	{
		fgets(buff, sizeof(buff), stdin);
		write(fdcmd, buff, strlen(buff));
	}
}
