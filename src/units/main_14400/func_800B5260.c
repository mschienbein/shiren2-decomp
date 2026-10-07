#include "common.h"
typedef unsigned char u8;
typedef struct { u8 unk0; u8 unk1; u8 unk2; } Rec;
typedef struct { u8 pad[3]; u8 unk3; u8 pad4[3]; u8 unk7; } Obj;
Rec *func_800B51D4(Obj *);
void func_800B4E7C(Obj *);
s32 func_80049CB4(s32 id, ...);
void func_800B5260(Obj *self, u8 arg) {
    Rec *r;
    if (arg) arg -= 7;
    r = func_800B51D4(self);
    if (r) {
        r->unk0 = self->unk7;
        r->unk1 = self->unk3;
        r->unk2 = arg;
        func_800B4E7C(self);
        func_80049CB4(0xD7, self);
    }
}
