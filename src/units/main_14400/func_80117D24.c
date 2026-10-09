#include "common.h"
/* Original base vtable D_80153AA0 (rodata: adjustment words and function pointers),
 * modeled as one opaque whole object; only its address is stored here. */
typedef struct VTable VTable;
typedef struct { s32 field_00[2]; const VTable *field_08; } Object;
extern const VTable D_80153AA0;
extern void func_800AC68C(void *);
void func_80117D24(Object *self, s32 flags) {
    self->field_08 = &D_80153AA0;
    if (flags & 1) func_800AC68C(self);
}
