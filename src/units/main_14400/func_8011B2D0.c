#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad0[0xC]; u8 flags_C; } Obj;

s32 func_8011B2D0(Obj *o, s32 kind) {
    if (kind == 0x17) {
        return (o->flags_C & 1) ^ 1;
    }
    return kind == 5 || kind == 0xB;
}
