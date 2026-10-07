#include "common.h"
typedef struct { short field_00, field_02, field_04, field_06, field_08, field_0A; unsigned char field_0C[8]; unsigned char field_14; unsigned char field_15[3]; unsigned char field_18, field_19, field_1A; unsigned char field_1B[5]; } Object;
extern Object D_80165960;
extern void func_800265E0(void *, s32);
extern void func_8005CD40(s32, s32, s32, s32, s32, s32);
extern void func_8005DB6C(void);
void func_8005DA84(void) { Object *object = &D_80165960; func_800265E0(object, 0x20); func_8005CD40(0, 0x188, 0xFA, 0xBE, 0, 0); object->field_00 = 0x18; object->field_02 = 0x18; object->field_04 = 0xFA; object->field_06 = 0xBE; object->field_08 = 0xFA; object->field_0A = 0xBE; object->field_14 = 0x21; object->field_18 = 0; object->field_19 = 0; object->field_1A = 0xFF; func_8005DB6C(); }
