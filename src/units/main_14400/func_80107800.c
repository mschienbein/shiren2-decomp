#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[8];
    u8 field_8;
} Info80107800;

typedef struct {
    u8 pad0[0x1C];
    u16 field_1C;
    u8 pad1E[6];
    void *field_24;
} Obj80107800;

extern u8 D_8015C390[];
extern const u8 D_8015488C[8]; /* Complete bit-mask table; +2 is not a separate object. */


void *func_800EFC70(Obj80107800 *obj, s32 kind, u8 arg);
Info80107800 *func_800C9E10(void);

Obj80107800 *func_80107800(Obj80107800 *obj, u8 arg)
{
    s32 flag;

    func_800EFC70(obj, 0x53, arg);
    obj->field_24 = D_8015C390;
    flag = 0;
    {
        u8 bits = func_800C9E10()->field_8;

        if ((bits & D_8015488C[2]) == 0) {
            flag = D_80142F18.mode == 0x4E;
        }
    }
    if (flag) {
        obj->field_1C |= 2;
    }
    return obj;
}
