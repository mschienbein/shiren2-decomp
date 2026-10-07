#include "common.h"

typedef unsigned char u8;
typedef struct { char pad0[0x1E]; unsigned char unk1E; } Ent;
typedef struct VtEntry { short delta; short pad; s32 (*fn)(void *, Ent *); } VtEntry;
typedef struct { s32 x; s32 y; } Vec2i;
typedef struct { Vec2i pos; char pad8[0x1C]; VtEntry *vtable; } Obj;
Ent *func_800A6CF0(Obj *);
s32 func_800F0EC4(Obj *);
s32 func_800F3358(Obj *);
s32 func_800E20CC(void *);
s32 func_80049CB4(s32, ...);
char *func_800A3B20(void *);
void func_800497F0(s32, ...);
s32 func_800F9AC0(Obj *self) {
    Ent *ent = func_800A6CF0(self);
    s32 blocked = 0;
    s32 ret;
    if (func_800F0EC4(self) != 0 || (ent != 0 && ((ent->unk1E >> 1) & 1))) {
        blocked = 1;
    }
    if (blocked) {
        ret = func_800F3358(self);
    } else if (func_800E20CC(self) == 0) {
        Vec2i pos;
        Vec2i *p = &pos;
        s32 id;
        pos.x = self->pos.x;
        p->y = self->pos.y;
        id = func_80049CB4(0xDA, p);
        func_800497F0(0x22A, id, func_800A3B20(self));
        ret = 1;
    } else {
        ret = self->vtable[22].fn((char *)self + self->vtable[22].delta, ent);
    }
    return ret;
}
