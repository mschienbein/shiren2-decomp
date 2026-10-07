#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef float f32;
typedef struct { u8 pad[0xA]; s8 step; u8 padB; } Frame;
typedef struct { u8 pad[0xC]; Frame *frames; } Anim;
typedef struct {
    u8 pad0[2];
    s16 id;
    u8 pad4[2];
    u8 kind;
    u8 pad7;
    u8 state;
    u8 pad9[3];
    s16 z;
    u8 padE[2];
    s16 x;
    u8 type;
    u8 sub;
    u8 pad14[0x3F - 0x14];
    u8 frame;
    u8 pad40[4];
    u8 visible;
    u8 slot;
    u8 pad46;
    u8 active;
    u8 pad48;
    u8 flags;
    u8 pad4A[2];
    Anim *anim;
    u8 pad50[0x88 - 0x50];
    void *model;
    u8 pad8C[4];
    f32 dist;
    u8 pad94[0xB0 - 0x94];
} Entity;
typedef struct { u8 pad[2]; u8 size; } Def;
typedef struct { Def *def; } Info;
typedef struct { f32 x; f32 y; f32 z; } Vec3;
typedef struct { f32 m[4][4]; } Mtx;
extern Entity D_801DEAB4[30];
extern Entity D_801D8FFC[4];
extern s32 D_8013D920;
Info *func_80074784(s32 id, s32 kind);
void func_800593F8(Vec3 *pos);
f32 func_80032B40(f32 x);
void func_8002CE80(Mtx *m, void *model);
void func_80079800(Mtx *m, f32 x, f32 y, f32 z);
void func_8002CD40(Mtx *m, void *model);
s32 func_80076044(s32 slot, s32 a, s32 b, s32 c, s32 d);
void *func_8007946C(s32 type, s32 sub);
#define ABSF(v) ((v) >= 0.0f ? (v) : -(v))
#define DX(e) (cam.z - (f32)((e)->x / 4))
#define DZ(e) (cam.x - (f32)((e)->z / 4))
#define DIST(e) __builtin_sqrtf(ABSF(DZ(e)) * ABSF(DZ(e)) + ABSF(DX(e)) * ABSF(DX(e)))
void func_8007A680(void) {
    Entity *list[32];
    Mtx mtx;
    Vec3 cam;
    Entity **far;
    Entity **q;
    Entity **best;
    s32 count = 0;
    Entity **p = list;
    s32 i;
    Entity *e;
    s32 step;
    Info *info;
    for (i = 0; i < 30; i++) {
        info = func_80074784(D_801DEAB4[i].id, D_801DEAB4[i].kind);
        if ((D_801DEAB4[i].flags & 0x80) && info->def->size >= 0x3C) {
            *p++ = &D_801DEAB4[i];
            D_801DEAB4[i].slot = i;
        }
    }
    far = p;
    for (i = 0; i < 30; i++) {
        info = func_80074784(D_801DEAB4[i].id, D_801DEAB4[i].kind);
        if ((D_801DEAB4[i].flags & 0x80) && info->def->size < 0x3C) {
            *p++ = &D_801DEAB4[i];
            D_801DEAB4[i].slot = i;
        }
    }
    *p = 0;
    func_800593F8(&cam);
    for (p = far; *p != 0; p++) {
        best = p;
        for (q = p + 1; *q != 0; q++) {
            f32 d = DIST(*best);
            if (d > DIST(*q)) best = q;
        }
        if (best != p) {
            e = *best;
            *best = *p;
            *p = e;
        }
    }
    for (p = list; *p != 0; p++) {
        e = *p;
        step = e->anim->frames[e->frame].step;
        info = func_80074784(e->id, e->kind);
        if (count < 8 && (info->def->size >= 0x3C || (s32)e->dist < D_8013D920)) {
            e->slot = count;
            count++;
            e->visible = 1;
        } else if (step != 0) {
            e->visible = 0;
            e->frame += step;
            func_8002CE80(&mtx, e->model);
            func_80079800(&mtx, 2.0f, 2.0f, 2.0f);
            func_8002CD40(&mtx, e->model);
            e->flags |= 1;
        } else if (count < 8) {
            e->slot = count;
            count++;
            e->visible = 1;
        } else {
            e->visible = 0;
            func_80076044(e->slot, 0x4C, 2, 0, 0);
            e->frame = 0;
        }
    }
    for (i = 0; i < 4; i++) {
        Entity *t;
        e = &D_801D8FFC[i];
        if (e->id == -1) continue;
        if (e->state == 1) continue;
        if (e->active == 0) continue;
        t = func_8007946C(e->type, e->sub);
        if (t == 0) continue;
        if (t->id == -1) continue;
        step = e->anim->frames[e->frame].step;
        if (!(t->flags & 1)) continue;
        if (step != 0) {
            e->frame += step;
            func_8002CE80(&mtx, e->model);
            func_80079800(&mtx, 2.0f, 2.0f, 2.0f);
            func_8002CD40(&mtx, e->model);
            e->flags |= 1;
        }
    }
}
