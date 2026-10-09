#include "common.h"
typedef unsigned char u8;
/* Original base vtable D_80153AA0 (rodata: adjustment words and function pointers),
 * modeled as one opaque whole object; only its address is stored here. */
typedef struct VTable VTable;
typedef struct { u8 pad_0[8]; const VTable *field_8; } Object;
extern const VTable D_80153AA0;
extern void func_800AC68C(void *a);
void func_80118C50(Object *object, s32 flags)
{
    object->field_8 = &D_80153AA0;
    if (flags & 1) {
        func_800AC68C(object);
    }
}
