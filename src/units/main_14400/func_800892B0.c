#include "common.h"

typedef unsigned char u8;
typedef float f32;

typedef struct {
    u8 pad0[0x1C];
    s32 steps;
    u8 pad20[0x1C];
    f32 stepX;
    f32 unk40;
    u8 pad44[4];
    f32 stepZ;
    f32 unk4C;
    u8 pad50[0xC];
    s32 targetX;
    s32 currentX;
    u8 pad64[4];
    s32 targetZ;
    s32 currentZ;
} Mover;
void func_800892B0(Mover *self, f32 speed) {
    f32 dx;
    f32 dz;
    f32 dist;
    f32 steps;

    if (self->targetX == self->currentX && self->targetZ == self->currentZ) {
        self->steps = 0;
        return;
    }
    dx = (f32)(self->targetX - self->currentX) * 128.0f;
    dz = (f32)(self->targetZ - self->currentZ) * 128.0f;
    dist = __builtin_sqrtf(dx * dx + dz * dz);
    self->unk40 = 0.0f;
    self->unk4C = 0.0f;
    steps = dist / (64.0f / speed);
    self->stepX = dx / steps;
    self->stepZ = dz / steps;
    self->steps = (s32)(steps + 0.5f) - 1;
}
