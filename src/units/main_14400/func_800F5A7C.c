#include "common.h"

typedef unsigned char u8;

typedef struct {
    s32 x;
    s32 y;
} Position;

typedef struct {
    Position pos;
    u8 pad08[0x29 - 0x08];
    u8 field29;
} Obj800F5A7C;

typedef struct {
    s32 kind;
} Event800F4F8C;

extern s32 func_800F4A70(void *self, Event800F4F8C *ev);
extern void func_800A59A4(Obj800F5A7C *self);
extern s32 func_80049CB4(s32 id, ...);
extern void func_800A00C4(Position *origin, u8 percentage, void *attacker, s32 kind);
extern u8 D_80156A79;

s32 func_800F5A7C(Obj800F5A7C *self, Event800F4F8C *ev)
{
    Position pos;
    Position *at;
    s32 result;

    if (ev->kind != 10) {
        result = func_800F4A70(self, ev);
    } else {
        self->field29 = 0;
        func_800A59A4(self);
        func_80049CB4(0x70, self);
        at = &pos;
        at->x = self->pos.x;
        at->y = self->pos.y;
        func_80049CB4(0xF6, at);
        func_800A00C4(at, D_80156A79, self, 3);
        result = 1;
    }
    return result;
}
