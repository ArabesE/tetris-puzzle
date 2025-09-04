#ifndef TETROMINO_H
#define TETROMINO_H

#include "types.h"

#define CANDS_MAX_SIZE 19 // (O=1; I/S/Z: 2 each; T/J/L: 4 each)

/* Unified coordinates/rotation contract:
 * - Board coordinates: origin (0,0) at top-left; x increases to the right; y increases downward.
 * - Shape anchor: for any shape/rotation, the top-left of the shape's 4-cell bounding box is the anchor (0,0).
 * - tetro_get_cells returns four offsets relative to the anchor; each offset has non-negative x,y.
 * - Rotation indices and counts:
 *     O:1; I/S/Z:2; T/J/L:4. Index k means rotate rotation-0 clockwise by k×90°, then translate so min x,y == 0.
 * - Canonical unit coordinates (relative to anchor) for rotation 0:
 *     I0: {(0,0),(1,0),(2,0),(3,0)}
 *     O0: {(0,0),(1,0),(0,1),(1,1)}
 *     T0: {(0,0),(1,0),(2,0),(1,1)}
 *     S0: {(1,0),(2,0),(0,1),(1,1)}
 *     Z0: {(0,0),(1,0),(1,1),(2,1)}
 *     J0: {(0,0),(0,1),(1,1),(2,1)}
 *     L0: {(2,0),(0,1),(1,1),(2,1)}
 */

/* Shape type naming and parsing:
 * - Names are one-letter uppercase strings: "I","O","T","S","Z","J","L".
 * - Parse a single-letter token (case-insensitive); invalid returns TETRO_TYPE_COUNT. */
const char *tetro_type_name(TetrominoType type);
TetrominoType tetro_type_from_token(const char *token);

/* Return the number of legal rotations for a shape (see the rules above). */
size_t tetro_rotation_count(TetrominoType type);

/* Get the 4 relative cell coordinates for a shape at a given rotation.
 * Requirement: out_cells length >= 4; returns the number of cells written (always 4).
 * Coordinates/anchor/rotation follow the contract above. */
size_t tetro_get_cells(TetrominoType type, size_t rotation, Cell out_cells[4]);

#endif /* TETROMINO_H */