#include <stdlib.h>
#include <string.h>
#include "solver.h"
#include "tetromino.h"

/* 在此文件中实现求解逻辑：
 *   - 按需要组织尝试顺序与中间状态；
 *   - 在找到第一组完整覆盖后通过 out_list 返回；
 *   - 无法覆盖时返回无解状态。 */

#define SOLVER_FRAME_SIZE sizeof(SolverFrame)
#define SOLVER_STACK_SIZE (SIZE_T_SIZE + INT_SIZE)
#define MARK_NUM 26

// a frame in the solver stack
typedef struct SolverFrame
{
    size_t cands_size;
    int idx;
    Placement cands[CANDS_MAX_SIZE];
} SolverFrame;

// solver stack for backtracking
typedef struct SolverStack
{
    size_t capacity;
    int top;
    SolverFrame frames[];
} SolverStack;

// create stack
static inline SolverStack *create_stack(size_t max_frame_num)
{
    SolverStack *stack = calloc(SOLVER_STACK_SIZE + max_frame_num * SOLVER_FRAME_SIZE, 1);
    if (!stack)
        return NULL;
    stack->capacity = max_frame_num;
    stack->top = -1;
    return stack;
}

// destroy stack
static inline void destroy_stack(SolverStack *stack)
{
    if (stack == NULL)
        return;
    free(stack);
}

// stack empty or not
static inline bool stack_is_empty(SolverStack *stack)
{
    return stack->top == -1;
}

// stack full or not
static inline bool stack_is_full(SolverStack *stack)
{
    return stack->top == stack->capacity - 1;
}

// push a new frame to stack
static inline int push_frame(size_t cands_size, Placement *cands, SolverStack *stack)
{
    if (cands == NULL)
        return -1;

    if (stack_is_full(stack))
        return -1;

    stack->top++;
    SolverFrame *frame = stack->frames + stack->top;
    frame->cands_size = cands_size;
    frame->idx = -1;
    memcpy(frame->cands, cands, cands_size * PLACEMENT_SIZE);
    return 0;
}

// pop a frame from stack, return top frame after popping
static inline SolverFrame *pop_frame(SolverStack *stack)
{
    if (stack == NULL)
        return NULL;

    stack->top--;
    if (stack->top == -1)
        return NULL;

    return stack->frames + stack->top;
}

// generate candidate placements at cell
static inline size_t generate_candidates(int x, int y, char mark, size_t *counts, Placement *out_cands)
{
    size_t idx = 0;
    for (TetrominoType type = 0; type < TETRO_TYPE_COUNT; ++type)
    {
        if (counts[type] > 0)
        {
            size_t rotation_count = tetro_rotation_count(type);
            for (size_t rot = 0; rot < rotation_count; ++rot)
            {
                Placement *cand = out_cands + idx;
                cand->type = type;
                cand->rotation = rot;
                cand->x = x;
                cand->y = y;
                cand->mark = mark;
                idx++;
            }
        }
    }
    return idx;
}

// frame has remaining candidates or not
static inline bool has_candidate(SolverFrame *frame)
{
    return frame->idx + 1 < frame->cands_size;
}

// find next empty cell on board
static inline Cell find_next_empty(Board *board)
{
    Cell cell = {-1, -1};
    int H = board_height(board);
    int W = board_width(board);
    for (int y = 0; y < H; ++y)
    {
        for (int x = 0; x < W; ++x)
        {
            if (board_is_empty(board, x, y))
            {
                cell.x = x;
                cell.y = y;
                return cell;
            }
        }
    }
    return cell;
}

// get mark based on index in stack
static inline char mark_at(int depth)
{
    return (char)('A' + (depth % MARK_NUM));
}

// main solver function
StatusCode solver_solve(Board *board,
                        const TetrominoBag *bag,
                        Placement *out_list,
                        size_t *inout_count)
{
    if (board == NULL || bag == NULL || out_list == NULL || inout_count == NULL)
        return STATUS_ERR_INVALID_ARGUMENT;

    size_t bag_total = bag->total;
    size_t counts[TETRO_TYPE_COUNT];
    memcpy(counts, bag->counts, TETRO_TYPE_COUNT * SIZE_T_SIZE);
    int target_cells = bag_total * 4;
    if (target_cells != board_cell_count(board))
    {
        // piece and board cell numbers do not match, unsolvable
        *inout_count = 0;
        return STATUS_ERR_UNSOLVABLE;
    }

    // create stack
    SolverStack *stack = create_stack(bag_total);
    if (stack == NULL)
        return STATUS_ERR_MEMORY;

    // iterative backtracking
    bool need_new_frame = true;
    while (true)
    {
        // 1) terminate, solved
        if (board_filled_count(board) == target_cells)
        {
            // solved
            *inout_count = bag_total;
            // write to out_list
            for (size_t i = 0; i < bag_total; ++i)
            {
                SolverFrame *frame = stack->frames + i;
                size_t cand_idx = frame->idx;
                memcpy(out_list + i, frame->cands + cand_idx, PLACEMENT_SIZE);
            }
            destroy_stack(stack);
            return STATUS_OK;
        }

        // 2) need to push new frame
        if (need_new_frame)
        {
            Cell pos = find_next_empty(board);
            char mark = mark_at(stack->top + 1);
            Placement cands[CANDS_MAX_SIZE];
            size_t cands_size = generate_candidates(pos.x, pos.y, mark, counts, cands);
            if (cands_size == 0)
                continue;
            int res = push_frame(cands_size, cands, stack);
            if (res != 0)
            {
                destroy_stack(stack);
                return STATUS_ERR_MEMORY;
            }
            need_new_frame = false;
        }

        // 3) try next candidate in the current frame
        SolverFrame *top_frame = stack->frames + stack->top;
        if (has_candidate(top_frame))
        {
            top_frame->idx++;
            Placement *cand = top_frame->cands + top_frame->idx;
            if (board_place(board, cand->x, cand->y, cand->type, cand->rotation, cand->mark))
            {
                counts[cand->type]--;
                need_new_frame = true;
            }
            else
            {
                continue;
            }
        }

        // 4) current frame exhausted, backtrack
        SolverFrame *prev_frame = pop_frame(stack);
        if (prev_frame != NULL)
        {
            Placement *prev_placement = prev_frame->cands + prev_frame->idx;
            board_remove(board, prev_placement->x, prev_placement->y, prev_placement->type, prev_placement->rotation);
            counts[prev_placement->type]++;
            need_new_frame = false;
            continue;
        }

        // unsolvable
        *inout_count = 0;
        board_clear(board);
        destroy_stack(stack);
        return STATUS_ERR_UNSOLVABLE;
    }
}
