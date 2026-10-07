#include "common.h"

typedef unsigned char u8;

typedef struct {
    char pad0[4];
    s32 unk4;
} Obj80048130;

extern s32 D_8013A294;
extern s32 D_80138BF0;
extern s32 D_80140110[];
void func_8004633C(void);
void func_80060C54(u32 mode);

void func_80048130(Obj80048130 *obj, u8 index) {
    D_8013A294 = index;
    D_80138BF0 = 1;
    obj->unk4 = D_80140110[index];
    func_8004633C();
    func_80060C54(7);
}
