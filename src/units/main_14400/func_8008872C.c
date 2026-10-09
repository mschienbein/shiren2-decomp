#include "common.h"

typedef struct {
    unsigned char pad0[4];
    short unk4;
    unsigned char pad6[8];
    short unkE;
    unsigned char pad10[0x14];
    s32 unk24;
    s32 unk28;
    s32 unk2C;
} Obj;

extern s32 func_80077C4C(s32 arg0, s32 arg1, s32 arg2); /* returns the model index or -1; unused here */

void func_8008872C(Obj *obj) {
    func_80077C4C(obj->unk24, obj->unk28, obj->unk2C);
    obj->unkE = 1;
    obj->unk4 = 4;
}
