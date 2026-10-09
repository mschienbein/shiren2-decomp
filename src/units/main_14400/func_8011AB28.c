#include "common.h"

/* Original base vtable D_80153AA0 (rodata: adjustment words and function pointers),
 * modeled as one opaque whole object; only its address is stored here. */
typedef struct VTable VTable;

extern const VTable D_80153AA0;
extern void func_800AC68C(void *a);

typedef struct {
    char pad0[8];
    const VTable *field_8;
} Obj_8011AB28;

void func_8011AB28(Obj_8011AB28 *obj, s32 flags) {
    obj->field_8 = &D_80153AA0;
    if (flags & 1) {
        func_800AC68C(obj);
    }
}
