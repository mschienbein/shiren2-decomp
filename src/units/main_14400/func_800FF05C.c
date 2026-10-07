#include "common.h"
typedef struct { unsigned char field_00[0x1C]; unsigned short field_1C; unsigned char field_1E[0x38]; unsigned char field_56; unsigned char field_57[0x1B]; unsigned char field_72; unsigned char field_73[2]; unsigned char field_75; unsigned char field_76[0x13]; unsigned char field_89; } Object;
extern s32 func_800E1CD4(Object *, s32), func_800E1CC4(Object *, s32), func_800E20CC(void *);
extern unsigned short func_800E08B0(Object *), func_800E08F0(Object *);
extern s32 func_80049CB4(s32, ...);
extern void func_800A7C1C(Object *);
void func_800FF05C(Object *object) {
    if (!func_800E1CD4(object, 0xF)) {
        unsigned short amount = func_800E08B0(object);
        s32 state = 1;
        s32 failed;
        object->field_1C &= 0xFFFD;
        failed = func_800E1CC4(object, 2) != 1;
        if (failed) {
            if (amount <= object->field_89) {
                state = 3;
                if (func_800E20CC(object)) object->field_1C |= 2;
                else { object->field_56 = 0; object->field_72 |= 4; }
            } else if (amount <= (unsigned short)func_800E08F0(object) / 2U) state = 2;
        }
        if (state != object->field_75) { object->field_75 = state; func_80049CB4(0xA2, object, state); func_800A7C1C(object); }
    }
}
