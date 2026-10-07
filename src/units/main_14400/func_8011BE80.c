#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0xC];
    u8 flags;
} Obj;

s32 func_8011BE80(Obj *obj, s32 kind)
{
    if (kind == 0x22) {
        return (obj->flags & 1) ^ 1;
    }
    return kind == 5 || kind == 0xB;
}
