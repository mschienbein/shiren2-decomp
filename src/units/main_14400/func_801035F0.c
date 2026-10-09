#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    s32 x;
    s32 y;
} Pos801035F0;

typedef struct Obj801035F0 Obj801035F0;
struct Obj801035F0 {
    u8 pad00[0x1C];
    u16 flags1C;
    u8 pad1E[0x54 - 0x1E];
    u8 flags54;
    u8 pad55[3];
    Obj801035F0 *target58;
    u8 pad5C[0xA0 - 0x5C];
    Pos801035F0 goalA0;
};

extern u8 func_800A6420(Obj801035F0 *obj, Obj801035F0 *target);
extern void func_800F06E4(Obj801035F0 *obj);
extern s32 func_800A650C(Obj801035F0 *object, Pos801035F0 *value);
extern s32 func_800F1024(Obj801035F0 *obj);
extern s32 func_800E0F40(Obj801035F0 *obj);
extern s32 func_80103004(Obj801035F0 *obj);
extern s32 func_800E7104(Obj801035F0 *unit);

s32 func_801035F0(Obj801035F0 *self)
{
    Pos801035F0 *goal;
    s32 retreat;

    self->flags1C &= ~0x200;
    switch (func_800A6420(self, self->target58)) {
    case 0:
        func_800F06E4(self);
        return 0;
    case 1:
    case 2:
        goal = &self->goalA0;
        retreat = (goal->y | goal->x) == 0 || func_800A650C(self, goal) >= 2;
        if (retreat) {
            s32 blocked = 0;
            if (func_800F1024(self)) {
                u8 level = func_800E0F40(self);

                blocked = !(level < 2);
            }
            if (blocked) {
                func_800F06E4(self);
                self->target58 = 0;
                return 0;
            }
        }
        if (func_80103004(self)) {
            break;
        }
        goto perform_action;
    default:
        if (func_80103004(self)) {
            return 1;
        }
        if (!func_800F1024(self)) {
perform_action:
            return func_800E7104(self);
        }
        self->target58 = 0;
        self->flags54 |= 4;
        return 0;
    }
    return 1;
}
