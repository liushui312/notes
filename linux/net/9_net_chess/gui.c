#include <chess.h>

void draw_cur(SDL_Surface *dst, int vx, int vy)
{
    int x = X + vx * SPACE, y = Y + vy * SPACE;
    int w = R * 2 / 5;

    draw_rect(dst, x - R, y - R, w, 2, BC);
    draw_rect(dst, x - R, y - R, 2, w, BC);

    draw_rect(dst, x - R, y + R, w, 2, BC);
    draw_rect(dst, x - R, y + R - w, 2, w, BC);

    draw_rect(dst, x + R - w, y - R, w, 2, BC);
    draw_rect(dst, x + R, y - R, 2, w, BC);

    draw_rect(dst, x + R - w, y + R, w, 2, BC);
    draw_rect(dst, x + R, y + R - w + 2, 2, w, BC);
}

void draw_board(SDL_Surface *screen)
{
    int i;
    draw_rect(screen, 0, 0, WIDTH, HEIGHT, BG);
    draw_box(screen, X - 5, Y - 5, W + 10, H + 10, 2, BG, BC);
    draw_box(screen, X, Y, W, H, 1, BG, BC);

    for (i = 1; i < ROW - 1; i++)
    {
        draw_rect(screen, X, Y + i * SPACE, W, 1, BC); 
    }

    for (i = 1; i < COL - 1; i++)
    {
        draw_rect(screen, X + i * SPACE, Y, 1, H, BC); 
    }

    draw_circle(screen, X + COL / 2 * SPACE, Y + ROW / 2 * SPACE, 4, BC);
    draw_circle(screen, X + 2 * SPACE, Y + 2 * SPACE, 4, BC);
    draw_circle(screen, X + 2 * SPACE, Y + (ROW - 3) * SPACE, 4, BC);
    draw_circle(screen, X + (COL - 3) * SPACE, Y + (ROW - 3) * SPACE, 4, BC);
    draw_circle(screen, X + (COL - 3) * SPACE, Y + 2 * SPACE, 4, BC);
}

void show_board(struct chess_t *chess)
{
    int i, j;

    draw_board(chess->sdl.screen);

    for (i = 0; i < ROW; i++)
    {
        for (j = 0; j < COL; j++)
        {
            if (chess->board[i][j] == '#')
                draw_circle(chess->sdl.screen, X + j * SPACE, Y + i * SPACE, R, BC);
            else if (chess->board[i][j] == '@')
                draw_circle(chess->sdl.screen, X + j * SPACE, Y + i * SPACE, R, WC);
        }
    }
    if (chess->flag)
        draw_circle(chess->sdl.screen, 590, 480 / 2, R, BC);
    else
        draw_circle(chess->sdl.screen, 590, 480 / 2, R, WC);

    draw_cur(chess->sdl.screen, chess->x, chess->y);

    SDL_Flip(chess->sdl.screen);
}

int pos_op(int x, int y, struct chess_t *chess)
{
    int tx = (x - (X - R)) / SPACE, ty = (y - (Y - R)) / SPACE;

    if (x >= X - R && x <= X + W + R && 
        y >= Y - R && y <= Y + H + R &&
        x >= X + tx * SPACE - R && x <= X + tx * SPACE + R &&
        y >= Y + ty * SPACE - R && y <= Y + ty * SPACE + R) 
    {
        chess->x = tx;
        chess->y = ty;
        return TRUE;
    }
    return FALSE;
}


