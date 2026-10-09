#include "common.h"
typedef struct { s32 x; s32 y; } Pos;
typedef struct { unsigned char pad0[0xC]; unsigned char field_C; signed char field_D; signed char field_E; } Item801169D0;
extern void *func_800B4D80(Pos *p);
extern s32 func_80049CB4(s32 id, ...);
void func_80115780(Item801169D0 *item) {
    Pos pos;
    if ((item->field_C >> 4) & 1) {
        Pos *p = &pos;
        s32 x = item->field_D;
        s32 y = item->field_E;
        p->x = x;
        p->y = y;
        if (func_800B4D80(p) == item) func_80049CB4(0x107, p);
    }
}
