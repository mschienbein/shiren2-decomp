#include "common.h"
/* Original base vtable D_80153AA0 (rodata: adjustment words and function pointers),
 * modeled as one opaque whole object; only its address is stored here. */
typedef struct VTable VTable;
typedef struct { unsigned char pad0[8]; const VTable *field_8; } Obj;
extern const VTable D_80153AA0;
extern void func_800AC68C(void *a);
void func_8011A8A8(Obj *obj, s32 flags) {
    obj->field_8 = &D_80153AA0;
    if (flags & 1) func_800AC68C(obj);
}
