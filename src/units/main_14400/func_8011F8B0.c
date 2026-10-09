#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef float f32;

extern void func_800A7B18(void *target, void *source, s32 amount, s32 kind);

/* Trap apply slot +0x54 supplies five pointers; self, direction and attacker are unused. */
void func_8011F8B0(void *self, void *source, void *target, void *direction, void *attacker) {
    func_800A7B18(target, source, 10, 6);
}
