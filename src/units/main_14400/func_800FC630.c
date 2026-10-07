#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct { s32 x; s32 y; } Pos800FC630;

typedef struct {
    u8 pad0[0x68];
    s16 delta_68;
    s16 pad6A;
    u32 (*func_6C)(void *self); /* slot 13: u32 func_800E0E88(Obj *) */
} VTable800FC630;

typedef struct {
    u8 pad0[0x24];
    VTable800FC630 *vtable_24;
    u8 pad28[0x30];
    Pos800FC630 *target_58;
    u8 pad5C[0x2D];
    u8 range_89;
} Obj800FC630;

void *func_800F13A0(void *out, void *obj, s32 range);
s32 func_800F1244(void *obj, void *target, s32 range, s32 a3);
s32 func_800A65B8(Obj800FC630 *obj, Pos800FC630 *target);
s32 func_800A692C(Obj800FC630 *obj, s32 kind);
void func_800A56D8(void *obj, void *pos, s16 dir, u16 a3);

/* Monster slot +0xB4 act(self, target): the target supplied by the call contract is unused here. */
s32 func_800FC630(Obj800FC630 *obj, void *target_unused) {
    Pos800FC630 pos;
    Pos800FC630 *target;
    s32 ok;

    func_800F13A0(&pos, obj, obj->range_89);
    switch (func_800F1244(obj, obj->target_58, obj->range_89, 0)) {
    case 1:
        return 0;
    case 2:
        target = obj->target_58;
        ok = 0;
        if (target != 0 && func_800A65B8(obj, target) <= obj->range_89) {
            ok = func_800A692C(obj, 0x12) == 0;
        }
        if (!ok) {
            return 1;
        }
        pos = *target;
        break;
    }
    func_800A56D8(obj, &pos, (s16)obj->vtable_24->func_6C((u8 *)obj + obj->vtable_24->delta_68), 0);
    return 1;
}
