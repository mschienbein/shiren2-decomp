#include "common.h"

typedef struct Obj800BCB18 Obj800BCB18;
typedef struct { unsigned char pad_00[0x18]; Obj800BCB18 *field_18; } Owner;
typedef struct { unsigned char pad_00[8]; Owner *field_08; } Obj;
extern s32 func_800DADCC(unsigned char *object);
extern s32 func_800AC670(Obj800BCB18 *obj);
s32 func_800DCC3C(Obj *obj)
{
    if ((func_800DADCC((unsigned char *)obj) ^ 1) != 0) {
        return 0;
    }
    return obj->field_08->field_18 != 0 ? (func_800AC670(obj->field_08->field_18) ^ 1) : 0;
}
