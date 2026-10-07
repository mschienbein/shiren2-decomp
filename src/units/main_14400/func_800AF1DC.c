#include "common.h"

typedef short s16;
typedef unsigned short u16;
typedef unsigned char u8;
typedef struct Obj Obj;
typedef struct {
    s16 delta;
    s16 index;
    s32 (*func)(void *self, s32 kind);
} VEntry;
struct Obj {
    u8 kind;
    u8 pad1[7];
    VEntry *vtbl;
};
extern s32 func_800A6EE0(void *obj);
extern void *func_800B31E8(void *pos, s32 team);

u16 func_800AF1DC(Obj *o, void *a1, void *a2, s32 *out) {
    u16 flags = 0;
    s32 mode;

    if (o->vtbl[3].func((u8 *)o + o->vtbl[3].delta, 0x21)) {
        flags = 0x30;
    }
    if (a1 != 0) {
        flags |= func_800A6EE0(a1);
    }
    if (func_800B31E8(a2, 2)) {
        flags |= 0x30;
    }
    mode = 10;
    if (o->kind == 10) {
        mode = 3;
    }
    *out = mode;
    return flags;
}
