#include "common.h"

typedef unsigned char u8;

typedef float f32;

typedef struct {
    u8 pad0[0x2C];
    f32 x2C;
    f32 y30;
    f32 scale34;
    f32 alpha38;
    u8 pad3C[4];
    s32 frames40;
    u8 pad44[0xC];
} Body80073D5C;

typedef struct {
    s32 timer;
    Body80073D5C body;
    f32 dx54;
    f32 dy58;
    u8 path5C[8];
} Particle80073D5C;

extern Particle80073D5C *D_801A7224;
extern u8 D_8013D598[];
extern u8 D_801E4E48[];
extern s32 func_80073E78(Body80073D5C *body, void *path, void *table, f32 speed);
extern s32 func_80070598(void *list, void *value);

void func_80073D5C(void) {
    Particle80073D5C *p = D_801A7224;
    Body80073D5C *body;
    s32 i;

    for (i = 0; i < 28; i++, p++) {
        if (p->timer < 0) {
            continue;
        }
        body = &p->body;
        if (func_80073E78(body, p->path5C, D_8013D598, 1.0f)) {
            p->timer = -1;
            continue;
        }
        if (p->timer != 0) {
            body->x2C += p->dx54;
            body->y30 += p->dy58;
            body->scale34 += 0.01f;
            body->alpha38 += 0.01f;
            body->frames40--;
        }
        p->timer++;
        func_80070598(D_801E4E48, body);
    }
}
