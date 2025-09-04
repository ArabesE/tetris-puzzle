#ifndef TYPES_H
#define TYPES_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define PROJECT_NAME "tetromino_solver"

#define INT_SIZE sizeof(int)
#define SIZE_T_SIZE sizeof(size_t)
#define CELL_SIZE sizeof(Cell)
#define PLACEMENT_SIZE sizeof(Placement)

/* General status codes used across the program.
 * These values are also used as the process exit codes; main should return them verbatim. */
typedef enum StatusCode
{
    STATUS_OK = 0,
    STATUS_ERR_INPUT = 1,
    STATUS_ERR_MEMORY = 2,
    STATUS_ERR_UNSOLVABLE = 3,
    STATUS_ERR_INVALID_ARGUMENT = 4
} StatusCode;

/* A single grid cell offset (or absolute board coordinate). */
typedef struct Cell
{
    int x;
    int y;
} Cell;

/* The 7 standard tetromino types. */
typedef enum TetrominoType
{
    TETRO_I = 0,
    TETRO_O,
    TETRO_T,
    TETRO_S,
    TETRO_Z,
    TETRO_J,
    TETRO_L,
    TETRO_TYPE_COUNT
} TetrominoType;

/* A placement (to be tried or already placed) on the board. */
typedef struct Placement
{
    TetrominoType type; /* tetromino type */
    size_t rotation;    /* rotation index starting at 0; valid range depends on the shape */
    int x;              /* anchor position on the board (see coordinate/anchor contract in tetromino.h) */
    int y;
    char mark; /* output mark used to distinguish pieces, e.g., 'A'..'Z' */
} Placement;

/* Bag of available tetrominoes: counts per type and total. */
typedef struct TetrominoBag
{
    size_t counts[TETRO_TYPE_COUNT];
    size_t total; /* total number of pieces across all types */
} TetrominoBag;

#endif /* TYPES_H */