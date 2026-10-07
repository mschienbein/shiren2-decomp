#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef float f32;
typedef struct {
    u8 pad[2];
    s16 x2;
    u8 pad4[3];
    u8 b7;
} Track;
typedef struct Sound {
    void *handler;
    u16 state;
    u16 index;
    u16 phase;
    u8 padA[4];
    s16 x0E;
    union {
        struct {
            u16 group;
            u16 flags;
        } h;
        u32 w;
    } u;
    s32 handle;
    s32 x18;
    u32 retries;
    u8 pad20[4];
    s32 track;
    s32 x28;
    s32 x2C;
    f32 x30;
    u8 pad34[8];
    u8 x3C[0x28];
    s32 x64;
    u8 pad68[8];
    s32 x70;
} Sound;
extern Sound D_801BA380[0xAE];
void func_80085A80(Sound *);
void func_800865A0(Sound *);
void func_800893D8(Sound *);
s32 func_8008BF14(s32, s32, s32, s32, s32, void *, f32);
void func_8008C558(s32, s32);
void func_800840C0(Sound *);
void func_8008C528(s32 index, u8 value);
Track *func_8007946C(s32, s32);
void func_80079560(s32, s32, s32);
void func_8008C334(s32);
s32 func_8008C1C8(s32 index, f32 delta);
f32 func_8008C440(s32);
void func_80052260(s16);
void func_8008C478(s32 index, u8 on);
void func_8008C194(s32);

void func_80085C24(Sound *e) {
    s32 extra = 0;
    s32 i;
    s32 j;
    s32 found;
    u8 ok;
    s32 h;
    u32 r;
    Sound *o;
    Sound *p;
    s32 n;
    Track *t;

    switch (e->phase) {
    case 0:
        found = 0;
        for (i = e->index - 1; i >= 0; i--) {
            Sound *other = &D_801BA380[i];
            if (other->handler != func_80085C24) continue;
            if (other->u.h.flags & 4) continue;
            if (e->track != other->track) continue;
            if (other->state != 4) {
                found++;
                break;
            }
        }
        if (found) return;
        ok = !((e->u.h.flags >> 5) & 1);
        if (!ok && (e->u.h.flags & 0x4000)) {
            extra = 0x60;
            ok = 1;
        }
        h = func_8008BF14(e->x2C, e->x64, e->x70, ok, e->x18, e->x3C, 1.0f);
        e->handle = h;
        if (h < 0) {
            if (++e->retries < 11) return;
            e->state = 4;
            return;
        }
        if (extra) func_8008C558(h, extra);
        func_800840C0(e);
        if (e->u.h.flags & 0x40) func_8008C528(h, 0x60);
        if (!(e->u.h.flags & 4) && e->track >= 0) {
            t = func_8007946C(0, e->track);
            func_80079560(0, e->track, 1);
            t->b7 = 2;
        }
        func_8008C334(e->handle);
        e->phase++;
        /* fallthrough */
    case 1:
        r = (u32)func_8008C1C8(e->handle, 1.0f);
        if (func_8008C440(e->handle) == e->x30 && e->x28 >= 0) {
            func_80052260(e->x28);
        }
        if (r & 2) {
            if (e->u.h.flags & 8) {
                func_8007946C(0, e->track);
                func_80079560(0, e->track, 1);
            }
            e->x0E = 1;
        }
        if (!(r & 1)) return;
        if (e->u.h.flags & 0x800) {
            e->x2C = -1;
            for (i = 0; i < 0xAE; i++) {
                o = &D_801BA380[i];
                if ((o->handler == func_80085C24 || o->handler == func_80085A80) && o->index != e->index
                    && o->u.h.group == e->u.h.group && (o->u.h.flags & 0x400)) {
                    e->x2C = i;
                    break;
                }
            }
            if (e->x2C < 0) e->u.h.flags &= ~0x800;
            e->phase++;
            break;
        }
        if (e->u.h.flags & 0x200) {
            e->x2C = -1;
            for (j = e->index + 1; j < 0xAE; j++) {
                if (D_801BA380[j].handler == func_800865A0 || D_801BA380[j].handler == func_800893D8) {
                    e->x2C = j;
                    break;
                }
            }
            if (e->x2C < 0) {
                e->retries = 3;
                e->u.h.flags &= ~0x200;
            } else {
                e->u.h.flags &= ~0x200;
            }
            e->phase++;
            break;
        }
        /* fallthrough */
    case 2:
        if (e->u.h.flags & 0x200) {
            if (D_801BA380[e->x2C].state == 4) e->u.h.flags &= ~0x200;
            return;
        }
        if (e->u.h.flags & 0x800) return;
        func_8008C478(e->handle, 0);
        func_8008C194(e->handle);
        if ((e->u.w & 0xC) != 4 && !(e->u.h.flags & 0x80) && e->track >= 0) {
            t = func_8007946C(0, e->track);
            if (t->x2 != -1) {
                func_80079560(0, e->track, 0);
                t->b7 = 1;
            }
        }
        e->state = 4;
        if (!(e->u.h.flags & 0x400)) return;
        for (n = 0; n < 0xAE; n++) {
            p = &D_801BA380[n];
            if (p->handler != func_80085C24) continue;
            if (p->index == e->index) continue;
            if (p->u.h.group != e->u.h.group) continue;
            if (!(p->u.h.flags & 0x800)) continue;
            func_8008C478(p->handle, 0);
            func_8008C194(p->handle);
            if ((p->u.w & 0xC) != 4 && !(p->u.h.flags & 0x80) && p->track >= 0) {
                t = func_8007946C(0, p->track);
                if (t->x2 != -1) {
                    func_80079560(0, p->track, 0);
                    t->b7 = 1;
                }
            }
            p->state = 4;
        }
        return;
    }
}
