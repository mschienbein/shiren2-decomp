#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef float f32;
typedef struct {
    u8 pad0[0x2C];
    f32 x2C;
    f32 x30;
    u8 pad34[0x10];
    s32 x44;
    s32 x48;
} Sprite;
typedef struct { s32 pos; f32 timer; } ScriptState;
typedef struct {
    s32 state;
    f32 x4;
    f32 speed;
    u8 xC[4];
    ScriptState scriptState;
    u8 pad18[8];
    Sprite sprite;
} Scroller;
typedef struct {
    u8 pad0[0x1C];
    f32 x1C;
    u8 pad20[0x3C];
    f32 x5C;
} Target;
extern u8 D_8013D540[];
extern u8 D_801E4E48[];
s32 func_80073920(void *p);
s32 func_80073E78(void *object, void *state, void *script, f32 delta);
s32 func_80070598(void *list, void *value);
void func_800734C0(Scroller *sc, Target *t) {
    Sprite *s = &sc->sprite;
    f32 dist = t->x5C - s->x2C;
    switch (sc->state) {
    case 0:
        s = 0;
        if (func_80073920(sc->xC)) {
            sc->state = 1;
            sc->scriptState.pos = 0;
            sc->scriptState.timer = 0.0f;
            sc->speed = t->x1C;
        }
        break;
    case 1:
        if (dist < 32.0f) {
            sc->speed = 0.0f;
        } else if (dist < 48.0f) {
            sc->speed -= 0.5f;
            if (sc->speed < 0.5f) {
                sc->speed = 0.5f;
            }
        } else if (dist >= 80.0f) {
            sc->speed += 0.5f;
            if (sc->speed > 8.0f) {
                sc->speed = 8.0f;
            }
        }
        s->x2C += sc->speed;
        if (s->x2C >= 448.0f) {
            s->x2C = 448.0f;
        }
        s->x30 = sc->x4;
        s->x44 = 0x61;
        func_80073E78(s, &sc->scriptState, D_8013D540, sc->speed);
        break;
    }
    if (s) {
        func_80070598(D_801E4E48, s);
    }
}
