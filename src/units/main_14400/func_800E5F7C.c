#include "common.h"
typedef struct { s32 x; s32 y; } Vec2_800A2544;
typedef struct Obj {
    Vec2_800A2544 pos_00;
    unsigned char pad_08[0x4A];
    unsigned char state_52;
    unsigned char state_53;
    unsigned char pad_54[0x10];
    Vec2_800A2544 pos_64;
} Obj;
extern Vec2_800A2544 *func_800A2544(Vec2_800A2544 *out, Vec2_800A2544 *a, Vec2_800A2544 *b);
void func_800E5F7C(Obj *self, Vec2_800A2544 *delta) {
    Vec2_800A2544 result;
    func_800A2544(&result, &self->pos_00, delta);
    self->pos_64 = result;
    self->state_52 = 2;
    self->state_53 = 1;
}
