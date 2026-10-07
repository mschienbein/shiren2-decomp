#include "common.h"

typedef unsigned char u8;

typedef struct {
    s32 unk0;
    s32 unk4;
    void *unk8;
} Object_801172E0;

extern u8 D_8015DB80[];
Object_801172E0 *func_80116D50(Object_801172E0 *obj, s32 arg1);

Object_801172E0 *func_801172E0(Object_801172E0 *obj) {
    func_80116D50(obj, 1);
    obj->unk8 = D_8015DB80;
    return obj;
}
