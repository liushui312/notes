#include <stdio.h>

int main(int argc, char *argv[])
{
	char buf[512];
	int n;
	FILE *fp = popen("ls", "r");

	while( fgets(buf, sizeof(buf), fp) )
	{
		fputs(buf, stdout);
	}

	pclose(fp);

	return 0;
}
