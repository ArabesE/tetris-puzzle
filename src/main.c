#include <stdio.h>
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
 *     无解：输出 "No solution
" 到 stdout；返回 3。
 *     输入错误/资源错误等：将简要错误信息与用法说明输出到 stderr；分别返回相应错误码。 */
