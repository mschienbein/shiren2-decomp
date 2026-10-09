#include "common.h"

typedef union {
    u32 all;
    struct {
        u32 pad0 : 8;
        u32 bit23 : 1;
        u32 pad9 : 15;
        u32 bit7 : 1;
        u32 pad25 : 7;
    } bits;
} Flags;

typedef struct {
    unsigned char pad0[0x20];
    Flags flags;
    unsigned char pad24[0xE8 - 0x24];
    u32 extra_flags;
} Obj;

void func_800EA254(Obj *obj);
unsigned char func_800A9958(void);
void func_800C94E8(void);

void func_800ECC74(Obj *obj)
{
    Flags before;
    Flags after;
    Flags *prev;
    Flags *next;
    s32 changed;
    u32 extra;
    unsigned char mode;

    prev = &before;
    next = &after;
    before = obj->flags;
    func_800EA254(obj);
    extra = obj->extra_flags;
    obj->flags.all |= extra;
    mode = func_800A9958();
    changed = 0;
    if (mode == 6) {
        obj->flags.all |= 0x800000;
    }
    after = obj->flags;
    if (prev->bits.bit23 != next->bits.bit23) {
        changed = 1;
    } else if (prev->bits.bit7 != next->bits.bit7) {
        changed = 1;
    }
    if (changed) {
        func_800C94E8();
    }
}
