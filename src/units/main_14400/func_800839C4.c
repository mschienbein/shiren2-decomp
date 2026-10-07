#include "common.h"

typedef struct {
    void *unk0;
    s32 unk4;
    u32 unk8;
    void *unkC;
    s32 unk10;
} Obj;

extern void func_8006AAF0(void *dst, u32 devAddr, s32 size);

void func_800839C4(Obj *obj) {
    obj->unk8 += obj->unk4;
    func_8006AAF0(obj->unk0, obj->unk8, obj->unk4);
    obj->unk10 = obj->unk4;
    obj->unkC = obj->unk0;
}
