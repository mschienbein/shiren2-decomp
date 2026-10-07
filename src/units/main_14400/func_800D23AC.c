#include "common.h"
typedef struct { s32 unk0; s32 unk4; } Pos;
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 value; } Dir;
typedef struct { char pad[4]; void *unk4; } Obj;
s32 func_800B68B0(void *);
void *func_800B6A98(void *out, void *room, s32 index);
void *func_800A2594(void *out, void *from, Dir dir);
s32 func_800A31C8(void *, Pos *);
void *func_800B4D80(void *pos);
void func_800B1B58(Pos *pos, u16 flags);
static inline void setDir(Dir *d, s32 v) { d->value = v & 7; }
void func_800D23AC(Obj *o) {
    Pos base;
    Pos pos;
    s32 i, j;
    s32 n = func_800B68B0(o->unk4);
    for (i = 0; ; i++) {
        void *map;
        if (!(i < n)) break;
        map = o->unk4;
        func_800B6A98(&base, map, i);
        for (j = 0; ; j++) {
            Dir d;
            s32 ok;
            if (!(j < 8)) break;
            setDir(&d, j);
            func_800A2594(&pos, &base, d);
            ok = 0;
            if (func_800A31C8(o->unk4, &pos)) ok = func_800B4D80(&pos) == 0;
            if (ok) func_800B1B58(&pos, 0x10);
        }
    }
}
