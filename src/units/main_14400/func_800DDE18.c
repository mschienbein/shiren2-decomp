#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct {
    u8 pad0[0x20];
    s16 delta_20;
    s16 pad22;
    s32 (*func_24)(void *self);
} VTable800DDE18;

typedef struct {
    u8 pad0[0x4];
    VTable800DDE18 *vtable_4;
} Sub800DDE18;

typedef struct {
    u8 pad0;
    u8 id_1;
    u8 pad2[0xAE];
    Sub800DDE18 sub_B0;
} Obj800DDE18;

void func_800DDB64(Obj800DDE18 *obj, u8 *out, s32 count);

s32 func_800DDE18(Obj800DDE18 *obj, u8 *out) {
    Sub800DDE18 *sub = &obj->sub_B0;
    s32 count = sub->vtable_4->func_24((u8 *)sub + sub->vtable_4->delta_20);

    *out++ = obj->id_1;
    *out++ = count + 1;
    func_800DDB64(obj, out, count);
    return count + 3;
}
