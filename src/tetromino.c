#include <ctype.h>
#include <string.h>
#include "tetromino.h"

/* Implement functions declared in tetromino.h for tetromino utilities.
 * Includes:
 *   - Name and token parsing for 7 shapes.
 *   - Relative coordinates for each rotation.
 *   - Consistency of coordinates across the project. */

const char *tetro_type_name(TetrominoType type)
{
    switch (type)
    {
    case TETRO_I:
        return "I";
    case TETRO_O:
        return "O";
    case TETRO_T:
        return "T";
    case TETRO_S:
        return "S";
    case TETRO_Z:
        return "Z";
    case TETRO_J:
        return "J";
    case TETRO_L:
        return "L";
    case TETRO_TYPE_COUNT:
        return "";
    default:
        return "";
    }
}

TetrominoType tetro_type_from_token(const char *token)
{
    if (!token || token[0] == '\0' || !isalpha((unsigned char)token[0]) || token[1] != '\0')
        return TETRO_TYPE_COUNT;
    switch (toupper((unsigned char)token[0]))
    {
    case 'I':
        return TETRO_I;
    case 'O':
        return TETRO_O;
    case 'T':
        return TETRO_T;
    case 'S':
        return TETRO_S;
    case 'Z':
        return TETRO_Z;
    case 'J':
        return TETRO_J;
    case 'L':
        return TETRO_L;
    default:
        return TETRO_TYPE_COUNT;
    }
}

size_t tetro_rotation_count(TetrominoType type)
{
    switch (type)
    {
    case TETRO_O:
        return 1;
    case TETRO_I:
    case TETRO_S:
    case TETRO_Z:
        return 2;
    case TETRO_T:
    case TETRO_J:
    case TETRO_L:
        return 4;
    case TETRO_TYPE_COUNT:
        return 0;
    default:
        return 0;
    }
}

// assume rotation is valid for type
size_t tetro_get_cells(TetrominoType type, size_t rotation, Cell out_cells[4])
{
    switch (type)
    {
    case TETRO_I:
        switch (rotation)
        {
        case 0:
            memcpy(
                out_cells,
                (Cell[]){{0, 0}, {1, 0}, {2, 0}, {3, 0}},
                sizeof(Cell[4]));
            return 4;
        case 1:
            memcpy(
                out_cells,
                (Cell[]){{0, 0}, {0, 1}, {0, 2}, {0, 3}},
                sizeof(Cell[4]));
            return 4;
        default:
            return 0;
        }
        break;
    case TETRO_O:
        switch (rotation)
        {
        case 0:
            memcpy(
                out_cells,
                (Cell[]){{0, 0}, {1, 0}, {0, 1}, {1, 1}},
                sizeof(Cell[4]));
            return 4;
        default:
            return 0;
        }
        break;
    case TETRO_T:
        switch (rotation)
        {
        case 0:
            memcpy(
                out_cells,
                (Cell[]){{0, 0}, {1, 0}, {2, 0}, {1, 1}},
                sizeof(Cell[4]));
            return 4;
        case 1:
            memcpy(
                out_cells,
                (Cell[]){{1, 0}, {0, 1}, {1, 1}, {1, 2}},
                sizeof(Cell[4]));
            return 4;
        case 2:
            memcpy(
                out_cells,
                (Cell[]){{1, 0}, {0, 1}, {1, 1}, {2, 1}},
                sizeof(Cell[4]));
            return 4;
        case 3:
            memcpy(
                out_cells,
                (Cell[]){{0, 0}, {0, 1}, {1, 1}, {0, 2}},
                sizeof(Cell[4]));
            return 4;
        default:
            return 0;
        }
        break;
    case TETRO_S:
        switch (rotation)
        {
        case 0:
            memcpy(
                out_cells,
                (Cell[]){{1, 0}, {2, 0}, {0, 1}, {1, 1}},
                sizeof(Cell[4]));
            return 4;
        case 1:
            memcpy(
                out_cells,
                (Cell[]){{0, 0}, {0, 1}, {1, 1}, {1, 2}},
                sizeof(Cell[4]));
            return 4;
        default:
            return 0;
        }
        break;
    case TETRO_Z:
        switch (rotation)
        {
        case 0:
            memcpy(
                out_cells,
                (Cell[]){{0, 0}, {1, 0}, {1, 1}, {2, 1}},
                sizeof(Cell[4]));
            return 4;
        case 1:
            memcpy(
                out_cells,
                (Cell[]){{1, 0}, {0, 1}, {1, 1}, {0, 2}},
                sizeof(Cell[4]));
            return 4;
        default:
            return 0;
        }
        break;
    case TETRO_J:
        switch (rotation)
        {
        case 0:
            memcpy(
                out_cells,
                (Cell[]){{0, 0}, {0, 1}, {1, 1}, {2, 1}},
                sizeof(Cell[4]));
            return 4;
        case 1:
            memcpy(
                out_cells,
                (Cell[]){{0, 0}, {1, 0}, {0, 1}, {0, 2}},
                sizeof(Cell[4]));
            return 4;
        case 2:
            memcpy(
                out_cells,
                (Cell[]){{0, 0}, {1, 0}, {2, 0}, {2, 1}},
                sizeof(Cell[4]));
            return 4;
        case 3:
            memcpy(
                out_cells,
                (Cell[]){{1, 0}, {1, 1}, {0, 2}, {1, 2}},
                sizeof(Cell[4]));
            return 4;
        default:
            return 0;
        }
        break;
    case TETRO_L:
        switch (rotation)
        {
        case 0:
            memcpy(
                out_cells,
                (Cell[]){{2, 0}, {0, 1}, {1, 1}, {2, 1}},
                sizeof(Cell[4]));
            return 4;
        case 1:
            memcpy(
                out_cells,
                (Cell[]){{0, 0}, {0, 1}, {0, 2}, {1, 2}},
                sizeof(Cell[4]));
            return 4;
        case 2:
            memcpy(
                out_cells,
                (Cell[]){{0, 0}, {1, 0}, {2, 0}, {0, 1}},
                sizeof(Cell[4]));
            return 4;
        case 3:
            memcpy(
                out_cells,
                (Cell[]){{0, 0}, {1, 0}, {1, 1}, {1, 2}},
                sizeof(Cell[4]));
            return 4;
        default:
            return 0;
        }
        break;
    case TETRO_TYPE_COUNT:
        return 0;
    default:
        return 0;
    }
}