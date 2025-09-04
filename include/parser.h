#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "types.h"

/* Input format:
 * First line: two positive integers <width> <height>, e.g.,
 *   8 5
 * Following lines: tetromino list (case-insensitive). Two token forms, can be mixed:
 *   1) Single letter: I O T S Z J L; each adds one piece.
 *   2) Counted: I*3 t*2; '*' followed by a positive integer.
 * From '#' to end of line is a comment. Blank lines ignored. Read until EOF.
 * Parsing does not judge tileability (e.g., non-divisible area is still valid input).
 * Output: width/height and TetrominoBag (total aggregated). On invalid input returns STATUS_ERR_INPUT. */
StatusCode parse_from_stream(FILE *in, int *out_w, int *out_h, TetrominoBag *out_bag);

/* Print usage help to the given stream; should briefly show the format above. */
void parser_print_usage(FILE *out);

#endif /* PARSER_H */