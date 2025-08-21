#ifndef TYPES_H
#define TYPES_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define PROJECT_NAME "tetromino_solver"

/* 通用返回码：用于区分正常、输入问题、资源问题与无解等情况。
 * 决定：这些枚举值同时作为程序的进程退出码使用；main 必须原样返回。 */
typedef enum StatusCode
{
    STATUS_OK = 0,
    STATUS_ERR_INPUT = 1,
    STATUS_ERR_MEMORY = 2,
    STATUS_ERR_UNSOLVABLE = 3,
    STATUS_ERR_INVALID_ARGUMENT = 4
} StatusCode;

/* 平面上的一个格子偏移（或绝对坐标） */
typedef struct Cell
{
    int x;
    int y;
} Cell;

/* 7 种标准 tetromino 形状类型 */
typedef enum TetrominoType
{
    TETRO_I = 0,
    TETRO_O,
    TETRO_T,
    TETRO_S,
    TETRO_Z,
    TETRO_J,
    TETRO_L,
    TETRO_TYPE_COUNT
} TetrominoType;

/* 待放置/已放置的一块积木的信息 */
typedef struct Placement
{
    TetrominoType type; /* 形状类型 */
    int rotation;       /* 旋转编号，从 0 开始，范围由具体形状决定 */
    int x;              /* 在棋盘上的锚点位置（见 tetromino.h 坐标/锚点约定） */
    int y;
    char mark; /* 输出字符标记，如 'A'..'Z'（按 solver 约定分配） */
} Placement;

/* 供选择的积木袋：记录每种形状的数量及总数 */
typedef struct TetrominoBag
{
    size_t counts[TETRO_TYPE_COUNT];
    size_t total; /* 所有积木总数 */
} TetrominoBag;

#endif /* TYPES_H */