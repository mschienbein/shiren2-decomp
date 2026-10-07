#include "common.h"

typedef unsigned char u8;

typedef struct {
    s32 unk0;
    s32 unk4;
    void *unk8;
} Object_80124BF0;

extern u8 D_8015FF70[];
Object_80124BF0 *func_80115690(Object_80124BF0 *obj, s32 arg1);

Object_80124BF0 *func_80124BF0(Object_80124BF0 *obj) {
    func_80115690(obj, 216);
    obj->unk8 = D_8015FF70;
    return obj;
}
