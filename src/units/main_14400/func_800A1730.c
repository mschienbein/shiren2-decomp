#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

typedef struct { u8 kind; u8 id; } Obj800A1730;
u8 D_80142920[0x10] = { 0 };
s32 func_800A1630(u8 *flags, u8 id);
s32 func_800A1730(Obj800A1730 *obj) {
    if (obj->kind != 3 && obj->kind != 4) {
        return 0;
    }
    return func_800A1630(D_80142920, obj->id) ^ 1;
}
