#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

FILE *dir_file(char *dname)
{
	FILE *fp = tmpfile();
	DIR *dir;
	struct dirent *dirent;
	struct stat st;
	char type;

	chdir(dname);
	perror("chdir");

	dir = opendir(".");
	if(dir == NULL)
	{
		perror("opendir");
		return NULL;
	}

	while( (dirent = readdir(dir)) != NULL)
	{
		printf("%s", dirent->d_name);
		if( stat(dirent->d_name, &st) == -1)
		{
			perror("stat");
			return NULL;
		}

		if(S_ISREG(st.st_mode))
			type = '-';
		else if(S_ISDIR(st.st_mode))
			type = 'd';
		else continue;

		fprintf(fp, "%c %d %s\n", type, st.st_size, dirent->d_name);
	}
	
	fseek(fp, 0, SEEK_SET);

	return fp;
}

int main(int argc, char **argv)
{
	char buf[1024];
	FILE *fp = dir_file(argv[1]);

	while( fgets(buf, sizeof(buf), fp) )
	{
		fputs(buf, stdout);
	}

	fclose(fp);

	return 0;
}











