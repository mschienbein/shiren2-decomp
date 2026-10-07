#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

typedef struct { u8 pad0[0x50]; s16 field_50; u8 pad52[0x8]; s16 field_5A; u8 pad5C[0x4]; } Obj80055D44;
extern Obj80055D44 D_801D40DC[];
void func_800556D4(s32 index);
void func_80055D44(Obj80055D44 *obj) {
    obj->field_50 = 0;
    obj->field_5A = 0;
    func_800556D4(obj - D_801D40DC);
}
