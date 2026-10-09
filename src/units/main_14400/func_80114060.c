#include "common.h"

typedef unsigned char u8;

typedef struct Obj80114060 Obj80114060;

/* 0x1C-byte list child at +0xC owned by the object: func_800CFF00 installs its
 * tables at +0x0/+0x4, storage state through +0x17 and the owner word at +0x18. */
typedef struct {
    u8 data[0x1C];
} Child80114060;

struct Obj80114060 {
    u8 pad0[0x8];
    void *vtable;
    Child80114060 child;
    u8 x28;
    u8 x29;
};

extern u8 D_8015D938[];

void *func_800AC0C0(Obj80114060 *self, s32 a, s32 b);
Child80114060 *func_800CFF00(Child80114060 *self, Obj80114060 *owner, u8 count);
void func_801141B0(Obj80114060 *obj);

/* Constructor: base object of type 9, this vtable, embedded child, then refresh. */
Obj80114060 *func_80114060(Obj80114060 *obj, s32 kind)
{
    func_800AC0C0(obj, 9, kind);
    obj->vtable = D_8015D938;
    func_800CFF00(&obj->child, obj, 8);
    obj->x29 = 0;
    obj->x28 = 1;
    func_801141B0(obj);
    return obj;
}
