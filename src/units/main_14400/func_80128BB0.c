#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0xF];
    u8 unkF;
} Obj80128BB0;

u8 func_80128BB0(Obj80128BB0 *obj) {
    return obj->unkF;
}
