#include <stack.h>

#define ROW     10
#define COL     20

struct pos_t {
    int x;
    int y;
};

char map[ROW][COL] = {
    "####################",
    "###      S#       ##",
    "#   ## #### ## # # #",
    "# ## #      #  #   #",
    "#    # #### ########",
    "###### #          ##",
    "#      # ### ####  #",
    "# #### #####   #E# #",
    "#    #     ### #   #",
    "####################",
};

struct pos_t direct[4] = {
    {0, -1},
    {0, 1}, 
    {-1, 0},
    {1, 0}
};

struct pos_t getxy(int ch)
{
    struct pos_t cur;
    int i, j;

    for (i = 0; i < ROW; i++)
    {
        for (j = 0; j < COL; j++) 
        {
            if (map[i][j] == ch)
            {
                cur.x = i; 
                cur.y = j;
                return cur;
            }
        }
    }
}

void show(void)
{
    int i, j;

    printf("\033[1;1H");

    for (i = 0; i < ROW; i++)
    {
        for (j = 0; j < COL; j++) 
        {
            printf("%c", map[i][j]); 
        }
        printf("\n");
    }
}


#if 0
void test(struct pos_t cur, struct pos_t end)
{
    int i;
    struct pos_t start;

    map[cur.x][cur.y] = '@';
    show();
    getchar();

    if (cur.x == end.x && cur.y == end.y)
        exit(0);

    for (i = 0; i < 4; i++)
    {
        start.x = cur.x + direct[i].x;
        start.y = cur.y + direct[i].y;

        if (map[start.x][start.y] == '#' ||
                map[start.x][start.y] == '@')
            continue;
        test(start, end);
    }
    
    map[cur.x][cur.y] = ' ';
    show();
    getchar();
}
#endif 

struct data_t {
    struct pos_t pos;
    int i;
};

void test(struct pos_t cur, struct pos_t end)
{
    int i;
    STACK *handle = NULL;
    struct pos_t new;
    struct data_t data, *save = NULL;

    handle = stack_create(sizeof(struct data_t), 1024);

START:
    map[cur.x][cur.y] = '@';
    show();
    getchar();

    if (cur.x == end.x && cur.y == end.y)
        goto EXIT;

    for (i = 0; i < 4; i++)
    {
        new.x = cur.x + direct[i].x;
        new.y = cur.y + direct[i].y;

        if (map[new.x][new.y] == '#' ||
                map[new.x][new.y] == '@')
            continue;
        /*test(start, end);*/
        data.i = i;
        data.pos = cur;
        stack_push(&data, handle);  //保存当前状态
        cur = new;      //模拟函数传参
        goto START;     //模拟函数跳转
END:
        ;
    }
    
    map[cur.x][cur.y] = ' ';
    show();
    getchar();

    save = stack_pop(handle);       //模拟函数退栈
    i = save->i;
    cur = save->pos;
    goto END;
EXIT:
    stack_destroy(&handle);
}

int main(void)
{
    struct pos_t start, end, *cur = NULL;
    int i;

    start = getxy('S');
    end = getxy('E');

    printf("\033[2J\033[?25l");
    fflush(stdout);

    test(start, end);

    printf("main End!\n");
#if 0
    handle = stack_create(sizeof(struct pos_t), 100);

    stack_push(&start, handle);


    while (1)
    {
        cur = stack_pop(handle);
        if (cur == NULL)
            goto EXIT;

        map[cur->x][cur->y] = '@';
        show();
        getchar();
        if (cur->x == end.x && cur->y == end.y)
            break;

        for (i = 0; i < 4; i++)
        {
            start.x = cur->x + direct[i].x;
            start.y = cur->y + direct[i].y;

            if (map[start.x][start.y] == '#' ||
                map[start.x][start.y] == '@')
                continue;
            stack_push(&start, handle);
        }
    }

EXIT:
    stack_destroy(&handle);
#endif

    printf("\033[?25h\n");

    return 0;
}
