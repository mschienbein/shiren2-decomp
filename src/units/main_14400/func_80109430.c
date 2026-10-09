#include "common.h"
typedef unsigned char u8;
typedef struct Object { u8 pad_0[0x54]; u8 flags_54; u8 pad_55[3]; struct Object *target_58; u8 pad_5C[0x64]; s32 field_C0; } Object;
typedef Object Obj;
typedef Object Obj800E0F40;
typedef Object Unit800E7104;
typedef struct { u8 pad_0[7]; u8 field_7; } Parameters;
extern s32 func_800EE504(Obj *);
extern s32 func_800EF184(Object *arg, unsigned char mode);
extern s32 func_800E0F40(Obj800E0F40 *obj);
/* The original passes the same object pointer in a0; this is an unused receiver. */
extern void *func_80044DCC(void *receiver, u8 index);
extern u8 func_800A6420(Object *obj, Object *target);
extern void *func_800A492C(void *a, s32 b, s32 c, s32 d);
extern s32 func_80109370(void *arg0, void *arg1, s32 arg2);
extern s32 func_800E7104(Unit800E7104 *unit);
extern s32 func_800E776C(void *arg0, u8 arg1);
s32 func_80109430(Object *self, u8 mode) {
    Object *target;
    Parameters *params;
    s32 active = func_800EE504(self) == 1;
    if (!active) return func_800EF184(self, mode);
    params = func_80044DCC(self, func_800E0F40(self));
    target = self->target_58;
    self->field_C0 = 0;
    if (!func_800A6420(self, target)) { self->flags_54 |= 4; return 0; }
    if (mode != 1) target = func_800A492C(self, 2, 1, 1);
    if (target != 0) {
        self->target_58 = target;
        if (!func_800A6420(self, target)) { self->flags_54 |= 4; return 0; }
        if (func_80109370(self, target, params->field_7)) {
            self->field_C0 = 1;
            self->flags_54 |= 4;
            return 0;
        }
        if (mode != 1) return func_800E7104(self);
    }
    return func_800E776C(self, 3);
}
