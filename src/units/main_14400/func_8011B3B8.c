#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    u8 pad0[0x8];
    void *vtable_08;
} Obj8011B3B8;

/* Initialized original vtable; its full type is unresolved. */
extern u8 D_80153AA0[];

extern void func_800AC68C(void *a);

void func_8011B3B8(Obj8011B3B8 *obj, s32 flags)
{
    obj->vtable_08 = D_80153AA0;
    if (flags & 1) {
        func_800AC68C(obj);
    }
}
