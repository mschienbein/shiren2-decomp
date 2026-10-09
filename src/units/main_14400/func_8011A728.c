#include "common.h"

typedef struct VTable VTable;

typedef struct {
    u32 opaque_00;
    u32 opaque_04;
    const VTable *vtable_08;
} Obj8011A728;

/* Original base vtable D_80153AA0 (rodata: adjustment words and function pointers),
 * modeled as one opaque whole object; only its address is stored here. */
extern const VTable D_80153AA0;

void func_800AC68C(void *a);

void func_8011A728(Obj8011A728 *obj, s32 flags) {
    obj->vtable_08 = &D_80153AA0;
    if (flags & 1) {
        func_800AC68C(obj);
    }
}
