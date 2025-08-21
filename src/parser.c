#include "parser.h"
#include "tetromino.h"

/* 在此文件中实现输入解析：
 *   - 读取宽高；
 *   - 逐个解析形状记号与可选计数；
 *   - 支持注释与空白；
 *   - 填充 TetrominoBag 结构并进行基本一致性校验。 */