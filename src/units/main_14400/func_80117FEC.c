#include "common.h"

/* Original base vtable D_80153AA0 (rodata: adjustment words and function pointers),
 * modeled as one opaque whole object; only its address is stored here. */
typedef struct VTable VTable;
typedef struct { s32 unknown00[2]; const VTable *field08; } Object;
extern const VTable D_80153AA0;
extern void func_800AC68C(Object *);
/* Destructor bound at D_8015DDE8+0xC (destroy slot): void (void *self, s32 flags). */
void func_80117FEC(Object *object, s32 flags) {
    object->field08 = &D_80153AA0;
    if (flags & 1) func_800AC68C(object);
}
