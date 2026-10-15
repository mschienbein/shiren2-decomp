#include "common.h"
typedef unsigned char u8;
typedef struct { u8 unk0; u8 unk1; u8 unk2; } Rec;
/* Full coordinate words; entry bytes keep only their low eight bits. */
typedef struct { s32 x; s32 y; } Obj;
Rec *func_800B51D4(Obj *);
void func_800B4E7C(Obj *);
s32 func_80049CB4(s32 id, ...);
void func_800B5260(Obj *self, u8 arg) {
    Rec *r;
    if (arg) arg -= 7;
    r = func_800B51D4(self);
    if (r) {
        r->unk0 = (u8)self->y;
        r->unk1 = (u8)self->x;
        r->unk2 = arg;
        func_800B4E7C(self);
        func_80049CB4(0xD7, self);
    }
}
