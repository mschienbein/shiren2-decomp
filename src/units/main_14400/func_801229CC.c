#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef float f32;

typedef struct Obj Obj;

extern void func_8011414C(Obj *self, s32 flags);
extern void func_800AC68C(void *a);

/* Destructor in slot 1 of D_8015F990 (GCC 2.x in-charge flags). */
void func_801229CC(Obj *self, s32 flags) {
    func_8011414C(self, 0);
    if (flags & 1) {
        func_800AC68C(self);
    }
}
