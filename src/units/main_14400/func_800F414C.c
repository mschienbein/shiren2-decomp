#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct Obj800F414C Obj800F414C;
typedef struct {
    s16 delta;
    s16 index;
    void (*fn)(Obj800F414C *self, s16 step);
} StepEntry800F414C;
typedef struct {
    s16 delta;
    s16 index;
    void (*fn)(Obj800F414C *self, s32 amount);
} AddEntry800F414C;
typedef struct {
    u8 pad0[0x78];
    StepEntry800F414C step;
    AddEntry800F414C add;
} Vtbl800F414C;
struct Obj800F414C {
    u8 pad0[0x24];
    Vtbl800F414C *vtbl;
    u8 pad28[0x44];
    s32 field_6C;
    u8 pad70[4];
    u8 field_74;
    u8 pad75[0x8F];
    Obj800F414C *field_104;
};

extern Obj800F414C *D_801476B8;
u16 func_800E08B0(Obj800F414C *self);

void func_800F414C(Obj800F414C *self, s32 amount) {
    Obj800F414C *owner;
    Obj800F414C *target;
    s32 isTarget;
    s32 step;

    if (amount != 0 && func_800E08B0(self) != 0) {
        owner = D_801476B8;
        target = owner->field_104;
        isTarget = 0;
        if (target != 0) {
            isTarget = self == target;
        }
        if (isTarget) {
            owner->vtbl->add.fn((Obj800F414C *)((u8 *)owner + owner->vtbl->add.delta), amount);
        } else if (self->field_74 != 0) {
            self->field_6C += amount;
        } else {
            step = -1;
            if (amount > 0) {
                step = 1;
            }
            self->vtbl->step.fn((Obj800F414C *)((u8 *)self + self->vtbl->step.delta), step);
        }
    }
}
