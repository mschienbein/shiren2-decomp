#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct {
    s32 field_0;
    s32 field_4;
    u8 field_8;
    u8 pad9[0x1C - 0x9];
    u16 field_1C;
} Obj800A3964;
/* Whole RNG object (state pointers plus backing words); only its base is passed here. */
extern u8 D_80147620[];
u8 func_800C57A0(void *rng);
void func_800A5A70(Obj800A3964 *obj, u32 arg1);
void func_800A5A88(Obj800A3964 *obj, s32 arg1);

void func_800A3964(Obj800A3964 *obj) {
    u8 dir[8];

    obj->field_0 = 0;
    obj->field_4 = 0;
    dir[0] = func_800C57A0(D_80147620) & 7;
    obj->field_8 = dir[0];
    func_800A5A70(obj, 2);
    func_800A5A88(obj, 1);
    obj->field_1C = 0;
}
