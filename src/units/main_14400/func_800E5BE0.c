#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x, y; } Pos;
typedef struct { u32 hi : 27; u32 b4 : 1; u32 lo : 4; } Flags;
/* Only slot +0x5C (vtbl[11], message handler) is called through this view. */
typedef struct { short delta; short index; s32 (*fn)(void *self, void *msg); } VEntry;
typedef struct {
    Pos pos;
    char pad8[0x1C - 0x8];
    u16 unk1C;
    u16 pad1E;
    Flags flags;
    VEntry *vtbl;
} Obj;
typedef struct { s32 pad0; void *unk4; s32 pad8[2]; s32 unk10; } Arg;
typedef struct { s32 unk0; s32 pad[5]; } Msg;
extern u8 D_80147620[];
extern u8 D_80156A59;
extern u8 D_80156A5B;
void func_800E20F0(Obj *);
s32 func_80049CB4(s32, ...);
u16 func_800E08B0(Obj *);
s32 func_800E1CD4(Obj *, s32);
char *func_800A3B20(Obj *);
void func_800497F0(s32, ...);
s32 func_800C5844(void *, u8, u8);
s32 func_800A692C(Obj *, s32);
void func_800A7B18(void *, void *, s32, s32);
static inline void Pos_copy(Pos *dst, Pos *src) {
    dst->x = src->x;
    dst->y = src->y;
}
static inline s32 Flags_b4(Flags *f) {
    return f->b4;
}
s32 func_800E5BE0(Obj *self, Arg *arg) {
    Pos pos;
    s32 h;
    s32 stop;
    s32 blocked;
    s32 item;
    s32 failed;
    Flags f;
    func_800E20F0(self);
    Pos_copy(&pos, &self->pos);
    h = func_80049CB4(0xDA, &pos);
    stop = 0;
    if (func_800E08B0(self) == 0 || (self->unk1C & 1)) stop = 1;
    if (stop) return 0;
    f = self->flags;
    blocked = 0;
    if (Flags_b4(&f) || func_800E1CD4(self, 0xA) || func_800E1CD4(self, 0xB) || func_800E1CD4(self, 0xD)
        || func_800E1CD4(self, 0xF)) {
        blocked = 1;
    }
    if (blocked) {
        func_800497F0(0x74, h, func_800A3B20(self));
        return 0;
    }
    func_80049CB4(0x1131);
    func_80049CB4(6);
    func_80049CB4(0x42, self);
    func_80049CB4(7);
    func_80049CB4(6);
    func_800497F0(0x73, h, func_800A3B20(self));
    func_80049CB4(7);
    item = arg->unk10;
    if (item == -1) item = (u8)func_800C5844(D_80147620, D_80156A59, D_80156A5B);
    failed = func_800A692C(self, 0x12) != 1;
    if (failed) {
        Msg m;
        VEntry *e;
        m.unk0 = 0xC;
        e = &self->vtbl[11];
        e->fn((char *)self + e->delta, &m);
    }
    if (item) {
        func_80049CB4(6);
        func_800A7B18(self, arg->unk4, item, 0x27);
        func_80049CB4(7);
    }
    func_80049CB4(0x12D);
    return func_800E08B0(self) != 0;
}
