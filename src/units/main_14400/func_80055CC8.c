#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

typedef struct { u8 pad0[0x17]; u8 field_17; u8 pad18[0x38]; s16 field_50; u8 pad52[0x6]; s16 field_58; s16 field_5A; } Obj80055CC8;
void func_80055CC8(Obj80055CC8 *obj) {
    s32 alpha = 255 - (s32)((f32)++obj->field_5A / (f32)obj->field_58 * 255.0f);
    obj->field_17 = alpha;
    if ((alpha & 0xFF) == 0) {
        obj->field_50 = 4;
        obj->field_5A = 0;
    }
}
