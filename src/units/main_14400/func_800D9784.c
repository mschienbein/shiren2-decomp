#include "common.h"

typedef unsigned char u8;

typedef struct {
    s32 field0;
    void *vtable4;
    u8 pad8[0xBC];
    s32 flagC4;
} Obj800D9784;

extern u8 D_80158068[];
extern void *func_800DDAD0(Obj800D9784 *obj, s32 arg1);
extern void func_800DDC0C(Obj800D9784 *obj, u8 *data, s32 length);

Obj800D9784 *func_800D9784(Obj800D9784 *obj, u8 *data) {
    s32 length;

    func_800DDAD0(obj, 8);
    obj->vtable4 = D_80158068;
    length = *data++;
    obj->flagC4 = *data != 0;
    func_800DDC0C(obj, data + 1, length - 2);
    return obj;
}
