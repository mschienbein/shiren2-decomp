#include "common.h"
typedef unsigned char u8;
typedef struct { s32 a, b; } Buf;
typedef struct { char pad[2]; u8 flags; } Item;
typedef struct { void *table; char pad4[4]; signed char mode; } S;
extern void *D_80154824;
extern s32 func_800D4A10(S *);
extern void *func_800D4A60(void *owner, s32 key);
extern Buf *func_800D4B60(Buf *, S *, s32);
extern void func_800AD7E0(Item *, Buf *, s32);
void func_800D4CE4(S *p, signed char mode) {
    s32 i;
    short mask = ~0x20;
    p->mode = mode;
    if (mode == 0) {
        p->table = D_80154824;
        i = func_800D4A10(p) - 1;
        for (;;) {
            Item *it;
            if (i < 0) break;
            it = func_800D4A60(p, i);
            if (it != 0) {
                Buf buf;
                func_800D4B60(&buf, p, i);
                func_800AD7E0(it, &buf, 1);
                it->flags &= mask;
            }
            i--;
        }
    }
}
