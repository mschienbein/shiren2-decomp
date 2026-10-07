#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef struct { s16 delta; s16 index; void (*fn)(void *self, s32 flags); } VEntry;
typedef struct { s32 x, y; } Pos;
typedef struct { u8 key; u8 kind; u8 arg; } Spawn;
typedef struct { u8 pad[0x24]; VEntry *vtbl; u8 pad28[0x58]; u8 kind; } Obj;
extern u8 D_801480CE;
extern u8 D_801480CF;
extern u16 D_801480CC;
extern Spawn D_801480B0[];
extern u8 D_801480A0[];
extern u8 D_80147620[];
s32 func_800A9070(s32 *, s32);
Obj *func_800A910C(s32 *);
u16 func_800E08B0(Obj *);
void func_800F8980(Obj *);
void func_800F89F4(Obj *);
s32 func_800C587C(void *, u8);
void *func_800A38FC(s32 size);
void *func_800F8900(void *obj, u8 kind);
s32 func_800A3934(Obj *);
void *func_800A33DC(void *out, void *rect);
s32 func_800A4314(Obj *, Pos *);
s32 func_80049CB4(s32 id, ...);
void func_800F8998(Obj *, Pos *);
void func_801F212C(u16 id, u8 flag);
s32 func_800D74AC(void) {
    s32 it;
    s32 it2;
    Obj *o;
    Spawn *e;
    s32 n;
    VEntry *v;

    if (D_801480CE == 0) {
        return 0;
    }
    it = 0;
    while (func_800A9070(&it, 0x5D)) {
        o = func_800A910C(&it);
        if (func_800E08B0(o) == 0) continue;
        func_800F8980(o);
        if (o->kind == 0) {
            func_800F89F4(o);
        }
    }
    e = D_801480B0;
    for (;;) {
        u8 count;
        if (e->key == 0) break;
        if (e->key == D_801480CF) {
            count = func_800C587C(D_80147620, e->arg) ? 2 : 1;
            o = func_800F8900(func_800A38FC(0x84), count);
            if (func_800A3934(o)) continue;
            n = 1000;
            for (;;) {
                Pos pos;
                if (--n == -1) break;
                func_800A33DC(&pos, D_801480A0);
                if (func_800A4314(o, &pos)) {
                    func_80049CB4(6);
                    func_800F8998(o, &pos);
                    func_80049CB4(7);
                    o->kind = e->kind;
                    break;
                }
            }
            if (n < 0 && o != 0) {
                v = &o->vtbl[1];
                v->fn((u8 *)o + v->delta, 3);
            }
        }
        e++;
    }
    if (D_801480CF >= D_801480CE) {
        it2 = 0;
        while (func_800A9070(&it2, 0x5D)) {
            o = func_800A910C(&it2);
            if (func_800E08B0(o) != 0) {
                func_800F89F4(o);
            }
        }
        D_801480CE = 0;
        func_801F212C(D_801480CC, 0);
        return 0;
    }
    D_801480CF++;
    return 1;
}
