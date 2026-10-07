#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { char pad[0x20]; float unk20; char pad24[0x68 - 0x24]; float unk68; char pad6C[0xAA - 0x6C]; u16 unkAA; char padAC[0xB6 - 0xAC]; u8 unkB6; char padB7[0xDC - 0xB7]; float unkDC; } Obj;
float func_80032360(float);
float func_8012B9BC(Obj *self) {
    s32 d = self->unkAA - self->unkB6;
    if (d > 0) {
        return self->unk68 = func_80032360(d * self->unkDC) * self->unk20;
    }
    return 0.0f;
}