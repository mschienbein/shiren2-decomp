#include "common.h"

typedef short s16;
typedef unsigned char u8;
typedef struct {
    s16 delta;
    s16 pad;
    void (*fn)(void *, s32, void *);
} VEntry;
typedef struct {
    u8 pad[0x28];
    VEntry entry28;
} VTable;
typedef struct {
    u8 pad[0x18];
    VTable *vtable;
} Obj;
extern u8 D_80158DF8[];
void func_800E032C(u8 *, Obj *);
void func_800CA4E8(Obj *, void *);

void func_800E9614(u8 *self, Obj *obj)
{
    func_800E032C(self, obj);
    func_800CA4E8(obj, D_80158DF8);
    obj->vtable->entry28.fn((u8 *)obj + obj->vtable->entry28.delta, 8, self + 0x78);
}
