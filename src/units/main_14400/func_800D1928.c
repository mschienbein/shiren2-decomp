#include "common.h"

typedef struct { char pad00[0x34]; unsigned char cells[15]; } Board;
extern s32 func_800D1A70(Board *board, s32 row, signed char *out);
extern s32 func_800D18A8(signed char *columns);

s32 func_800D1928(void *board, s32 row)
{
    signed char columns[5];
    func_800D1A70(board, row, columns);
    return func_800D18A8(columns);
}
