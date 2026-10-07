#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad0[0x28]; s16 delta; u8 pad2A[2]; s32 (*get)(void *self); } VTable800C5634;
typedef struct { void *field_0; u8 pad4[8]; VTable800C5634 *vtable; } Obj800C5634;
void *func_80032D94(void *dst, const void *src, u32 size);
void func_800C5634(Obj800C5634 *obj, void *dst) {
    s32 value = obj->vtable->get((u8 *)obj + obj->vtable->delta);
    func_80032D94(dst, obj->field_0, value);
}
