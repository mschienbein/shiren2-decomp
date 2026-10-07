#include "common.h"

extern char D_80157FA8[];
typedef struct Collection Collection;
void func_800D03C4(Collection *, s32);
void func_800D8FE8(void *);
typedef struct { s32 unk0; s32 unk4; } Elem;
/* func_800D03B4 initializes the element pointer, capacity and count. */
struct Collection { s32 unk0; void *unk4; Elem *elems8; s32 capacityC; s32 count10; };
typedef struct { char pad0[4]; void *unk4; Elem elems[21]; Collection unkB0; } Obj;
void func_800DEB34(Obj *obj, s32 flags) {
    func_800D03C4(&obj->unkB0, 2);
    if (obj->elems != 0) {
        Elem *p = obj->elems + 21;
        while (obj->elems != p) {
            p--;
        }
    }
    obj->unk4 = D_80157FA8;
    if (flags & 1) {
        func_800D8FE8(obj);
    }
}
