#include "common.h"

typedef unsigned char u8;

typedef short s16;
typedef unsigned short u16;
typedef struct {
    s16 delta;
    s16 index;
    u32 (*func)(void *self); /* slot 13 target for this class: u32 func_800E0E88(Obj *) */
} VTableEntry;

typedef struct Message {
    void *field_0;
    s32 field_4;
    s32 field_8;
    s16 field_C;
    u16 field_E;
    u8 field_10;
} Message;

typedef struct {
    u8 pad0[0x24];
    VTableEntry *vtable24;
    u8 pad28[0x61];
    u8 bonus89;
} Obj800FB59C;

extern s32 func_800F1024(Obj800FB59C *obj);
extern s32 func_800E1CC4(Obj800FB59C *obj, s32 arg1);
extern s32 func_800E33D0(void *obj, Message attack, u8 arg2, u8 arg3);

s32 func_800FB59C(Obj800FB59C *obj) {
    Message attack;
    Message copy;
    Message *p = &attack;
    VTableEntry *entry = &obj->vtable24[13];
    s32 failed;

    p->field_C = (s16)entry->func((u8 *)obj + entry->delta);
    p->field_E = 0;
    p->field_8 = 0;
    p->field_10 = 10;
    if (func_800F1024(obj) != 0) {
        s32 bonus = obj->bonus89;

        attack.field_C += attack.field_C * bonus / 100;
        attack.field_E |= 0x80;
        failed = func_800E1CC4(obj, 5) != 1;
        if (failed) {
            attack.field_E |= 1;
        }
    }
    copy = attack;
    func_800E33D0(obj, copy, 1, 1);
    return 1;
}
