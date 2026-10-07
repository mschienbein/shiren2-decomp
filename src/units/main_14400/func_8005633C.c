#include "common.h"

typedef float f32;

typedef struct {
    char pad0[0x30];
    f32 unk30;
    char pad34[0x50 - 0x34];
    short unk50;
    short unk52;
    char pad54[0x5A - 0x54];
    short unk5A;
} Obj8005633C;

extern s32 D_80139B30;

void func_8005633C(Obj8005633C *obj) {
    if (D_80139B30 == 1) {
        obj->unk30 -= 1.0f;
        if (obj->unk30 <= obj->unk52) {
            obj->unk50 = 2;
            obj->unk5A = 0;
        }
    }
}
