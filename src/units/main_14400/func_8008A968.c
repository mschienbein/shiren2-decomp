#include "common.h"
typedef unsigned short u16;
typedef struct { char pad0[0xC]; float unkC; char pad10[8]; float unk18; char pad1C[4]; } Params8008A968;
typedef struct { char pad0[4]; u16 unk4; char pad6[2]; u16 state; char padA[0xA]; s32 unk14; char pad18[4]; s32 timer; } Obj8008A968;
void func_80059728(Params8008A968 *);
void func_8005ABBC(Params8008A968 *, s32, s32);
void func_8005A464(s32 mode, void *params, s32 blend, s32 arg3);
void func_8008A968(Obj8008A968 *self) {
    Params8008A968 params;
    switch (self->state) {
    case 0:
        func_80059728(&params);
        params.unkC = 6.08f;
        params.unk18 = 200.0f;
        func_8005ABBC(&params, 0x1E, 0);
        self->timer = 42;
        self->state++;
        break;
    case 1:
        if (self->timer-- == 0) {
            func_8005A464(self->unk14, 0, 0, 0);
            self->unk4 = 4;
        }
        break;
    }
}
