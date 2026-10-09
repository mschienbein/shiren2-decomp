#include "common.h"

typedef struct { s32 x; s32 y; } Pos;
typedef struct { unsigned char field_00[2]; unsigned char flags_02; } Item;
void *func_800B4D80(Pos *position);

/* Returns 1 when an item lies at the cell and has flag 0x20 set. The result is a full
 * int: func_8008AA20 tests it without narrowing (bnezl v0 at 0x8008AD48). */
s32 func_800421D4(s32 y, s32 x)
{
    Pos position;
    Item *item;
    s32 flag;
    s32 result;

    position.y = y;
    position.x = x;
    item = func_800B4D80(&position);
    if (item == 0) {
        result = 0;
    } else {
        flag = item->flags_02 & 0x20;
        result = flag != 0;
    }
    return result;
}
