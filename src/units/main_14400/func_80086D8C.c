#include "common.h"
typedef struct { unsigned char pad00[0x14]; float height; unsigned char pad18[0x24]; unsigned short animation; } Actor;
typedef struct {
    unsigned char pad00[4]; unsigned short state; unsigned short id; unsigned short phase;
    unsigned char pad0A[8]; unsigned short flags; s32 actorId; s32 field18; s32 ticks;
    unsigned char pad20[0x28]; float speed;
} Event;
extern Actor *func_8007946C(s32 side, s32 slot);
extern s32 func_80076044(s32 id, s32 animation, s32 mode, s32 command, s32 flags);
extern void func_80079560(s32 side, s32 id, s32 mode);
void func_80086D8C(Event *event) {
    Actor *actor = func_8007946C(0, event->actorId);
    switch (event->phase) {
    case 0:
        func_80076044(event->actorId, actor->animation, 2, 0x1B, 0);
        event->ticks = 10;
        event->speed = 20.0f;
        event->phase++;
        break;
    case 1:
        if (event->ticks-- != 0) {
            actor->height += event->speed;
            if (event->flags & 0x2000) event->speed *= 1.8f;
        } else {
            if (!(event->flags & 0x2000)) func_80079560(0, event->actorId, 1);
            event->state = 4;
        }
        break;
    }
}
