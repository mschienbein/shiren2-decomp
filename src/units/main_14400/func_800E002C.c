#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef float f32;

typedef struct {
    u8 pad0[0x8];
    u8 kind;
} Obj800E002C;

extern u16 D_80158BF4[];
extern void func_800498E4(s32 id, ...);
extern s32 func_80049CB4(s32 id, ...);
extern s32 func_8006D44C(s32 a, s32 b);

/* Vtable slot 2 of D_80158C40. */
s32 func_800E002C(Obj800E002C *obj) {
    if (obj->kind < 0x25 && D_80158BF4[obj->kind] != 0x53D) {
        func_800498E4(D_80158BF4[obj->kind]);
        func_80049CB4(2);
        func_8006D44C(5, 0x3D0900);
        func_80049CB4(0x138);
        func_80049CB4(2);
    }
    return 1;
}
