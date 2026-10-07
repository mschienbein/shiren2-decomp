#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x1];
    u8 field_1;
} Obj_80112EAC;

extern u8 D_80148644[];
extern u8 D_8015488C[];

void func_80112EAC(Obj_80112EAC *obj) {
    s32 index = obj->field_1 - 0x17;
    u8 *flags = &D_80148644[index >> 3];

    *flags |= D_8015488C[index & 7];
}
