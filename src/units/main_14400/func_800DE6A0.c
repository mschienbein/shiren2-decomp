#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 data[8];
} Elem_800DE6A0;

typedef struct {
    u8 pad0[4];
    void *vtable_4;
    u8 pad8[8];
    s32 unk_10;
} Child_800DE6A0;

typedef struct {
    u8 pad0[0x4];
    void *vtable_4;
    Elem_800DE6A0 elems_8[21];
    Child_800DE6A0 sub_B0;
} Obj_800DE6A0;

extern u8 D_80157FA8[];
extern void func_800D03C4(Child_800DE6A0 *sub, s32 flags);
extern void func_800D8FE8(Obj_800DE6A0 *obj);

void func_800DE6A0(Obj_800DE6A0 *obj, s32 flags) {
    Elem_800DE6A0 *begin;
    Elem_800DE6A0 *cur;

    func_800D03C4(&obj->sub_B0, 2);
    begin = obj->elems_8;
    if (begin != 0) {
        cur = begin + 21;
        while (begin != cur) {
            cur--;
        }
    }
    obj->vtable_4 = D_80157FA8;
    if (flags & 1) {
        func_800D8FE8(obj);
    }
}
