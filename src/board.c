#include <stdlib.h>
#include <string.h>
#include "board.h"

/* 在此文件中实现棋盘的内部数据结构与全部接口：
 *   - 构造/析构与尺寸查询；
 *   - 边界/占用判断；
 *   - 放置/移除给定形状；
 *   - 统计与打印。 */

#define STACK_MAX_SIZE 173
#define BOARD_SIZE sizeof(Board)
#define PLACEMENT_STACK_SIZE sizeof(Placement[STACK_MAX_SIZE])
#define EMPTY_MARK '.'

struct Board
{
    int W;
    int H;
    char *state;
    Placement placement_stack[STACK_MAX_SIZE];
    int placement_stack_top;
};

static inline size_t state_idx(int W, int x, int y)
{
    return y * W + x;
}

Board *board_create(int width, int height)
{
    if (width <= 0 || height <= 0)
        return NULL;

    Board *b = malloc(BOARD_SIZE);
    size_t state_size = width * height;
    char *state = malloc(state_size);
    if (!b || !state)
        return NULL;

    memset(state, EMPTY_MARK, state_size);
    b->W = width;
    b->H = height;
    b->state = state;
    memset(b->placement_stack, 0, PLACEMENT_STACK_SIZE);
    b->placement_stack_top = -1;
    return b;
}

void board_destroy(Board *b)
{
    free(b->state);
    free(b);
}

int board_width(const Board *b)
{
    return b->W;
}

int board_height(const Board *b)
{
    return b->H;
}

int board_cell_count(const Board *b)
{
    return b->W * b->H;
}

int board_filled_count(const Board *b)
{
    int W = b->W;
    int H = b->H;
    int count = 0;
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
    return x >= 0 && x < b->W && y >= 0 && y < b->H;
}

bool board_is_empty(const Board *b, int x, int y)
{
    return b->state[state_idx(b->W, x, y)] == EMPTY_MARK;
}

bool board_place(Board *b, int x, int y, TetrominoType type, int rotation, char mark)
{
    // check placement stack
    if (b->placement_stack_top == STACK_MAX_SIZE - 1)
        return false;

    // get 4 cells
    Cell cells[4];
    size_t cell_num = tetro_get_cells(type, rotation, cells);
    if (cell_num == 0)
        return false;

    // check if can place
    for (size_t i = 0; i < cell_num; ++i)
    {
        int cell_x = cells[i].x;
        int cell_y = cells[i].y;
        if (!board_in_bounds(b, cell_x, cell_y) || !board_is_empty(b, cell_x, cell_y))
            return false;
    }

    // update board state
    for (size_t i = 0; i < cell_num; ++i)
    {
        int cell_x = cells[i].x;
        int cell_y = cells[i].y;
        b->state[state_idx(b->W, cell_x, cell_y)] = mark;
    }

    // push placement
    Placement *placement = (Placement *)&b->placement_stack[++(b->placement_stack_top)];
    placement->type = type;
    placement->rotation = rotation;
    placement->x = x;
    placement->y = y;
    placement->mark = mark;

    return true;
}

void board_remove(Board *b, int x, int y, TetrominoType type, int rotation)
{
    int stack_top = b->placement_stack_top;

    // check placement stack
    if (stack_top < 0)
        return;

    // check if matches previous placement
    Placement *prev_placement = &b->placement_stack[stack_top];
    if (prev_placement->x != x || prev_placement->y != y || prev_placement->type != type || prev_placement->rotation != rotation)
    {
        return;
    }

    // get 4 cells
    Cell cells[4];
    size_t cell_num = tetro_get_cells(type, rotation, cells);
    if (cell_num == 0)
        return;

    // check if mark mataches. if not, return
    char prev_mark = prev_placement->mark;
    for (size_t i = 0; i < cell_num; ++i)
    {
        int cell_x = cells[i].x;
        int cell_y = cells[i].y;
        if (b->state[state_idx(b->W, cell_x, cell_y)] != prev_mark)
            return;
    }

    // update board state
    for (size_t i = 0; i < cell_num; ++i)
    {
        int cell_x = cells[i].x;
        int cell_y = cells[i].y;
        b->state[state_idx(b->W, cell_x, cell_y)] = EMPTY_MARK;
    }

    // pop placement
    b->placement_stack_top--;
}

void board_print(const Board *b, FILE *out)
{
    if (out == NULL)
    {
        return;
    }

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
    // clear state
    memset(b->state, EMPTY_MARK, (size_t)board_cell_count(b));

    // clear placement stack
    b->placement_stack_top = -1;
}