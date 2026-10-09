#include "common.h"

typedef signed char s8;
typedef unsigned char u8;

typedef struct {
    u8 pad0[0x56];
    s8 count56;
} Obj_800E4E90;

void func_800E4E90(Obj_800E4E90 *obj) {
    if (obj->count56 > 0) {
        obj->count56--;
    }
}
