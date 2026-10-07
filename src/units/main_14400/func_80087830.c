#include "common.h"
typedef struct { char pad[2]; short unk2; char pad4[3]; unsigned char unk7; char pad8[0x3C - 8]; unsigned short unk3C; } Data;
typedef struct { char pad[4]; unsigned short unk4; char pad6[2]; unsigned short unk8; char padA[0x14 - 0xA]; s32 unk14; char pad18[4]; s32 unk1C; char pad20[8]; s32 unk28; } Task;
Data *func_8007946C(s32, s32);
s32 func_80076044(s32 slot, s32 animation, s32 mode, s32 frame, s32 flags);
void func_80079560(s32, s32, s32);
void func_800885B8(Task *);
void func_80087830(Task *t) {
    Data *d = func_8007946C(0, t->unk14);
    switch (t->unk8) {
    case 0:
        if (d->unk2 == -1) break;
        func_80076044(t->unk14, d->unk3C, 2, 0, 1);
        t->unk1C = 6;
        t->unk8++;
        return;
    case 1:
        if (t->unk1C-- != 0) {
            func_80079560(0, t->unk14, (t->unk1C & 1) == 0);
            return;
        }
        if (t->unk28 != 0) {
            func_80079560(0, t->unk14, 1);
            d->unk7 = 1;
        } else {
            func_800885B8(t);
        }
        break;
    default:
        return;
    }
    t->unk4 = 4;
}
