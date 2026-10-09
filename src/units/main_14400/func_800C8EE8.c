#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    s32 x0;
    s32 x4;
} Iter;

/* 0x18-byte message passed to the entity handler. */
typedef struct {
    s32 type_0;
    s32 pad4[5];
} Msg800C8EE8;

typedef struct {
    u8 pad0[0x10];
    s16 delta_10;
    s16 pad12;
    s32 (*func_14)(void *self);
    u8 pad18[0x58 - 0x18];
    s16 delta_58;
    s16 pad5A;
    s32 (*func_5C)(void *self, Msg800C8EE8 *msg);
} VTable800C8EE8;

typedef struct {
    u8 pad0[0x24];
    VTable800C8EE8 *vtable_24;
} Obj800C8EE8;

extern u16 D_8014767C;

s32 func_80046240(void);
s32 func_800A8FC8(Iter *it, s32 kind);
void *func_800A910C(Iter *it);

/* Send message 1 to every kind-2 entity whose slot-0x14 query is false. */
void func_800C8EE8(void)
{
    Msg800C8EE8 msg;
    Iter it;
    Iter *ip;
    Obj800C8EE8 *obj;
    s32 blocked = 0;

    if (func_80046240() != 0) {
        blocked = 1;
    } else if ((D_8014767C >> 6) & 1) {
        blocked = 1;
    }
    if (blocked) {
        return;
    }
    ip = &it;
    ip->x0 = 0;
    while (func_800A8FC8(ip, 2)) {
        obj = func_800A910C(ip);
        if (obj->vtable_24->func_14((u8 *)obj + obj->vtable_24->delta_10)) {
            continue;
        }
        msg.type_0 = 1;
        obj->vtable_24->func_5C((u8 *)obj + obj->vtable_24->delta_58, &msg);
    }
}
