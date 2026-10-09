#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct { u8 pad0[0x54]; u8 flags_54; } Obj800C8D78;

extern u16 D_8014767C;
s32 func_80046240(void);
void func_800A7BA4(Obj800C8D78 *obj, s32 value);

/* Original callers supply signed-short turns; this implementation ignores it. */
void func_800C8D78(Obj800C8D78 *obj, short turns) {
    s32 blocked = 0;

    if (func_80046240() != 0 || ((D_8014767C >> 6) & 1)) {
        blocked = 1;
    }
    if (!blocked && (obj->flags_54 & 2)) {
        func_800A7BA4(obj, 0xFF);
    }
}
