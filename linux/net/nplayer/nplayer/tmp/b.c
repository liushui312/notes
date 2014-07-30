#include <stdio.h>


int main(void)
{
	FILE *fp;
	char buf[1024];

	fp = popen("./aa", "w");

	fprintf(fp, "hello\n");

	while(1)
	{
		getchar();
		fprintf(fp, "hello\n");
	}
}
