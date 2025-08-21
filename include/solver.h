#ifndef SOLVER_H
#define SOLVER_H

#include <stddef.h>
#include "types.h"
#include "board.h"

/* 求解接口：
 * - out_list 的容量必须 >= bag->total；调用前 *inout_count 需设为该容量；
 * - 成功（STATUS_OK）：
 *     *inout_count 写为 bag->total；board 被填充为该解；
 *     out_list[i].mark 由求解器按顺序分配为 'A'+(i%26)。
 * - 无解（STATUS_ERR_UNSOLVABLE）：
 *     *inout_count 写为 0；board 被清空（与调用前一致）。
 * - 其他错误码：不修改 board 与 out_list 的已写入部分（如有）。 */
StatusCode solver_solve(Board *board,
                        const TetrominoBag *bag,
                        Placement *out_list,
                        size_t *inout_count);

#endif /* SOLVER_H */