#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <vt.h>

#define ROW     12
#define COL     30

#define MAX     10
#define BASE    40000 

#define ESC     27
#define SPACE   32
#define UP      65
#define DOWN    66
#define LEFT    68
#define RIGHT   67

char bg[ROW][COL];
int x = 3, y = 3, level = 0;
int fx, fy;
int sx[100], sy[100];
int count = 0;
int direct = RIGHT;

void draw_bg(char (*bg)[COL], int fx, int fy);
void draw_snake(char (*bg)[COL], int x, int y, int *sx, int *sy, int count);
void stop_timer(void);
void start_timer(int level);
void run_game(int sig);
void move_snake(void);
void snake_exit(void);
void show_snake(char (*bg)[COL]);
int get_input(void);
void snake_init(void);

void draw_bg(char (*bg)[COL], int fx, int fy)
{
    int i, j;

    for (i = 0; i < ROW; i++)
    {
        for (j = 0; j < COL; j++) 
        {
            if (i == 0 || i == ROW - 1 || j == 0 || j == COL - 1) 
                bg[i][j] = '#';
            else
                bg[i][j] = ' ';
        }
    }
    bg[fx][fy] = '0';
}

void draw_snake(char (*bg)[COL], int x, int y, int *sx, int *sy, int count)
{
    int i;

    bg[x][y] = '@';

    for (i = 0; i < count; i++)
    {
        bg[sx[i]][sy[i]] = '0';
    }
}

void show_snake(char (*bg)[COL])
{
    int i, j;

    gotoxy(1, 1);
    fflush(NULL);
    for (i = 0; i < ROW; i++)
    {
        for (j = 0; j < COL; j++) 
        {
            printf("%c", bg[i][j]);
        }
        printf("\n");
    }
}

void snake_exit(void)
{
    show_cur();
    fflush(NULL);
    system("stty echo icanon");
    stop_timer();
    exit(0);
}

void move_snake(void)
{
    if (x - 1 < 0 || x + 1 > ROW - 1 || 
        y - 1 < 0 || y + 1 > COL - 1)
        snake_exit();

    if (x == fx && y == fy)
    {
        sx[count] = x;
        sy[count] = y;
        count++;
        fx = rand() % (ROW - 2) + 1;
        fy = rand() % (COL - 2) + 1;
        start_timer(level++);
    }

    if (count != 0)
    {
        memmove(sx + 1, sx, sizeof(int) * (count - 1));
        memmove(sy + 1, sy, sizeof(int) * (count - 1));
        sx[0] = x;
        sy[0] = y;
    }

    switch (direct)
    {
        case UP:
            x--;
            break;
        case DOWN:
            x++;
            break;
        case LEFT:
            y--;
            break;
        case RIGHT:
            y++;
            break;
        default:
            break;
    }
}

void run_game(int sig)
{
    draw_bg(bg, fx, fy); 
    move_snake();
    draw_snake(bg, x, y, sx, sy, count);
    show_snake(bg);
}

void start_timer(int level)
{
    struct itimerval it;

    it.it_interval.tv_sec = 0;
    it.it_interval.tv_usec = (MAX - level) * BASE;
    it.it_value.tv_sec = 0;
    it.it_value.tv_usec = 100;

    signal(SIGALRM, run_game);

    setitimer(ITIMER_REAL, &it, NULL);
}

void stop_timer(void)
{
    struct itimerval it;

    it.it_interval.tv_sec = 0;
    it.it_interval.tv_usec = 0;
    it.it_value.tv_sec = 0;
    it.it_value.tv_usec = 0;

    setitimer(ITIMER_REAL, &it, NULL);
}

int get_input(void)
{
    int key, ret;
    char ch[8];

    ret = read(0, ch, 8);
    if (ret == 1)
        key = ch[0];
    else if (ret == 3 && ch[0] == 27 && ch[1] == 91)
        key = ch[2];

    return key;
}

void snake_init(void)
{
    clean(0);
    hide_cur();
    fflush(NULL);

    fx = rand() % (ROW - 2) + 1;
    fy = rand() % (COL - 2) + 1;

    system("stty -echo -icanon");
    srand(time(NULL));

    start_timer(level);
}

int main(void)
{
    int quit = 0, key , flag = 1;

    snake_init();

    while (!quit)
    {
        key = get_input();
        switch (key)
        {
            case ESC: 
                quit = 1;
                break;
            case SPACE:
                if (flag)
                    stop_timer();
                else
                    start_timer(level);
                flag = !flag;
                break;
            case UP:
                if (direct != DOWN)
                    direct = UP;
                break;
            case DOWN:
                if (direct != UP)
                    direct = DOWN;
                break;
            case LEFT:
                if (direct != RIGHT)
                    direct = LEFT;
                break;
            case RIGHT:
                if (direct != LEFT)
                    direct = RIGHT;
                break;
        }
    }

    snake_exit();

    return 0;
}
