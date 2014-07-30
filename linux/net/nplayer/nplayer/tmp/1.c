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

	mkfifo("cmd", 0644);
	mkfifo("dat", 0644);

	if( fork() == 0 )
	{
		execlp("mplayer", "mplayer", "-slave", "-quiet", 
			"-input", "file=./cmd", "./dat", NULL);

		exit(0);
	}

	fdcmd = open("./cmd", O_WRONLY);
}
