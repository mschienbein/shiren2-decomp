#include "common.h"

typedef unsigned short u16;

typedef struct {
    char pad0[0xE4];
    u16 flags;
} Obj800EE314;

s32 func_800EE314(Obj800EE314 *obj) {
    return (obj->flags >> 1) & 1;
}
