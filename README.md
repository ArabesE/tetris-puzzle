# Tetromino Rectangle Tiling Solver

This is a small C11 program that solves the classic “tetromino tiling” puzzle: given a rectangular board and a multiset of the 7 standard Tetris tetrominoes (I, O, T, S, Z, J, L), it attempts to tile the board exactly with those pieces. If a solution is found, it prints a text visualization; otherwise it reports that there is no solution.

## Input format

Read from a file (first CLI argument) or stdin when no argument is provided. The format is line-oriented and case-insensitive for piece names:

- First non-empty, non-comment line: two positive integers `<width> <height>`.
- Following lines: a whitespace-separated list of tetromino tokens until EOF. Tokens can be either:
  - Single piece letter: `I O T S Z J L` (each adds 1 piece), or
  - Counted form: `I*3  t*2` (`*` followed by a positive integer, no spaces inside).
- From `#` to end-of-line is a comment. Blank lines are ignored.

Examples:

```
8 5
I*2 O*2 T*2 S Z J L
```

```
4 1
i
```

Notes:

- The parser validates the syntax but does not enforce tileability beyond area matching; an unsatisfiable but syntactically valid input will result in “No solution”.

## Output

- On success, prints `height` lines of `width` characters. Each placed tetromino is marked with a letter `A..Z` in placement order (wrapping every 26 pieces).
- If unsolvable, prints `No solution` and exits with code 3.
- On input or resource errors, prints a short message to stderr and exits with a non-zero code.

## Build

Requirements: a C11 compiler (e.g., gcc/clang) and make.

Targets:

- `make` or `make debug`: debug build (`-O0 -g3`).
- `make release`: optimized build (`-O2 -DNDEBUG`).
- `make release-strip`: optimized and stripped binary (smallest size).
- `make test`: run the included sample tests in `tests/`.
- `make clean`: remove build artifacts.

Optional sanitizers:

```
make SAN=address      # AddressSanitizer
make SAN=undefined    # UBSan
make SAN=thread       # ThreadSanitizer
```

## Run

From a file:

```
./bin/tetromino_solver tests/cases/solvable_4x1_I.in
```

From stdin:

```
./bin/tetromino_solver < tests/cases/solvable_2x2_O.in
```

## Design overview

- `include/` headers specify strict contracts for board coordinates, rotations, and solver IO.
- `src/parser.c` parses the input with robust error handling and overflow checks.
- `src/board.c` implements an opaque board with safe bounds checks and a flexible-array state.
- `src/tetromino.c` provides canonical rotations per shape.
- `src/solver.c` uses an iterative backtracking stack; it tracks filled cells to avoid repeated full scans.

## Tests

See `tests/cases/*` for solvable, unsolvable, malformed, and stress examples. Run all with `make test`.

## License

MIT License — see `LICENSE` for full text. © 2025 Loren Bian.
