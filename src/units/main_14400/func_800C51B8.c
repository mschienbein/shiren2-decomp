#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x; s32 y; } Pos;
typedef Pos Point;
typedef struct { u8 pad0[0xC]; s32 limitC; s32 index10; u8 pad14[8]; s32 remaining1C; u16 field20; u16 mask22; } Iterator;
extern void *func_800C2758(void *out, void *iterator);
extern u32 func_800B1C6C(Pos *pos);
extern void *func_800B4928(Point *pos);
/* ODD_C: Preserve the mask's halfword mode across the inline flag operation. */
static inline u16 blocked(u16 mask, Pos *pos) {
    mask &= func_800B1C6C(pos);
    return mask;
}
void *func_800C51B8(Iterator *iterator) {
    for (;;) {
        Pos position;
        Pos *pos;
        void *object;
        u16 mask;
        if (iterator->index10 >= iterator->limitC) break;
        func_800C2758(&position, iterator);
        mask = iterator->mask22;
        pos = &position;
        mask = blocked(mask, pos);
        if (mask) break;
        object = func_800B4928(pos);
        if (object) return object;
    }
    iterator->remaining1C = 0;
    return 0;
}
