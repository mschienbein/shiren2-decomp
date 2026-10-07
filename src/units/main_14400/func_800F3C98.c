#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x87];
    u8 field_87;
} Obj800F3C98;

u8 func_800F3C98(Obj800F3C98 *obj) {
    return obj->field_87;
}
