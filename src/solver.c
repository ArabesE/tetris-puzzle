#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "solver.h"
#include "tetromino.h"

/* Solver implementation:
 *   - Organize try order and intermediate state for backtracking.
 *   - Return the first full tiling via out_list.
 *   - Return UNSOLVABLE when coverage cannot be achieved. */

#define SOLVER_FRAME_SIZE sizeof(SolverFrame)
#define SOLVER_STACK_SIZE sizeof(SolverStack)
#define MARK_NUM 26 // 26 letters from A-Z

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
    // guard against overflow in allocation size
    if (max_frame_num > 0 && max_frame_num > (SIZE_MAX - SOLVER_STACK_SIZE) / SOLVER_FRAME_SIZE)
        return NULL;
    size_t bytes = SOLVER_STACK_SIZE + max_frame_num * SOLVER_FRAME_SIZE;
    SolverStack *stack = calloc(1, bytes);
    if (!stack)
        return NULL;
    stack->capacity = max_frame_num;
    stack->top = -1;
    return stack;
}

// destroy stack
static inline void destroy_stack(SolverStack *stack)
{
    if (!stack)
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
    return stack->top == (int)stack->capacity - 1;
}

// undo the placement represented by the current top frame (if any)
static inline bool undo_top_placement(Board *board,
                                      SolverStack *stack,
                                      size_t counts[TETRO_TYPE_COUNT],
                                      size_t *filled_cells)
{
    if (stack_is_empty(stack))
        return false;
    SolverFrame *frame = stack->frames + stack->top;
    if (frame->idx < 0)
        return false; // nothing placed at this frame yet
    Placement *prev_placement = frame->cands + frame->idx;
    board_remove(board, prev_placement->x, prev_placement->y, prev_placement->type, prev_placement->rotation);
    counts[prev_placement->type]++;
    if (*filled_cells >= 4)
        *filled_cells -= 4;
    return true;
}

// push a new frame to stack
static inline StatusCode push_frame(size_t cands_size, Placement *cands, SolverStack *stack)
{
    if (!cands)
        return STATUS_ERR_MEMORY;

    if (stack_is_full(stack))
        return STATUS_ERR_MEMORY;

    stack->top++;
    SolverFrame *frame = stack->frames + stack->top;
    frame->cands_size = cands_size;
    frame->idx = -1;
    memcpy(frame->cands, cands, cands_size * PLACEMENT_SIZE);
    return STATUS_OK;
}

// pop a frame from stack, return top frame after popping
static inline SolverFrame *pop_frame(SolverStack *stack)
{
    if (!stack)
        return NULL;
    if (stack->top == -1)
        return NULL; // already empty
    stack->top--;
    if (stack->top == -1)
        return NULL;                   // no previous frame
    return stack->frames + stack->top; // new top (previous frame)
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
                if (idx < CANDS_MAX_SIZE)
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
    }
    return idx;
}

// frame has remaining candidates or not
static inline bool has_candidate(SolverFrame *frame)
{
    return frame->idx + 1 < (int)frame->cands_size;
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
    if (!board || !bag || !out_list || !inout_count)
        return STATUS_ERR_INVALID_ARGUMENT;

    size_t bag_total = bag->total;
    size_t counts[TETRO_TYPE_COUNT];
    memcpy(counts, bag->counts, sizeof(counts));
    // overflow guard for computing target cells
    if (bag_total > SIZE_MAX / 4)
        return STATUS_ERR_INVALID_ARGUMENT;
    // index guard for internal stack indexing (top is int)
    if (bag_total > (size_t)INT_MAX)
        return STATUS_ERR_INVALID_ARGUMENT;
    size_t board_cells = board_cell_count(board);
    size_t target_cells = bag_total * 4;
    if (target_cells != board_cells)
    {
        // piece and board cell numbers do not match, unsolvable
        *inout_count = 0;
        return STATUS_ERR_UNSOLVABLE;
    }
    // compute initial filled cells once
    size_t filled_cells = board_filled_count(board);

    // create stack
    SolverStack *stack = create_stack(bag_total);
    if (!stack)
        return STATUS_ERR_MEMORY;

    // iterative backtracking
    bool need_new_frame = true;
    while (true)
    {
        // 1) terminate, solved
        if (filled_cells == target_cells)
        {
            // solved
            *inout_count = bag_total;
            // write to out_list
            for (size_t i = 0; i < bag_total; ++i)
            {
                SolverFrame *frame = stack->frames + i;
                size_t cand_idx = (size_t)(frame->idx);
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
            {
                // No candidates for next cell: undo the placement at current top frame
                if (!undo_top_placement(board, stack, counts, &filled_cells))
                {
                    // nothing to undo => unsolvable
                    *inout_count = 0;
                    board_clear(board);
                    destroy_stack(stack);
                    return STATUS_ERR_UNSOLVABLE;
                }
                need_new_frame = false;
                continue;
            }
            StatusCode res = push_frame(cands_size, cands, stack);
            if (res != STATUS_OK)
            {
                destroy_stack(stack);
                return res;
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
                filled_cells += 4;
                need_new_frame = true;
            }
            continue;
        }

        // 4) current frame exhausted, backtrack
        SolverFrame *prev_frame = pop_frame(stack);
        if (prev_frame != NULL)
        {
            // child frame removed; undo placement at new top (the parent)
            if (!undo_top_placement(board, stack, counts, &filled_cells))
            {
                *inout_count = 0;
                board_clear(board);
                destroy_stack(stack);
                return STATUS_ERR_UNSOLVABLE;
            }
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
