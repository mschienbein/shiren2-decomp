#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x4];
    s32 field_4;
    u32 field_8;
} Obj_8008E1F8;

s32 func_8008E1F8(Obj_8008E1F8 *obj, s32 amount) {
    if (amount > 0 && obj->field_8 < (u32)amount) {
        return 1;
    }
    obj->field_4 += amount;
    return 0;
}
