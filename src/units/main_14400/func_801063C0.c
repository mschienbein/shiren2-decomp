#include "common.h"
typedef unsigned char u8;
extern void *func_800A38FC(s32 size);
extern void *func_80106400(void *obj, u8 kind);
void *func_801063C0(u8 kind, void *obj) {
    void *result;
    if (obj == 0)
        result = func_80106400(func_800A38FC(0xA0), kind);
    else
        result = func_80106400(obj, kind);
    return result;
}
