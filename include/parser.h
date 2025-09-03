#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "types.h"

/* 输入格式：
 * 第一行：两个正整数，棋盘宽 高，例如：
 *   8 5
 * 其后若干行：可用 tetromino 清单（大小写不敏感），支持两种且可混用：
 *   1) 单个字母：I O T S Z J L ；每个记号表示 1 块。
 *   2) 计数记法：I*3 t*2 等；'*' 后为正整数。
 * 行内从 '#' 起至行尾为注释；空行忽略；读至 EOF 结束。
 * 解析不会判断是否可铺满（如面积不整除仍视为有效输入）。
 * 输出：width/height 与 TetrominoBag（统计 total）。无效输入返回 STATUS_ERR_INPUT。 */
StatusCode parse_from_stream(FILE *in, int *out_w, int *out_h, TetrominoBag *out_bag);

/* 打印用法说明到给定流；应简要展示上述格式。 */
void parser_print_usage(FILE *out);

#endif /* PARSER_H */