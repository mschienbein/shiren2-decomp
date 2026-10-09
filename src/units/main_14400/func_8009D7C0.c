#include "common.h"
typedef struct { unsigned char pad_00[0x5C]; void *field_5C; } Obj;
extern s32 func_800D15B0(void *obj);
s32 func_8009D7C0(Obj *obj) {
    if (func_800D15B0(obj->field_5C)) return 2;
    return 1;
}
