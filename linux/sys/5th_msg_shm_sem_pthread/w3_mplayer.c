#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define CTL "/tmp/kyo_ctl"
#define MES "/tmp/kyo_mes"

#define GETS(string, op, ...)   do          \
            {                               \
                int ret;                    \
                printf(string);             \
                ret = scanf(__VA_ARGS__);   \
                while (getchar() != '\n')   \
                    continue;               \
                if (ret == 0)               \
                    op;                     \
            } while (0)

#define GETLINES(string, buf)   do                  \
            {                                       \
                printf(string);                     \
                fgets(buf, sizeof(buf), stdin);     \
                if (buf[strlen(buf) - 1] == '\n')   \
                    buf[strlen(buf) - 1] = '\0';    \
                else                                \
                    while (getchar() != '\n')       \
                        continue;                   \
            } while (0)

int main(int argc, char *argv[])
{
    int quit = 0, n, offset = 10;
    int ctl, mes, ret;
    char buf[1024];

    if (argc < 2)
        return 0;

    mkfifo(CTL, 0777);
    mkfifo(MES, 0777);

    ctl = open(CTL, O_RDWR);
    if (ctl == -1)
        goto ERR1;
    mes = open(MES, O_RDWR);
    if (mes == -1)
        goto ERR1;

    if (fork() == 0)
    {
        close(2);
        dup2(mes, 1);
        execlp("mplayer", "mplayer", argv[1], "-quiet", "-slave", "-input", "file="CTL, NULL);
        perror("execlp");
        exit(0);
    }
    sleep(1);

    n = fcntl(mes, F_GETFL);
    fcntl(mes, F_SETFL, n | O_NONBLOCK);

    while (read(mes, buf, sizeof(buf)) > 0)
        continue;

    fcntl(mes, F_SETFL, n);


    while (!quit)
    {
START:
        printf("1. pause/play!\n");
        printf("2. seek\n");
        printf("3. get_time_pos!\n");
        printf("4. get_time_length!\n");
        printf("5. exit!\n");
        GETS("please input [ 1 - 5 ] : ", goto START, "%d", &n);

        switch (n)
        {
            case 1:
                write(ctl, "p\n", 2);
                break;
            case 2:
                GETS("\tplease input seek [10] :", offset = 10, "%d", &offset);
                snprintf(buf, sizeof(buf), "seek %d\n", offset);
                write(ctl, buf, strlen(buf));
                break;
            case 3:
                write(ctl, "get_time_pos\n", 13);
                ret = read(mes, buf, sizeof(buf));
                buf[ret] = '\0';
                printf("pos = %s\n", buf);
                break;
            case 4:
                write(ctl, "get_time_length\n", 16);
                ret = read(mes, buf, sizeof(buf));
                buf[ret] = '\0';
                printf("length = %s\n", buf);
                break;
            case 5:
                write(ctl, "quit\n", 5);
                quit = 1;
                break;
            default:
                break;
        }
    }

ERR1:
    close(ctl);
    close(mes);
    unlink(CTL);
    unlink(MES);
    return 0;
}
