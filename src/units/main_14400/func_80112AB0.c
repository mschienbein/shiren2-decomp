#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 field_0;
    u8 id;
    u8 pad2[0xC - 0x2];
    s32 level;
} Obj80112AB0;

extern u16 D_80157E9C[];
char *func_80048480(u16 value);

char *func_80112AB0(Obj80112AB0 *obj) {
    return func_80048480(D_80157E9C[(obj->id - 0xE9) * 3 + obj->level - 1]);
}
