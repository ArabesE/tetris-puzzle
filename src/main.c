#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "types.h"
#include "parser.h"
#include "board.h"
#include "solver.h"

/* 程序入口：
 * - 原型：int main(int argc, char** argv);
 * - 输入：若提供 argv[1]，从该文件读取；否则从 stdin 读取。
 * - 流程：解析 → 创建棋盘 → 调用 solver → 输出 → 按 StatusCode 作为进程退出码返回。
 * - 输出：
 *     成功：将解通过 board_print 输出到 stdout；返回 0。
 *     无解：输出 "No solution\n" 到 stdout；返回 3。
 *     输入错误/资源错误等：将简要错误信息与用法说明输出到 stderr；分别返回相应错误码。 */

int main(int argc, char **argv)
{
    // open input
    FILE *input;
    if (argc > 1)
    {
        input = fopen(argv[1], "r");
        if (!input)
        {
            fprintf(stderr, "Error: cannot open '%s': %s\n", argv[1], strerror(errno));
            parser_print_usage(stderr);
            return STATUS_ERR_INPUT;
        }
    }
    else
    {
        input = stdin;
    }

    // parse
    int w, h;
    TetrominoBag bag;
    StatusCode res = parse_from_stream(input, &w, &h, &bag);
    fclose(input);
    if (res != STATUS_OK)
    {
        fprintf(stderr, "Error: failed parsing '%s'\n", argc > 1 ? argv[1] : "stdin");
        parser_print_usage(stderr);
        return res;
    }

    // prepare for solving
    Board *board = board_create(w, h);
    if (!board)
    {
        fprintf(stderr, "Error: failed creating board\n");
        return STATUS_ERR_MEMORY;
    }
    size_t inout_count = bag.total;
    size_t alloc_count = inout_count > 0 ? inout_count : 1;
    Placement *list = (Placement *)malloc(alloc_count * PLACEMENT_SIZE);
    if (!list)
    {
        fprintf(stderr, "Error: failed allocating placements\n");
        board_destroy(board);
        return STATUS_ERR_MEMORY;
    }
    memset(list, 0, alloc_count * PLACEMENT_SIZE);

    // solve
    res = solver_solve(board, &bag, list, &inout_count);
    switch (res)
    {
    case STATUS_OK:
        board_print(board, stdout);
        break;
    case STATUS_ERR_UNSOLVABLE:
        fprintf(stdout, "No solution\n");
        break;
    case STATUS_ERR_INVALID_ARGUMENT:
        fprintf(stderr, "Error: invalid argument while solving\n");
        break;
    case STATUS_ERR_MEMORY:
        fprintf(stderr, "Error: memory error while solving\n");
        break;
    case STATUS_ERR_INPUT:
        fprintf(stderr, "Error: input error while solving\n");
        break;
    default:
        fprintf(stderr, "Error: unknown error while solving\n");
        break;
    }

    // exit
    free(list);
    board_destroy(board);
    return res;
}