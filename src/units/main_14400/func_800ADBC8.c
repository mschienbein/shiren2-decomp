#include "common.h"
typedef unsigned char u8;
typedef signed short s16;
typedef struct { s32 x, y; } Pair;
typedef Pair Pos;
typedef Pair Pos800AC6F8;
typedef struct { u8 pad0[8]; s16 adjust8; s16 padA; void (*destroyC)(void *, s32); } Methods;
typedef struct { u8 pad0[8]; const Methods *methods8; } Actor;
typedef Actor Item800AC6F8;
extern s32 func_800ADD50(Actor *, Pos *);
extern s32 func_800ADA68(void *, void *, Pair *, s32);
extern s32 func_800AD8AC(Item800AC6F8 *item, Pos800AC6F8 *dest);
extern s32 func_80049CB4(s32 id, ...);
s32 func_800ADBC8(void *arg, void *owner, Pair *pos, s32 check, s32 mode) {
    Actor *item = arg;
    s32 result;
    if (func_800ADD50(item, pos)) {
        result = 1;
        if (check) result = func_800ADA68(item, owner, pos, mode) ^ 1;
        if (result) {
            result = func_800AD8AC(item, pos);
            func_80049CB4(0xD7, pos);
            return result;
        }
    } else if (item) {
        item->methods8->destroyC((u8 *)item + item->methods8->adjust8, 3);
    }
    return 0;
}
