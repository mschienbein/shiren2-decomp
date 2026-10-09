#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef float f32;

/* Original base vtable D_80153AA0 (rodata: adjustment words and function pointers),
 * modeled as one opaque whole object; only its address is stored here. */
typedef struct VTable VTable;

typedef struct {
    u8 pad0[0x8];
    const VTable *vtbl;
} Obj8011AEEC;

extern const VTable D_80153AA0;
extern void func_800AC68C(void *a);

/* Destructor in slot 1 of D_8015E690 (GCC 2.x in-charge flags). */
void func_8011AEEC(Obj8011AEEC *self, s32 flags) {
    self->vtbl = &D_80153AA0;
    if (flags & 1) {
        func_800AC68C(self);
    }
}
