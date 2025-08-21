#include "solver.h"
#include "tetromino.h"

/* 在此文件中实现求解逻辑：
 *   - 按需要组织尝试顺序与中间状态；
 *   - 在找到第一组完整覆盖后通过 out_list 返回；
 *   - 无法覆盖时返回无解状态。 */