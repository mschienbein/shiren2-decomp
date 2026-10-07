#include "common.h"
typedef struct { char pad[0x72]; unsigned char unk72; char pad73[0xA8 - 0x73]; s32 unkA8; } Obj;
typedef struct { unsigned char b7 : 1; unsigned char b6 : 1; unsigned char b5 : 1; unsigned char b4 : 1; unsigned char b3 : 1; unsigned char b2 : 1; unsigned char b1 : 1; unsigned char b0 : 1; } Flags;
extern Flags D_80142F1B;
extern signed char D_80140164;
s32 func_80049CB4(s32, ...);
s32 func_800E2074(Obj *);
s32 func_800F7970(Obj *, void *);
void func_800C9870(void);
char *func_800A3B20(Obj *);
void func_800498E4(s32, ...);
void func_80049BF0(s32);
s32 func_800F7884(Obj *o, void *arg) {
    func_80049CB4(0x128, 0x1A6);
    if (func_800E2074(o)) {
        if (func_800F7970(o, arg)) {
            o->unkA8 = 0;
            { s32 on = D_80142F1B.b2; if (on) { s32 ne = D_80140164 != 2; if (!ne) func_800C9870(); } }
        }
        o->unk72 |= 4;
    } else {
        func_800498E4(0x1BD, func_800A3B20(o));
        func_80049BF0(0);
    }
    return 1;
}
