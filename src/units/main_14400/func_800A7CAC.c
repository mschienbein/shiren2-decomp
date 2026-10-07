#include "common.h"

typedef unsigned char u8;

typedef short s16;
typedef struct {
    s16 delta;
    s16 index;
    void *func;
} VTableEntry;

typedef struct {
    u8 pad0[0x18];
    VTableEntry *vtable18;
} Obj800A7CAC;

extern char D_801535DC[];
extern void func_800CA4E8(Obj800A7CAC *obj, char *name);

void func_800A7CAC(void *arg0, Obj800A7CAC *obj) {
    VTableEntry *entry;

    func_800CA4E8(obj, D_801535DC);
    entry = &obj->vtable18[5];
    ((void (*)(void *, s32, void *))entry->func)((u8 *)obj + entry->delta, 0xC, arg0);
}
