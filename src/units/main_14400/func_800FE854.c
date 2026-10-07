#include "common.h"

typedef struct {
    char pad0[0x24];
    void *vtable;
} Obj800FE854;

extern char D_8015AAC0[];
extern void func_800EFD28(Obj800FE854 *obj, s32 flags);
extern void func_800A3918(Obj800FE854 *obj);

void func_800FE854(Obj800FE854 *obj, s32 flags) {
    obj->vtable = D_8015AAC0;
    func_800EFD28(obj, 0);
    if (flags & 1) {
        func_800A3918(obj);
    }
}
