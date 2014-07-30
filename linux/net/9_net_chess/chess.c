#include <chess.h>

int check_win(char (*board)[COL], int x, int y, int *flag)
{
    int i, j, count = 0;
    char ch = -1, c;
    
    if (*flag)
        c = '#';
    else
        c = '@';

    for (i = y - 4; i <= y + 4; i++)
    {
        if (i < 0 || i > COL - 1)
            continue;
        if (ch == board[x][i])
            count++;
        else
        {
            ch = board[x][i];
            count = 1;
        }
        if (count == 5 && ch == c)
            return WIN;
    }

    for (i = x - 4, ch = -1; i <= x + 4; i++)
    {
        if (i < 0 || i > ROW - 1)
            continue;
        if (ch == board[i][y])
            count++;
        else
        {
            ch = board[i][y];
            count = 1;
        }
        if (count == 5 && ch == c)
            return WIN;
    }

    for (i = x - 4, j = y - 4, ch = -1; i <= x + 4; i++, j++)
    {
        if (i < 0 || j < 0 || i > ROW - 1 || j > COL - 1)
            continue;
        if (ch == board[i][j])
            count++;
        else
        {
            ch = board[i][j];
            count = 1;
        }
        if (count == 5 && ch == c)
            return WIN;
    }

    for (i = x - 4, j = y + 4, ch = -1; i <= x + 4; i++, j--)
    {
        if (i < 0 || j < 0 || i > ROW - 1 || j > COL - 1)
            continue;
        if (ch == board[i][j])
            count++;
        else
        {
            ch = board[i][j];
            count = 1;
        }
        if (count == 5 && ch == c)
            return WIN;
    }


    for (i = 0; i < ROW; i++)
    {
        for (j = 0; j < COL; j++) 
        {
            if (board[i][j] == '+') 
            {
                *flag = !(*flag);
                return SUCCESS;
            }
        }
    }
    
    return H;
}

int press_chess(char (*board)[COL], int x, int y, int *flag)
{
    if (board[x][y] != '+')
        return NONE;

    if (*flag)
        board[x][y] = '#';
    else
        board[x][y] = '@';

    return check_win(board, x, y, flag);
}

int key_handle(struct chess_t *chess, Uint32 key)
{
    switch (key)
    {
        case SDLK_ESCAPE:
            return EXIT;
        case SDLK_LEFT:
            if (chess->x > 0)
            {
                chess->x -= 1;
                show_board(chess);
            }
            break;
        case SDLK_RIGHT:
            if (chess->x < COL - 1)
            {
                chess->x += 1;
                show_board(chess);
            }
            break;
        case SDLK_UP:
            if (chess->y > 0)
            {
                chess->y -= 1;
                show_board(chess);
            }
            break;
        case SDLK_DOWN:
            if (chess->y < ROW - 1)
            {
                chess->y += 1;
                show_board(chess);
            }
            break;
        case SDLK_SPACE:
            return chess_handle(chess);
        default:
            break;
    }
    
    return NONE;
}
