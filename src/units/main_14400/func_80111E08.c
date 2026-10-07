#include "common.h"

typedef unsigned char u8;
typedef struct { s32 a, b; } Vec2;
typedef struct { u8 value; } Dir;
typedef struct {
    unsigned char pad0[0x1C];
    void *vtbl;
    void *field_20;
    void *field_24;
    void *field_28;
    Vec2 pos_2C;
    u8 field_34;
} Obj;
extern void *func_800A2594(Vec2 *, Vec2 *, Dir);
extern Obj *func_800C2CA0(Obj *, Vec2 *, u8 *, s32, s32);
extern s32 D_8015D5D0[];

Obj *func_80111E08(Obj *o, void *a1, void *a2, Vec2 *pos, u8 *color) {
    Vec2 tmp;

    func_800A2594(&tmp, pos, *(Dir *)color);
    func_800C2CA0(o, &tmp, color, 0xFF, 2);
    o->vtbl = D_8015D5D0;
    o->field_20 = a1;
    o->field_24 = a1;
    o->field_28 = a2;
    o->pos_2C = *pos;
    o->field_34 = 0;
    return o;
}
