#include "common.h"
typedef struct { s32 x, y, z; } Vec3i;
typedef struct {
    unsigned char pad00[0x12]; unsigned short flags;
    s32 field14, value18, ticks, field20, track, animation, id;
    float speed; unsigned char pad34[8]; float x, z, y;
    unsigned char pad48[0x1C]; s32 direction; unsigned char pad68[8]; s32 value70;
} Event;
typedef struct { short animation; unsigned char speed; unsigned char pad03; } EffectInfo;
extern Event D_801BA380[];
extern EffectInfo D_8013A9E0[];
extern s32 D_8013E900, D_8013E90C, D_801BF2CC;
extern s32 func_80084CD4(void (*handler)(void *));
extern void func_80085C24(void *event);
void *func_80085938(s32 id, s32 track, Vec3i pos, s32 value70, s32 direction, s32 flags, s32 value18) {
    Event *event;
    D_8013E900 = 1;
    event = &D_801BA380[func_80084CD4(func_80085C24)];
    event->x = pos.x;
    event->z = pos.z;
    event->y = pos.y;
    event->id = id;
    event->direction = (direction + 4) % 8;
    event->value70 = value70;
    event->track = track;
    event->flags = flags;
    event->value18 = value18;
    event->ticks = 0;
    D_801BF2CC++;
    event->animation = D_8013A9E0[id].animation;
    if (D_8013E90C) event->speed = D_8013A9E0[id].speed;
    else event->speed = -1.0f;
    return event;
}
