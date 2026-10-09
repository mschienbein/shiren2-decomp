#include "common.h"
typedef struct {
    unsigned char pad00[0x14]; float height; unsigned char pad18[0x24]; unsigned short animation;
    unsigned char pad3E[8]; unsigned char opacity; unsigned char pad47[0x29]; unsigned char color[8];
} Actor;
typedef struct {
    unsigned char pad00[4]; unsigned short state, id, phase; unsigned char pad0A[8]; unsigned short flags;
    s32 actorId, field18, ticks; unsigned char pad20[0x10]; float speed, scale;
} Event;
extern Actor *func_8007946C(s32 side, s32 slot);
extern s32 func_80076044(s32 id, s32 animation, s32 mode, s32 command, s32 flags);
extern s32 func_800751B4(s32 id, s32 flag);
extern void func_80079560(s32 side, s32 id, s32 mode);
void func_80087E4C(Event *event) {
    Actor *actor = func_8007946C(0, event->actorId);
    unsigned char *color = actor->color;
    switch (event->phase) {
    case 0:
        if (!(event->flags & 0x4000)) func_80076044(event->actorId, actor->animation, 2, 6, 0);
        func_800751B4(event->actorId, 0x8000);
        color[0] = 0; color[1] = 0; color[2] = 0; color[3] = 0;
        color[4] = 0; color[5] = 0; color[6] = 0; color[7] = 255;
        event->speed = 0.5f;
        event->scale = 1.0f;
        actor->opacity = 255;
        event->ticks = 8;
        event->phase++;
        break;
    case 1:
        if (event->ticks-- == 0) { event->ticks = 15; event->phase++; }
        break;
    case 2:
        if (event->ticks-- != 0) {
            color[3] += 10;
            actor->height -= event->speed;
            event->speed = event->speed * 1.5;
            actor->opacity -= 15;
        } else {
            func_80079560(0, event->actorId, 1);
            event->state = 4;
        }
        break;
    }
}
