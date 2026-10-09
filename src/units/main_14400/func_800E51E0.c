#include "common.h"
typedef struct { unsigned char pad_00[0x54]; unsigned char flags_54; } Obj;
typedef struct { unsigned char pad_00[0x13]; unsigned char count_13; } Source;
extern void func_800A578C(Obj *obj);
void func_800E51E0(Obj *obj, Source *source) {
    s32 count;
    if (((obj->flags_54 & 1) ^ 1) != 0) {
        count = source->count_13;
        while (--count != -1) func_800A578C(obj);
    }
}
