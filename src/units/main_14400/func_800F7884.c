#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
typedef struct { char pad[0x72]; unsigned char unk72; char pad73[0xA8 - 0x73]; s32 unkA8; } Obj;


extern signed char D_80140160[];
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
            { s32 on = ((D_80142F18.flags >> 2) & 1); if (on) { s32 ne = D_80140160[4] != 2; if (!ne) func_800C9870(); } }
        }
        o->unk72 |= 4;
    } else {
        func_800498E4(0x1BD, func_800A3B20(o));
        func_80049BF0(0);
    }
    return 1;
}
