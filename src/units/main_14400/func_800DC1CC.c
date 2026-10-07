#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;
typedef float f32;

typedef struct { void *field_0; void *field_4; } Entry800D01B8;
typedef struct { s32 field_0; void *vtable; u8 pad8[0x8]; Entry800D01B8 field_10; } Obj800DC1CC;
extern u8 D_80158598[];
Obj800DC1CC *func_800DA904(Obj800DC1CC *obj, s32 kind, u8 *params);
Entry800D01B8 *func_800D0180(Entry800D01B8 *sub);
void func_800DAA58(u8 id, Entry800D01B8 *sub);
Obj800DC1CC *func_800DC1CC(Obj800DC1CC *obj, u8 *params) {
    func_800DA904(obj, 0x16, params++);
    obj->vtable = D_80158598;
    func_800D0180(&obj->field_10);
    func_800DAA58(*params, &obj->field_10);
    return obj;
}
