#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

typedef struct { u8 field_0; u8 field_1; u8 pad2[0xA]; u8 field_C; u8 field_D; } Obj800AE444;
void *func_800AC5F4(s32 size, Obj800AE444 *obj);
void *func_80128C40(void *obj);
void func_800AE444(Obj800AE444 *obj, u8 value) {
    u8 saved = obj->field_1;
    func_80128C40(func_800AC5F4(0x10, obj));
    obj->field_D = value;
    obj->field_C = saved;
}
