#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef struct ShirenDirection { s8 value; } ShirenDirection;

typedef struct { s16 delta; s16 pad; void *func; } VEntry;
typedef struct { u8 pad0[0x24]; VEntry *vtable; } Sub;
typedef struct { s32 a; s32 b; } Pair;
typedef struct { u8 data[0x28]; } Ray;
typedef struct {
    u8 pad0[8];
    u8 field_8;
    u8 pad9[0x1B];
    VEntry *vtable;
    u8 pad28[0x30];
    Sub *field_58;
    u8 pad5C[0x2C];
    s32 field_88;
    u8 pad8C[0x34];
    s32 field_C0;
} Obj;
s32 func_800E20CC(void *obj);
void func_800E8FAC(Obj *obj);
void *func_800A6CC0(void *out_position, void *obj);
void *func_800C5150(void *self, void *position, ShirenDirection direction, s32 limit, u32 mask);
void *func_800C51B8(void *iterator);
s32 func_800A44F4(void *self, void *target);
void *func_800A65E4(void *out_direction, void *obj, void *target);
void func_800A665C(Obj *obj, Pair *pos);
s32 func_800EF210(Obj *obj);
s32 func_80109CC0(Obj *obj) {
    Ray ray;
    Pair pos;
    Pair dest;
    Sub *sub;
    Obj *target;
    s32 idle;

    if (func_800E20CC(obj) != 0) {
        func_800E8FAC(obj);
        return 1;
    }
    if (obj->field_88 == 0) {
        return 0;
    }
    sub = obj->field_58;
    if (obj->field_C0 != 0 && sub != 0) {
        idle = ((s32 (*)(void *))sub->vtable[2].func)((u8 *)sub + sub->vtable[2].delta) ^ 1;
        if (idle) {
            ShirenDirection facing;

            func_800A6CC0(&pos, obj);
            facing.value = obj->field_8;
            func_800C5150(&ray, &pos, facing, 0xFF, 0x4000);
            target = func_800C51B8(&ray);
            if (target != 0 && func_800A44F4(obj, target) == 2) {
                func_800A65E4(&dest, obj, target);
                func_800A665C(obj, &dest);
                return ((s32 (*)(void *, void *))obj->vtable[26].func)((u8 *)obj + obj->vtable[26].delta, 0);
            }
        }
    }
    return func_800EF210(obj);
}
