#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	char buf[1024];
	int  ret;
	struct stat st;
	double f;

	f = atof(NULL);
	printf("%f\n", f);
	
	ret = stat(argv[1], &st);
	printf("[%d]", ret);
	perror("stat");

	return 0;
}
