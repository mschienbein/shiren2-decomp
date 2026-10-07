#include "common.h"

typedef unsigned char u8;

typedef struct {
    s32 unk0;
    s32 unk4;
    void *unk8;
} Object_80118DB0;

extern u8 D_8015E1B0[];
Object_80118DB0 *func_80116D50(Object_80118DB0 *obj, s32 arg1);

Object_80118DB0 *func_80118DB0(Object_80118DB0 *obj) {
    func_80116D50(obj, 19);
    obj->unk8 = D_8015E1B0;
    return obj;
}
