#include "common.h"
typedef struct { unsigned char field_00[0x24]; const void *field_24; } Object;
extern const unsigned char D_8015C148[192];
extern void *func_800EFC70(Object *, s32, unsigned char);
extern void func_800E4D88(Object *, s32);
extern void func_800E4D90(Object *, s32);
Object *func_80106CB0(Object *object, unsigned char value) { func_800EFC70(object, 0x4F, value); object->field_24 = D_8015C148; func_800E4D88(object, 4); func_800E4D90(object, 3); return object; }
