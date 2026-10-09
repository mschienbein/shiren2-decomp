#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x; s32 y; } Pos;
typedef struct { char data[0x18]; } Slot;
extern s32 D_80143444;
extern u16 D_8014767C;
extern u8 D_80143391;
extern Pos *D_801476B8;
extern Pos D_80143388;
static inline s32 cached_x(Pos *p) { return p->x; }
static inline s32 cached_y(Pos *p) { return p->y; }
extern u8 D_80143448;
extern Slot D_80143330[];
s32 func_800A251C(Pos *, Pos *);
s32 func_800D35CC(Slot *, Pos *, Pos *);
void func_800B5D54(void) {
    s32 blocked = 0;
    Pos pos;
    Pos *p;
    s32 i;
    if (D_80143444 == 0) {
        blocked = 1;
    } else if (D_8014767C & 0xC) {
        blocked = 1;
    } else if (D_80143391 & 4) {
        blocked = 1;
    } else if (D_80143391 & 8) {
        blocked = 1;
    }
    if (blocked) {
        return;
    }
    p = &pos;
    p->x = D_801476B8->x;
    p->y = D_801476B8->y;
    if (func_800A251C(p, &D_80143388) != 0) {
        return;
    }
    if ((cached_y(&D_80143388) | cached_x(&D_80143388)) == 0) {
        D_80143388 = pos;
        return;
    }
    for (i = 0; i < D_80143448; i++) {
        if (func_800D35CC(&D_80143330[i], &D_80143388, &pos) != 0) {
            break;
        }
    }
    D_80143388 = pos;
}
