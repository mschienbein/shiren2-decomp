#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    s32 x;
    s32 y;
} Pos;

typedef struct {
    u8 kind;
    u8 pad1[7];
} Event;

typedef struct {
    Pos pos;
    u8 pad8[2];
    u8 field_A;
    u8 padB[0x1D];
    u8 field_28;
    u8 field_29;
} Obj;

extern u8 D_80147620[];
extern u8 D_801577F0[];
extern u8 D_80157808[];
s32 func_800C5844(void *rng, u8 base, u8 top);
void func_800A58FC(Obj *, Pos *);
void func_800A665C(Obj *, Event *);
void func_800C9544(void);

void func_800F4830(Obj *obj, Pos *pos) {
    Event event;

    obj->field_28 = obj->field_A - 2;
    obj->field_29 = func_800C5844(D_80147620, D_801577F0[obj->field_28], D_80157808[obj->field_28]);
    obj->pos = *pos;
    func_800A58FC(obj, pos);
    event.kind = 6;
    func_800A665C(obj, &event);
    if (obj->field_A == 3) {
        func_800C9544();
    }
}
