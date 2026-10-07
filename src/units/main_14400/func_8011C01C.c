#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0xC];
    u8 flags;
} Obj;

s32 func_8011C01C(Obj *obj, s32 kind) {
    if (kind == 0x22) {
        return (obj->flags & 1) ^ 1;
    }
    return kind == 5 || kind == 0xB;
}
