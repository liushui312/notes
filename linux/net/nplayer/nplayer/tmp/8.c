#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

struct cmd_t{
    char *cmd;
    char *val;
};

struct cmd_t cmd_tab[]=
{
    "get_file_name", "ANS_LENGTH",
    "get_time_pos", "ANS_TIME_POSITION",
    "get_percent_pos", "ANS_PERCENT_POSITION",
    "get_audio_samples", "ANS_AUDIO_SAMPLES",
    NULL, NULL
};

int main(int argc, char **argv)
{
    FILE *fp;
    int fcmd;
    char *p;
    char buff[1024];
	
    close(2);


    fp = popen("mplayer -slave -quiet -input file=cmd 1.rmvb", "r");
    fcmd = open("cmd", O_WRONLY);

    p = "get_file_name\n";
    write(fcmd, p, strlen(p));

    printf("$>");
    fflush(NULL);
    while(1)
    {
	fgets(buff, sizeof(buff), fp);
	fputs(buff, stdout);
	fflush(NULL);
    }
    pclose(fp);
    close(fcmd);
}
