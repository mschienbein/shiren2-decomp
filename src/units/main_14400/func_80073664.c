#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad0[0x17]; u8 alpha; u8 pad18[0x14]; float x, y; u8 pad34[0x10]; s32 tile; u8 pad48[8]; } Sprite;
typedef struct { s32 position; float timer; } ScriptState;
typedef struct { s32 state; float y, speed; u8 padC[4]; ScriptState script; u32 phase; u8 pad1C[4]; Sprite sprite; } Scroller;
typedef struct { u8 pad0[0x14]; s32 state; u8 pad18[4]; float speed; u8 pad20[0x3C]; float x; } Target;
extern const double D_8014C9E8;
extern u8 D_8013D55C[];
extern u8 D_801E4E48[];
extern float func_80032360(float angle);
extern s32 func_80073E78(void *object, void *state, void *script, float delta);
extern s32 func_80070598(void *list, void *value);

void func_80073664(Scroller *self, Target *target) {
    Sprite *sprite = &self->sprite;
    float distance = target->x - sprite->x;
    switch (self->state) {
    case 0:
        sprite = 0;
        if (target->state != 0) {
            self->state = 1;
            self->script.position = 0;
            self->script.timer = 0.0f;
            self->speed = 2.0f;
        }
        break;
    case 1: {
        float baseline;
        sprite->x += self->speed;
        if (sprite->x >= 448.0f) sprite->x = 448.0f;
        baseline = self->y + 8.0f;
        sprite->y = baseline + func_80032360((float)((double)self->phase * D_8014C9E8)) * 8.0f;
        sprite->tile = 0x6A;
        func_80073E78(sprite, &self->script, D_8013D55C, 1.0f);
        if (target->state != 3) {
            sprite->alpha = sprite->alpha < 0xB8 ? sprite->alpha + 8 : 0xC0;
        } else {
            sprite->alpha = sprite->alpha >= 0x29 ? sprite->alpha - 8 : 0x20;
        }
        if (distance <= 64.0f) {
            self->speed = 0.0f;
        } else if (distance >= 96.0f) {
            self->speed += 0.1f;
        } else {
            float difference = target->speed - self->speed;
            if (difference >= 0.05f) self->speed += 0.05f;
            else if (difference <= -0.05f) self->speed -= 0.05f;
        }
        self->speed = self->speed < 0.0f ? 0.0f : self->speed > 4.0f ? 4.0f : self->speed;
        break;
    }
    }
    if (sprite != 0) func_80070598(D_801E4E48, sprite);
    self->phase = (self->phase + 1) % 60;
}
