#include "common.h"
typedef unsigned char u8;
typedef struct { s32 x, y; } Coord;
typedef struct Object Object;
struct Object {
    Coord position;
    u8 pad8[0x50];
    Object *target;
    u8 pad5C[0x2D];
    u8 distance;
    u8 pad8A[0x16];
    Object *selected;
};
extern s32 func_800A692C(Object *, s32);
extern s32 func_800F3310(Object *);
extern u8 func_800A6420(Object *, Object *);
extern s32 func_800A23E8(Coord *, Coord *);
extern u32 func_800B1C6C(Coord *);
extern s32 func_800A6E90(void *);
extern s32 func_800F1024(Object *);
extern s32 func_80103F24(Object *);
extern void func_800F06E4(Object *);
extern s32 func_800E1CD4(Object *, s32);
extern s32 func_800E7104(Object *);
extern s32 func_800E8350(Object *);

s32 func_8010442C(Object *self)
{
    Object *target;
    if (func_800A692C(self, 0x12)) {
        return func_800F3310(self);
    }
    target = self->target;
    self->selected = 0;
    switch (func_800A6420(self, target)) {
    case 0:
        self->selected = target;
        goto act;
    case 1:
    case 2:
        {
            Coord position;
            Coord origin;
            Coord *where = &position;
            s32 suitable;
            position.x = target->position.x;
            where->y = target->position.y;
            origin.x = self->position.x;
            origin.y = self->position.y;
            suitable = func_800A23E8(where, &origin) <= self->distance &&
                !(func_800B1C6C(where) & 0x4000) && !func_800A6E90(target);
            if (suitable) {
                s32 attack = func_800F1024(self) && func_80103F24(self);
                if (attack) {
act:
                    func_800F06E4(self);
                    return 0;
                }
            }
        }
        break;
    }
    if (func_800E1CD4(self, 0x10)) {
        return func_800E8350(self);
    } else {
        return func_800E7104(self);
    }
}
