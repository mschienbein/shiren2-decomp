#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad0[0x28]; s16 delta; s16 pad2A; s32 (*fn)(void *); } Vtbl800C5684;
typedef struct { void *unk0; u8 pad4[0x8]; Vtbl800C5684 *vtable; } Obj800C5684;
void *func_80032D94(void *dst, const void *src, u32 size);
void func_800C5684(Obj800C5684 *obj, void *arg1) {
    Vtbl800C5684 *vt = obj->vtable;
    func_80032D94(obj->unk0, arg1, vt->fn((u8 *)obj + vt->delta));
}
