#ifndef TETROMINO_H
#define TETROMINO_H

#include "types.h"

#define CANDS_MAX_SIZE 19

/* 坐标与旋转的统一约定：
 * - 棋盘坐标系：原点 (0,0) 在左上角；x 向右递增，y 向下递增。
 * - 形状锚点：对任意形状在任意旋转下，其 4 个格子的最小包围框的左上角作为锚点 (0,0)。
 * - tetro_get_cells 返回的是相对锚点的 4 个偏移，满足每个偏移的 x,y 均为非负。
 * - 旋转编号与数量：
 *     O:1；I/S/Z:2；T/J/L:4。编号 k 表示将“旋转 0”顺时针旋转 k×90°后，再把坐标平移使最小 x,y 归零。
 * - 各形状“旋转 0”的规范单元坐标（相对锚点）：
 *     I0: {(0,0),(1,0),(2,0),(3,0)}
 *     O0: {(0,0),(1,0),(0,1),(1,1)}
 *     T0: {(0,0),(1,0),(2,0),(1,1)}
 *     S0: {(1,0),(2,0),(0,1),(1,1)}
 *     Z0: {(0,0),(1,0),(1,1),(2,1)}
 *     J0: {(0,0),(0,1),(1,1),(2,1)}
 *     L0: {(2,0),(0,1),(1,1),(2,1)}
 */

/* 形状类型名称与解析：
 * - 名称是稳定的一字符大写字符串："I","O","T","S","Z","J","L"。
 * - 从记号解析形状类型；供解析的记号应为长度 1 的字母，大小写不敏感；无效输入返回 TETRO_TYPE_COUNT */
const char *tetro_type_name(TetrominoType type);
TetrominoType tetro_type_from_token(const char *token);

/* 返回某形状的合法旋转数量（参见上面的固定规定） */
size_t tetro_rotation_count(TetrominoType type);

/* 获取指定形状在指定旋转下的 4 个相对格子坐标。
 * 要求：out_cells 长度至少为 4；返回实际写入的格子数（固定为 4）。
 * 坐标系/锚点/旋转定义严格遵循本文件顶部的“统一约定”。 */
size_t tetro_get_cells(TetrominoType type, size_t rotation, Cell out_cells[4]);

#endif /* TETROMINO_H */