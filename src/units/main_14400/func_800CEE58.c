#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad0[0x2]; u8 flags; } Item800CEE58;
typedef struct { u8 pad0[0x60]; s16 delta; s16 pad62; void (*fn)(void *); } VtblB800CEE58;
typedef struct { u8 pad0[0x1E]; u8 flags; u8 pad1F[0x5]; VtblB800CEE58 *vtable; } Owner800CEE58;
typedef struct { u8 pad0[0x38]; s16 delta; s16 pad3A; void *(*fn)(void *, u32); } VtblA800CEE58;
typedef struct { s32 unk0; VtblA800CEE58 *vtable; u8 pad8[0x8]; Owner800CEE58 *owner;
    s32 usable; } Obj800CEE58;
extern u32 D_8013960C;
s32 func_800AC670(Item800CEE58 *item);
void func_800AE518(Item800CEE58 *item, Owner800CEE58 *owner, s32 arg2, s32 arg3);
void func_800CE81C(Obj800CEE58 *obj, s32 arg1);
void func_800CEE58(Obj800CEE58 *obj, s32 arg1) {
    VtblA800CEE58 *vt = obj->vtable;
    Item800CEE58 *item = vt->fn((u8 *)obj + vt->delta, (u32)arg1);
    Owner800CEE58 *owner;
    s32 usable;
    if (item != 0 && (usable = func_800AC670(item) ^ 1)) {
        if (item->flags & 4) {
            D_8013960C *= 2;
            func_800AE518(item, obj->owner, 0, 0);
            D_8013960C /= 2;
        }
    }
    func_800CE81C(obj, arg1);
    owner = obj->owner;
    if (owner->flags & 0xC) {
        VtblB800CEE58 *vt2 = owner->vtable;
        vt2->fn((u8 *)owner + vt2->delta);
    }
}
