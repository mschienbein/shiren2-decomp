#include "common.h"
typedef unsigned char u8;
typedef struct { s32 x; s32 y; } Point;
typedef struct { u8 pad_00[0x90]; short adjust_90; short pad_92; s32 (*method_94)(void *, s32, s32, u8, s32); } VTable;
typedef struct { Point position; u8 pad_08[0x16]; u8 field_1E; u8 pad_1F[5]; const VTable *field_24; } Actor;
extern u32 D_8013960C;
extern s32 func_80049CB4(s32 id, ...);
/* Trap apply slot +0x54 supplies five pointers; only the target actor is used here. */
void func_8011FA58(void *self, void *source, Actor *actor, void *direction, void *attacker) {
    Point point;
    if (actor->field_1E & 0x7C) {
        D_8013960C *= 2;
        actor->field_24->method_94((char *)actor + actor->field_24->adjust_90, 0, 1, 0xFE, 0);
        {
            Point *position = &actor->position;
            s32 y;
            point.x = position->x;
            D_8013960C >>= 1;
            y = position->y;
            position = &point;
            position->y = y;
            func_80049CB4(0xD7, position);
        }
    }
}
