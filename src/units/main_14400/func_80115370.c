#include "common.h"

typedef struct {
    short delta;
    short index;
    void (*func)(void *self, s32 arg1, void *arg2);
} VtableEntry;

typedef struct {
    unsigned char pad0[0x18];
    VtableEntry *vtable;
} Obj;

extern unsigned char D_8015D934[];
extern void func_800AF174(unsigned char *self, Obj *obj);
extern void func_800CA4E8(Obj *obj, void *arg1);
extern void func_800D00E0(unsigned char *arg0, Obj *obj);
extern void func_801153FC(unsigned char *self);

void func_80115370(unsigned char *self, Obj *obj) {
    func_800AF174(self, obj);
    func_800CA4E8(obj, D_8015D934);
    func_800D00E0(self + 0xC, obj);
    obj->vtable[5].func((unsigned char *)obj + obj->vtable[5].delta, 1, self + 0x28);
    obj->vtable[5].func((unsigned char *)obj + obj->vtable[5].delta, 1, self + 0x29);
    func_801153FC(self);
}
