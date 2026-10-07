#include "common.h"
typedef struct { s32 field_0; s32 field_4; } Elem800DEE38;
/* Collection member prefix: its destructor writes the vtable and the +0x10 word. */
typedef struct { s32 opaque_0; void *vtable; s32 opaque_8[2]; s32 field_10; } Collection800DEE38;
typedef struct { s32 field_0; void *vtable; Elem800DEE38 elems[21]; Collection800DEE38 field_B0; } Obj800DEE38;
extern s32 D_80157FA8;
void func_800D03C4(void *member, s32 mode);
void func_800D8FE8(void *obj);
void func_800DEE38(Obj800DEE38 *obj, s32 flags) {
    func_800D03C4(&obj->field_B0, 2);
    if (obj->elems != 0) {
        Elem800DEE38 *e = &obj->elems[21];
        while (obj->elems != e) e--;
    }
    obj->vtable = &D_80157FA8;
    if (flags & 1) func_800D8FE8(obj);
}
