#include "common.h"
typedef struct { unsigned char pad_00[0x9A]; unsigned short flags_9A; } Obj;
extern s32 func_800E7794(void *obj);
extern s32 func_800F3310(void *obj);
s32 func_800FFF18(Obj *obj) {
    s32 result;
    if (!(obj->flags_9A & 0x40)) result = func_800E7794(obj);
    else result = func_800F3310(obj);
    return result;
}
