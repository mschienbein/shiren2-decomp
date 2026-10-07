#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct VTable80109994 VTable80109994;

typedef struct {
    char pad0[0x20];
    s32 unk20;
    VTable80109994 *vtbl;
    char pad28[0x78 - 0x28];
    s32 unk78;
    char pad7C[4];
    s32 unk80;
    char pad84[0xB8 - 0x84];
    short unkB8;
    char padBA[0xC0 - 0xBA];
    s32 unkC0;
} Obj80109994;

struct VTable80109994 {
    char pad0[0xA0];
    short adjustA0;
    s32 *(*funcA4)(char *self, u8 index);
};

void func_800E4D88(Obj80109994 *obj, s32 arg1);
void func_800E4D90(Obj80109994 *obj, s32 arg1);
void func_800E946C(void *obj, s16 count);

void func_80109994(Obj80109994 *obj, s32 index) {
    obj->unkC0 = 0;
    obj->unkB8 = 0x161;
    func_800E4D88(obj, 4);
    func_800E4D90(obj, 3);
    obj->unk20 = obj->unk80 |= 0x2000000;
    obj->unk78 = *obj->vtbl->funcA4((char *)obj + obj->vtbl->adjustA0, index);
    func_800E946C(obj, (index & 0xFF) - 1);
}
