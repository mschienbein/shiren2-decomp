#include "common.h"

typedef signed char s8;

typedef struct {
    unsigned char pad0[3];
    unsigned char unk3;
    unsigned char pad4;
    s8 unk5;
} Obj;

extern void *func_800AC5F4(s32 size, Obj *place);
extern void *func_80120090(void *obj);
extern void func_800AE974(void *obj, s8 value);

void func_8010E2CC(Obj *obj) {
    unsigned char saved3 = obj->unk3;
    s32 saved5 = obj->unk5;

    func_80120090(func_800AC5F4(0xC, obj));
    obj->unk3 = saved3;
    func_800AE974(obj, (s8)saved5);
}
