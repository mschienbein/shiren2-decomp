#include "common.h"
typedef unsigned char u8;
typedef struct { u8 pad[0x13]; u8 field13; u8 pad14[3]; u8 field17; } Object;
extern s32 func_800E1CD4(void *, s32);
extern void func_800E43EC(void *, unsigned short, u8);
s32 func_800E5DFC(void *obj, Object *source) {
    if (func_800E1CD4(obj, 15)) return 0;
    func_800E43EC(obj, source->field13, source->field17);
    return 1;
}
