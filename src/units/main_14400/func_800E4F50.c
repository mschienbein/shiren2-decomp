#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

/* Slot +0x6C targets func_800E0E88 (D_80158C98 family) and func_800E96D4
 * (D_80158E80/D_80159008), both returning u32; the caller narrows to s16. */
typedef struct VirtualEntry {
    s16 delta;
    s16 index;
    u32 (*func)(void *self);
} VirtualEntry;

typedef struct VTable800E4F50 {
    u8 pad_00[0x68];
    VirtualEntry getValue_68;
} VTable800E4F50;

typedef struct Object {
    u8 pad_00[0x8];
    u8 dir_08;
    u8 pad_09[0x24 - 0x9];
    VTable800E4F50 *vtable_24;
} Object;

typedef struct Message {
    void *field_0;
    s32 field_4;
    s32 field_8;
    s16 field_C;
    u16 field_E;
    u8 field_10;
} Message;

extern s32 func_800E1CC4(Object *obj, s32 kind);
void func_800A665C(Object *obj, u8 *value);
s32 func_800E33D0(Object *arg, Message message, u8 rays, u8 range);

s32 func_800E4F50(Object *obj) {
    Message message;
    Message copy;
    Message *init;
    s16 value;

    if (func_800E1CC4(obj, 4) != 0) {
        u8 dir = (obj->dir_08 + 4) & 7;

        func_800A665C(obj, &dir);
    }
    value = obj->vtable_24->getValue_68.func((u8 *)obj + obj->vtable_24->getValue_68.delta);
    init = &message;
    init->field_C = value;
    init->field_10 = 10;
    init->field_E = 0;
    init->field_8 = 0;
    copy = message;
    func_800E33D0(obj, copy, 1, 1);
    return 1;
}
