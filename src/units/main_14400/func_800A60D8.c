#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    s32 x;
    s32 y;
} Pos;

extern u8 D_8014344C;
Pos *func_800C5F60(void);
s32 func_800A5E24(void *obj, Pos *pos, s32 arg2, s32 arg3);
s32 func_800A5FCC(void *obj, Pos *pos);
static inline void copy_pos(Pos *dst, Pos *src) { dst->x = src->x; dst->y = src->y; }
s32 func_800A60D8(void *obj, Pos *out) {
    Pos pos;
    copy_pos(&pos, func_800C5F60());
    if (D_8014344C >= 2 && func_800A5E24(obj, &pos, 0, 1)) {
        *out = pos;
        return 1;
    }
    if (func_800A5E24(obj, &pos, 1, 1) || func_800A5FCC(obj, &pos)) {
        *out = pos;
        return 1;
    }
    return 0;
}
