#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef float f32;

typedef struct Obj Obj;

extern void *func_800AC5B4(s32 size, s32 alternate);
extern Obj *func_801209F0(Obj *obj);

/* Item factory slot of D_80157BD0 (no arguments, like func_80121788). */
void *func_80120A28(void) {
    return func_801209F0(func_800AC5B4(0x2C, 0));
}
