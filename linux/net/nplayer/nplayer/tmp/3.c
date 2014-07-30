#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>

#define mwrite(cmd) write(fmi[1], cmd, strlen(cmd))
#define mread(cmd)  read(fmo[0], cmd, sizeof(cmd))

int main(int argc, char *argv[])
{
	char cmd[1024];
	int fmi[2];
	int fmo[2];

	pipe(fmi);
	pipe(fmo);

	if( fork() == 0 )
	{
		close(fmi[1]);
		close(fmo[0]);
		dup2(fmi[0], 0);
		dup2(fmo[1], 1);

		close(2);

		execlp("mplayer", "mplayer", "-slave", "-quiet", 
			"yewen.rmvb", NULL);
	}
	
	close(fmi[0]);
	close(fmo[1]);

	while(1)
	{
		fgets(cmd, sizeof(cmd), stdin);
		mwrite(cmd);

		if( strncmp(cmd, "quit", 4) == 0 )
		{
			break;
		}
	}

	close(fmi[1]);
	close(fmo[0]);

	return 0;
}
