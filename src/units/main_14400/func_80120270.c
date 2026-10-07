#include "common.h"

typedef unsigned char u8;

typedef struct {
    s32 unk0;
    s32 unk4;
    void *unk8;
} Object_80120270;

extern u8 D_8015F6B8[];
void func_8010E290(Object_80120270 *obj, s32 arg1);

Object_80120270 *func_80120270(Object_80120270 *obj) {
    func_8010E290(obj, 165);
    obj->unk8 = D_8015F6B8;
    return obj;
}
