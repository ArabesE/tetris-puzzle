#ifndef BOARD_H
#define BOARD_H

#include <stdio.h>
#include "types.h"
#include "tetromino.h"

/* 棋盘为不透明类型，内部表示对外隐藏。坐标系与锚点约定与 tetromino.h 保持一致：
 * 原点在左上角，x→右，y→下；放置位置 (x,y) 表示将该形状在当前旋转下的锚点对齐到棋盘的 (x,y)。 */

typedef struct Board Board;

/* 创建与销毁（width,height 必须 > 0） */
Board *board_create(int width, int height);
void board_destroy(Board *b);

/* 基础属性查询 */
int board_width(const Board *b);
int board_height(const Board *b);
size_t board_cell_count(const Board *b);   /* width * height */
size_t board_filled_count(const Board *b); /* 当前已占用格子数 */

/* 边界与占用检查 */
bool board_in_bounds(const Board *b, int x, int y);
bool board_is_empty(const Board *b, int x, int y);

/* 放置与移除：
 * - board_place：若 4 个目标格均在界内且为空，则以给定 mark 填入并返回 true；否则不修改并返回 false。
 * - board_remove：前置条件——必须与先前一次成功放置的 (x,y,type,rotation,mark) 完全一致；否则行为未定义。 */
bool board_place(Board *b, int x, int y, TetrominoType type, size_t rotation, char mark);
void board_remove(Board *b, int x, int y, TetrominoType type, size_t rotation);

/* 文本可视化：精确输出 height 行、每行 width 个字符；空格子用 '.'，占用格子输出其 mark；每行以 '\n' 结尾。 */
void board_print(const Board *b, FILE *out);

/* 清空棋盘内容，保留尺寸，所有格子复位为 '.' */
void board_clear(Board *b);

#endif /* BOARD_H */