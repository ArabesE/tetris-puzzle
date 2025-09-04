#ifndef BOARD_H
#define BOARD_H

#include <stdio.h>
#include "types.h"
#include "tetromino.h"

/* The board is an opaque type; its internal representation is hidden.
 * Coordinates/anchor follow tetromino.h: origin (0,0) at top-left; x→right, y→down.
 * Placing at (x,y) aligns the shape's anchor at the current rotation to board (x,y). */

typedef struct Board Board;

/* Create and destroy (width,height must be > 0). */
Board *board_create(int width, int height);
void board_destroy(Board *b);

/* Basic properties */
int board_width(const Board *b);
int board_height(const Board *b);
size_t board_cell_count(const Board *b);   /* width * height */
size_t board_filled_count(const Board *b); /* currently filled cell count */

/* Bounds and occupancy checks */
bool board_in_bounds(const Board *b, int x, int y);
bool board_is_empty(const Board *b, int x, int y);

/* Placement/removal:
 * - board_place: if the 4 target cells are in bounds and empty, fill them with mark and return true; otherwise return false and do not modify.
 * - board_remove: precondition—must match a prior successful placement (x,y,type,rotation,mark) exactly; otherwise behavior is undefined. */
bool board_place(Board *b, int x, int y, TetrominoType type, size_t rotation, char mark);
void board_remove(Board *b, int x, int y, TetrominoType type, size_t rotation);

/* Text visualization: print exactly height lines of width chars; '.' for empty, piece mark for filled; each line ends with '\n'. */
void board_print(const Board *b, FILE *out);

/* Clear the board content, keep dimensions, reset all cells to '.'. */
void board_clear(Board *b);

#endif /* BOARD_H */