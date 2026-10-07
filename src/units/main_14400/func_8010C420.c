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

extern unsigned char D_8015CECC[];
extern void func_800AF174(unsigned char *self, Obj *obj);
extern void func_800CA4E8(Obj *obj, void *arg1);

void func_8010C420(unsigned char *self, Obj *obj) {
    func_800AF174(self, obj);
    func_800CA4E8(obj, D_8015CECC);
    obj->vtable[5].func((unsigned char *)obj + obj->vtable[5].delta, 0x14, self + 0xC);
}
