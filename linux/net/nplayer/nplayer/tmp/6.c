#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
	//execlp("mplayer", "mplayer", "-slave", "-quiet", argv[1], NULL);
	system("mplayer 1.flv");
}
