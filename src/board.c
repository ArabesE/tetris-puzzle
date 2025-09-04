#include <stdlib.h>
#include <string.h>
#include "board.h"

/* 在此文件中实现棋盘的内部数据结构与全部接口：
 *   - 构造/析构与尺寸查询；
 *   - 边界/占用判断；
 *   - 放置/移除给定形状；
 *   - 统计与打印。 */

#define BOARD_SIZE sizeof(Board)
#define EMPTY_MARK '.'

struct Board
{
    int W;
    int H;
    char state[];
};

static inline int state_idx(int W, int x, int y)
{
    return y * W + x;
}

Board *board_create(int width, int height)
{
    if (width <= 0 || height <= 0)
        return NULL;

    // memory allocation
    size_t state_size = (size_t)width * (size_t)height;
    if (state_size > SIZE_MAX - BOARD_SIZE)
        return NULL;
    Board *b = malloc(BOARD_SIZE + state_size);
    if (!b)
        return NULL;

    // initialize board
    b->W = width;
    b->H = height;
    memset(b->state, EMPTY_MARK, state_size);
    return b;
}

void board_destroy(Board *b)
{
    if (!b)
        return;
    free(b);
}

int board_width(const Board *b)
{
    if (!b)
        return -1;
    return b->W;
}

int board_height(const Board *b)
{
    if (!b)
        return -1;
    return b->H;
}

size_t board_cell_count(const Board *b)
{
    if (!b)
        return 0;
    return (size_t)b->W * (size_t)b->H;
}

size_t board_filled_count(const Board *b)
{
    if (!b)
        return 0;
    int W = b->W;
    int H = b->H;
    size_t count = 0;
    for (int y = 0; y < H; ++y)
    {
        for (int x = 0; x < W; ++x)
        {
            if (!board_is_empty(b, x, y))
                count++;
        }
    }
    return count;
}

bool board_in_bounds(const Board *b, int x, int y)
{
    if (!b)
        return false;
    return x >= 0 && x < b->W && y >= 0 && y < b->H;
}

bool board_is_empty(const Board *b, int x, int y)
{
    if (!b)
        return false;
    if (!board_in_bounds(b, x, y))
        return false;
    return b->state[state_idx(b->W, x, y)] == EMPTY_MARK;
}

bool board_place(Board *b, int x, int y, TetrominoType type, size_t rotation, char mark)
{
    if (!b)
        return false;

    // get 4 cells
    Cell cells[4];
    size_t cell_num = tetro_get_cells(type, rotation, cells);
    if (cell_num == 0)
        return false;

    // check if can place
    for (size_t i = 0; i < cell_num; ++i)
    {
        int cell_x = x + cells[i].x;
        int cell_y = y + cells[i].y;
        if (!board_in_bounds(b, cell_x, cell_y) || !board_is_empty(b, cell_x, cell_y))
            return false;
    }

    // update board state
    for (size_t i = 0; i < cell_num; ++i)
    {
        int cell_x = x + cells[i].x;
        int cell_y = y + cells[i].y;
        b->state[state_idx(b->W, cell_x, cell_y)] = mark;
    }

    return true;
}

void board_remove(Board *b, int x, int y, TetrominoType type, size_t rotation)
{
    if (!b)
        return;

    // get 4 cells
    Cell cells[4];
    size_t cell_num = tetro_get_cells(type, rotation, cells);
    if (cell_num == 0)
        return;

    // update board state
    for (size_t i = 0; i < cell_num; ++i)
    {
        int cell_x = x + cells[i].x;
        int cell_y = y + cells[i].y;
        b->state[state_idx(b->W, cell_x, cell_y)] = EMPTY_MARK;
    }
}

void board_print(const Board *b, FILE *out)
{
    if (!b || !out)
        return;

    int W = b->W;
    int H = b->H;
    for (int row = 0; row < H; ++row)
    {
        for (int col = 0; col < W; ++col)
            fputc(b->state[state_idx(W, col, row)], out);
        fputc('\n', out);
    }
}

void board_clear(Board *b)
{
    if (!b)
        return;

    // clear state
    memset(b->state, EMPTY_MARK, (size_t)board_cell_count(b));
}