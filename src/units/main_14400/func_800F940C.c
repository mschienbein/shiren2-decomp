#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x24];
    const void *vtable24;
} Obj800F940C;

extern const unsigned char D_80159E98[188];
extern void func_800EFD28(Obj800F940C *obj, s32 flags);
extern void func_800A3918(void *ptr);

void func_800F940C(Obj800F940C *obj, s32 flags) {
    obj->vtable24 = D_80159E98;
    func_800EFD28(obj, 0);
    if (flags & 1) {
        func_800A3918(obj);
    }
}
