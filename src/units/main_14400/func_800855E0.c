#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct {
    void (*func)(void *);
    u16 state;
    u8 unk6[0xC];
    u16 flags;
    u8 unk14[0x10];
    s32 id;
    u8 unk28[0x4C];
} Task;
typedef struct {
    u8 unk0[4];
    u16 state;
    u16 index;
    u16 phase;
    u8 unkA[0xA];
    s32 id;
    u8 unk18[4];
    s32 timer;
    u8 unk20[4];
    s32 savedA;
    s32 savedB;
    s32 keepVisible;
    u8 unk30[0x2C];
    s32 fade;
    s32 alpha;
    s32 sound;
    s32 red;
    s32 green;
    s32 blue;
} Fader;
typedef struct {
    u8 unk0[7];
    u8 mode;
    u8 unk8;
    u8 visible;
    u16 sound;
    u8 unkC[0x32];
    u8 b;
    u8 unk3F;
    u8 a;
    u8 unk41[0x2F];
    u8 color[4];
} Actor;
extern Task D_801BA380[];
/* Four 8-colour RGBA fade palettes (32 bytes each), indexed by fade kind 1..4. */
extern u8 D_8013E928[4][32];
void func_80085C24(void *task);
Actor *func_8007946C(s32 kind, s32 id);
s32 func_800751B4(s32 id, s32 sound);
/* Task handler stored in the task table (TaskFn slot); the canonical handler
   func_80085C24 likewise takes its typed task record. */
void func_800855E0(Fader *t) {
    Actor *a = func_8007946C(0, t->id);
    u8 *color = a->color;
    s32 i;
    s32 busy;
    Task *e;
    u8 *src;
    s32 timer;

    switch (t->phase) {
        case 0:
            busy = 0;
            for (i = t->index - 1; i >= 0; i--) {
                e = &D_801BA380[i];
                if (e->func == func_80085C24 && !(e->flags & 4) && t->id == e->id && e->state != 4) {
                    busy++;
                    break;
                }
            }
            if (busy) return;
            t->fade = t->savedA;
            t->savedA = a->a;
            t->savedB = a->b;
            a->mode = 2;
            a->a = 1;
            a->b = 0;
            if (t->fade) {
                t->sound = a->sound;
                func_800751B4(t->id, 0x8000);
                t->red = color[0];
                t->green = color[1];
                t->blue = color[2];
                t->alpha = color[3];
                color[3] = 0xFF;
            }
            t->timer = 8;
            t->phase++;
        case 1:
            timer = t->timer - 1;
            t->timer = timer;
            if (timer != 0) {
                if (t->fade) {
                    s32 offset = (7 - timer) * 4;
                    u8 *row = D_8013E928[t->fade - 1];
                    src = row + offset;
                    color[0] = src[0];
                    color[1] = src[1];
                    color[2] = src[2];
                    color[3] = src[3];
                }
                return;
            }
            if (t->fade) {
                color[0] = t->red;
                color[1] = t->green;
                color[2] = t->blue;
                color[3] = t->alpha;
            }
            if (t->keepVisible == 0) {
                a->a = t->savedA;
                a->b = t->savedB;
            } else {
                a->visible = 0;
            }
            if (t->fade) a->mode = 1;
            t->phase++;
            return;
        case 2:
            if (t->fade) func_800751B4(t->id, t->sound);
            t->state = 4;
            break;
    }
}
